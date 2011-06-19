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
//差Projection的一部分没有收录进来
#include "../../Base.h"
#include "D3DDefs.h"
#ifdef __ONEU_USE_GE
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#define __ONEUE_MATRIX_VERSION 100

namespace OneU
{

	inline MATRIX & MatrixIdentity( MATRIX & Out )
	{
		return *D3DXMatrixIdentity( reinterpret_cast< D3DXMATRIX * > ( &Out ) );
	}
	inline MATRIX & MatrixInverse( MATRIX & Out, const MATRIX & M )
	{
		return *D3DXMatrixInverse( reinterpret_cast< D3DXMATRIX * > ( &Out ), NULL,
			reinterpret_cast< const D3DXMATRIX * > ( &Out ) );
	}

	inline float MatrixDeterminant( const MATRIX & M )
	{
		return D3DXMatrixDeterminant( reinterpret_cast< const D3DXMATRIX * > ( & M ) );
	}
	inline MATRIX & MatrixMultiply( MATRIX & Out, const MATRIX & M1, const MATRIX & M2 )
	{
		return *D3DXMatrixMultiply( reinterpret_cast< D3DXMATRIX * > ( &Out ),
			reinterpret_cast< const D3DXMATRIX * > ( &M1 ),
			reinterpret_cast< const D3DXMATRIX * > ( &M2 ) );
	}
	inline MATRIX & MatrixTranspose( MATRIX & Out, const MATRIX & M )
	{
		return *D3DXMatrixTranspose( reinterpret_cast< D3DXMATRIX * > ( &Out ),
			reinterpret_cast< const D3DXMATRIX * > ( &M ) );
	}
	inline MATRIX & MatrixAffineTransformation( MATRIX & Out, float Scaling, const VECTOR3 * pRotationCenter, const VECTOR4 * pRotation, const VECTOR3 * pTranslation )
	{
		return *D3DXMatrixAffineTransformation( reinterpret_cast< D3DXMATRIX * > ( & Out ),
			Scaling,
			reinterpret_cast< const D3DXVECTOR3 * > ( pRotationCenter ),
			reinterpret_cast< const D3DXQUATERNION * > ( pRotation ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pTranslation ) );
	}

	inline MATRIX & MatrixTransformation( MATRIX & Out, const VECTOR3 *pScalingCenter,
		const VECTOR4 *pScalingRotation, const VECTOR3 *pScaling,
		const VECTOR3 *pRotationCenter, const VECTOR4 *pRotation,
		const D3DXVECTOR3 *pTranslation )
	{
		return *D3DXMatrixTransformation( reinterpret_cast< D3DXMATRIX * > ( &Out ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pScalingCenter ),
			reinterpret_cast< const D3DXQUATERNION * > ( pScalingRotation ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pScaling ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pRotationCenter ),
			reinterpret_cast< const D3DXQUATERNION * > ( pRotation ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pTranslation ) );
	}
	inline MATRIX & MatrixTransformation2D( MATRIX & Out, const VECTOR2 *pScalingCenter,
		float ScalingRotation, const VECTOR2 *pScaling,
		const VECTOR2 *pRotationCenter, float Rotation,
		const VECTOR2 *pTranslation )
	{
		return *D3DXMatrixTransformation2D( reinterpret_cast< D3DXMATRIX * > ( &Out ),
			reinterpret_cast< const D3DXVECTOR2 * > ( pScalingCenter ),
			ScalingRotation,
			reinterpret_cast< const D3DXVECTOR2 * > ( pScaling ),
			reinterpret_cast< const D3DXVECTOR2 * > ( pRotationCenter ),
			Rotation,
			reinterpret_cast< const D3DXVECTOR2 * > ( pTranslation ) );
	}
	inline MATRIX & MatrixPerspectiveFovLH( MATRIX & Out, float fovY, float Aspect, float zn, float zf )
	{
		return *D3DXMatrixPerspectiveFovLH( reinterpret_cast< D3DXMATRIX * > ( &Out ), fovY, Aspect, zn, zf );
	}
	inline MATRIX & MatrixPerspectiveFovRH( MATRIX & Out, float fovY, float Aspect, float zn, float zf )
	{
		return *D3DXMatrixPerspectiveFovRH( reinterpret_cast< D3DXMATRIX * > ( &Out ), fovY, Aspect, zn, zf );
	}
	inline MATRIX & MatrixOrthoOffCenterLH( MATRIX & Out, float l, float r, float b, float t, float zn, float zf )
	{
		return *D3DXMatrixOrthoOffCenterLH( reinterpret_cast< D3DXMATRIX * > ( &Out ), l, r, b, t, zn, zf );
	}
	inline MATRIX & MatrixTranslation( MATRIX & Out, float x, float y, float z )
	{
		return *D3DXMatrixTranslation( reinterpret_cast< D3DXMATRIX * > ( & Out ), x, y, z );
	}
	inline MATRIX & MatrixRotationAxis( MATRIX & Out, const VECTOR3 * pV, float Angle )
	{
		return *D3DXMatrixRotationAxis( reinterpret_cast< D3DXMATRIX * > ( & Out ),
			reinterpret_cast< const D3DXVECTOR3 * > ( pV ), Angle );
	}
#define defineMatrixRotation( Axis )\
	inline MATRIX & MatrixRotation##Axis( MATRIX & Out, float Angle )\
	{\
		return *D3DXMatrixRotation##Axis( reinterpret_cast< D3DXMATRIX * > ( & Out ), Angle );\
	}

	defineMatrixRotation( X )
	defineMatrixRotation( Y )
	defineMatrixRotation( Z )

#undef defineMatrixRotation

	inline MATRIX & MatrixRotationQuaternion( MATRIX & Out, const VECTOR4 & pQ )
	{
		return *D3DXMatrixRotationQuaternion( reinterpret_cast< D3DXMATRIX * > ( & Out ),
			reinterpret_cast< const D3DXQUATERNION * > ( & pQ ) );
	}
	inline MATRIX & MatrixRotationYawPitchRoll( MATRIX & Out, float Yaw, float Pitch, float Roll )
	{
		return *D3DXMatrixRotationYawPitchRoll( reinterpret_cast< D3DXMATRIX * > ( & Out ), Yaw, Pitch, Roll );
	}

	inline MATRIX & MatrixScaling( MATRIX & Out, float sx, float sy, float sz )
	{
		return *D3DXMatrixScaling( reinterpret_cast< D3DXMATRIX * > ( & Out ), sx, sy, sz );
	}
	inline MATRIX & MatrixShadow( MATRIX & Out, const VECTOR4 & Light, const PLANE & Plane )
	{
		return *D3DXMatrixShadow( reinterpret_cast< D3DXMATRIX * > ( & Out ),
			reinterpret_cast< const D3DXVECTOR4 * > ( & Light ),
			reinterpret_cast< const D3DXPLANE * > ( & Plane ) );
	}
	inline MATRIX & MatrixReflect( MATRIX & Out, const PLANE & Plane )
	{
		return *D3DXMatrixReflect( reinterpret_cast< D3DXMATRIX * > ( & Out ),
			reinterpret_cast< const D3DXPLANE * > ( & Plane ) );
	}

	VECTOR3& MatrixTransform( VECTOR3& Out, MATRIX &Mat, const VECTOR3 Vertex );
}
#endif