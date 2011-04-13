//点在多边形内判断函数是对的
//其他函数可能有笔误 正确性待验证
#pragma once
#include "FengDef.h"
#include "Polygon.h"

namespace OneU
{
	inline dword IsIn( const CPoint2 &p, const CRect &rc )
	{
		return p.x >= rc.left && p.x < rc.right && p.y >= rc.top && p.y < rc.bottom;
	}
	inline dword operator^( const CPoint2 &p, const CRect &rc )
	{
		return IsIn( p, rc );
	}
	template< class Contents, template< class > class Impl >
	dword IsIn( const CPoint2 &p, const Polygon_t< Contents, Impl > &pol )
	{
		unsigned long times = 0;
		for( uint i = 0; i < pol.NoP(); i++ )
		{
			for( uint j = 0; j < pol[i].Size(); j++ )
			{
				uint a = j, b = j + 1 == pol[i].Size() ? 0 : j + 1;
				if( pol[i].Y( b ) > p.y && pol[i].Y( a ) <= p.y )
				{
					uint t = a;
					a = b;
					b = t;
					goto InPolygon_Doit;
				}

				if( pol[i].Y( a ) > p.y && pol[i].Y( b ) <= p.y )
				{
InPolygon_Doit:
					CPoint2 A( pol[i].X( a ), pol[i].Y( a ) );
					CPoint2 B( pol[i].X( b ), pol[i].Y( b ) );
					float t = ( p >> A ) % ( p >> B ); 
					if( t > 0 ) times++;
					else if( t > -Feng::Eps )
					{
						return 2;//点在边或端点上
					}

				}
			}
			if( times % 2 )
				return 1;//根据相交次数确定点是否在多边形内
		}
		return 0;
	}

	template< class Contents, template< class > class Impl >
	inline dword operator^( const CPoint2 &p, const Polygon_t< Contents, Impl > &pol )
	{
		return IsIn( p, pol );
	}

	dword Cross( const CLine2 &l1, const CLine2 &l2 );

	inline dword operator^( const CLine2 &l1, const CLine2 &l2 )
	{
		return Cross( l1, l2 );
	}

	template< class Contents >
	dword Cross( const CLine2 &line, const PolyLine_t< Contents > & line_brk )
	{
		for( uint i = 0; i < line_brk.Size() - 1; i++ )
			if( line ^ CLine2( line_brk.X( i ), line_brk.Y( i ), line_brk.X( i + 1 ), line_brk.Y( i + 1 ) ) )
				return 1;
		return 0;
	}
	template< class Contents >
	inline dword operator^( const CLine2 &line, const PolyLine_t< Contents > &line_brk )
	{
		return Cross( line, line_brk );
	}

	template< class Contents, template< class > class Impl >
	dword Cross( const CLine2 &line, const Polygon_t< Contents, Impl > &polygon )
	{
		for( uint k = 0; k < polygon.NoP(); k++ )
		{
			for( uint i = 0; i < polygon[k].Size() - 1; i++ )
			{
				if( line ^ CLine2( polygon[k].X( i ), polygon[k].Y( i ), polygon[k].X( i + 1 ), polygon[k].Y( i + 1 ) ) )
					return 1;
			}
			if( line ^ CLine2( polygon[k].X( polygon[k].Size() - 1 ), polygon[k].Y( polygon[k].Size() - 1 ),
				polygon[k].X( 0 ), polygon[k].Y( 0 ) ) )
				return 1;
		}
		return 0;
	}
	template< class Contents, template< class > class Impl  >
	inline dword operator^( const CLine2 &line, const Polygon_t< Contents, Impl > &polygon )
	{
		return Cross( line, polygon );
	}

	template< class Contents1, class Contents2 >
	dword Cross( const PolyLine_t< Contents1 > &l1, const PolyLine_t< Contents2 > &l2 )
	{
		for( uint i = 0; i < l1.Size() - 1; i++ )
			if( CLine2( l1.X( i ), l1.Y( i ), l1.X( i + 1 ), l1.Y( i + 1 ) )
				^ l2 )
			return 1;
		return 0;
	}

	template< class Contents1, class Contents2 >
	inline dword operator ^( const PolyLine_t< Contents1 > &l1, const PolyLine_t< Contents2 > &l2 )
	{
		return Cross( l1, l2 );
	}

	template< class Contents1, template< class > class Impl, class Contents2 >
	dword Cross( const PolyLine_t< Contents1 > &line, const Polygon_t< Contents2, Impl > &pol )
	{
		for( uint i = 0; i < line.Size(); i++ )
		{
			if( CPoint2( line.X( i ), line.Y( i ) ) ^ pol )
				return 1;
		}
		for( uint k = 0; k < pol.NoP(); k++ )
		{
			for( uint i = 0; i < pol[k].Size() - 1; i++ )
			{
				if( CLine2( pol[k].X( i ), pol[k].Y( i ), pol[k].X( i + 1 ), pol[k].Y( i + 1 ) ) ^ line )
					return 1;
			}
			if( CLine2( pol[k].X( pol[k].Size() - 1 ), pol[k].Y( pol[k].Size() - 1 ),
				pol.X( 0 ), pol.Y( 0 ) ) ^ line )
				return 1;
		}
		return 0;
	}

	template< class Contents1, template< class > class Impl, class Contents2 >
	inline dword operator^( const PolyLine_t< Contents1 > &line, const Polygon_t< Contents2, Impl > &pol )
	{
		return Cross( line, pol );
	}

	template< class Contents1, template< class > class Impl1, class Contents2, template< class > class Impl2 >
	dword Cross( const Polygon_t< Contents1, Impl1 > &pol1, const Polygon_t< Contents2, Impl2 > &pol2 )
	{
		for( uint i = 0; i < pol1.Size(); i++ )
		{
			if( CPoint2( pol1.X( i ), pol1.Y( i ) ) ^ pol2 )
				return 1;
		}
		for( uint i = 0; i < pol2.Size(); i++ )
		{
			if( CPoint2( pol2.X( i ), pol2.Y( i ) ) ^ pol1 )
				return 1;
		}
		return 0;
	}

	template< class Contents1, template< class > class Impl1, class Contents2, template< class > class Impl2 >
	inline dword operator^( const Polygon_t< Contents1, Impl1 > &pol1, const Polygon_t< Contents2, Impl2 > &pol2 )
	{
		return Cross( pol1, pol2 );
	}
}//namespace OneU