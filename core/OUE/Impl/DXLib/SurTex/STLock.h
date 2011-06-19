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
 * @file STLock.h
 * @brief Graphics缓冲锁
 *
 * 包含表面锁、纹理锁。
 * @todo 可能在未来会扩充：深度缓冲锁 （长期）可能在其它头文件扩充：顶点缓冲锁 索引缓冲锁
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-23
 */
#pragma once
#include "SurTex.h"
#include "..\PixelFormat.h"

namespace OneU
{
	namespace DX
	{
		//锁标志 目前只提供两个
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 锁标志
		 *
		 * 表示锁的类型。被SurfaceLock和TextureLock使用。
		 * @sa SurfaceLock
		 * @sa TextureLock
		 */
		/* ----------------------------------------------------------------------------*/
		enum LK_FLAG
		{
			LK_DISCARD = D3DLOCK_DISCARD,
			LK_READONLY = D3DLOCK_READONLY
		};
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 表面锁
		 *
		 * @tparam Format 表面像素格式
		 */
		/* ----------------------------------------------------------------------------*/
		template< PXLFORMAT Format = PXLFMT_UNKNOWN >
		class SurfaceLock
		{
		private:
			D3DLOCKED_RECT m_DDLR;
			IDirect3DSurface9 * const m_pSurface;
			uint32 m_Width, m_Height;
		public:
			typedef PixelFormat< Format > PxlFmt;
			SurfaceLock( Surface_Base * pSurface, const RECT *pRect = NULL, LK_FLAG Flag = LK_DISCARD )
				: m_pSurface( pSurface->_Obtain() )
			{
				DXCHECK( m_pSurface->LockRect( &m_DDLR, pRect, Flag ), L"锁定表面失败！" );
				if( pRect )
				{
					m_Width = pRect->right - pRect->left;
					m_Height = pRect->bottom - pRect->top;
				}
				else
				{
					Surface_Base::INFO info = pSurface->GetInfo();
					m_Width = pSurface->GetWidth( info );
					m_Height = pSurface->GetHeight( info );
		
				}
			}

			//{从这开始与TextureLock部分相同
			uint32 GetLockWidth() const { return m_Width; }
			uint32 GetLockHeight() const { return m_Height; }

			void * GetBuffer() const
			{
				return m_DDLR.pBits;
			}
			int GetPitch() const
			{
				return m_DDLR.Pitch;
			}

			typename PxlFmt::Pixel SetPixel( uint32 x, uint32 y, COLOR crColor )
			{
				//未检查x y范围
				return *reinterpret_cast< typename PxlFmt::Pixel * > (
					( reinterpret_cast< char * >( GetBuffer() ) + y * GetPitch() + x * sizeof( PxlFmt::Pixel ) )
					) = PxlFmt::GetPixel( crColor );
			}
			COLOR GetPixel( uint32 x, uint32 y )
			{
				//未检查x y范围
				return PxlFmt::GetColor( *reinterpret_cast< PxlFmt::Pixel * > (
					( reinterpret_cast< char * >( GetBuffer() ) + y * GetPitch() + x * sizeof( PxlFmt::Pixel ) )
					) );
			}
			//解锁后长宽可以正常获取
			void Unlock()
			{
				DXCHECK( m_pSurface->UnlockRect(), L"解锁表面失败！" );
			}
			//到这结束}

			~SurfaceLock()
			{
				Unlock();
			}
		};
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 纹理锁
		 *
		 * @tparam Format 表面像素格式
		 */
		/* ----------------------------------------------------------------------------*/
		//只有标志为DISCARD的情形
		template< PXLFORMAT Format = PXLFMT_UNKNOWN >
		class TextureLock
		{
		private:
			D3DLOCKED_RECT m_DDLR;
			IDirect3DTexture9 * const m_pTexture;
			const uint32 m_Level;
			uint32 m_Width, m_Height;
		public:
			typedef PixelFormat< Format > PxlFmt;
			TextureLock( Texture_Base * pTexture, uint32 Level = 0, const RECT * pRect = NULL, LK_FLAG Flag = LK_DISCARD )
				: m_pTexture( pTexture->_Obtain() ), m_Level( Level )
			{
				DXCHECK_RAISE( m_pTexture->LockRect( Level, &m_DDLR, pRect, Flag ), L"锁定纹理失败！" );
				if( pRect )
				{
					m_Width = pRect->right - pRect->left;
					m_Height = pRect->bottom - pRect->top;
				}
				else
				{
					Texture_Base::INFO info = pTexture->GetInfo( Level );
					m_Width = pTexture->GetWidth( info );
					m_Height = pTexture->GetHeight( info );
				}
			}
			
			uint32 GetLockWidth() const { return m_Width; }
			uint32 GetLockHeight() const { return m_Height; }

			void * GetBuffer() const
			{
				return m_DDLR.pBits;
			}
			int GetPitch() const
			{
				return m_DDLR.Pitch;
			}

			typename PxlFmt::Pixel SetPixel( uint32 x, uint32 y, COLOR crColor )
			{
				//未检查x y范围
				return *reinterpret_cast< typename PxlFmt::Pixel * > (
					( reinterpret_cast< char * >( GetBuffer() ) + y * GetPitch() + x * sizeof( PxlFmt::Pixel ) )
					) = PxlFmt::GetPixel( crColor );
			}
			COLOR GetPixel( uint32 x, uint32 y )
			{
				//未检查x y范围
				return PxlFmt::GetColor( *reinterpret_cast< PxlFmt::Pixel * > (
					( reinterpret_cast< char * >( GetBuffer() ) + y * GetPitch() + x * sizeof( PxlFmt::Pixel ) )
					) );
			}
			//解锁后长宽可以正常获取
			void Unlock()
			{
				DXCHECK( m_pTexture->UnlockRect( m_Level ), L"解锁纹理失败！" );
			}
			~TextureLock()
			{
				Unlock();
			}
		};
	}
}
