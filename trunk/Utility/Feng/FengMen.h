/** 
 * @file FengMen.h
 * @brief 与Men的连接
 *
 * 提供一些基本图形变换元的支持
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-09
 */
#ifndef __FENGMEN_H
#define __FENGMEN_H

#include "FengDef.h"
#include "Polygon.h"
#include "..\Men\Men.h"

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @name XY引用打包
	 *
	 * 为了提高代码复用性而产生的内部代码
	 * @internal
	 * @{
	 */
	/* ----------------------------------------------------------------------------*/
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief XY引用打包器
	 */
	/* ----------------------------------------------------------------------------*/
	struct XYMakePair
	{
		float &x, &y;
		XYMakePair( float &x, float &y )
			: x( x ), y( y )
		{}
	};
	inline void operator<<( XYMakePair &lhs, const Translate2 &rhs )
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
	}
	inline void operator<<( XYMakePair &lhs, const Rotate2c &rhs )
	{
		float x = ( lhs.x - rhs.cx ) * cos( -rhs.val ) + ( lhs.y - rhs.cy ) * sin( -rhs.val ) + rhs.cx;
		float y = ( lhs.y - rhs.cy ) * cos( -rhs.val ) - ( lhs.x - rhs.cx ) * sin( -rhs.val ) + rhs.cy;
		lhs.x = x; lhs.y = y;
	}
	inline void operator<<( XYMakePair &lhs, const Scale2c &rhs )
	{
		lhs.x = ( lhs.x - rhs.cx ) * rhs.valx + rhs.cx;
		lhs.y = ( lhs.y - rhs.cy ) * rhs.valy + rhs.cy;
	}
	inline void operator<<( XYMakePair &lhs, const MATRIX &rhs )
	{
		lhs.x = lhs.x * rhs._11 + lhs.y * rhs._21 + rhs._41;
		lhs.y = lhs.x * rhs._12 + lhs.y * rhs._22 + rhs._42;
	}
	/** @}*/
	/* ----------------------------------------------------------------------------*/

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @name Feng对Men组件的支持
	 *
	 * Feng的一些基本图形对流运算的支持。
	 * 右操作符可以为Translate2，Rotate2c，Scale2c
	 * ( Rotate2, Scale2可分别强制转换成Rotate2c,Scale2c )
	 *  
	 * @{
	 */
	/* ----------------------------------------------------------------------------*/
	inline CPoint2& operator << ( CPoint2 &lhs, const Translate2 &rhs )
	{
		XYMakePair( lhs.x, lhs.y ) << rhs;
		return lhs;
	}
	inline CPoint2& operator << ( CPoint2 &lhs, const Rotate2c &rhs )
	{
		XYMakePair( lhs.x, lhs.y ) << rhs;
		return lhs;
	}
	inline CPoint2& operator << ( CPoint2 &lhs, const Scale2c &rhs )
	{
		XYMakePair( lhs.x, lhs.y ) << rhs;
		return lhs;
	}
	inline CPoint2& operator << ( CPoint2 &lhs, const MATRIX &rhs )
	{
		XYMakePair( lhs.x, lhs.y ) << rhs;
		return lhs;
	}
	inline CLine2& operator << (CLine2& lhs, const Translate2& rhs)
	{
		XYMakePair(lhs.p1.x, lhs.p1.y) << rhs;
		XYMakePair(lhs.p2.x, lhs.p2.y) << rhs;
		return lhs;
	}
	inline CLine2& operator << (CLine2& lhs, const Rotate2c& rhs)
	{
		XYMakePair(lhs.p1.x, lhs.p1.y) << rhs;
		XYMakePair(lhs.p2.x, lhs.p2.y) << rhs;
		return lhs;
	}
	inline CLine2& operator << (CLine2& lhs, const Scale2c& rhs)
	{
		XYMakePair(lhs.p1.x, lhs.p1.y) << rhs;
		XYMakePair(lhs.p2.x, lhs.p2.y) << rhs;
		return lhs;
	}
	inline CLine2& operator << (CLine2& lhs, const MATRIX& rhs)
	{
		XYMakePair(lhs.p1.x, lhs.p1.y) << rhs;
		XYMakePair(lhs.p2.x, lhs.p2.y) << rhs;
		return lhs;
	}
	inline CRect& operator<<( CRect &lhs, const Translate2 &rhs )
	{
		XYMakePair( lhs.left, lhs.top ) << rhs;
		XYMakePair( lhs.right, lhs.bottom ) << rhs;
		return lhs;
	}

	inline CRect& operator<<( CRect &lhs, const Scale2c &rhs )
	{
		XYMakePair( lhs.left, lhs.top ) << rhs;
		XYMakePair( lhs.right, lhs.bottom ) << rhs;
	}


	template< class Contents >
	PolyLine_t< Contents >& operator<<( PolyLine_t< Contents > &lhs, const Translate2 &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	PolyLine_t< Contents >& operator<<( PolyLine_t< Contents > &lhs, const Scale2c &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	PolyLine_t< Contents >& operator<<( PolyLine_t< Contents > &lhs, const Rotate2c &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	PolyLine_t< Contents >& operator<<( PolyLine_t< Contents > &lhs, const MATRIX &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, template< class > class Impl >
	Polygon_t< Contents, Impl >& operator<<( Polygon_t< Contents, Impl > &lhs, const Translate2 &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, template< class > class Impl >
	Polygon_t< Contents, Impl >& operator<<( Polygon_t< Contents, Impl > &lhs, const Scale2c &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, template< class > class Impl >
	Polygon_t< Contents, Impl >& operator<<( Polygon_t< Contents, Impl > &lhs, const Rotate2c &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, template< class > class Impl >
	Polygon_t< Contents, Impl >& operator<<( Polygon_t< Contents, Impl > &lhs, const MATRIX &rhs )
	{
		for( uint i = 0; i < lhs.Size(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}
	/** @}*/
	/* ----------------------------------------------------------------------------*/

	//丰富VertexUP功能 位置保留
	//namespace DX
	//{
	//	template< dword FVF, class T >
	//	DX::VertexUP< FVF >& operator <<( DX::VertexUP< FVF > &lhs, const T &rhs )
	//	{
	//		XYMakePair( lhs.X(), lhs.Y() ) << rhs;
	//		return lhs;
	//	}
	//}
}//namespace OneU

#endif //ifndef __FENGMEN_H
