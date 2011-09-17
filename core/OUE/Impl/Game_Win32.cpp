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
#include "Win32.h"
#include "../OS.h"
#include "Game_Win32.h"
#include "../Event.h"
#include "../Broadcast.h"
#include "../Video.h"
#include "../Stereo.h"
#include "../Scene.h"
#include "../Control.h"
//#include "Shell.h"

namespace OneU
{
	HWND Window_create(pcwstr pWndName, bool bPopUp, int iWidth, int iHeight);
}

namespace OneU
{
	Game_Win32::~Game_Win32(){
		if(m_pScene != NULL){ ONEU_DELETE m_pScene; m_pScene = NULL;}
		if(m_pBroadcast != NULL) m_pBroadcast->sendEvent(event::DESTROY);
		if(m_pControl != NULL){ ONEU_DELETE m_pControl; m_pControl = NULL;}
		if(m_pStereo != NULL){ ONEU_DELETE m_pStereo; m_pStereo = NULL; }
		if(m_pVideo != NULL){ ONEU_DELETE m_pVideo; m_pVideo = NULL;}
		::DestroyWindow(g_hWnd);
		if(m_pBroadcast != NULL){ ONEU_DELETE m_pBroadcast; m_pBroadcast = NULL;}
	}
	void Game_Win32::init(pcwstr WindowName, uint32 width, uint32 height, bool bWindowed){
		//初始化随机种子
		srand(::GetTickCount());
		//创建广播系统
		ONEU_ASSERT(m_pBroadcast == NULL);
		m_pBroadcast = BroadCast_create();

		//设定好全局变量
		g_hInstance = GetModuleHandle(NULL);

		//创建窗口
		Window_create(WindowName, !bWindowed, width, height);
		ONEU_ASSERT(m_pVideo == NULL);
		m_pVideo = Video_create();


		m_pStereo = Stereo_create();
		ONEU_ASSERT(m_pControl == NULL);
		m_pControl = Control_create();

		//shell初始化
		//m_pShell = tool::Shell_config();
		//showInfo();
		//m_pVideo->showInfo();
	}
	int Game_Win32::run(){
		while(true){
			MSG msg;

			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				if(msg.message == WM_QUIT) goto end;
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			try{
				OneU::os::Sleep(1);
				this->onFrame();
			}
			catch(...){

				::MessageBoxW(NULL, L"运行时发生错误退出。", L"错误", MB_OK | MB_ICONERROR);
				ONEU_LOG(L"运行时发生错误退出。");

				return -1;
			}
		}
end:
		return 0;
	}
	void Game_Win32::quit(){
		PostQuitMessage(0);
	}


	IScene* Game_Win32::replaceScene( IScene* newscene )
	{
		if(newscene == m_pScene) return NULL;
		IScene* ret = m_pScene;
		m_pScene = newscene;

		if(newscene) m_pVideo->setRenderScene(newscene->getRenderScene());
		else m_pVideo->setRenderScene(NULL);
		return ret;
	}


	void Game_Win32::onFrame(){
		if(!m_bActive) return;

		//计算FPS
		++m_Frames;
		if(m_lastTime != 0){
			uint32 now = ::timeGetTime();
			uint32 interval = now - m_lastTime;
			if(interval > 1000){
				m_lastTime = now;
				m_FPS = ((float)m_Frames) / interval * 1000;
				m_Frames = 0;
			}
			m_TimeInterval = float(now - m_TimeLastFrame) / 1000.0f;
			m_TimeLastFrame = now;
		}
		else {
			m_TimeLastFrame = m_lastTime = ::timeGetTime();
		}

		m_pControl->update();
		m_pBroadcast->sendEvent(event::ENTER_FRAME);
		if(m_pScene != NULL) m_pScene->update(m_TimeInterval);
		m_pStereo->update();
		m_pVideo->update(m_TimeInterval);
		m_pVideo->render();
		m_pVideo->flip();
	}
	void Game_Win32::onActiveWindow(bool bActive){
		m_bActive = bActive;
		m_lastTime = 0;
		m_Frames = 0;
		if(m_bActive)
			m_pBroadcast->sendEvent(event::WINDOW_ACTIVE);
		else
			m_pBroadcast->sendEvent(event::WINDOW_DEACTIVE);
		//refresh control
		//ignore the event when window is not in front
		if(m_pControl != NULL) {  m_pControl->update();}
	}

	void Game_Win32::setWindowTitle(pcwstr title){
		SetWindowTextW(g_hWnd,title);
	}

