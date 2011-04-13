/**
 * @file Drawer.h
 * @brief 矢量绘图类
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-09-01
 */
#pragma once

#include "../Utility.h"
#ifdef __ONEU_USE_GE
#include "..\..\DXGraphics\LineE.h"
#else
#pragma message( __FILE__":Feng Needs OneU GE Library!")
AnErrorOccured!!!
#endif

#include <vector>
#include "Polygon.h"
#include "FengAlgo.h"
#include "..\..\DXGraphics\DXGraphics.h"
#include "..\..\DXGraphics\VertexUP.h"
#include "FengMen.h"

//便利类
//底层(0)
//想写顶点组类 有绘制函数
//将Drawer类重构成两部分1.从图形获取可直接绘制的Vertex顶点组 可从Polygon_t< VertexCt >派生2.绘制部分（就很简单了）
//外部接口一样
//@todo 这样可以在程序直接保存顶点组 用来加速 使用定点着色器来绘制线条和面 不然矩阵变换会很慢
//可以改进的地方 使用顶点着色器来绘制线条和面 避免CPU逐个对顶点做矩阵变换
//原则上不能与ScreenPainter共存

namespace OneU
{
	namespace Feng
	{
		class ONEU_UTIL_API ScreenDrawer
		{
		private:
			static int ClassNum;
			DX::RenderManip &m_Rd;
			MATRIX m_Mat;
			bool m_UseMatrix;

#ifdef __ONEU_USE_GE
#if __ONEUE_LINE_VERSION == 100
		public:
			static float DefaultWidth;

			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 设定线条宽度
			 *
			 * 需要辅助库支持。 
			 *
			 * @param fWidth 宽度大小
			 */
			/* ----------------------------------------------------------------------------*/
			void SetWidth( float fWidth )
			{
				DX::LineE::I.SetWidth( fWidth );
				m_Width = fWidth;
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 获得线条宽度
			 *
			 * 需要辅助库支持。
			 *
			 * @return 宽度大小
			 */
			/* ----------------------------------------------------------------------------*/
			float GetWidth() const
			{
				return m_Width;
			}
		private:
			float m_Width;
#else
NeedCode
#endif
#else
NeedCode
#endif
		public:
			ScreenDrawer( DX::RenderManip &Rd )
				: m_Rd( Rd ), m_UseMatrix( false )
#ifdef __ONEU_USE_GE
				, m_Width(DX::LineE::I.GetWidth())
#endif
			{
				ClassNum++;
				ASSERT( ClassNum == 1 );
				SetWidth( DefaultWidth );//默认线条宽度值

				//DX::LineE::I.Begin(); //写在这里会出现运行错误 因为Begin之后如果画不是线条的东西会出现错误
			}
			~ScreenDrawer()
			{
				//DX::LineE::I.End();
				ClassNum--;
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 设定变换矩阵
			 *
			 * @param mat 矩阵
			 */
			/* ----------------------------------------------------------------------------*/
			void SetTransform( const MATRIX &mat )
			{
				m_Mat = mat;
				if( memcmp( &mat, &matIMatrix, sizeof( mat ) ) == 0 ) m_UseMatrix = false;
				else m_UseMatrix = true;
			}
		private:
			//PolyLine
			//内容非CPoint2的慢速版本
			template< class Contents, class T >
			void _Draw( const PolyLine_t< Contents > &line, COLOR crColor, T );

			//内容为CPoint2的快速版本
			template< class Contents >
			void _Draw( const PolyLine_t< Contents > &line, COLOR crColor, CPoint2 );

			//Polygon
			//内容非CPoint2的慢速版本
			template< class Contents, template< class > class Impl, class T >
			void _Draw( const Polygon_t< Contents, Impl > &polygon, COLOR crColor, T );

			//内容为CPoint2的快速版本
			template< class Contents, template< class > class Impl >
			void _Draw( const Polygon_t< Contents, Impl > &pol, COLOR crColor, CPoint2 );
		public:
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 绘制二维点
			 *
			 * 点的大小为线条的宽度大小。
			 * @param p 二维点
			 * @param crColor 点颜色
			 */
			/* ----------------------------------------------------------------------------*/
			void Draw( const CPoint2 &p, COLOR crColor );
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 绘制二维线
			 *
			 * @param line 二维线
			 * @param crColor 线条颜色
			 */
			/* ----------------------------------------------------------------------------*/
			void Draw( const CLine2 &line, COLOR crColor );
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 绘制矩形边线
			 *
			 * @param rc 矩形
			 * @param crColor 颜色
			 */
			/* ----------------------------------------------------------------------------*/
			void Draw( const CRect &rc, COLOR crColor );
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 绘制折线
			 *
			 * 效率与Contents有关。
			 * @todo 当前实现下，Contents使用CPoint2且不使用矩阵的效率最高。否则偏低。
			 * @param line 折线
			 * @param crColor 折线颜色
			 */
			/* ----------------------------------------------------------------------------*/
			template< class Contents >
			void Draw( const PolyLine_t< Contents > &line, COLOR crColor )
			{
				//根据不同的value_type执行不同的函数 考虑减少最后一个参数 占据一定时间 这不知道能不能被优化掉
				_Draw( line, crColor, typename PolyLine_t< Contents >::value_type() );
			}
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 绘制多边形边线
			 *
			 * @param polygon 多边形
			 * @param crColor 颜色
			 */
			/* ----------------------------------------------------------------------------*/
			template< class Contents, template< class > class Impl >
			void Draw( const Polygon_t< Contents, Impl > &polygon, COLOR crColor )
			{
				//根据不同的value_type执行不同的函数
				_Draw( polygon, crColor, typename Contents::value_type() );
			}

			//填充图形
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 填充矩形
			 *
			 * @param rc 矩形
			 * @param crColor 矩形颜色
			 */
			/* ----------------------------------------------------------------------------*/
			void Fill( const CRect &rc, COLOR crColor );

			//凸多边形填充 绘制方式是以第一个顶点画三角扇
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 填充凸多边形
			 *
			 * 要求每一个子多边形都是凸多边形才可以正确绘制。
			 * @param polygon 多边形
			 * @param crColor 多边形颜色
			 */
			/* ----------------------------------------------------------------------------*/
			template< class Contents, template< class > class Impl >
			void Fill( const Polygon_t< Contents, Impl > &polygon, COLOR crColor );

			//凹多边形填充 如果确定是凸多边形应使用Fill
			//尽量不要使用该函数 调整顶点顺序有可能用Fill成功绘制 参见Fill
			//绘制边自交的多边形显示结果不可预期，但不会引起错误
			//@todo 实现有未知错误 定义能够把凹多边形分割成若干多边形的函数。
			//可能是由于浮点误差 待单元测试
			/* ----------------------------------------------------------------------------*/
			/**
			 * @brief 填充凹多边形
			 *
			 * 为了正确渲染凹多边形而定义的函数。效率很低。不建议使用。
			 * @param polygon 多边形
			 * @param crColor 多边形颜色
			 */
			/* ----------------------------------------------------------------------------*/
			template< class Contents, template< class > class Impl >
			void FillConcave( const Polygon_t< Contents, Impl > &polygon, COLOR crColor );
		private:
			//实现FillConcave的递归函数
			//注意First Last是前开后闭区间
			template< class Contents, template< class > class Impl >
			void _FillConcave( const Polygon_t< Contents, Impl > &polygon, std::vector< DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > > &Use,
				uint Center, uint First, uint Last, COLOR crColor );
		public:
		};//class Drawer
		
		template< class Contents, class T >
		void ScreenDrawer::_Draw( const PolyLine_t< Contents > &line, COLOR crColor, T )
		{
			using DX::LineE;
			
			//@todo 改成着色器方式
			static std::vector< CPoint2 > point;

			point.resize( line.Size() );

			for( uint i = 0; i < line.Size(); i++ )
			{
				point[ i ].x = line.X( i );
				point[ i ].y = line.Y( i );
				if (m_UseMatrix) point[i] << m_Mat;
			}
			LineE::I.Draw( point, line.Size(), crColor );
		}
		template< class Contents >
		void ScreenDrawer::_Draw( const PolyLine_t< Contents > &line, COLOR crColor, CPoint2 )
		{
			if (m_UseMatrix) _Draw(line, crColor, void*());//使用慢速方式
			else
				DX::LineE::I.Draw( line.Data(), line.Size(), crColor );
		}

		template< class Contents, template < class > class Impl, class T >
		void ScreenDrawer::_Draw( const Polygon_t< Contents, Impl > &polygon, COLOR crColor, T )
		{
			using DX::LineE;

			//@todo 改成着色器方式 会加速很多
			const uint num = polygon.NoP();

			std::vector< CPoint2 > point;

			uint t;

			for( uint i = 0; i < num; i++ )
			{
				t = 0;

				point.resize( polygon[i].Size() + 1 );
				for( uint j = 0; j < polygon[i].Size(); ++j )
				{
					point[t].x = polygon[i].X( j );
					point[t].y = polygon[i].Y( j );
					if (m_UseMatrix) point[t] << m_Mat;
					t++;
				}
				point[ t ] = point[ 0 ];

				//如果这个段是空的 就不进行绘制
				if( t == 0 ) continue;

				LineE::I.Draw( &point[0], t + 1, crColor );//未检查返回值
			}
		}

		template< class Contents, template< class > class Impl >
		void ScreenDrawer::_Draw( const Polygon_t< Contents, Impl > &pol, COLOR crColor, CPoint2 )
		{
			using DX::LineE;

			//@todo 改成着色器方式
			if( m_UseMatrix )
			{
				_Draw(pol, crColor, (void*)(0));//使用慢速方式
				return;
			}
			for( uint i = 0; i < pol.NoP(); i++ )
			{
				LineE::I.Draw( pol[i].Data(), pol[i].Size(), crColor );
				static CPoint2 v[ 2 ];
				v[ 0 ].x = pol[i].X( pol[i].Size() - 1 ); v[ 0 ].y = pol[i].Y( pol[i].Size() - 1 );
				v[ 1 ].x = pol[i].X( 0 ); v[ 1 ].y = pol[i].Y( 0 );
				LineE::I.Draw( v, 2, crColor );
			}
		}

		template< class Contents, template< class > class Impl >
		void ScreenDrawer::Fill( const Polygon_t< Contents, Impl > &polygon, COLOR crColor )
		{
			if( polygon.Size() < 3 ) return;

			//@todo 改成着色器方式
			//设定矩阵变换
			if( m_UseMatrix ) Graphics.SetWorldTransform( &m_Mat );
			//else Graphics.SetWorldTransform( NULL );

			std::vector< DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > > v;

			v.resize( polygon.Size() );
			for( uint i = 0; i < polygon.Size(); i++ )
			{
				v[i].X() = polygon.X( i );
				v[i].Y() = polygon.Y( i );
				v[i].Z() = 0.0f;
				v[i].RhW() = 1;
				v[i].Diffuse() = crColor;
			}
			Graphics.SetFVF( &v[0] );

			for( uint i = 0; i < polygon.NoP(); i++ )
			{
				const uint nElem = polygon[i].Size() - 2;
				if( nElem >= 1 )
				{
					m_Rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, &v[ polygon.PolyBegin( i ) ],
						nElem );
				}
			}
		}
		template< class Contents, template< class > class Impl >
		void ScreenDrawer::FillConcave( const Polygon_t< Contents, Impl > &polygon, COLOR crColor )
		{
			if( polygon.Size() < 3 ) return;

			//设定矩阵变换
			if( m_UseMatrix ) Graphics.SetWorldTransform( &m_Mat );
			//else Graphics.SetWorldTransform( NULL );

			std::vector< DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > > v;

			Graphics.SetFVF( DX::FVF_XYZRHW | DX::FVF_DIFFUSE );

			for( uint k = 0; k < polygon.NoP(); k++ )
			{
				_FillConcave( polygon, v, polygon.PolyBegin( k ), polygon.PolyBegin( k ) + 1, polygon.PolyEnd( k ), crColor );
			}
		}

