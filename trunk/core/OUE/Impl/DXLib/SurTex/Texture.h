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
* @file Texture.h
* @brief 表面和纹理
* 目前只有基本表面和纹理类型
* @author Ladace
* @version 1.0.0.1
* @date 2010-07-09
*/
//CreateFromSurface属于非正常函数
//这是D3D所没有的功能 是用UpdateSurface来实现
//而UpdateSurface会给添加脏矩形，无法去除。而且这也违反了UpdateSurface函数的本意。
#pragma once

#include "Surface.h"

namespace OneU
{
	namespace DX
	{

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理基类
		 *
		 * 把所有的DirectX调用封装到各个函数里 并为各种表面提供一个基类。
		 * 这些类没有多态性，接口不同仅仅是为了实现DirectX的编译期检查。
		 * 在2D程序中，因只会使用IDirect3DTexture9，所以构架如此。
		 * 若扩展到3D，则需重构。
		 */
		/* ----------------------------------------------------------------------------*/
		class ONEU_DXGRAPHICS_API Texture_Base
		{
		private:
			//禁用拷贝构造函数和赋值函数
			Texture_Base( Texture_Base & ){}
			Texture_Base & operator=( Texture_Base & ){ return ( * this ); }

			IDirect3DTexture9 * m_pTexture;

#ifdef __ONEU_USE_CLASS_NUM
			static int s_Num;
			static void IncClassNum(){ s_Num++; }
			static void DecClassNum(){ s_Num--; }
		public:
			static int GetClassNum(){ return s_Num;  }
#endif

		protected:
			//参数
			void _Create( uint32 nWidth, uint32 nHeight, PXLFORMAT Format, uint32 Level, uint32 Usage, D3DPOOL Pool )
			{
				ONEU_ASSERT( m_pTexture == NULL );
				DXCHECK_RAISE(
					_pD3DDevice->CreateTexture( nWidth, nHeight, Level, Usage, static_cast< D3DFORMAT >( Format ), Pool, &m_pTexture, NULL ),
					L"纹理创建失败！"
					);
			}
			
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 从表面创建纹理，该方法是为了渲染表面而使用。
			 *
			 * 只能创建Level为1的纹理
			 * 从表面创建的纹理非正常用途
			 * 因此Level为1 仅为了让表面可渲染
			 *
			 * 实现上内部调用了UpdateSurface，而这让纹理添加了一个脏矩形，使该纹理处于不正常的状态无法解决。
			 * 因此此实现是一个非正常实现。
			 *
			 * @param Surface 表面
			 * @param Usage 使用方式
			 * @param Pool 内存池
			 */
			/* ----------------------------------------------------------------------------*/
			void CreateFromSurface( const Surface_Base & Surface, uint32 Usage, D3DPOOL Pool )
			{
				Surface_Base::INFO info = Surface.GetInfo();
				_Create( Surface.GetWidth( info ), Surface.GetHeight( info ), Surface.GetFormat( info ), 1, Usage, Pool );
				Surface_Texture_Base(_GetSurfaceLevel( 0 )).UpdateSurface( NULL, &Surface, NULL );
			}

			IDirect3DSurface9* _GetSurfaceLevel( uint32 Level ) const
			{
				IDirect3DSurface9* pSurface;
				DXCHECK_RAISE(
					m_pTexture->GetSurfaceLevel( Level, &pSurface ),
					L"获取纹理表面失败！"
					);
				return pSurface;
			}
			//const IDirect3DSurface9* _GetSurfaceLevel( uint Level ) const
			//{
			//	IDirect3DSurface9* pSurface;
			//	DXCHECK_ST(
			//		m_pTexture->GetSurfaceLevel( Level, &pSurface ),
			//		L"获取纹理表面失败！"
			//		);
			//	return pSurface;
			//}
		public:
			typedef D3DSURFACE_DESC INFO;
			Texture_Base()
				: m_pTexture( NULL )
			{
#ifdef __ONEU_USE_CLASS_NUM
				IncClassNum();
#endif
				//保证构造函数的顺序
				GetGraphics();
				//拷贝构造函数和赋值函数不需要 因为那样必有Texture构造Graphics必然构造
			}
			//@internal
			explicit Texture_Base( IDirect3DTexture9* pTexture )
				: m_pTexture( pTexture )
			{
#ifdef __ONEU_USE_CLASS_NUM
				IncClassNum();
#endif
			}
			~Texture_Base()
			{
#ifdef __ONEU_USE_CLASS_NUM
				DecClassNum();
#endif
				Destroy();
			}
			IDirect3DTexture9 * _Obtain() const { return m_pTexture; }

