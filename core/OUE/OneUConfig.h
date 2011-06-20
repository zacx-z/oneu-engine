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
 * @file OneUConfig.h
 * @brief 配置信息
 *
 * 通过定义宏来配置该工程的编译选项
 * @author Ladace
 * @version 1.0.0.1
 * @date 2009-11-11
 */

#pragma once

//#pragma warning (disable : 4996)
#pragma warning (disable : 4819)
#pragma warning (error : 4150)


#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#define ONEU_DEPRECATED __declspec(deprecated)

#ifdef _DEBUG
//使用断言
#define ONEU_USE_ASSERT
//使用边界检查
#define ONEU_USE_BOUND_CHECK
#endif


//使用Dump文件
#define ONEU_USE_DUMPFILE

//使用3D图形处理
#define ONEU_USE_GRAPHICS_3D
//使用Direct3D
#define ONEU_USE_DIRECT3D
//使用DirectInput
#define ONEU_USE_DIRECTINPUT


//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
