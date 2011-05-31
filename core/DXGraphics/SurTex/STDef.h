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
 * @file STDef.h
 * @brief 表面和纹理的预定义
 *
 * 与Surface.h Texture.h紧耦合
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-01-27
 *
 * @sa Surface.h
 * @sa Texture.h
 */
#pragma once

#include "..\D3DDefs.h"
#include "..\DXGraphics.h"

#include < dxerr.h >

namespace OneU
{
	namespace DX
	{
		/*@cond INTERNAL_MPL*/
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 离屏表面的Pool特性萃取
		 *
		 * @tparam Pool 内存池类型
		 *
		 * @internal
		 */
		/* ----------------------------------------------------------------------------*/
		template< D3DPOOL Pool >
		struct __Surface_Pool_Traits
		{
		};
		template<>
		struct __Surface_Pool_Traits< D3DPOOL_DEFAULT >
		{
			enum
			{
				bColorFill = 1,
				bUpdateSurfaceDestination = 1,
				bStretchRectDestination = 1
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
		};
		template<>
		struct __Surface_Pool_Traits< D3DPOOL_MANAGED >
		{
			enum
			{
				bColorFill = 0,
				bUpdateSurfaceDestination = 0,
				bStretchRectDestination = 0
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
		};
		template<>
		struct __Surface_Pool_Traits< D3DPOOL_SYSTEMMEM >
		{
			enum
			{
				bColorFill = 0,
				bUpdateSurfaceDestination = 0,
				bStretchRectDestination = 0
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
		};
		template<>
		struct __Surface_Pool_Traits< D3DPOOL_SCRATCH >
		{
			enum
			{
				bColorFill = 0,
				bUpdateSurfaceDestination = 0,
				bStretchRectDestination = 0
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
		};
		
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理的Usage、Pool特性萃取
		 *
		 * @tparam _Usage 使用方式类型
		 * @tparam _Pool 内存池类型
		 */
		/* ----------------------------------------------------------------------------*/
		template< uint32 _Usage, D3DPOOL _Pool >
		struct __Texture_UgPl_Traits
		{
		};
		template<>
		struct __Texture_UgPl_Traits< D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT >
		{
			enum
			{
				bColorFill = 1,
				bUpdateSurfaceDestination = 1,
				bStretchRectDestination = 1,
				bStretchRectSource = 1,
				bUpdateSurfaceSource = 0
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
			typedef __Bool_Category< bStretchRectSource >::Category StretchRectSource_Category;
			typedef __Bool_Category< bUpdateSurfaceSource >::Category UpdateSurfaceSource_Category;
		};
		template<>
		struct __Texture_UgPl_Traits< 0, D3DPOOL_DEFAULT >
		{
			enum
			{
				bColorFill = 0,
				bUpdateSurfaceDestination = 1,
				bStretchRectDestination = 0,
				bStretchRectSource = 0,
				bUpdateSurfaceSource = 0
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
			typedef __Bool_Category< bStretchRectSource >::Category StretchRectSource_Category;
			typedef __Bool_Category< bUpdateSurfaceSource >::Category UpdateSurfaceSource_Category;
		};
		template<>
		struct __Texture_UgPl_Traits< 0, D3DPOOL_SYSTEMMEM >
		{
			enum
			{
				bColorFill = 0,
				bUpdateSurfaceDestination = 0,
				bStretchRectDestination = 0,
				bStretchRectSource = 0,
				bUpdateSurfaceSource = 1
			};
			typedef __Bool_Category< bColorFill >::Category ColorFill_Category;
			typedef __Bool_Category< bUpdateSurfaceDestination >::Category UpdateSurfaceDestination_Category;
			typedef __Bool_Category< bStretchRectDestination >::Category StretchRectDestination_Category;
			typedef __Bool_Category< bStretchRectSource >::Category StretchRectSource_Category;
			typedef __Bool_Category< bUpdateSurfaceSource >::Category UpdateSurfaceSource_Category;
		};
		/*@endcond*///INTERNAL_MPL
		/* ----------------------------------------------------------------------------*/
	}
}
