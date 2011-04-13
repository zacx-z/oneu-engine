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
/** 
 * @file WindowsEnc.cpp
 * @brief Windows封装的实现部分
 * @author Ladace
 * @version 1.0.0.1
 * @date 2009-11-10
 */
#include "Win32.h"
#include <windows.h>
#include "WinException.h"
#include "../Game.h"


namespace OneU
{
	

	//nothing now
	//all go to game_win32.cpp
	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 标准Main函数
	* 
	* @param hInstance 程序句柄
	* @param hPrevInstance 前一个程序句柄
	* @param lpCmdLine 控制台命令
	* @param nCmdShow 显示参数
	* 
	* @return 退出代码
	*/
	/* ----------------------------------------------------------------------------*/
	//	extern "C" int ONEU_API Windows_Main(IGame* (*gamefactory)(),HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	//		OneU::g_hInstance = hInstance;
	//		OneU::g_hPrevInstance = hPrevInstance;
	//
	//		//初始化日志系统
	//
	//		Game_build(gamefactory);
	//		IGame* game = &GetGame();
	//		try{
	//			game->init();
	//		}
	//		catch( ... ){
	//			ONEU_LOG( L"初始化错误退出" );
	//			Game_destroy();
	//			return -1;
	//		}
	//
	//		while ( TRUE ){
	//			MSG msg;
	//
	//			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE )){
	//				if(msg.message == WM_QUIT) goto end;
	//				::TranslateMessage(&msg);
	//				::DispatchMessage(&msg);
	//			}
	//
	//			try{
	//				OneU::Sleep(1);
	//				game->onFrame();
	//			}
	//			catch(...){
	//				ONEU_LOG(L"运行时发生错误退出。");
	//				Game_destroy();
	//
	//				return -1;
	//			}
	//		}
	//end:
	//		Game_destroy();
	//
	//		return 0;
	//	}
}
