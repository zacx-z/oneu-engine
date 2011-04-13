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
#include "DXException.h"
#include <d3d9.h>

namespace OneU
{
	DXException::~DXException(void)
	{
	}

	pcwstr DXException::GetType() const
	{
		return L"DirectX´íÎó";
	}

	pcwstr DXException::GetString() const
	{
		return ::DXGetErrorDescription( m_hRes );
	}

	ONEU_BASE_API void _ThrowHResult(HRESULT hr, const char * FileName, const int Line)
	{
		switch(hr)
		{
		case D3DERR_DEVICELOST:
			_ThrowExp(FileName, Line, DXLostDevice(hr));
		default:
			_ThrowExp(FileName, Line, DXException(hr));
		}
	}
}
