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
	//class String_Imp
	//	: public IString
	//{
	//	std::wstring m_str;
	//public:
	//	String_Imp(pcwstr str) : m_str(str){}
	//	String_Imp(){}
	//	virtual void destroy(){ONEU_DELETE(this); }
	//	virtual void append(const IString& others){m_str.append(others.c_str());}
	//	virtual void eval(const pcwstr str){m_str = str;}
	//	virtual pcwstr c_str() const { return m_str.c_str();}
	//	virtual pwstr getBuffer(){ return &m_str[0];}
	//	virtual uint getLength() const{ return (uint)m_str.length();}
	//	virtual void vformat(pcwstr Format, va_list marker)
	//	{
	//		m_str.resize(_vsnwprintf(NULL, 0, Format, marker) + 1);
	//		vswprintf_s(&m_str[0], m_str.size() + 1, Format, marker);
	//		m_str.resize(m_str.size() - 1);
	//	}
	//};

	//extern "C" ONEU_BASE_API IString* String_make(pcwstr str)
	//{
	//	return ONEU_NEW(String_Imp(str));
	//}
	//extern "C" ONEU_BASE_API IString* String_create()
	//{
	//	return ONEU_NEW(String_Imp());
	//}

	//×ª»»
	extern "C" ONEU_BASE_API wchar* _Char2Wide(pcstr cstr)
	{
		if(!cstr) return NULL;
		const size_t l = strlen(cstr);
		wchar* str = (wchar*)ONEU_ALLOC(sizeof(wchar) * (l + 2));
		if(!MultiByteToWideChar(CP_ACP, 0, cstr, int(l + 1), str, int(l + 2)))
		{
			//throw Exception(L"can't translate char to unicode");
		}
		return str;
	}
	extern "C" ONEU_BASE_API char* _Wide2Char(pcwstr cstr)
	{
		if(!cstr) return NULL;
		const size_t l = wcslen(cstr);
		char* str = (char*)ONEU_ALLOC(sizeof(char) * (l + 2));
		if(!WideCharToMultiByte(CP_ACP, 0, cstr, int(l + 1), str, int(l + 2), NULL, NULL))
		{
			//throw Exception(L"can't translate char to ansi");
		}
		return str;
	}
}