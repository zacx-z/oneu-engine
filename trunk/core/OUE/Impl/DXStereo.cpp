#include "DXStereo.h"
#include "Win32.h"
#include "DXLib/DXDefs.h"

#pragma comment(lib, "strmiids.lib")

namespace OneU
{
	void DXStereo::init(){
		CoInitialize(NULL);

		//DirectMusic
		try{
			XV_THROW(CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
				CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**)&m_pDMPfm));
			XV_THROW(CoCreateInstance(CLSID_DirectMusicLoader, NULL,
				CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&m_pDMLoader));
			XV_THROW(m_pDMPfm->InitAudio(&m_pDM, &m_pDS, g_hWnd,
				DMUS_APATH_SHARED_STEREOPLUSREVERB, 128,
				DMUS_AUDIOF_ALL, NULL));

			wchar str[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, str);
			XV_THROW(m_pDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, str, FALSE));
			//DirectShow
			XV_THROW(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder));
			XV_THROW(m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl));
			XV_THROW(m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_pMediaEvent));
		}
		catch(...){
			ONEU_RAISE(L"Init DX Audio Error!");
		}
	}

	void DXStereo::playMusic(pcwstr filename){
		m_pMediaControl->Stop();
		IBaseFilter *t = NULL;
		//ֹ֮ͣǰ��BGM
		m_pGraphBuilder->FindFilterByName(L"BGM", &t);
		if(t) m_pGraphBuilder->RemoveFilter(t);

		IBaseFilter* newfilter = NULL;
		IEnumPins* ep = NULL;IPin* pin = NULL;
		
		try{
			XV_THROW(m_pGraphBuilder->AddSourceFilter(filename, L"BGM", &newfilter));
			XV_THROW(newfilter->EnumPins(&ep));
			XV_THROW(ep->Next(1, &pin, NULL));
			XV_THROW(m_pGraphBuilder->Render(pin));
		}
		catch(...){
			SAFE_RELEASE(newfilter);
			SAFE_RELEASE(ep);
			SAFE_RELEASE(pin);
			ONEU_RAISE(String().format(L"Play Music Error.\nPath:%s", filename).c_str());
		}
		m_pMediaControl->Run();
	}

	void DXStereo::playSound(pcwstr filename){
		if(FAILED(m_pGraphBuilder->RenderFile(filename, NULL)))
			ONEU_RAISE(String().format(L"Play Sound Error.\nPath:%s", filename).c_str());
	}

	DXStereo::~DXStereo(){
		SAFE_RELEASE(m_pMediaEvent);
		SAFE_RELEASE(m_pMediaControl);
		SAFE_RELEASE(m_pGraphBuilder);
		SAFE_RELEASE(m_pDM);
		SAFE_RELEASE(m_pDS);
		SAFE_RELEASE(m_pDMLoader);
		SAFE_RELEASE(m_pDMPfm);
	}
}