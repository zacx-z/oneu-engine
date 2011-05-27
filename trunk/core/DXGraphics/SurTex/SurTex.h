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
#include "Texture.h"
namespace OneU
{
	namespace DX
	{
		//一堆typedef 
		//__Surface_Texture请用Texture类里的SurfaceType
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 普通表面 
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Surface_t< D3DPOOL_DEFAULT > Surface;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 可锁定的表面
		 *
		 * 可被CPU锁定读写，但渲染较慢。
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Surface_t< D3DPOOL_SYSTEMMEM > Surface_Lk;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 被D3D默认管理的表面
		 *
		 * 可以被锁定，同时渲染起来较快。
		 * 只是锁定读写的时候需要比Surface_Lk多做一部分工作。
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Surface_t< D3DPOOL_MANAGED > Surface_Mg;
		//一堆typedef
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 普通纹理
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Texture_t< 0, D3DPOOL_DEFAULT > Texture;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 可被锁定的纹理
		 *
		 * 可被CPU锁定读写，但渲染较慢。
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Texture_t< 0, D3DPOOL_SYSTEMMEM > Texture_Lk;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 被D3D默认管理的纹理
		 *
		 * 可以被锁定，同时渲染起来较快。
		 * 只是锁定读写的时候需要比Texture_Lk多做一部分工作。
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Texture_t< 0, D3DPOOL_MANAGED > Texture_Mg;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 渲染目标纹理
		 *
		 * 纹理中的表面可用作渲染目标
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Texture_t< D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT > Texture_RT;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 动态纹理
		 *
		 * 需要硬件支持
		 * 具体需查阅文档D3DUSAGE_DYNAMIC
		 */
		/* ----------------------------------------------------------------------------*/
		typedef Texture_t< D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT > Texture_D;//需要硬件支持 有机会再查阅文档
	}

};
