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
 * @file DXException.h
 * @brief DXÒì³£
 *
 * ÒÑ·ÏÆú¡£
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once

#include "../Logger.h"
#include "Exception.h"
#include <windows.h>
#include <dxerr.h>

#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxerr9.lib" )

#define DXERRGET_STR(hr) ::DXGetErrorDescription(hr)
#define ONEU_LOG_DXERR(hr) ONEU_LOG(::DXGetErrorDescription(hr))
	
namespace OneU
{
	class DXException
		: public Exception
	{
	private:
		HRESULT m_hRes;

		static wchar * s_pMsgBuf;
	public:
		explicit DXException( HRESULT hRes, bool MakeDump = true )
			:Exception( L"DirectX´íÎó", MakeDump ), m_hRes( hRes )
		{
			LOGMSG( ::DXGetErrorDescription( m_hRes ) );
		}
		~DXException(void);

		operator HRESULT(){ return m_hRes; }

		pcwstr GetType() const;
		pcwstr GetString() const;

	};
	class DXLostDevice
		: public DXException
	{
	public:
		explicit DXLostDevice( HRESULT hRes, bool MakeDump = true )
			: DXException( hRes, MakeDump )
		{}
	};
}
