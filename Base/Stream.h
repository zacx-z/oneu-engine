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
 * @file Stream.h
 * @brief 流接口及一些工具。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
//level 2
#include "OneUPreDef.h"
#include "String.h"

namespace OneU
{
	//interface of ostream
	class IOStream;
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 自定义流运算符操作符类型
	 *
	 * 当该变量被流运算符输出到某流时，会调用该函数指针所指向的函数，参数为输出流。
	 */
	/* ----------------------------------------------------------------------------*/
	typedef IOStream& (*StreamSymbol_t)(IOStream& Target);
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 输出流接口
	 * @attention 在继承类析构函数里必须调用flush。
	 */
	/* ----------------------------------------------------------------------------*/
	class IOStream
	{
	public:
		IOStream(){}
		virtual ~IOStream(){}
		virtual IOStream& operator <<(int) = 0;
		virtual IOStream& operator <<(float) = 0;
		virtual IOStream& operator <<(wchar) = 0;
		virtual IOStream& operator <<(char) = 0;
		virtual IOStream& operator <<(pcwstr) = 0;
		virtual IOStream& operator <<(pcstr) = 0;
		virtual IOStream& operator <<(void*) = 0;
		IOStream& operator <<(StreamSymbol_t v){return v(*this);}
		virtual void flush() = 0;
	};

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 输出流产生器
	 *
	 * 用户可继承此类，重写output函数和flush（可选）函数来实现一个IOStream。
	 */
	/* ----------------------------------------------------------------------------*/
	class IOStreamMaker
		: public IOStream
	{
	public:
		virtual void output(pcwstr data) = 0;

		//保证operator<<(StreamSymbol_t)不被掩盖
		using IOStream::operator <<;
		virtual IOStream& operator <<(int v){
			output(String().format(L"%d", v).c_str());
			return *this;
		}
		virtual IOStream& operator <<(float v){
			output(String().format(L"%f", v).c_str());
			return *this;
		}
		virtual IOStream& operator <<(wchar v){
			output(String().format(L"%c", v).c_str());
			return *this;
		}
		virtual IOStream& operator <<(char v){
			output(String().format(L"%C", v).c_str());
			return *this;
		}
		virtual IOStream& operator <<(pcwstr v){
			output(v);
			return *this;
		}
		virtual IOStream& operator <<(pcstr v){
			output(String().format(L"%S", v).c_str());
			return *this;
		}
		virtual IOStream& operator <<(void* v){
			output(String().format(L"%p", v).c_str());
			return *this;
		}
		virtual void flush(){}
	};

	//@addtogroup group_ostream_manipulator 预定义流操作符
	//@{
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 时间信息
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API IOStream& TimeInfo(IOStream& target);
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 换行符
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API IOStream& endl(IOStream& target);
	//@}
}
