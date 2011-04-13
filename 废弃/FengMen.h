#pragma once
//与Men的连接 
//用ScrMapper的话 支持使用Screen映射系统
#define _USE_MATH_DEFINES
//使用 MATH_PI
#include <cmath>

#include "FengDef.h"
#include "PaintDef.h"
#include "Polygon.h"
#include "Screen.h"
#include "..\Men\Men.h"

namespace OneU
{
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

	template< class Left, class Right >
	inline CRect& operator<<( CRect &lhs, const Men::Composition< Left, Right >& cps )
	{
		cps.Interpret( lhs );
		return lhs;
	}

	template< class Contents >
	LineBrk_t< Contents >& operator<<( LineBrk_t< Contents > &lhs, const Translate2 &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	LineBrk_t< Contents >& operator<<( LineBrk_t< Contents > &lhs, const Scale2c &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	LineBrk_t< Contents >& operator<<( LineBrk_t< Contents > &lhs, const Rotate2c &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, class Left, class Right >
	LineBrk_t< Contents >& operator<<( LineBrk_t< Contents > &lhs, const Men::Composition< Left, Right > &cps )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			cps.Interpret( XYMakePair( lhs.X( i ), lhs.Y( i ) ) );
		return lhs;
	}

	template< class Contents >
	Polygon_t< Contents >& operator<<( Polygon_t< Contents > &lhs, const Translate2 &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	Polygon_t< Contents >& operator<<( Polygon_t< Contents > &lhs, const Scale2c &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents >
	Polygon_t< Contents >& operator<<( Polygon_t< Contents > &lhs, const Rotate2c &rhs )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			XYMakePair( lhs.X( i ), lhs.Y( i ) ) << rhs;
		return lhs;
	}

	template< class Contents, class Left, class Right >
	Polygon_t< Contents >& operator<<( Polygon_t< Contents > &lhs, const Men::Composition< Left, Right > &cps )
	{
		for( UINT i = 0; i < lhs.GetN(); i++ )
			cps.Interpret( XYMakePair( lhs.X( i ), lhs.Y( i ) ) );
		return lhs;
	}

	namespace Feng
	{
		template< ALIGN alignment >
		struct ScrXYMakePair
		{
			floatX< alignment > &x;
			floatY< alignment > &y;
			ScrXYMakePair( floatX< alignment > &x, floatY< alignment > &y )
				: x( x ), y( y )
			{}
		};

		template< ALIGN alignment >
		inline void operator<<( ScrXYMakePair< alignment > &lhs, const Translate2 &rhs )
		{
			//坐标的两次映射隐含在里面
			lhs.x = float( lhs.x ) + rhs.x;
			lhs.y = float( lhs.y ) + rhs.y;
		}
		template< ALIGN alignment >
		inline void operator<<( ScrXYMakePair< alignment > &lhs, const Rotate2c &rhs )
		{
			//坐标的两次映射隐含在里面
			float x = ( float( lhs.x ) - rhs.cx ) * cos( -rhs.val )
				+ ( float( lhs.y ) - rhs.cy ) * sin( -rhs.val ) + rhs.cx;
			float y = ( float( lhs.y ) - rhs.cy ) * cos( -rhs.val ) - ( float( lhs.x ) - rhs.cx ) * sin( -rhs.val ) + rhs.cy;
			lhs.x = x; lhs.y = y;
		}
		template< ALIGN alignment >
		inline void operator<<( ScrXYMakePair< alignment > &lhs, const Scale2c &rhs )
		{
			//坐标的两次映射隐含在里面
			lhs.x = ( float( lhs.x ) - rhs.cx ) * rhs.valx + rhs.cx;
			lhs.y = ( float( lhs.y ) - rhs.cy ) * rhs.valy + rhs.cy;
		}
		
		template< class T, ALIGN alignment, class U >
		inline ScrMapper< T, alignment >& operator<<( ScrMapper< T, alignment > &lhs, const U &rhs )
		{
			for( UINT i = 0; i < lhs.GetContents().GetN(); i++ )
				ScrXYMakePair< alignment >( lhs.X( i ), lhs.Y( i ) ) << rhs;
			return lhs;
		}

		template< ALIGN alignment, class U >
		inline ScrMapper< CRect, alignment >& operator<<( ScrMapper< CRect, alignment > &lhs, const U &rhs )
		{
			ScrXYMakePair< alignment >( lhs.left, lhs.top ) << rhs;
			ScrXYMakePair< alignment >( lhs.right, lhs.bottom ) << rhs;
		}

		//========================================================================
		//我是分割线
		//========================================================================
		template< class T >
		inline TexPaintTag< T >& operator<<( TexPaintTag< T > &lhs, const Translate2 &rhs )
		{
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			//XYMakePair( lhs.x, lhs.y ) << rhs;
			return lhs;
		}
		template< class T >
		inline TexPaintTag< T >& operator<<( TexPaintTag< T > &lhs, const Rotate2 &rhs )
		{
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			//XYMakePair( lhs.x, lhs.y ) << rhs;
			//lhs.rot += rhs.val;
			return lhs;
		}
		template< class T >
		inline TexPaintTag< T >& operator<<( TexPaintTag< T > &lhs, const Scale2 &rhs )
		{
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			//XYMakePair( lhs.x, lhs.y ) << rhs;
			//lhs.sclx *= rhs.valx;
			//lhs.scly *= rhs.valy;
			return lhs;
		}

		template< ALIGN alignment, class T >
		inline ScrMapper< TexPaintTag< T >, alignment >& operator<<( ScrMapper< TexPaintTag< T >, alignment > &lhs, const Translate2& rhs )
		{
			//ScrXYMakePair< alignment >( lhs.x, lhs.y ) << rhs;
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			return lhs;
		}
		template< ALIGN alignment, class T >
		inline ScrMapper< TexPaintTag< T >, alignment >& operator<<( ScrMapper< TexPaintTag< T >, alignment > &lhs, const Rotate2& rhs )
		{
			//ScrXYMakePair< alignment >( lhs.x, lhs.y ) << rhs;
			//lhs.rot += rhs.val;
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			return lhs;
		}
		template< ALIGN alignment, class T >
		inline ScrMapper< TexPaintTag< T >, alignment >& operator<<( ScrMapper< TexPaintTag< T >, alignment > &lhs, const Scale2& rhs )
		{
			//ScrXYMakePair< alignment >( lhs.x, lhs.y ) << rhs;
			//lhs.sclx *= rhs.valx;
			//lhs.scly *= rhs.valy;
			internal::temp.Transformation( rhs );
			lhs.mat = lhs.mat * internal::temp;
			return lhs;
		}
	}
}