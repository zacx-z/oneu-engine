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

#include "D3DDefs.h"
#include "DXGraphics.h"

#include < dxerr.h >

namespace OneU
{
	namespace DX
	{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 表面基类型
		 *
		 * 把所有的DirectX调用封装到各个函数里 并为各种表面提供一个基类。
		 * 这些类没有多态性，接口不同仅仅是为了实现DirectX的编译期检查。
		 *
		 */
		/* ----------------------------------------------------------------------------*/
		class Surface
		{
		private:
		
			IDirect3DSurface9 * m_pSurface;
		protected:
			void _Create( uint32 nWidth, uint32 nHeight, PXLFORMAT Format, D3DPOOL Pool )
			{
				ONEU_ASSERT( m_pSurface == NULL );
				DXCHECK_RAISE( _pD3DDevice->CreateOffscreenPlainSurface( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ), Pool, &m_pSurface, NULL ),
					L"表面创建失败！" );
			}
			void _CreateRenderTarget( uint32 nWidth, uint32 nHeight, PXLFORMAT Format, uint32 MultiSample, uint32 MultisampleQuality, bool Lockable )
			{
				ONEU_ASSERT( m_pSurface == NULL );
				DXCHECK_RAISE(
					_pD3DDevice->CreateRenderTarget( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ),
					static_cast< D3DMULTISAMPLE_TYPE > ( MultiSample ), MultisampleQuality, Lockable, &m_pSurface, NULL ),
					L"渲染目标表面创建失败！" );
			}
			void _CreateDepthSurface( uint32 nWidth, uint32 nHeight, PXLFORMAT Format, uint32 MultiSample, uint32 MultisampleQuality, bool Discard )
			{
				ONEU_ASSERT( m_pSurface == NULL );
				DXCHECK_RAISE(
					_pD3DDevice->CreateDepthStencilSurface( nWidth, nHeight, static_cast< D3DFORMAT > ( Format ),
					static_cast< D3DMULTISAMPLE_TYPE > ( MultiSample ), MultisampleQuality, Discard, &m_pSurface, NULL ),
					L"深度表面创建失败！" );
			}
		public:
			typedef D3DSURFACE_DESC INFO;

			Surface()
				: m_pSurface( NULL )
			{
				//保证构造函数的顺序
				GetGraphics();
				//拷贝构造函数和赋值函数不需要 因为那样必有Surface构造Graphics必然构造
			}
			explicit Surface( IDirect3DSurface9 * pSurface )
				: m_pSurface( pSurface )
			{
			}
			Surface( Surface & rhs )
				: m_pSurface( rhs.m_pSurface )
			{
				rhs.m_pSurface = NULL;
			}
			Surface & operator = ( Surface & rhs )
			{
				ONEU_ASSERT( !m_pSurface );
					m_pSurface = rhs.m_pSurface;
				rhs.m_pSurface = NULL;
				return ( * this );
			}
			~Surface()
			{
				Destroy();
			}
		
			IDirect3DSurface9 * _Obtain() const { return m_pSurface; }
			D3DSURFACE_DESC & _GetDesc() const
			{
				static D3DSURFACE_DESC s_SD = {};
				DXCHECK( m_pSurface->GetDesc( &s_SD ), L"获取表面信息失败" );
					return s_SD;
			}

