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
#include "../Base/Base.h"
#include "Shader.h"
#ifdef __ONEU_USE_DIRECT3D
#include <d3dx9.h>
namespace OneU
{
	namespace DX
	{
		void PixelShader::CompileFile( pcwstr pSrcFile, pcstr pFuncName )
		{
			ASSERT( m_pShader == 0 );
			HRESULT hr;
			ID3DXBuffer *pError = 0;
			if( FAILED( hr = D3DXCompileShaderFromFile( pSrcFile, NULL, NULL, pFuncName, "ps_2_0", 0, &m_pShaderBuffer, &pError, &m_pConstTable ) ) )
			{
				ONEU_LOG( L"编译像素着色器失败！" );
				ONEU_LOG( ( char* ) pError->GetBufferPointer() );
				THROW_HRESULT(hr);
			}

			XV_THROW( _pD3DDevice->CreatePixelShader( ( dword* )m_pShaderBuffer->GetBufferPointer(), &m_pShader ) );
		}

		//不知丢失设备时需不需要Release
		void PixelShader::OnLostDevice()
		{
			SAFE_RELEASE( m_pShader );
		}

		//不知丢失设备时是否需要这种操作
		void PixelShader::OnResetDevice()
		{
			ASSERT( m_pShader == 0 );
			XV_THROW( _pD3DDevice->CreatePixelShader( ( dword* )m_pShaderBuffer->GetBufferPointer(), &m_pShader ) );
		}

		//分割线------------------------------------------------------------------------
		void VertexShader::CompileFile( pcwstr pSrcFile, pcstr pFuncName )
		{
			ASSERT( m_pShader == 0 );
			HRESULT hr;
			ID3DXBuffer *pError = 0;
			if( FAILED( hr = D3DXCompileShaderFromFile( pSrcFile, NULL, NULL, pFuncName, "vs_2_0", 0, &m_pShaderBuffer, &pError, &m_pConstTable ) ) )
			{
				ONEU_LOG( L"编译顶点着色器失败！" );
				ONEU_LOG( ( char* ) pError->GetBufferPointer() );
				THROW_HRESULT(hr);
			}

			XV_THROW( _pD3DDevice->CreateVertexShader( ( dword* )m_pShaderBuffer->GetBufferPointer(), &m_pShader ) );
		}

		//不知丢失设备时需不需要Release
		void VertexShader::OnLostDevice()
		{
			SAFE_RELEASE( m_pShader );
		}

		//不知丢失设备时是否需要这种操作
		void VertexShader::OnResetDevice()
		{
			ASSERT( m_pShader == 0 );
			XV_THROW( _pD3DDevice->CreateVertexShader( ( dword* )m_pShaderBuffer->GetBufferPointer(), &m_pShader ) );
		}
	}
}
#endif