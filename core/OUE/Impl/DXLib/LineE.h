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

#include <d3dx9.h>
//#include "D3DDefs.h"

#define __ONEUE_LINE_VERSION 100

namespace OneU
{
	namespace DX
	{
		extern IDirect3DDevice9 *_pD3DDevice;
		class ONEU_DXGRAPHICS_API LineE
		{
		private:
			ID3DXLine* m_pLine;
			LineE( const LineE& );
			LineE& operator=( const LineE& );
		public:
			static LineE I;
			LineE()
				: m_pLine( NULL )
			{
				//保证构造函数的顺序
				GetGraphics();
			}
			~LineE()
			{
				Destroy();
			}

			void Create()
			{
				ONEU_ASSERT( m_pLine == NULL );
				XV( D3DXCreateLine( _pD3DDevice, &m_pLine ) );
			}
			void Destroy() 
			{
				SAFE_RELEASE( m_pLine );
			}

			void OnLostDevice()
			{
				m_pLine->OnLostDevice();
			}
			void OnResetDevice()
			{
				m_pLine->OnResetDevice();
			}
			void Begin()
			{
				XV( m_pLine->Begin() );
			}
			void End()
			{
				XV_DEBUG( m_pLine->End() );
			}
			void Draw( const VECTOR2 *pVertex, uint32 Count, COLOR crColor )//画线
			{
				XV_DEBUG( m_pLine->Draw( reinterpret_cast< const D3DXVECTOR2* >( pVertex ), Count, crColor ) );
			}
			void SetWidth( float fWidth )
			{
				m_pLine->SetWidth( fWidth );
			}
			float GetWidth()
			{
				return m_pLine->GetWidth();
			}
		};
	}
}
#endif