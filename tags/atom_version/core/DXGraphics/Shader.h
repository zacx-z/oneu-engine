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
#ifdef __ONEU_USE_DIRECT3D
#include "D3DDefs.h"
#include <d3d9.h>
#include <d3dx9shader.h>

namespace OneU
{
	namespace DX
	{
		extern IDirect3DDevice9 * _pD3DDevice;
		class ONEU_DXGRAPHICS_API PixelShader
		{
			IDirect3DPixelShader9 *m_pShader;
			ID3DXBuffer *m_pShaderBuffer;
			ID3DXConstantTable * m_pConstTable;

			PixelShader( const PixelShader& );
			PixelShader& operator=( const PixelShader& );
		public:
			IDirect3DPixelShader9* _Obtain() const
			{
				return m_pShader;
			}			
			ID3DXConstantTable* _ObtainConstTable() const
			{
				return m_pConstTable;
			}
		public:
			void Destroy()
			{
				SAFE_RELEASE( m_pShader );
				SAFE_RELEASE( m_pShaderBuffer );
				SAFE_RELEASE( m_pConstTable );
			}
			PixelShader()
				: m_pShader( NULL ), m_pShaderBuffer( NULL ), m_pConstTable( NULL )
			{}
			~PixelShader()
			{
				Destroy();
			}
			void CompileFile( pcwstr pSrcFile, pcstr pFuncName );
			void OnLostDevice();
			void OnResetDevice();

			bool IsCreated() const
			{
				return m_pShader != 0;
			}

			void SetConstDefaults()
			{
				m_pConstTable->SetDefaults( _pD3DDevice );
			}

			static void SetShader( PixelShader * pShader )
			{
				if( pShader != NULL )
				{
					DXCHECK_DEBUG( _pD3DDevice->SetPixelShader( pShader->m_pShader ), L"设定像素着色器失败！" );
				}
				else
					_pD3DDevice->SetPixelShader( 0 );
			}
		};

		class ONEU_DXGRAPHICS_API VertexShader
		{
			IDirect3DVertexShader9 *m_pShader;
			ID3DXBuffer *m_pShaderBuffer;
			ID3DXConstantTable * m_pConstTable;

			VertexShader( const VertexShader& );
			VertexShader& operator=( const VertexShader& );
		public:
			IDirect3DVertexShader9* _Obtain() const
			{
				return m_pShader;
			}			
			ID3DXConstantTable* _ObtainConstTable() const
			{
				return m_pConstTable;
			}
		public:
			void Destroy()
			{
				SAFE_RELEASE( m_pShader );
				SAFE_RELEASE( m_pShaderBuffer );
				SAFE_RELEASE( m_pConstTable );
			}
			VertexShader()
				: m_pShader( NULL ), m_pShaderBuffer( NULL ), m_pConstTable( NULL )
			{}
			~VertexShader()
			{
				Destroy();
			}
			void CompileFile(pcwstr pSrcFile, pcstr pFuncName);
			void OnLostDevice();
			void OnResetDevice();

			bool IsCreated() const
			{
				return m_pShader != 0;
			}

			void SetConstDefaults()
			{
				m_pConstTable->SetDefaults(_pD3DDevice);
			}

			static void SetShader(VertexShader * pShader)
			{
				if( pShader != NULL )
				{
					DXCHECK_DEBUG( _pD3DDevice->SetVertexShader( pShader->m_pShader ), L"设定顶点着色器失败！" );
				}
				else
					_pD3DDevice->SetVertexShader( 0 );
			}
		};

		class ONEU_DXGRAPHICS_API ShaderConst
		{
			ID3DXConstantTable *m_pTable;
			D3DXHANDLE m_Const;
			ShaderConst( const ShaderConst& );
			ShaderConst& operator=( const ShaderConst& );
		public:
			ShaderConst(PixelShader &Shader, pcstr pName)
				: m_pTable( Shader._ObtainConstTable() ), m_Const( m_pTable->GetConstantByName( NULL, pName ) )
			{}
			ShaderConst(VertexShader &Shader, pcstr pName)
				: m_pTable( Shader._ObtainConstTable() ), m_Const( m_pTable->GetConstantByName( NULL, pName ) )
			{}
			template< typename T >
			void SetValue( T Value )
			{
				m_pTable->SetValue( _pD3DDevice, m_Const, &Value, sizeof( T ) );
			}
		};
	}
}
#endif