	vector2i_t Game_Win32::getWindowPos(){
		RECT rc;
		GetWindowRect(g_hWnd, &rc);
		return vector2i_t(rc.left, rc.top);

	}
	void Game_Win32::setWindowPos(const vector2i_t& newPos){
		SetWindowPos(g_hWnd, HWND_TOP, newPos.x, newPos.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	void Game_Win32::showCursor(bool bShow){
		::ShowCursor( bShow );
	}

	float Game_Win32::getFPS(){
		return m_FPS;
	}
	float Game_Win32::getTimeInterval(){
		return m_TimeInterval;
	}

	void Game_Win32::runShell( pcwstr command ){
		//if(m_pShell) m_pShell->runCommand(command);
	}
	void Game_Win32::output( pcwstr data ){
		//if(m_pShell) m_pShell->output(data);
	}

	void Game_Win32::showInfo()
	{
		stream() << TimeInfo << L"OneU 1.0.0.1 Ladace 2011\nWin32 OS\n\n";
	}

}

#pragma comment( lib, "winmm.lib" )
namespace OneU
{
	/** 
	* @name Windows部分
	* @{ */

	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 窗口类名字
	*/
	/* ----------------------------------------------------------------------------*/
	const wchar_t * szClassName = L"万有引擎";

	HINSTANCE g_hInstance = NULL;
	static HINSTANCE g_hPrevInstance = NULL;
	HWND g_hWnd = NULL;

	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 消息处理函数
	*/
	/* ----------------------------------------------------------------------------*/
	LRESULT CALLBACK WindowProcedure(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);

	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 初始化窗口函数
	*/
	/* ----------------------------------------------------------------------------*/
	HWND Window_create(pcwstr pWndName, bool bPopUp, int iWidth, int iHeight){
		ONEU_ASSERT(g_hInstance != NULL);

		WNDCLASSEX wincl;

		wincl.hInstance = g_hInstance;
		wincl.lpszClassName = szClassName;
		wincl.lpfnWndProc = WindowProcedure;      
		wincl.style = CS_DBLCLKS;                 
		wincl.cbSize = sizeof(WNDCLASSEX);

		wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
		wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
		wincl.lpszMenuName = NULL;                 
		wincl.cbClsExtra = 0;                      
		wincl.cbWndExtra = 0;
		wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

		if (!::RegisterClassEx(&wincl)){
			ONEU_LOG( L"注册窗口类失败" );
			ONEU_RAISE(WinErrorString(::GetLastError()).c_str());
		}

		ONEU_ASSERT(g_hWnd == NULL);

		
		int iX = (GetSystemMetrics(SM_CXFULLSCREEN) - iWidth) / 2,
			iY = (GetSystemMetrics(SM_CYFULLSCREEN) - iHeight) / 2;

		RECT rc = {iX, iY, iWidth + iX, iHeight + iY};
		::AdjustWindowRectEx(&rc, bPopUp ? WS_POPUP : WS_OVERLAPPEDWINDOW, FALSE, 0);

		g_hWnd = ::CreateWindowEx(
			0,
			szClassName,
			pWndName ? pWndName : L"万有引擎",
			bPopUp ? WS_POPUP : WS_OVERLAPPEDWINDOW,
			iX,
			iY,
			rc.right - rc.left,
			rc.bottom - rc.top,
			HWND_DESKTOP,
			NULL,
			g_hInstance,
			NULL
			);

		if(!g_hWnd){
			ONEU_LOG(L"创建窗口失败");
			ONEU_RAISE(WinErrorString(::GetLastError()).c_str());
		}

		//TODO: This tells you how to set icon, but we can't use it because we don't have a icon in the dll. we want to user to configure it.
		//::SendMessage(g_hWnd, WM_SETICON, ICON_BIG, ::LoadIcon(g_hInstance, IDI_ICON));

		::ShowWindow (g_hWnd, SW_SHOW);

		return g_hWnd;
	}


	LRESULT CALLBACK WindowProcedure (HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam){
		switch (message){
			case WM_DESTROY:
				PostQuitMessage(0);//调用该函数后会无法弹出对话框
				break;
				//！！注意：这个消息会在DX某种配置下引起bug。如果调用DefWindowProc前台表面会被填充成白色。
			case WM_ERASEBKGND:
				break;
			case WM_ACTIVATE:
				if((LOWORD( wParam ) == WA_ACTIVE || LOWORD( wParam ) == WA_CLICKACTIVE))
					GetGame().onActiveWindow(true);
				else
					GetGame().onActiveWindow(false);
				break;
			case WM_CHAR:
				{
					GetGame().onChar(CharEvent((::WCHAR)wParam));
					break;
				}
			default:
				return ::DefWindowProc( hwnd, message, wParam, lParam );
		}

		return 0;
	}
	/**  @} */
}