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
#include "../Game.h"


namespace OneU
{
	WinErrorString::~WinErrorString(){
		if( m_pMsgBuf )
		{
			::LocalFree((void*)m_pMsgBuf );
			m_pMsgBuf = 0;
		}
	}

	pcwstr WinErrorString::c_str() const{
		if( !m_pMsgBuf )
			::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
			m_hRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (pwstr) &m_pMsgBuf, 0, NULL );


		return m_pMsgBuf;
	}
}
