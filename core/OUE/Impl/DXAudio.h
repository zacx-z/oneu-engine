/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "../Audio.h"
#include "DXLib/DXDefs.h"
#include <dsound.h>
#include <dxerr.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "../List.h"

namespace OneU
{
	class DXAudio
		: public IAudio
	{
		//DirectSound
		IDirectSound8* m_pDS;
		IDirectSoundBuffer* m_pDSPrimary;
		sound_t m_Music;
		List<IDirectSoundBuffer*> m_playingFX;
		List<sound_t> m_playingMusic;
	public:
		DXAudio()
			: m_pDS(NULL), m_pDSPrimary(NULL){}
		~DXAudio();
		void init();
		audio::ISound* _loadSound(pcwstr filename, bool streamed);
		void playMusic(sound_t filename, bool looped);
		void stopMusic();
		sound_t getPlayingMusic(){ return m_Music; }
		void playFX(sound_t filename);
		void update();
	};
}