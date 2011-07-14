#include "DXStereo.h"
#include "Win32.h"
#include "DXLib/DXDefs.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libogg_static.lib")


namespace OneU
{
	namespace stereo
	{
		static DWORD WINAPI _DecodeThreadProc(LPVOID lpParameter);
		class OggSound
			: public ISound
		{
			IDirectSoundBuffer8* m_pBuffer;
			OggVorbis_File* m_pVorbisFile;
			WAVEFORMATEX m_wfx;

			struct _StreamInfo
			{
				IDirectSoundNotify8* m_pNotify;

				char* m_decodeBuffer;

				HANDLE m_decodeThread;
				HANDLE m_streamEvent[3];//3为线程结束事件
				HANDLE m_hMutex;
				class ScopedMutex
				{
					HANDLE m_hMutex;
				public:
					ScopedMutex(HANDLE hMutex) : m_hMutex(hMutex){
						if(m_hMutex) WaitForSingleObject(m_hMutex, INFINITE);
					}
					~ScopedMutex(){
						if(m_hMutex) ::ReleaseMutex(m_hMutex);
					}
				};

				//当m_waitEnd非0时解码线程将要停止播放缓冲。只有在非loop播放才会用到。
				//为1或2时表示m_streamEvent[m_waitEnd - 1]激活时停止播放。
				int m_waitEnd;
			}* si;
			bool m_bLooped;
		public:
			OggSound(IDirectSound* pDS, pcwstr filename, bool streamed);
			~OggSound();
			bool isStreamed(){
				return si != NULL;
			}
			bool isPlaying(){
				DWORD status;
				XV(m_pBuffer->GetStatus(&status));
				return (bool)(status & DSBSTATUS_PLAYING);
			}
			long getVolume(){
				long ret;
				XV(m_pBuffer->GetVolume(&ret));
				return ret;
			}
			void setVolume(long volume){
				XV(m_pBuffer->SetVolume(volume));
			}
			uint32 getFrequency(){
				uint32 ret;
				XV(m_pBuffer->GetFrequency(&ret));
				return ret;
			}
			void setFrequency(uint32 freq){
				XV(m_pBuffer->SetFrequency(freq));
			}
			long getPan(){
				long ret;
				XV(m_pBuffer->GetPan(&ret));
				return ret;
			}
			void setPan(long pan){
				XV(m_pBuffer->SetPan(pan));
			}
			void _play(bool bLooped){
				//互斥锁
				m_bLooped = bLooped;
				_StreamInfo::ScopedMutex _(isStreamed() ? si->m_hMutex : NULL);
				if(isStreamed()) si->m_waitEnd = 0;
				if(isStreamed() || bLooped) m_pBuffer->Play(0, 0, DSBPLAY_LOOPING);
				else m_pBuffer->Play(0, 0, 0);
			}
			void _stop(){
				_StreamInfo::ScopedMutex _(isStreamed() ? si->m_hMutex : NULL);
				m_pBuffer->Stop();
			}
		private:
			void _fillBuffer(int n_frac);
			void _fillAll();
		public:
			//解码线程循环
			void _decodeLoop();

			IDirectSoundBuffer* _dupBuffer(IDirectSound8* pDS){
				IDirectSoundBuffer* ret = NULL;
				pDS->DuplicateSoundBuffer(m_pBuffer, &ret);
				return ret;
			}
		};

