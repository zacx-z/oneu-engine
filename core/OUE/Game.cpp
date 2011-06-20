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
#include "Game.h"
#include "Video.h"
#include "Scene.h"

#ifdef _WIN32
#include "Impl/Game_Win32.h"
#endif
#ifdef ONEU_USE_DIRECT3D
#include "Impl/DXVideo.h"
#endif
#include "Impl/DXStereo.h"
#ifdef ONEU_USE_DIRECTINPUT
#include "Impl/DXControl.h"
#endif

namespace OneU
{
	static IGame* s_pGame = NULL;

	ONEU_API IGame& GetGame(){ONEU_ASSERT(s_pGame != NULL); return *s_pGame;}
	ONEU_API bool HasGame(){ return s_pGame != NULL; }

	static TerminateHandler _last_eh = NULL;
	static void _destroyGame();
	ONEU_API void Game_build(Factory<IGame>::type gf)
	{
		ONEU_ASSERT(s_pGame == NULL);
		Allocator_build(Allocator_create);
		Logger_build(LoggerNull_Factory);//FIXME: 见LoggerDisk.cpp ruby的fclose符号如何消除？
		s_pGame = gf();

		_last_eh = SetTerminateHandler(_destroyGame);
	}	
	ONEU_API void Game_destroy(){
		if(s_pGame != NULL)
			ONEU_DELETE s_pGame;
		s_pGame = NULL;
		//销毁日志系统	其中Logger在Game_build中初始化
		Logger_destroy();
		GetAllocator().checkLeaks();
		Allocator_destroy();
	}

	void _destroyGame()
	{
		Game_destroy();
		s_pGame = NULL;
		if(_last_eh != NULL)
			_last_eh();
	}

	ONEU_API IGame* Game_create(){
#ifdef _WIN32
		return ONEU_NEW Game_Win32;
#endif
	}
	ONEU_API IVideo* Video_create(){
#ifdef ONEU_USE_DIRECT3D
		return ONEU_NEW DXVideo;
#endif
	}
	ONEU_API IStereo* Stereo_create(){
		return ONEU_NEW DXStereo;
	}
	ONEU_API IControl* Control_create(){
#ifdef ONEU_USE_DIRECTINPUT
		return ONEU_NEW DXControl;
#endif
	}
}