			void _Create( IDirect3DTexture9 * pTexture )
			{
				ONEU_ASSERT( m_pTexture == NULL );
				m_pTexture = pTexture;
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 释放纹理
			 */
			/* ----------------------------------------------------------------------------*/
			void Destroy()
			{
				SAFE_RELEASE( m_pTexture );
			}

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 判断纹理是否被创建
			 *
			 * @return true纹理已创建，false纹理未创建或已释放。
			 */
			/* ----------------------------------------------------------------------------*/
			bool IsCreated() const { return m_pTexture != 0; }


			D3DSURFACE_DESC & _GetDesc( uint32 Level ) const
			{
				static D3DSURFACE_DESC s_SD = {};
				DXCHECK( m_pTexture->GetLevelDesc( Level, &s_SD ), L"获取纹理表面信息失败" );
				return s_SD;
			}

			INFO& GetInfo( uint32 Level ) const
			{
				return _GetDesc( Level );
			}
			uint32 GetWidth( const INFO& Info ) const
			{
				return Info.Width;
			}
			uint32 GetWidth( uint32 Level ) const
			{
				return GetWidth( GetInfo( Level ) );
			}
			uint32 GetHeight( const INFO& Info ) const
			{
				return Info.Height;
			}
			uint32 GetHeight( uint32 Level ) const
			{
				return GetHeight( GetInfo( Level ) );
			}
			PXLFORMAT GetFormat( const INFO& Info ) const
			{
				return static_cast< PXLFORMAT > ( Info.Format );
			}
			PXLFORMAT GetFormat( uint32 Level ) const
			{
				return GetFormat( GetInfo( Level ) );
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 更新纹理的脏矩形部分
			 *
			 * 脏矩形随着各种操作而被记录下来。更新纹理仅仅更新脏矩形部分。
			 * 属于DX内置功能。可参考DirectX手册UpdateTexture。
			 * 目前脏矩形的记录，待更新。
			 * UpdateSurface的目标会记录脏矩形。
			 * Lock没有使用READONLY会记录脏矩形。
			 * 
			 * @param pSrcTexture 源纹理
			 */
			/* ----------------------------------------------------------------------------*/
			void UpdateTexture( const Texture_Base * pSrcTexture )
			{
				DXCHECK( _pD3DDevice->UpdateTexture( pSrcTexture->_Obtain(), m_pTexture ), L"拷贝纹理失败" );
			}

		};


		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理类
		 *
		 * 带上编译器检查的纹理类
		 * @tparam _Usage 使用类型
		 * @tparam _Pool 内存池类型
		 */
		/* ----------------------------------------------------------------------------*/
		template< uint32 _Usage, D3DPOOL _Pool >
		class Texture_t
			: public Texture_Base
		{
		private:
			template< uint32 Usage, D3DPOOL Pool >
			void _UpdateTexture( Texture_t< Usage, Pool >* pSrcTexture, __True_Category )
			{
				Texture_Base::UpdateTexture( pSrcTexture );
			}
		public:
			enum { Usage = _Usage, Pool = _Pool };
			typedef Surface_Texture< _Usage, _Pool > SurfaceType;
			void Create( uint32 nWidth, uint32 nHeight, PXLFORMAT Format, uint32 Level = 1 )
			{
				_Create( nWidth, nHeight, Format, Level, _Usage, _Pool );
			}

			template< uint32 Usage, D3DPOOL Pool >
			void UpdateTexture( const Texture_t< Usage, Pool >* pSrcTexture )
			{
				_UpdateTexture( pSrcTexture, __Bool_Category< _Pool == D3DPOOL_DEFAULT && Pool == D3DPOOL_SYSTEMMEM >::Category );
			}
			//属非正常使用 该函数仅仅为了能让表面能够被渲染
			//因为函数中调用了UpdateSurface，而这在纹理中添加了一个脏矩形。			
			//CreateFromSurface2连发 在UpdateSurface中进行类型检查
			/*@name 从表面创建纹理 */
			/*@{ */
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 从表面创建纹理，该方法是为了渲染表面而使用。
			 *
			 * 只能创建Level为1的纹理
			 * 从表面创建的纹理非正常用途
			 * 因此Level为1 仅为了让表面可渲染
			 *
			 * 实现上内部调用了UpdateSurface，而这让纹理添加了一个脏矩形，使该纹理处于不正常的状态无法解决。
			 * 因此此实现是一个非正常实现。
			 *
			 * @param Surface 表面
			 */
			/* ----------------------------------------------------------------------------*/
			template< class T >
			void CreateFromSurface( const T & Surface )
			{
				typename T::INFO info = Surface.GetInfo();
				Create( Surface.GetWidth( info ), Surface.GetHeight( info ), Surface.GetFormat( info ), 1 );//如果Level不为1需拷贝很多次 所以为了防止潜在的效率影响（在用户不知情情况下） 这个函数Level只能为1
				GetSurfaceLevel( 0 ).UpdateSurface( NULL, &Surface, NULL );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 从表面创建纹理，该方法是为了渲染表面而使用。
			 *
			 * 只能创建Level为1的纹理
			 * 从表面创建的纹理非正常用途
			 * 因此Level为1 仅为了让表面可渲染
			 *
			 * 实现上内部调用了UpdateSurface，而这让纹理添加了一个脏矩形，使该纹理处于不正常的状态无法解决。
			 * 因此此实现是一个非正常实现。
			 *
			 * @param Surface 表面
			 */
			/* ----------------------------------------------------------------------------*/
			void CreateFromSurface( const Surface_Base & Surface )
			{
				Texture_Base::CreateFromSurface( Surface, _Usage, _Pool );
			}
			/** @} */


			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取纹理表面
			 * 
			 * @param Level 表面级数
			 *
			 * 如果Level超过纹理所含表面的最大级数，会抛出错误。
			 * 
			 * @return 纹理的第Level级表面
			 */
			/* ----------------------------------------------------------------------------*/
			SurfaceType GetSurfaceLevel( uint32 Level )
			{
				return Surface_Texture< _Usage, _Pool > ( _GetSurfaceLevel( Level ) );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取纹理表面
			 * 
			 * @param Level 表面级数
			 *
			 * 如果Level超过纹理所含表面的最大级数，会抛出错误。
			 * 
			 * @return 纹理的第Level级表面
			 */
			/* ----------------------------------------------------------------------------*/
			const SurfaceType GetSurfaceLevel( uint32 Level ) const
			{
				return Surface_Texture< _Usage, _Pool > (_GetSurfaceLevel( Level ));
			}
		};
	}
}
