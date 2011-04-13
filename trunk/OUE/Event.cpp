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
#include "Event.h"
namespace OneU
{
	namespace event
	{
		//无参数事件
		//广播事件
		//广播对象接收到的事件
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 进入一帧的时候被广播
		*
		* 在场景更新之前。
		*/
		/* ----------------------------------------------------------------------------*/
		const wchar* ENTER_FRAME = L"enter frame";
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 窗口被激活时被广播
		*/
		/* ----------------------------------------------------------------------------*/
		const wchar* WINDOW_ACTIVE = L"wnd active";
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 窗口失去焦点时被广播
		*/
		/* ----------------------------------------------------------------------------*/
		const wchar* WINDOW_DEACTIVE = L"wnd deactive";
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 游戏对象被销毁时被广播
		*
		* 在场景销毁之后，各种系统销毁之前。
		* @sa section_run
		*/
		/* ----------------------------------------------------------------------------*/
		const wchar* DESTROY = L"destroy";

		const wchar* CHAR = L"char";
		const wchar* KEY = L"key";
	}
}
