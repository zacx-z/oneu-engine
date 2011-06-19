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

#include "D3DDefs.h"

#include <d3d9.h>

namespace OneU
{
	namespace DX
	{
		extern IDirect3DDevice9 *_pD3DDevice;
		class ONEU_DXGRAPHICS_API TStage
		{
			unsigned int index;
		public:
			TStage( unsigned i )
				: index( i )
			{
				ONEU_ASSERT( _pD3DDevice != 0 );
			}

			template< class T >
			void SetTexture( T &texture )
			{
				XV_DEBUG( _pD3DDevice->SetTexture( index, texture._Obtain() ) );
			}
			void DisableTexture()
			{
				XV_DEBUG( _pD3DDevice->SetTexture( index, NULL ) );
			}

			enum StateType
			{
				COLOR_OP             = D3DTSS_COLOROP,
				COLOR_ARG1           = D3DTSS_COLORARG1,
				COLOR_ARG2           = D3DTSS_COLORARG2,
				ALPHA_OP             = D3DTSS_ALPHAOP,
				ALPHA_ARG1           = D3DTSS_ALPHAARG1,
				ALPHA_ARG2           = D3DTSS_ALPHAARG2,
				CONSTANT             = D3DTSS_CONSTANT
			};
			enum ArgValue
			{
				ARG_CONSTANT         = D3DTA_CONSTANT,
				ARG_CURRENT          = D3DTA_CURRENT,
				ARG_DIFFUSE          = D3DTA_DIFFUSE,
				//ARG_SELECTMASK       = D3DTA_SELECTMASK,
				//ARG_SPECULAR         = D3DTA_SPECULAR,
				//ARG_TEMP             = D3DTA_TEMP,
				ARG_TEXTURE          = D3DTA_TEXTURE,
				//ARG_TFACTOR          = D3DTA_TFACTOR
			};
			enum OpValue
			{
				OP_ADD               = D3DTOP_ADD,
				OP_SUB               = D3DTOP_SUBTRACT,
				OP_MODULATE          = D3DTOP_MODULATE,
				OP_SELECTARG1        = D3DTOP_SELECTARG1,
				OP_SELECTARG2        = D3DTOP_SELECTARG2
			};
			void SetState( StateType StType, ArgValue Val )
			{
				XV_DEBUG( _pD3DDevice->SetTextureStageState( index, static_cast< D3DTEXTURESTAGESTATETYPE >( StType ), Val ) );
			}
			void SetState( StateType StType, OpValue Val )
			{
				XV_DEBUG( _pD3DDevice->SetTextureStageState( index, static_cast< D3DTEXTURESTAGESTATETYPE >( StType ), Val ) );
			}
		};
	}
}