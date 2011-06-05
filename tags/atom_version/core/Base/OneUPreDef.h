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
 * @file OneUPreDef.h
 * @brief 最底层的声明
 *
 * 包括类型声明和宏声明
 * @author Ladace
 * @version 1.0.0.1
 * @date 2009-11-11
 */
#pragma once

#include "OneUConfig.h"
#include <stddef.h>

#define ONEU_VERSION "OneU 1001"

namespace OneU
{
	typedef unsigned long long uint64;
	typedef unsigned long uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef long long int64;
	typedef long int32;
	typedef short int16;
	typedef char int8;

	typedef wchar_t wchar;
	typedef wchar* pwstr;
	typedef const wchar* pcwstr;
	typedef char* pstr;
	typedef const char* pcstr;
	typedef char byte;
	typedef unsigned char ubyte;
}


//assert作为调试手段 在正式程序用错误处理
#ifdef __ONEU_USE_ASSERT
#include <cassert>
/* ----------------------------------------------------------------------------*/
/** 
* @brief 断言
*
* 表达式的值为false就弹出对话框
* 
* @param exp 表达式的值
* @sa EF_ASSERT
*/
/* ----------------------------------------------------------------------------*/
#define ONEU_ASSERT(exp) assert(exp)

template < bool expression, class T >
class Static_Assert
{
public:
	Static_Assert(){}
};

template< class T >
class Static_Assert< false, T >
{
private:
	Static_Assert(){}
};
/* ----------------------------------------------------------------------------*/
/** 
* @brief 静态断言
*
* 表达式的值为false就会编译错误
* 
* @param exp 表达式的值
* @param str 错误名
* @sa EF_ASSERT
*/
/* ----------------------------------------------------------------------------*/
#define STATIC_ASSERT( exp, str )\
clasn Error_##str{};\
	Static_Assert< ( exp ), Error_##str > Assert##str;
#else
#define	ONEU_ASSERT(exp) void( 0 )
#endif

//template<class T>
//inline void SafeDelete(T* ptr)
//{
//	if( ptr )
//	{
//		delete ptr;
//		ptr = NULL;
//	}
//}
//template<class T>
//inline void SafeArrayDelete(T* ptr)
//{
//	if( ptr )
//	{
//		delete [] ptr;
//		ptr = NULL;
//	}
//}

//删除或修正
//#define SAFE_DELETE( ptr ) SafeDelete( ptr );
//#define SAFE_ARRAY_DELETE( ptr ) SafeArrayDelete( ptr );

//关于导出的几点说明
//模板类不能导出，但源码在头文件中，大部分情况下能正常使用。只需要注意：
//不能在模板函数（或模板类的函数）中有静态的变量或模板类含有静态成员。
#ifdef _MSC_VER
#ifdef ONEU_EXPORTS
#define ONEU_BASE_API __declspec(dllexport)
#else
#define ONEU_BASE_API __declspec(dllimport)
#endif
#elif defined __GNUC__
#define ONEU_BASE_API
#endif

#include "Memory.h"