		OggSound::OggSound( IDirectSound* pDS, pcwstr filename, bool streamed ) : si(NULL), m_bLooped(false)
		{
			//打开文件
			FILE* f = _wfopen(filename, L"rb");
			if(!f) ONEU_RAISE(String().format(L"音频：找不到%s文件。", filename).c_str());
			m_pVorbisFile = new (OggVorbis_File);
			memset(m_pVorbisFile, 0, sizeof(OggVorbis_File));
			ov_clear(m_pVorbisFile);
			if(ov_open(f, m_pVorbisFile, NULL, 0) != 0){
				ov_clear(m_pVorbisFile);
				delete (m_pVorbisFile);
				ONEU_RAISE(String().format(L"文件%s不符合Ogg格式！", filename).c_str());
			}

			//创建缓冲
			memset(&m_wfx, 0, sizeof(m_wfx));
			m_wfx.cbSize = sizeof(m_wfx);
			m_wfx.wFormatTag = WAVE_FORMAT_PCM;
			m_wfx.nSamplesPerSec = m_pVorbisFile->vi->rate;
			m_wfx.wBitsPerSample = 16;
			m_wfx.nChannels = m_pVorbisFile->vi->channels;

			m_wfx.nBlockAlign = m_wfx.nChannels * m_wfx.wBitsPerSample / 8;
			m_wfx.nAvgBytesPerSec = m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;

			DSBUFFERDESC dsbd;
			memset(&dsbd, 0, sizeof(dsbd));
			dsbd.dwSize = sizeof(dsbd);
			dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLPOSITIONNOTIFY;
			dsbd.dwBufferBytes = streamed ? m_wfx.nAvgBytesPerSec * 2 : (DWORD)ov_pcm_total(m_pVorbisFile, -1);
			dsbd.lpwfxFormat = &m_wfx;

			HRESULT hr;
			IDirectSoundBuffer* pBuffer;
			if(FAILED(hr = pDS->CreateSoundBuffer(&dsbd, &pBuffer, NULL))){
				ov_clear(m_pVorbisFile);
				delete (m_pVorbisFile);
				RAISE_HRESULT(hr);
			}
			XV_RAISE(pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pBuffer));
			pBuffer->Release();

			if(streamed){
				si = new _StreamInfo;
				//创建编码缓冲
				const long frac_len = m_wfx.nAvgBytesPerSec;
				si->m_decodeBuffer = new char[frac_len];

				_fillBuffer(0);

				//获取通告
				XV_RAISE(m_pBuffer->QueryInterface(IID_IDirectSoundNotify8, (void**)&si->m_pNotify));
				//创建流事件
				si->m_streamEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
				si->m_streamEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
				DSBPOSITIONNOTIFY dspn[2];
				dspn[0].dwOffset = 0;
				dspn[0].hEventNotify = si->m_streamEvent[0];
				dspn[1].dwOffset = frac_len;
				dspn[1].hEventNotify = si->m_streamEvent[1];
				//第3个流事件用于通知线程结束
				si->m_streamEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
				//设置通告
				XV_RAISE(si->m_pNotify->SetNotificationPositions(2, dspn));
				//创建互斥锁
				si->m_hMutex = CreateMutex(NULL, FALSE, NULL);

				//创建解码线程
				si->m_decodeThread = ::CreateThread(NULL, 0, _DecodeThreadProc, this,0, NULL);
			}else
				_fillAll();
		}
		
		OggSound::~OggSound()
		{
			_stop();
			if(si){
				SetEvent(si->m_streamEvent[2]);//通知解码线程结束
				WaitForSingleObject(si->m_decodeThread, INFINITE);//等待线程结束

				CloseHandle(si->m_hMutex);
				for(int i = 0; i < 3; ++i)
					CloseHandle(si->m_streamEvent[i]);
				SAFE_RELEASE(si->m_pNotify);
				delete [](si->m_decodeBuffer);
			}
			ov_clear(m_pVorbisFile);
			delete (m_pVorbisFile);
			SAFE_RELEASE(m_pBuffer);

			delete si;
		}

		void OggSound::_fillBuffer( int n_frac )
		{
			const long frac_len = m_wfx.nAvgBytesPerSec;

			long left = frac_len, curpos = 0;
			while(left > 0){
				int bitstream;
				long bytes_read = ov_read(m_pVorbisFile, si->m_decodeBuffer + curpos, left, 0, 2/*16-bit*/, 1, &bitstream);

				left -= bytes_read;
				curpos += bytes_read;

				//如果读到结尾则重头开始。
				if(bytes_read == 0){
					if(m_bLooped) ov_pcm_seek(m_pVorbisFile, 0);
					else {
						memset(si->m_decodeBuffer + curpos, 0, left);
						si->m_waitEnd = n_frac + 1;
						break;
					}
				}
				if(bytes_read < 0) break;
			}

			{
				char *ptr = NULL;
				DWORD size = 0;
				//互斥锁
				_StreamInfo::ScopedMutex _(si->m_hMutex);
				XV_RAISE(m_pBuffer->Lock(n_frac * frac_len, frac_len, (void**)&ptr, &size, NULL, NULL, 0));//以缓冲区一半为单位，锁定一半。
				memcpy(ptr, si->m_decodeBuffer, frac_len);
				XV_RAISE(m_pBuffer->Unlock((void*)ptr, size, NULL, 0));
			}
		}

