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
#pragma once
#include "../Base/Base.h"
#include "../Base/Internal/DXDefs.h"

//#ifdef ONEU_DLL
#ifdef ONEU_EXPORTS
#define ONEU_DXGRAPHICS_API __declspec(dllexport)
#else
#define ONEU_DXGRAPHICS_API __declspec(dllimport)
#endif
//#else
//#define ONEU_DXGRAPHICS_API
//#endif

#include <d3d9types.h>
namespace OneU
{
	namespace DX
	{
		class Graphics_t;
		//模仿单态模式 目的是确定构造对象的顺序
		inline Graphics_t * GetGraphics();
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 全局唯一图形模块对象
		 */
		/* ----------------------------------------------------------------------------*/
		extern Graphics_t& Graphics;
	}

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 颜色
	 */
	/* ----------------------------------------------------------------------------*/
	typedef D3DCOLOR COLOR;
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 矩阵
	 */
	/* ----------------------------------------------------------------------------*/
	typedef D3DMATRIX MATRIX;
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维浮点向量
	 */
	/* ----------------------------------------------------------------------------*/
	struct VECTOR2
	{
		union
		{
			struct 
			{
				float _1, _2;
			};
			struct 
			{
				float x, y;
			};
		};
		VECTOR2( float a, float b )
			: _1( a ), _2( b ){}
		VECTOR2(){}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 三维浮点向量
	 */
	/* ----------------------------------------------------------------------------*/
	struct VECTOR3
	{
		union
		{
			struct
			{
				float _1, _2, _3;
			};
			struct
			{
				float x,y,z;
			};
		};
		VECTOR3( float a, float b, float c )
			: _1( a ), _2( b ), _3( c ){}
		VECTOR3(){}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 四维浮点向量
	 */
	/* ----------------------------------------------------------------------------*/
	typedef struct VECTOR4{
		union
		{
			struct 
			{
				float _1,_2,_3,_4;
			};
			struct
			{
				float x, y, z, w;
			};
			struct 
			{
				float left, top, right, bottom;
			};
		};

		VECTOR4( float a, float b, float c, float d )
			: _1( a ), _2( b ), _3( c ), _4( d ){}
		VECTOR4(){}
	}PLANE;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 单位矩阵
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_DXGRAPHICS_API extern const MATRIX matIMatrix;//单位矩阵

	namespace DX
	{
		//纹理过滤类型 用于StretchRect
		extern D3DTEXTUREFILTERTYPE _TexFilter;
	}
}

