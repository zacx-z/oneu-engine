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
 * @file DXDefs.h
 * @brief DX基础定义
 * 
 * 定义一些操作DX所需的基本。在DXGraphics和DXInput之下，由于无处可放放到了底层。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-10
 */
#pragma once

#include <windows.h>
#include <dxerr.h>

#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxerr9.lib" )

#define SAFE_RELEASE( ptr ) OneU::DX::SafeRelease( ptr )

#define DXERRGET_STR(hr) ::DXGetErrorDescription(hr)
#define ONEU_LOG_DXERR(hr) ONEU_LOG(::DXGetErrorDescription(hr))

namespace OneU
{
	namespace DX
	{
		template < class T >
		inline void SafeRelease( T *& ptr ){
			if( ptr ){
				ptr->Release();
				ptr = NULL;
			}
		}
	}
}
#define RAISE_HRESULT(hr) ONEU_RAISE(::DXGetErrorDescription(hr))
//检查DX的HRESULT返回值 若有错误会记录到日志 程序继续运行
#define DXCHECK( fn, str ) {\
	HRESULT hr;\
	if( FAILED( hr = fn ) ) {\
	GetLogger().stream(ILogger::ML_CRITICAL) << __FILE__ << ':' << __LINE__ << '\n' << str << '\n' << ::DXGetErrorDescription( hr );\
	}\
}void( 0 )/*@记录格式不好看 待重构*/

//检查DX的HRESULT返回值 若有错误会记录到日志 程序抛出错误
#define DXCHECK_RAISE( fn, str ) {\
	HRESULT hr;\
	if( FAILED( hr = fn ) ){\
		ONEU_LOG( str );\
		RAISE_HRESULT(hr);\
}\
}void( 0 )
#define XV( fn ) DXCHECK( fn, #fn )
#define XV_RAISE( fn ) DXCHECK_RAISE( fn, #fn );
#ifdef _DEBUG
//仅在调试版本检查返回值 发布版本不会检查
#define DXCHECK_DEBUG( fn, str ) DXCHECK( fn, str )

//仅在调试版本检查返回值 发布版本不会检查
#define XV_DEBUG( fn ) DXCHECK_DEBUG( fn, #fn )
#else
#define DXCHECK_DEBUG( fn, str ) fn;
#define XV_DEBUG( fn ) fn;
#endif
namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/*@cond INTERNAL_MPL*/
	/*@todo 位置不太对*/
	/* ----------------------------------------------------------------------------*/
	//分别表示TRUE FALSE
	struct __True_Category{};
	struct __False_Category{};

	template< bool boolean >
	struct __Bool_Category
	{
	};
	template<>
	struct __Bool_Category< true >
	{
		typedef __True_Category Category;
	};
	template<>
	struct __Bool_Category< false >
	{
		typedef __False_Category Category;
	};
	/* ----------------------------------------------------------------------------*/
	/*@endcond*///INTERNAL_MPL
}