		void OggSound::_fillAll()
		{
			char* ptr = NULL; DWORD size = 0;
			XV_RAISE(m_pBuffer->Lock(0, 0, (void**)&ptr, &size, NULL, NULL, DSBLOCK_ENTIREBUFFER));
			long left = size, curpos = 0;
			while(left > 0){
				int bitstream;
				long bytes_read = ov_read(m_pVorbisFile, ptr + curpos, left, 0, 2, 1, &bitstream);
				left -= bytes_read;
				curpos += bytes_read;

				if(bytes_read <= 0) break;
			}
			XV_RAISE(m_pBuffer->Unlock((void*)ptr, size, NULL, 0));
		}

		void OggSound::_decodeLoop()
		{
			while(true){
				DWORD ret;
				if((ret = WaitForMultipleObjects(3, si->m_streamEvent, FALSE, INFINITE)) == WAIT_FAILED)
					break;
				DWORD n = ret - WAIT_OBJECT_0;
				if(n == 2)//线程结束的事件被触发
					break;
				else {
					if(n == 2 - si->m_waitEnd){
						_StreamInfo::ScopedMutex _(si->m_hMutex);
						m_pBuffer->Stop();
						si->m_waitEnd = 0;
					}
					_fillBuffer(1 - n);
				}
			}
		}

		static DWORD WINAPI _DecodeThreadProc(LPVOID lpParameter){
			((OggSound*)lpParameter)->_decodeLoop();
			return 0;
		}
	}


	void DXStereo::init(){
		XV_RAISE(DirectSoundCreate8(NULL, &m_pDS, NULL));
		XV_RAISE(m_pDS->SetCooperativeLevel(g_hWnd, DSSCL_PRIORITY));

		DSBUFFERDESC dsbd;
		memset(&dsbd, 0, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		dsbd.dwBufferBytes = 0;
		dsbd.lpwfxFormat = NULL;

		XV_RAISE(m_pDS->CreateSoundBuffer(&dsbd, &m_pDSPrimary, NULL));

		WAVEFORMATEX wfex;
		memset(&wfex, 0, sizeof(wfex));
		wfex.wFormatTag = WAVE_FORMAT_PCM;
		wfex.nChannels = 1;
		wfex.nSamplesPerSec = 22050;
		wfex.wBitsPerSample = 16;
		wfex.nBlockAlign = (wfex.wBitsPerSample / 8) * wfex.nChannels;
		wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
		XV_RAISE(m_pDSPrimary->SetFormat(&wfex));

		XV_RAISE(m_pDSPrimary->Play(0, 0, DSBPLAY_LOOPING));
	}

	sound_t DXStereo::loadSound(pcwstr filename, bool streamed){
		return ONEU_NEW stereo::OggSound(m_pDS, filename, streamed);
	}
	void DXStereo::playMusic(sound_t sound, bool looped){
		stereo::OggSound* m = ((stereo::OggSound*)(m_Music.get()));
		if(m) m->_stop();
		((stereo::OggSound*)(sound.get()))->_play(looped);
		m_Music = sound;
	}

	void DXStereo::playFX(sound_t sound){
		stereo::OggSound* s = ((stereo::OggSound*)(sound.get()));
		if(!s->isStreamed()){
			//若非流音频
			//复制缓冲并播放
			//该方法能使其重叠播放
			IDirectSoundBuffer* buf = s->_dupBuffer(m_pDS);
			buf->Play(0, 0, 0);
			m_playingFX.pushBack(buf);
		}else s->_play(false);
	}
	void DXStereo::update(){
		//检查播放完的音效并删除
		for(List<IDirectSoundBuffer*>::iterator it = m_playingFX.begin(); it != m_playingFX.end();){
			DWORD status;
			(*it)->GetStatus(&status);
			if(!(status & DSBSTATUS_PLAYING))
				m_playingFX.erase(it++);
			else
				++it;
		}
	}

	DXStereo::~DXStereo(){
		m_Music = NULL;//析构缓冲
		SAFE_RELEASE(m_pDSPrimary);
		SAFE_RELEASE(m_pDS);
	}
}