		template< class Contents, template< class > class Impl >
		void ScreenDrawer::_FillConcave( const Polygon_t< Contents, Impl > &polygon, std::vector< DX::VertexUP< DX::FVF_XYZRHW | DX::FVF_DIFFUSE > > &v, uint Center, uint First, uint Last, COLOR crColor )
		{

			if( First >= Last - 1 )return;

			v.resize( Last - First + 1 );
			v[0].X() = polygon.X( Center );
			v[0].Y() = polygon.Y( Center );
			v[0].RhW() = 1.0f;
			v[0].Diffuse() = crColor;
			v[1].X() = polygon.X( First );
			v[1].Y() = polygon.Y( First );
			v[1].RhW() = 1.0f;
			v[1].Diffuse() = crColor;
			
			uint i = First + 1;
			for( ; i < Last; i++ )
			{
				if( ( CLine2( polygon.X( Center ), polygon.Y( Center ), polygon.X( i ), polygon.Y( i ) ) ^ polygon )
					|| ( !( CPoint2( ( polygon.X( Center ) + polygon.X( i ) ) / 2, ( polygon.Y( Center ) + polygon.Y( i ) ) / 2 ) ^ polygon ) ) )//判断此线段是否在多边形内 用线段中点来实现
					break;
				v[i - First + 1].X() = polygon.X( i );
				v[i - First + 1].Y() = polygon.Y( i );
				v[i - First + 1].RhW() = 1.0f;
				v[i - First + 1].Diffuse() = crColor;
			}

			//一次绘制成功的情况
			if( Last - i <= 1 )
			{
				if( i - First - 1 > 0 )
					m_Rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, &v[0],
					i - First - 1 );
				return;
			}

			uint j = i + 1;
			for( ; j < Last; j++ )
				if( !( CLine2( polygon.X( Center ), polygon.Y( Center ), polygon.X( j ), polygon.Y( j ) ) ^ polygon 
					|| ( !( CPoint2( ( polygon.X( Center ) + polygon.X( j ) ) / 2, ( polygon.Y( Center ) + polygon.Y( j ) ) / 2 ) ^ polygon ) ) ) )
					break;

			if( j >= Last )
			{
				if( i - First - 1 > 0 )
					m_Rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, &v[0],
					i - First - 1 );
			}
			v[i - First + 1].X() = polygon.X( j );
			v[i - First + 1].Y() = polygon.Y( j );
			v[i - First + 1].RhW() = 1.0f;
			v[i - First + 1].Diffuse() = crColor;
			m_Rd.RenderVertexUP( DX::RenderManip::PT_TRIANGLEFAN, &v[0],
				i - First );

			_FillConcave( polygon, v, i - 1, i, j + 1, crColor );//因为是左开右闭 所以用j + 1
			_FillConcave( polygon, v, Center, j, Last, crColor );
		}
	}//namespace Feng
}//namespace OneU
