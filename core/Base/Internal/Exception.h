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
 * @file Exception.h
 * @brief 异常
 *
 * 已废弃。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */

#pragma once

#include "../OneUPreDef.h"
#include "../Error.h"
#include "../String.h"

namespace OneU
{
	class ONEU_BASE_API Exception  //一般错误
	{
	public:
		Exception(wchar_t * name = L"Error", bool MakeDump=true);
		virtual ~Exception(){}

		virtual pcwstr GetType() const;
		virtual pcwstr GetString() const;
	};

	extern "C" ONEU_BASE_API int PreThrow(const char * FileName, const int Line, Exception&);
	template<class ExpType>
	void _ThrowExp(const char * FileName, const int Line, ExpType& Exp){
		if(!PreThrow(FileName, Line, Exp)){
#ifdef _MSC_VER
			__asm{ int 3 }
#elif defined(__GNUC__)
			__asm("int 3");
#endif
		}
		else throw Exp;
	}

	//Exp 异常对象
	//因为涉及到dll 不能正常使用
#define ONEU_THROW(Exp) OneU::_ThrowExp(__FILE__, __LINE__, Exp)

	//deprecated
	//输入曾使用抛出异常来处理的遗留问题
	inline void _TerminateApp(const char * FileName, const int Line, Exception& Exp){
		_TerminateApp(FileName, Line, String().format(L"%s:%s", Exp.GetType(), Exp.GetString()).c_str());
	}

}
