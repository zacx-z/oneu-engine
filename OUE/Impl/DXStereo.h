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
#include "../Stereo.h"
#include <dshow.h>
#include <dsound.h>
#include <dmusici.h>
#include <dxerr.h>

namespace OneU
{
	class DXStereo
		: public IStereo
	{
		//DirectShow
		IGraphBuilder* m_pGraphBuilder;
		IMediaControl* m_pMediaControl;
		IMediaEvent* m_pMediaEvent;
		//DirectSound
		IDirectSound* m_pDS;
		IDirectMusic* m_pDM;
		IDirectMusicPerformance8* m_pDMPfm;
		IDirectMusicLoader8* m_pDMLoader;
		IDirectSoundBuffer8* m_pDSPrimary;
	public:
		DXStereo()
			: m_pGraphBuilder(NULL), m_pMediaControl(NULL), m_pMediaEvent(NULL),
			m_pDS(NULL), m_pDSPrimary(NULL), m_pDM(NULL), m_pDMPfm(NULL), m_pDMLoader(NULL){}
		~DXStereo(){}
		void init();
		void playMusic(pcwstr filename);
	};
}