			void Create( uint32 nWidth, uint32 nHeight, PXLFORMAT Format )
			{
				_Create( nWidth, nHeight, Format, D3DPOOL_DEFAULT );
			}
			void Destroy()
			{
				SAFE_RELEASE( m_pSurface );
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 判断表面是否被创建
			 *
			 * @return true表面已创建，false表面未创建或已释放。
			 */
			/* ----------------------------------------------------------------------------*/
			bool IsCreated() const { return m_pSurface != 0; }
		
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取表面信息句柄
			 *
			 * 当要获取多个表面信息的时候，先获得此句柄，
			 * 再将此句柄传入获取信息函数会快一些。参见GetWidth，GetHeight。
			 * 
			 * @return 信息句柄
			 *
			 * @sa GetWidth GetHeight
			 */
			/* ----------------------------------------------------------------------------*/
			//表面信息句柄
			const INFO& GetInfo() const
			{
				return _GetDesc();
			}
		
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取宽度
			 *
			 * 获取表面的宽度，以像素计。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面宽度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			//获取表面信息
			uint32 GetWidth( const INFO& Info ) const
			{
				return Info.Width;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取宽度
			 *
			 * 获取表面的宽度，以像素计。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面宽度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetWidth() const
			{
				return GetWidth( GetInfo() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取高度
			 *
			 * 获取表面的高度，以像素计。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面高度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetHeight( const INFO& Info ) const
			{
				return Info.Height;
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取高度
			 *
			 * 获取表面的高度，以像素计。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面高度，以像素计。
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			uint32 GetHeight() const
			{
				return GetHeight( GetInfo() );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取像素格式
			 *
			 * 获取表面的像素格式。
			 * 
			 * @param Info 表面信息句柄
			 *
			 * 句柄由GetInfo成员函数获得。
			 * 
			 * @return 表面格式
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			PXLFORMAT GetFormat( const INFO& Info ) const
			{
				return static_cast< PXLFORMAT > ( Info.Format );
			}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 获取像素格式
			 *
			 * 获取表面的像素格式。
			 * 如果在一处使用多个获取信息函数的话。
			 * 请使用GetInfo。
			 * 
			 * @return 表面格式
			 *
			 * @sa GetInfo
			 */
			/* ----------------------------------------------------------------------------*/
			PXLFORMAT GetFormat() const
			{
				return GetFormat( GetInfo() );
			}
		
			//以下是会被继承类掩盖掉的函数
			//没有静态检查的DXAPI函数
			void UpdateSurface( POINT * pDtnPoint, const Surface * pSrcSurface, RECT * pSrcRect )
			{
				DXCHECK( _pD3DDevice->UpdateSurface( pSrcSurface->m_pSurface, pSrcRect, m_pSurface, pDtnPoint ),
					L"拷贝表面失败！" );
			}
			void StretchRect( RECT * pDtnRect, const Surface * pSrcSurface, RECT * pSrcRect )
			{
				DXCHECK( _pD3DDevice->StretchRect( pSrcSurface->m_pSurface, pSrcRect, m_pSurface, pDtnRect, _TexFilter ),
					L"（无放缩）拷贝表面失败！" );
			}
			void ColorFill( COLOR crColor, RECT * pRect = NULL )
			{
				HRESULT hr = _pD3DDevice->ColorFill( m_pSurface, pRect, crColor );
				if( FAILED( hr ) )
				{
					ONEU_LOG( L"填充表面失败！");
					ONEU_LOG_DXERR( hr );
				}
			}
			//RenderTarget Surface专用
			void GetData( Surface * pDestSurface )
			{
				DXCHECK( _pD3DDevice->GetRenderTargetData( m_pSurface, pDestSurface->_Obtain() ), L"获取渲染目标表面数据失败！" );
			}
		};

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
		class Texture
		{
		private:
			//禁用拷贝构造函数和赋值函数
			Texture( Texture & ){}
			Texture & operator=( Texture & ){ return ( * this ); }

			IDirect3DTexture9 * m_pTexture;

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
			void CreateFromSurface( const Surface& surface, uint32 Usage, D3DPOOL Pool )
			{
				Surface::INFO info = surface.GetInfo();
				_Create( surface.GetWidth( info ), surface.GetHeight( info ), surface.GetFormat( info ), 1, Usage, Pool );
				Surface(_GetSurfaceLevel( 0 )).UpdateSurface( NULL, &surface, NULL );
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
		public:
			typedef D3DSURFACE_DESC INFO;
			Texture()
				: m_pTexture( NULL )
			{
				//保证构造函数的顺序
				GetGraphics();
				//拷贝构造函数和赋值函数不需要 因为那样必有Texture构造Graphics必然构造
			}
			//@internal
			explicit Texture( IDirect3DTexture9* pTexture )
				: m_pTexture( pTexture )
			{
			}
			~Texture()
			{
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
			void UpdateTexture( const Texture * pSrcTexture )
			{
				DXCHECK( _pD3DDevice->UpdateTexture( pSrcTexture->_Obtain(), m_pTexture ), L"拷贝纹理失败" );
			}

		};
	}
}
