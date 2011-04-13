/** 
 * @file Painter.h
 * @brief 便利绘制
 *
 * 为2D游戏提供简单的绘制函数。
 * 为了追求效率使得库比较复杂。不适合做DLL导出函数。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-16
 */
#pragma once

#include "../Utility.h"

#ifdef __ONEU_USE_GE
#include "..\..\DXGraphics\SpriteE.h"
#else
#pragma message( __FILE__":Feng Needs OneU GE Library!")
AnErrorOccured!!!
#endif
#include "FengDef.h"
//#ifdef __ONEU_USE_DIRECT3D
#include "..\..\DXGraphics\DXGraphics.h"
#include "..\..\DXGraphics\TStage.h"
#include "..\..\DXGraphics\VertexUP.h"
//namespace Lib = DX;
//#endif

namespace OneU
{
	namespace Feng
	{
		/**
		 * @defgroup FengPaint 纹理绘制模块
		 * @addtogroup Feng
		 */
		/*@{*/
		//标准绘制 该方法会经过矩阵变换
		//
		//两个函数
		//假定RenderState的LIGHT设为FALSE 即无光照明
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 便利标准绘制
		*
		* 使用三维顶点组绘制纹理。
		* 会经过几何流水线。
		* 函数里更改了TStage(0)的纹理 以及Graphics的FVF。
		* 注：如果要完全显示应设定LIGHTING为FALSE。CULLMODE为1或0。
		* 
		* @param rd 渲染结构体
		* @param Tex 待绘制的纹理
		* @param pDrawRect 绘制的矩形位置。
		* @param pSrcRect 源矩形 默认为单位矩形。
		*
		* @remarks
		* 该函数为便利函数，仅将绘制一个纹理的操作打包到了一起。可以在函数前面设定一些额外的参数（如TStage(0)或设定变换矩阵）来控制绘制过程）
		* @attention
		* 不可在有ScreenPainter的时候使用。\n
		* 不可在有ScreenDrawer的时候使用。
		*
		* 注：单位不以像素计，代表的是UV坐标。单位矩形表示整个页面。坐标大于1会Wrap。（效果取决于Wrap方式）
		*/
		/* ----------------------------------------------------------------------------*/
		template< class TextureType >
		void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, const CRect *pSrcRect );
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 便利标准绘制
		*
		* 使用三维顶点组绘制纹理。
		* 会经过几何流水线。
		* 函数里更改了TStage(0)的纹理 以及Graphics的FVF。
		* 注：如果要完全显示应设定LIGHTING为FALSE。CULLMODE为1或0。
		* 
		* @param rd 渲染结构体
		* @param Tex 待绘制的纹理
		* @param pDrawRect 绘制的矩形位置。
		* @param crColor 颜色效果 颜色的效果（叠加、调制等）与TStage(0)的参数相关。
		* @param pSrcRect 源矩形 默认为单位矩形。
		*
		* @remarks
		* 该函数为便利函数，仅将绘制一个纹理的操作打包到了一起。可以在函数前面设定一些额外的参数（如TStage(0)或设定变换矩阵）来控制绘制过程）
		* @attention
		* 不可在有ScreenPainter对象的时候使用。\n
		* 不可在有ScreenDrawer的时候使用。
		*
		* 注：单位不以像素计，代表的是UV坐标。单位矩形表示整个页面。坐标大于1会Wrap。（效果取决于Wrap方式）
		*/
		/* ----------------------------------------------------------------------------*/
		template< class TextureType >
		void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, COLOR crColor, const CRect *pSrcRect );
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 屏幕绘制类
		 *
		 * 定义此类的一个对象来在屏幕上绘制纹理。
		 * 如果使用了DX::SpriteE的库，则在此对象生存期范围内，所有关于设备的调用都会被影响。参考ID3DXSprite，以此为一条标准。
		 * 在此类被定义后的设备参数TStage(0)会改动为适合2D渲染的模式。如果在该对象生存期内改动参数，该类会接收。这可能导致渲染错误。
		 * 不可同时存在两个以上对象。
		 * 此类为便利类，因此可能会有些冗余操作。若追求效率，应直接使用SpriteE。
		 */
		/* ----------------------------------------------------------------------------*/
		class ONEU_UTIL_API ScreenPainter
		{
		private:
			DX::RenderManip &m_Rd;
			static int ClassNum;

			const MATRIX *_pLastMatrix;//设定过的上一个矩阵地址 用来通过比对 优化设定矩阵操作

			//友元函数
			//由于两个函数不可以在存在ScreenPainter对象的时候使用 因此定义为友元函数
			template< class TextureType >
			friend void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, const CRect *pSrcRect );
			template< class TextureType >
			friend void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, COLOR crColor, const CRect *pSrcRect );
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 构造函数
			 * 
			 * @param &rd 渲染结构体
			 */
			/* ----------------------------------------------------------------------------*/
			//实现决定了不能同时存在该类的两个实例
			ScreenPainter( DX::RenderManip &rd )
				: m_Rd( rd )
			{
				//预防意外的编程而作的断言
				ClassNum++;
				ASSERT( ClassNum == 1 );
				//
				DX::SpriteE::I.Begin();
			}
			~ScreenPainter()
			{
				DX::SpriteE::I.End();
				ClassNum--;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 绘制纹理
			 * 
			 * @param &Tex 纹理
			 * @param &Pos 绘制的屏幕位置
			 * @param pSrcRect 源矩形，以像素计。绘制纹理源矩形所包含的部分。若为NULL则绘制整个纹理。
			 * @param crColor 颜色效果。颜色的效果（叠加、调制等）与TStage(0)的参数相关。
			 */
			/* ----------------------------------------------------------------------------*/
			template< class TextureType >
			void Paint( const TextureType &Tex, const CVector2 &Pos, const RECT *pSrcRect = NULL, COLOR crColor = 0xffffffff )
			{
				if( _pLastMatrix != &matIMatrix ){ DX::SpriteE::I.SetTransform( &matIMatrix ); _pLastMatrix = &matIMatrix; }
				DX::SpriteE::I.Draw( &Tex, NULL, &CVector3( Pos.x, Pos.y, 0.0f ), pSrcRect, crColor );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 绘制纹理
			 * 
			 * @param &Tex 纹理
			 * @param &Pos 绘制的屏幕位置，以像素计
			 * @param &Center 以纹理为精灵，中心在纹理的位置。以像素计
			 * @param pSrcRect 源矩形，以像素计。绘制纹理源矩形所包含的部分。若为NULL则绘制整个纹理。
			 * @param crColor 颜色效果。颜色的效果（叠加、调制等）与TStage(0)的参数相关。
			 */
			/* ----------------------------------------------------------------------------*/
			template< class TextureType >
			void Paint( const TextureType &Tex, const CVector2 &Pos, const CVector2 &Center, const RECT *pSrcRect = NULL, COLOR crColor = 0xffffffff )
			{
				if( _pLastMatrix != &matIMatrix ){ DX::SpriteE::I.SetTransform( &matIMatrix ); _pLastMatrix = &matIMatrix; }
				DX::SpriteE::I.Draw( &Tex, &CVector3( Center.x, Center.y, 0.0f ), &CVector3( Pos.x, Pos.y, 0.0f ), pSrcRect, crColor );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 绘制纹理
			 * 
			 * @param &Tex 纹理
			 * @param &mat 纹理的变换矩阵。纹理会按照变换矩阵变换再绘制到屏幕上。
			 * @param pSrcRect 源矩形，以像素计。绘制纹理源矩形所包含的部分。若为NULL则绘制整个纹理。
			 * @param crColor 颜色效果
			 */
			/* ----------------------------------------------------------------------------*/
			template< class TextureType >
			void Paint( const TextureType &Tex, const MATRIX &mat, const RECT *pSrcRect = NULL, COLOR crColor = 0xffffffff );

			/*@name 简单绘制*/
			/*@{*/
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 设定变换矩阵
			 *
			 * Paint函数里都会自动设定变换矩阵。而该函数是与S_Paint配对使用的。
			 * 设定变换矩阵后调用S_Paint。可避免频繁设定变换矩阵所带来的效率影响。
			 * 较高级的使用方式。
			 *
			 * @param mat 矩阵
			 * @relates S_Paint
			 */
			/* ----------------------------------------------------------------------------*/
			void SetTransform( const MATRIX &mat )
			{
				DX::SpriteE::I.SetTransform( &mat );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 简单绘制
			 *
			 * 会受到SetTransform的影响。一般与SetTransform配对使用。
			 * 较高级的使用方式。
			 * 
			 * @param Tex 待绘制的纹理
			 * @param &Pos 绘制的屏幕位置，以像素计
			 * @param &Center 以纹理为精灵，中心在纹理的位置。以像素计
			 * @param pSrcRect 源矩形，以像素计。绘制纹理源矩形所包含的部分。若为NULL则绘制整个纹理。
			 * @param crColor 颜色效果
			 *
			 * 颜色的效果（叠加、调制等）与TStage(0)的参数相关。
			 *
			 * @relates SetTransform
			 */
			/* ----------------------------------------------------------------------------*/
			template< class TextureType >
			void S_Paint( const TextureType &Tex, const CVector2 &Pos, const CVector2 &Center, const RECT *pSrcRect = NULL, COLOR crColor = 0xffffffff )
			{
				DX::SpriteE::I.Draw( &Tex, &CVector3( Center.x, Center.y, 0.0f ), &CVector3( Pos.x, Pos.y, 0.0f ), pSrcRect, crColor );
			}
			/*@{*/
		};

		template< class TextureType >
		void ScreenPainter::Paint( const TextureType &Tex, const MATRIX &mat, const RECT *pSrcRect, COLOR crColor )
		{
			if( _pLastMatrix != &mat ){ DX::SpriteE::I.SetTransform( &mat ); _pLastMatrix = &mat; }
			DX::SpriteE::I.Draw( &Tex, NULL, NULL, pSrcRect, crColor );
		}

		namespace internal
		{
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 单位矩形
			 * 左右上下左边分别为0101
			 */
			/* ----------------------------------------------------------------------------*/
			extern CRect IRect;
		}
#ifdef __ONEU_USE_DIRECT3D
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 矩形化位置
		 *
		 * 以矩形逆时针初始化四个顶点的XY坐标。
		 * 默认为单位矩形。
		 * 
		 * @param v[] 顶点数组
		 * @param prc 矩形
		 */
		/* ----------------------------------------------------------------------------*/
		template< dword FVF >
		inline void Rectialize( DX::VertexUP< FVF > v[], const CRect *prc = &internal::IRect )
		{
			v[0].SetPos( prc->left,  prc->top );
			v[1].SetPos( prc->left,  prc->bottom );
			v[2].SetPos( prc->right, prc->bottom );
			v[3].SetPos( prc->right, prc->top );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 矩形化UV
		 *
		 * 以矩形逆时针初始化四个顶点的UV坐标。
		 * 默认为单位矩形。
		 * 
		 * @tparam index UV坐标层
		 * @param v[] 顶点数组
		 * @param prc 矩形
		 */
		/* ----------------------------------------------------------------------------*/
		template< int index, dword FVF >
		inline void Rectialize_UV( DX::VertexUP< FVF > v[], const CRect *prc = &internal::IRect )
		{
			v[0].SetUV< index >( prc->left,  prc->top );
			v[1].SetUV< index >( prc->left,  prc->bottom );
			v[2].SetUV< index >( prc->right, prc->bottom );
			v[3].SetUV< index >( prc->right, prc->top );
		}
		
		/* ----------------------------------------------------------------------------*/
		//标准绘制实现
		template< class TextureType >
		void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, const CRect *pSrcRect = &internal::IRect )
		{
			ASSERT( ScreenPainter::ClassNum == 0 );
			static DX::VertexUP< DX::FVF_XYZ | DX::FVF_TEX1 > v[4];
			Rectialize( v, pDrawRect );
			Rectialize_UV< 0 >( v, pSrcRect );
			DX::Graphics.SetFVF( v );
			DX::TStage( 0 ).SetTexture( Tex );
			rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, v, 2 );
		}
		template< class TextureType >
		void Paint( DX::RenderManip &rd, const TextureType &Tex, const CRect *pDrawRect, COLOR crColor, const CRect *pSrcRect = &internal::IRect )
		{
			ASSERT( ScreenPainter::ClassNum == 0 );
			static DX::VertexUP< DX::FVF_XYZ | DX::FVF_TEX1 | DX::FVF_DIFFUSE > v[4];
			Rectialize( v, pDrawRect );
			Rectialize_UV< 0 >( v, pSrcRect );
			v[0].Diffuse() = v[1].Diffuse() = v[2].Diffuse() = v[3].Diffuse() = crColor;
			DX::Graphics.SetFVF( v );
			DX::TStage( 0 ).SetTexture( Tex );
			rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, v, 2 );
		}
		/*@}*///FengPaint
#endif
	}//namespace Feng
}//namespace OneU
