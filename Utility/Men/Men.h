/** 
 * @file Men.h
 * @brief 变换元的定义
 *
 * 这里只是提供了变换元的定义。
 * 每个支持它的组件还要单独写关于变换元的支持。
 *
 * 注：旋转的单位均为弧度
 *
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-09
 */
#ifndef __MEN_H
#define __MEN_H

namespace OneU
{
	/**
	 * @defgroup Men 门组件
	 *
	 * 变换元的定义。
	 * 低层(0)
	 */
	/* ----------------------------------------------------------------------------*/
	/**
	 * @defgroup Men2 二维变换元
	 * @addtogroup Men
	 * 内部含有枚举值Dim = 2表示这是二维变换元
	 *
	 */
	/*@{*/
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维平移
	 */
	/* ----------------------------------------------------------------------------*/
	struct Translate2
	{
		enum{ Dim = 2 };
		float x,y;
		Translate2( float _x, float _y )
			: x( _x ), y( _y )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 有中心的二维旋转
	 *
	 * 可被Rotate2转换而成
	 * 不属于主要类，不提供过多的支持。
	 * 因为它可以由Translate2和Rotate2组合而成。
	 * @sa Rotate2
	 */
	/* ----------------------------------------------------------------------------*/
	struct Rotate2c//不提供过多支持
	{
		enum{ Dim = 2 };
		float cx, cy, val;
		Rotate2c( float cx, float cy, float val )
			: cx( cx ), cy( cy ), val( val )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 有中心的二维缩放
	 * \ingroup Men2
	 *
	 * 可被Scale2转换而成
	 * 不属于主要类，不提供过多的支持。
	 * 因为它可以由Translate2和Scale2组合而成。
	 * @sa Scale2
	 */
	/* ----------------------------------------------------------------------------*/
	struct Scale2c//不提供过多支持
	{
		enum{ Dim = 2 };
		float cx, cy, valx, valy, rotation;
		Scale2c( float cx, float cy, float valx, float valy, float rotation = 0.0f )
			: cx( cx ), cy( cy ), valx( valx ), valy( valy ), rotation( rotation )
		{}
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维旋转
	 *
	 * 可转换成Rotate2c
	 * @sa Rotate2c
	 */
	/* ----------------------------------------------------------------------------*/
	struct Rotate2
	{
		enum{ Dim = 2 };
		float val;
		Rotate2( float rot )
			: val( rot )
		{}
		operator Rotate2c() const
		{
			return Rotate2c( 0.0f, 0.0f, val );
		}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 二维缩放
	 *
	 * 可转换成Scale2c
	 * @sa Scale2c
	 */
	/* ----------------------------------------------------------------------------*/
	struct Scale2
	{
		enum{ Dim = 2 };
		float valx, valy;
		Scale2( float valx, float valy )
			: valx( valx ), valy( valy )
		{}
		operator Scale2c() const
		{
			return Scale2c( 0.0f, 0.0f, valx, valy );
		}
	};
	/*@}*/
	/* ----------------------------------------------------------------------------*/
	/* ----------------------------------------------------------------------------*/
	/**
	 * @defgroup Men3 三维变换元
	 * 
	 * @addtogroup Men
	 * 内部含有枚举值Dim = 3表示这是三维变换元
	 * 
	 */
	/*@{*/
	
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 三维平移
	 */
	/* ----------------------------------------------------------------------------*/
	struct Translate3
	{
		enum{ Dim = 3 };
		float x, y, z;
		Translate3( float x, float y, float z )
			: x( x ), y( y ), z( z )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 三维缩放
	 */
	/* ----------------------------------------------------------------------------*/
	struct Scale3
	{
		enum{ Dim = 3 };
		float valx, valy, valz;
		Scale3( float valx, float valy, float valz )
			: valx( valx ), valy( valy ), valz( valz )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief X轴旋转
	 */
	/* ----------------------------------------------------------------------------*/
	struct RotateX
	{
		enum{ Dim = 3 };
		float rot;
		RotateX( float rot )
			: rot( rot )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief Y轴旋转
	 */
	/* ----------------------------------------------------------------------------*/
	struct RotateY
	{
		enum{ Dim = 3 };
		float rot;
		RotateY( float rot )
			: rot( rot )
		{}
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief Z轴旋转
	 */
	/* ----------------------------------------------------------------------------*/
	struct RotateZ
	{
		enum{ Dim = 3 };
		float rot;
		RotateZ( float rot )
			: rot( rot )
		{}
	};
	/*@}*/
	/* ----------------------------------------------------------------------------*/
	/**
	 * @defgroup Men_Generator 变换元生成器
	 * 
	 * @addtogroup Men
	 * 自动根据参数数目选择对应的变换元
	 * 
	 */
	/*@{*/

	inline Translate2 Translate( float x, float y )
	{
		return Translate2( x, y );
	}
	inline Translate3 Translate( float x, float y, float z )
	{
		return Translate3( x, y, z );
	}
	inline Rotate2 Rotate( float val )
	{
		return Rotate2( val );
	}
	inline Rotate2c Rotate( float cx, float cy, float val )
	{
		return Rotate2c( cx, cy, val );
	}
	inline Scale2 Scale( float valx, float valy )
	{
		return Scale2( valx, valy );
	}
	inline Scale2c Scale( float cx, float cy, float valx, float valy, float rotation = 0.0f )
	{
		return Scale2c( cx, cy, valx, valy, rotation );
	}
	inline Scale3 Scale( float valx, float valy, float valz )
	{
		return Scale3( valx, valy, valz );
	}
	/*@}*/
	/* ----------------------------------------------------------------------------*/
}//namespace OneU

#endif // ifndef __MEN_H
