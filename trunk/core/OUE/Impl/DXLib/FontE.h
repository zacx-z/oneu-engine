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
#include "../../Base.h"

#ifdef __ONEU_USE_GE
#include "DXGraphics.h"
#include "SpriteE.h"
#include < d3dx9.h >

#define __ONEU_FONT_VERSION 0x100

namespace OneU
{
	namespace DX
	{
		class FontE
		{
		private:
			ID3DXFont *m_pFont;
		public:
			FontE()
				: m_pFont( NULL )
			{
				//保证构造函数的顺序
				GetGraphics();
			}
			~FontE()
			{
				Destroy();
			}
			void Create( uint32 Height, uint32 Width, uint32 Weight, uint32 MipLevels = 1, uint32 Quality = 0, pcwstr pFaceName = L"Arial" )
			{
				ONEU_ASSERT( m_pFont == NULL );
				XV( D3DXCreateFont( _pD3DDevice, Height, Width, Weight, MipLevels, FALSE, DEFAULT_CHARSET, 0, Quality, 0, pFaceName, &m_pFont ) );
			}
			void Destroy() 
			{
				SAFE_RELEASE( m_pFont );
			}
			int DrawText( pcwstr pString, RECT *pRect, COLOR crColor, uint32 Format = DT_TOP | DT_LEFT | DT_EXPANDTABS )
			{
				int ret = m_pFont->DrawText( NULL, pString, -1,  pRect, Format, crColor );
				if( !ret )
					ONEU_LOG( L"绘制文字失败！");
				return ret;
			}
			int DrawText( SpriteE* pSprite, pcwstr pString, RECT *pRect, COLOR crColor, uint32 Format = DT_TOP | DT_LEFT | DT_EXPANDTABS )
			{
				int ret = m_pFont->DrawText( pSprite->_Obtain(), pString, -1,  pRect, Format, crColor );
				if( !ret )
					ONEU_LOG( L"绘制文字失败！");
				return ret;
			}
			void OnLostDevice()
			{
				m_pFont->OnLostDevice();
			}
			void OnResetDevice()
			{
				m_pFont->OnResetDevice();
			}
		};
	}
}

#endif