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
 * @file OS.h
 * @brief 操作系统函数库
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once

#include "OUEDefs.h"
#include "../Base/Interface.h"
#include <windows.h>

#ifndef _WIN32
#pragma message("Error!This is not Win32 OS.")
#endif


namespace OneU
{
	namespace os
	{
		/** 
		* @name Windows封装库
		* @{ */

		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 系统时间结构体
		*
		* 传入GetSystemTime函数的参数类型
		* @sa GetSystemTime
		*/
		/* ----------------------------------------------------------------------------*/
		typedef struct tagSystemTime
		{
			uint wYear; 
			uint wMonth; 
			uint wDayOfWeek; 
			uint wDay; 
			uint wHour; 
			uint wMinute; 
			uint wSecond; 
			uint wMilliseconds; 
		}SYSTEMTIME;


		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 睡眠
		* 
		* @param dwMilliseconds 睡眠时间，以毫秒计
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API void Sleep(dword dwMilliseconds);
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取相对时间
		*
		* 一定程度上不准确
		* 
		* @return 相对时间的值
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API dword GetTime();
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取系统时间
		* 
		* @param pSystemTime 指向系统时间结构体的指针
		*
		* @sa SYSTEMTIME
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API void GetSystemTime(SYSTEMTIME * pSystemTime);
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 设置鼠标显示状态
		* 
		* @param bShow 为TRUE鼠标显示；为FALSE鼠标隐藏。
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API void ShowCursor(bool bShow);
		/* ----------------------------------------------------------------------------*/
		/**
		* @brief 设置鼠标位置
		*
		* @param X 鼠标X坐标
		* @param Y 鼠标Y坐标
		* @remarks 坐标按照屏幕像素为单位。且是GDI的坐标系(原点在左上，正方向分别为下和右）。
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API void SetCursorPos(int X, int Y);
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取当前目录位置
		* 
		* @return 指向目录位置字符串的指针
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API wchar * GetDirectory();

		/* ----------------------------------------------------------------------------*/
		/**
		* @brief 弹出错误对话框
		*
		* @param Errorinfo 对话框显示的错误信息
		*/
		/* ----------------------------------------------------------------------------*/
		ONEU_API void ErrorBox(pcwstr Errorinfo);

		ONEU_API float Rand();
		/**  @} */

	}
}
