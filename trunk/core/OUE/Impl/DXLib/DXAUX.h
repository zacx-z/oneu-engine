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
#include <d3dx9.h>

namespace OneU
{
	namespace DX
	{
		extern uint32 s_Filter;
		extern uint32 s_MipFilter;

		inline void CreateTextureFromFile( pcwstr pSrcFile, Texture& txTexture,
			uint32 Level, PXLFORMAT Format, uint32 Usage, D3DPOOL Pool, COLOR crColorKey = 0 )
		{
			IDirect3DTexture9 * pITexture = 0;

			HRESULT hr;
			if( FAILED( hr = ::D3DXCreateTextureFromFileEx(_pD3DDevice, pSrcFile, 0, 0, Level, Usage, static_cast< D3DFORMAT > ( Format ), Pool,
				s_Filter, s_MipFilter, crColorKey, NULL, NULL,  & pITexture ) ) )
			{
				GetLogger().stream() << TimeInfo << L"从文件创建纹理失败！\n文件名：" << pSrcFile
					<< L"\nDX错误：" << DXERRGET_STR(hr) << L"\n";
				ONEU_RAISE(L"无法读取图片。\n路径：%s\nDX错误：%s", pSrcFile, DXERRGET_STR(hr));
			}

			txTexture._Create( pITexture );
		}
		inline void CreateTextureFromMemory(LPVOID pSrcData, uint32 SrcDataSize, Texture& txTexture,
			uint32 Level, PXLFORMAT Format, uint32 Usage, D3DPOOL Pool, COLOR crColorKey = 0)
		{
			IDirect3DTexture9 * pITexture = 0;

			DXCHECK_RAISE( ::D3DXCreateTextureFromFileInMemoryEx( _pD3DDevice, pSrcData, SrcDataSize, 0, 0, Level, Usage, static_cast< D3DFORMAT > ( Format ), Pool,
				s_Filter, s_MipFilter, crColorKey, NULL, NULL, & pITexture ),
				L"从内存创建纹理失败！" );

			txTexture._Create( pITexture );
		}
		inline void LoadSurfaceFromFile(pcwstr pFileName, Surface& Surface, RECT *pDestRect = 0, RECT* pSrcRect = 0, COLOR crColorKey = 0)
		{
			DXCHECK_RAISE( ::D3DXLoadSurfaceFromFile( Surface._Obtain(), NULL, pDestRect, pFileName, pSrcRect, s_Filter, crColorKey, NULL ),
				L"读取文件到表面失败！");
		}

		//D3DXFont
		inline ID3DXFont* CreateXFont( uint32 Height, uint32 Width, uint32 Weight, uint32 MipLevels = 1, uint32 Quality = 0, pcwstr pFaceName = L"Arial" )
		{
			ID3DXFont* pFont = NULL;
			XV( D3DXCreateFont( _pD3DDevice, Height, Width, Weight, MipLevels, FALSE, DEFAULT_CHARSET, 0, Quality, 0, pFaceName, &pFont ) );
			return pFont;
		}
		inline int XDrawText(ID3DXFont* pFont, ID3DXSprite* pSprite, pcwstr pString, RECT* pRect, COLOR crColor, uint32 Format = DT_TOP | DT_LEFT | DT_EXPANDTABS )
		{
			int ret = pFont->DrawText(pSprite, pString, -1,  pRect, Format, crColor);
			if( !ret )
				ONEU_LOG( L"绘制文字失败！");
			return ret;
		}

		//D3DXSprite
		inline ID3DXSprite* CreateXSprite()
		{
			ID3DXSprite* pSprite = NULL;
			XV(D3DXCreateSprite(_pD3DDevice, &pSprite));
			return pSprite;
		}

		inline void XSpriteBegin(ID3DXSprite* pSprite, uint32 Flags = D3DXSPRITE_ALPHABLEND)/*注:此函数需要调用在渲染结构体Graphics::Render?创建的子程序中 */
		{
			XV(pSprite->Begin( Flags ));
		}
		inline void XSpriteEnd(ID3DXSprite* pSprite)
		{
			XV_DEBUG(pSprite->End());
		}
		inline void XSpriteDraw(ID3DXSprite* pSprite, const Texture* _Texture, const VECTOR3* pCenter, const VECTOR3* pPosition, const RECT* pSrcRect, const COLOR crColor = 0xffffffff)
		{
			XV_DEBUG(pSprite->Draw( _Texture->_Obtain(), pSrcRect,
				reinterpret_cast< const D3DXVECTOR3* > (pCenter),
				reinterpret_cast< const D3DXVECTOR3* > (pPosition), crColor));
		}
		inline void XSpriteSetTransform(ID3DXSprite* pSprite, const MATRIX* Transform)
		{
			XV_DEBUG(pSprite->SetTransform(reinterpret_cast< const D3DXMATRIX * > (Transform)));
		}
		inline MATRIX* XSpriteGetTransform(ID3DXSprite* pSprite, MATRIX* Transform)
		{
			XV(pSprite->GetTransform( reinterpret_cast< D3DXMATRIX * > (Transform)));
			return Transform;
		}
	}
}