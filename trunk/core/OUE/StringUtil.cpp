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
#include "String.h"
#include <windows.h>
#include <string>
#include <vector>

#pragma warning(disable : 4996)

namespace OneU
{
	//×ª»»
	extern "C" ONEU_API wchar* _Char2Wide(pcstr cstr, uint32 codepage)
	{
		if(!cstr) return NULL;
		const size_t l = strlen(cstr);
		wchar* str = (wchar*)ONEU_ALLOC(sizeof(wchar) * (l + 2));
		if(!MultiByteToWideChar(codepage, 0, cstr, int(l + 1), str, int(l + 2)))
		{
			//throw Exception(L"can't translate char to unicode");
		}
		return str;
	}
	extern "C" ONEU_API char* _Wide2Char(pcwstr cstr, uint32 codepage)
	{
		if(!cstr) return NULL;
		const size_t l = wcslen(cstr);
		char* str = (char*)ONEU_ALLOC(sizeof(char) * (l + 2));
		if(!WideCharToMultiByte(codepage, 0, cstr, int(l + 1), str, int(l + 2), NULL, NULL))
		{
			//throw Exception(L"can't translate char to ansi");
		}
		return str;
	}
}