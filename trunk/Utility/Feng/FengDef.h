/** 
 * @file FengDef.h
 * @brief Feng基本定义
 *
 * Feng组件的最基本定义，有图形结构体定义和浮点误差定义。
 *
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-08
 */
#pragma once
#include "..\..\DXGraphics\D3DDefs.h"

namespace OneU
{
	/** 
	 * @defgroup FengDef 基本图形类型
	 * @addtogroup Feng
	 */
	/*@{*/
	namespace Feng
	{
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 浮点误差
		 *
		 * 仅限于Feng组件使用
		 */
		/* ----------------------------------------------------------------------------*/
		const float Eps = 0.000001f;
	}
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维向量
	 */
	/* ----------------------------------------------------------------------------*/
	struct CVector2
		: public VECTOR2
	{
		CVector2(){}
		CVector2( float X, float Y )
			: VECTOR2( X, Y )
		{}

		operator POINT()
		{
			POINT ret = { ( LONG )x, ( LONG )y };
			return ret;
		}
	};

	//重载二维向量的运算符
	inline CVector2 operator -( const CVector2 &lhs )
	{
		return CVector2( -lhs.x, -lhs.y );
	}
	inline CVector2 operator +( const CVector2 &lhs, const CVector2 &rhs )//向量加法
	{
		return CVector2( lhs.x + rhs.x, lhs.y + rhs.y );
	}
	inline CVector2 operator -( const CVector2 &lhs, const CVector2 &rhs )//向量减法
	{
		return CVector2( lhs.x - rhs.x, lhs.y - rhs.y );
	}
	inline float operator *( const CVector2 &lhs, const CVector2 &rhs )//点乘
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	inline float operator %( const CVector2 &lhs, const CVector2 & rhs )//叉乘后的向量长度
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}



	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 三维向量
	 */
	/* ----------------------------------------------------------------------------*/
	struct CVector3
		: public VECTOR3
	{
		CVector3( float X, float Y, float Z )
			: VECTOR3( X, Y, Z )
		{}
	};

	//重载三维向量的运算符
	inline CVector3 operator -( const CVector3 &lhs )
	{
		return CVector3( -lhs.x, -lhs.y, -lhs.z );
	}
	inline CVector3 operator +( const CVector3 &lhs, const CVector3 &rhs )
	{
		return CVector3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
	}
	inline CVector3 operator -( const CVector3 &lhs, const CVector3 &rhs )
	{
		return CVector3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
	}
	inline float operator *( const CVector3 &lhs, const CVector3 &rhs )//点乘
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	inline CVector3 operator %( const CVector3 &lhs, const CVector3 &rhs )//叉乘
	{
		return CVector3( lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x );
	}

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 四维向量
	 */
	/* ----------------------------------------------------------------------------*/
	struct CVector4
		: public VECTOR4
	{
		CVector4()
		{}
		CVector4( float X, float Y, float Z, float W )
			: VECTOR4( X, Y, Z, W )
		{}
	};

	//重载四维向量的运算符
	inline CVector4 operator -( const CVector4 &lhs )
	{
		return CVector4( -lhs.x, -lhs.y, -lhs.z, -lhs.w );
	}
	inline CVector4 operator +( const CVector4 &lhs, const CVector4 &rhs )
	{
		return CVector4( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w );
	}
	inline CVector4 operator -( const CVector4 &lhs, const CVector4 &rhs )
	{
		return CVector4( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w );
	}
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维点
	 *
	 * 用一个二维向量来表示。
	 */
	/* ----------------------------------------------------------------------------*/
	typedef CVector2 CPoint2;
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 三维点
	 *
	 * 用一个三维向量来表示
	 */
	/* ----------------------------------------------------------------------------*/
	typedef CVector3 CPoint3;
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 矩形
	 *
	 * 用浮点型存储矩形的上下左右边界。
	 * 用一个四维向量来表示。
	 */
	/* ----------------------------------------------------------------------------*/
	typedef CVector4 CRect;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获得从点A指向点B的自由向量
	 * 
	 * @param &lhs 点A
	 * @param &rhs 点B
	 * 
	 * @return 从A指向B自由向量
	 */
	/* ----------------------------------------------------------------------------*/
	inline CVector2 operator >> ( const CPoint2 &lhs, const CPoint2 &rhs )
	{
		return rhs - lhs;
	}
	/* ----------------------------------------------------------------------------*/
	/** 
	* @brief 获得从点A指向点B的自由向量
	* 
	* @param &lhs 点A
	* @param &rhs 点B
	* 
	* @return 从A指向B自由向量
	*/
	/* ----------------------------------------------------------------------------*/
	inline CVector3 operator >> ( const CPoint3 &lhs, const CPoint3 &rhs )
	{
		return rhs - lhs;
	}

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维线
	 *
	 * 用两个二维点来表示
	 */
	/* ----------------------------------------------------------------------------*/
	struct CLine2
	{
		CPoint2 p1, p2;
		CLine2( float x1, float y1, float x2, float y2 )
			: p1( x1, y1 ), p2( x2, y2 )
		{}
		CLine2( CPoint2 & point1, CPoint2 &point2 )
			: p1( point1 ), p2( point2 )
		{}
	};




	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 整型二维向量
	 *
	 * 派生自VC里的POINT，因此与它兼容。
	 */
	/* ----------------------------------------------------------------------------*/
	struct CVector2_n
		: public ::POINT
	{
		CVector2_n( LONG X, LONG Y )
		{
			x = X;
			y = Y;
		}
		explicit CVector2_n( const CVector2 &v )
		{
			x = static_cast< LONG >( v.x );
			y = static_cast< LONG >( v.y );
		}
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 整型二维点
	 *
	 * 用一个整型二维向量来表示
	 */
	/* ----------------------------------------------------------------------------*/
	typedef CVector2_n CPoint2_n;
		
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 整型四维向量
	 *
	 * 派生自VC里的RECT，因此与RECT兼容
	 */
	/* ----------------------------------------------------------------------------*/
	struct CVector4_n
		: public ::RECT
	{
		CVector4_n()
		{}
		CVector4_n( LONG Left, LONG Top, LONG Right, LONG Bottom )
		{
			left = Left;
			top = Top;
			right = Right;
			bottom = Bottom;
		}
		explicit CVector4_n( const CRect &rc )
		{
			left = ( LONG )rc.left;
			top = ( LONG )rc.top;
			right = ( LONG )rc.right;
			bottom = ( LONG )rc.bottom;
		}
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 整型矩形
	 *
	 * 用一个整型四维向量来表示
	 */
	/* ----------------------------------------------------------------------------*/
	typedef CVector4_n CRecti;
	/*@}*///defgroup FengDef
}//namespace OneU
