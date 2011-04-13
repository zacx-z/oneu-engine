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
//¸¨Öú¿âcpp
#include "DXGraphics.h"
#include "SpriteE.h"
#include "LineE.h"

//Ê¹ÓÃ¸¨Öú¿â
#ifdef __ONEU_USE_GE

namespace OneU
{
	namespace DX
	{
#ifdef __ONEU_USE_DIRECT3D
		dword s_Filter = D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER;
		dword s_MipFilter = D3DX_FILTER_BOX;

#endif
		ONEU_DXGRAPHICS_API SpriteE SpriteE::I;
		ONEU_DXGRAPHICS_API LineE LineE::I;

		void InitializeOneUE()
		{
#ifdef __ONEU_USE_DIRECT3D
			SpriteE::I.Create();
			LineE::I.Create();
#endif
		}
		void DestroyOneUE()
		{
#ifdef __ONEU_USE_DIRECT3D
			SpriteE::I.Destroy();
			LineE::I.Destroy();
#endif
		}
		void OnLostDeviceOneUE()
		{
#ifdef __ONEU_USE_DIRECT3D
			SpriteE::I.OnLostDevice();
			LineE::I.OnLostDevice();
#endif
		}
		void OnResetDeviceOneUE()
		{
#ifdef __ONEU_USE_DIRECT3D
			SpriteE::I.OnResetDevice();
			LineE::I.OnResetDevice();
			//m_pFont->OnResetDevice();
#endif
		}
	}
	VECTOR3& MatrixTransform( VECTOR3& Out, MATRIX &Mat, const VECTOR3 Vertex )
	{
		Out.x = Mat._11 * Vertex.x + Mat._21 * Vertex.y + Mat._31 * Vertex.z + Mat._41;
		Out.y = Mat._12 * Vertex.x + Mat._22 * Vertex.y + Mat._32 * Vertex.z + Mat._42;
		Out.z = Mat._13 * Vertex.x + Mat._23 * Vertex.y + Mat._33 * Vertex.z + Mat._43;
		return Out;
	}
}


#endif
