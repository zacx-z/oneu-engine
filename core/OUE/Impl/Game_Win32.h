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
#pragma once
#include "../Game.h"

namespace OneU
{
	namespace tool
	{
		class Shell;
	}

	class Game_Win32
		: public IGame
	{
		bool m_bActive;

		float m_FPS;
		uint32 m_lastTime;//计算m_FPS用
		uint32 m_Frames;//计算m_FPS用

		float m_TimeInterval;
		uint32 m_TimeLastFrame;

		//
		tool::Shell* m_pShell;
	public:
		Game_Win32() : m_bActive(true), m_FPS(0.0f), m_lastTime(0), m_Frames(0), m_TimeInterval(0.01f/*避免主逻辑不小心除零*/), m_pShell(NULL){}
		~Game_Win32();
		void init(pcwstr WindowName, uint32 width, uint32 height, bool bWindowed);
		int run();
		void quit();


		IScene* replaceScene(IScene* newscene);

		void onFrame();
		void onActiveWindow(bool bActive);

		void setWindowTitle(pcwstr title);
		vector2i_t getWindowPos();
		void setWindowPos(const vector2i_t& newPos);
		void showCursor(bool bShow);
		float getFPS();
		float getTimeInterval();

		//tools
		void output(pcwstr data);

		//不太重要的
		void showInfo();
	};
}