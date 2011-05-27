#include "DXStereo.h"
#include "Win32.h"

#pragma comment(lib, "strmiids.lib")

namespace OneU
{
	void DXStereo::init(){
		CoInitialize(NULL);

		//DirectMusic
		if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
			CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**)&m_pDMPfm)))
			return;
		if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL,
			CLSCTX_INPROC, IID_IDirectMusicLoader8, (void**)&m_pDMLoader)))
			return;
		m_pDMPfm->InitAudio(&m_pDM, &m_pDS, g_hWnd,
			DMUS_APATH_SHARED_STEREOPLUSREVERB, 128,
			DMUS_AUDIOF_ALL, NULL);

		wchar str[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, str);
		m_pDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, str, FALSE);
		//DirectShow
		if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder)))
		{
			return;
		}
		m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
		m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_pMediaEvent);
	}

	void DXStereo::playMusic(pcwstr filename)
	{
		m_pMediaControl->Stop();
		IEnumFilters *pEF;
		m_pGraphBuilder->EnumFilters(&pEF);
		IBaseFilter *t = NULL;
		pEF->Next(1, &t, NULL);
		while(t)
		{
			m_pGraphBuilder->RemoveFilter(t);
			t = NULL;
			pEF->Next(1, &t, NULL);
		}
		pEF->Release();
		if(FAILED(m_pGraphBuilder->RenderFile(filename, NULL)))
			return;
		m_pMediaControl->Run();
	}
}