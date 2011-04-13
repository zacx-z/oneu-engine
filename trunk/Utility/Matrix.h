/** 
 * @file Matrix.h
 * @brief 矩阵
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-09
 */
#ifndef __MATRIX_H
#define __MATRIX_H

#include "Utility.h"

#ifdef __ONEU_USE_GE
#include "../DXGraphics/MatrixE.h"
#if __ONEUE_MATRIX_VERSION != 100
#pragma message( __FILE__":MatrixE Version Unmatched!")
AnErrorOccured
#endif
#else
#pragma message( __FILE__":CMatrix Needs OneU GE Library!")
AnErrorOccured!!!
#endif

#include "Feng\FengDef.h"
#include "Men\Men.h"

namespace OneU
{
	//前置声明 表达式模板 内部结构体
	template< class Left >
	struct MatrixFactor;

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 4*4矩阵类
	 *
	 * 派生自MATRIX 因此与它兼容
	 */
	/* ----------------------------------------------------------------------------*/
	class Matrix
		: public MATRIX
	{
	public:
		Matrix(){}
		Matrix( const MATRIX & M )
			: MATRIX( M )
		{}
		template< class T >
		explicit Matrix( const T& rhs )
		{
			Transformation( rhs );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 单位矩阵
		 */
		/* ----------------------------------------------------------------------------*/
		static const Matrix E;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 行列式
		 * 
		 * @return 矩阵的行列式值
		 */
		/* ----------------------------------------------------------------------------*/
		float operator~() const
		{
			return MatrixDeterminant( *this );
		}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 从表达式模板拷贝
		 * 
		 * @param rhs 表达式模板
		 */
		/* ----------------------------------------------------------------------------*/
		template< class Left >	
		explicit Matrix( const MatrixFactor< Left >& rhs )
		{
			operator= ( rhs );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 从表达式模板赋值
		 * 
		 * @param rhs 表达式模板
		 * 
		 * @return 左操作符的引用
		 */
		/* ----------------------------------------------------------------------------*/
		template< class Left >
		Matrix& operator= ( const MatrixFactor< Left > &rhs )
		{
			rhs.Interpret( *this );
			return *this;
		}

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 赋值函数
		 *
		 * @param rhs 另一矩阵对象
		 *
		 * @return 左操作符的引用
		 */
		/* ----------------------------------------------------------------------------*/
		Matrix& operator= (const MATRIX& rhs)
		{
			MATRIX::operator=(rhs);
			return *this;
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @name 从变换元导出矩阵
		 *
		 * 根据变换元求所对应的矩阵
		 * @{
		 */
		/* ----------------------------------------------------------------------------*/
		Matrix& Transformation( const Translate2 &Trans )
		{
			MatrixTranslation( *this, Trans.x, Trans.y, 0 );
			return *this;
		}
		Matrix& Transformation( const Rotate2 &Rot )
		{
			MatrixRotationZ( *this, Rot.val );
			return *this;
		}
		Matrix& Transformation( const Scale2 &Scl )
		{
			MatrixScaling( *this, Scl.valx, Scl.valy, 1.0f );
			return *this;
		}
		Matrix& Transformation( const Translate3 &Trans )
		{
			MatrixTranslation( *this, Trans.x, Trans.y, Trans.z );
			return *this;
		}
		Matrix& Transformation( const RotateX & Rot )
		{
			MatrixRotationX( *this, Rot.rot );
			return *this;
		}
		Matrix& Transformation( const RotateY & Rot )
		{
			MatrixRotationY( *this, Rot.rot );
			return *this;
		}
		Matrix& Transformation( const RotateZ & Rot )
		{
			MatrixRotationZ( *this, Rot.rot );
			return *this;
		}
		Matrix& Transformation( const Scale3 & Scl )
		{
			MatrixScaling( *this, Scl.valx, Scl.valy, Scl.valz );
			return *this;
		}
		template <class T>
		Matrix& operator=(const T& Transform)
		{
			return Transformation(Transform);
		}
		/** @}*/
		/* ----------------------------------------------------------------------------*/
	};


	/* ----------------------------------------------------------------------------*/
	/** 
	 * @name 表达式模板
	 *
	 * 加的功能没意义，只有乘法的功能
	 * @internal
	 * @{
	 */
	/* ----------------------------------------------------------------------------*/
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 表达式模板 乘法
	 *
	 * @internal
	 */
	/* ----------------------------------------------------------------------------*/
	template< class Left >
	struct MatrixFactor
	{
		const Left &m_lhs;const Matrix &m_rhs;
		MatrixFactor( const Left &lhs, const Matrix &rhs )
			: m_lhs( lhs ), m_rhs( rhs )
		{}
		Matrix& Interpret( const Matrix &Result )
		{
			MatrixMultiply( Result, m_lhs.Interpret( Result ), m_rhs )
		}
		operator Matrix()
		{
			return Interpret( Matrix() );
		}
	};
	template<>
	struct MatrixFactor< Matrix >
	{
		const Matrix &m_lhs, &m_rhs;
		MatrixFactor( const Matrix &lhs, const Matrix &rhs )
			: m_lhs( lhs ), m_rhs( rhs )
		{}
		Matrix& Interpret( Matrix &Result )
		{
			MatrixMultiply( Result, m_lhs, m_rhs );
			return Result;
		}
		operator Matrix()
		{
			return Interpret( Matrix() );
		}
	};


	template< class Left >
	MatrixFactor< Left > operator*( const Left &lhs, const Matrix &rhs )
	{
		return MatrixFactor< Left >( lhs, rhs );
	}
	/** @}*/
	/* ----------------------------------------------------------------------------*/

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @name Matrix Internal
	 * @internal
	 * @{
	 */
	/* ----------------------------------------------------------------------------*/
	namespace internal
	{
		extern ONEU_UTIL_API Matrix matT1;
		extern ONEU_UTIL_API Matrix matT2;
		//若参数为矩阵返回矩阵本身 若为变换元(Translate2)一类则用其生成矩阵返回 为了operator*而定义的函数 以根据T不同做不同处理
		inline const Matrix &MatrixTransformation( const Matrix &m ) { return m; }
		template< class T >
		inline const Matrix &MatrixTransformation( const T &v ){ static Matrix m; return m.Transformation( v ); }
	}	
	/** @}*/
	/* ----------------------------------------------------------------------------*/

	////////////////////////////
	//（即连接变换元）返回CMatrix

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @name 变换元与矩阵的乘法
	 *
	 * 支持变换元之间相乘
	 * 支持矩阵与变换元相乘（矩阵在左，变换元在右）
	 *
	 * 返回矩阵
	 * 
	 * 不支持Rotate2c和Scale2c
	 * @{
	 */
	/* ----------------------------------------------------------------------------*/
	template< class T >
	inline Matrix operator *( const T& lhs, const Translate2 &rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const Rotate2& rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const Scale2& rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const Translate3& rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const Scale3& rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const RotateX &rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const RotateY &rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	template< class T >
	inline Matrix operator *( const T &lhs, const RotateZ &rhs )
	{
		return internal::MatrixTransformation( lhs ) * internal::matT1.Transformation( rhs );
	}
	/** @}*/
	/* ----------------------------------------------------------------------------*/
}
#endif // ifndef __MATRIX_H

