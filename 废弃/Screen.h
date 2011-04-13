#pragma once
//#include"BaseLayer.h"
#include <boost\static_assert.hpp>
//纯C++
//处理屏幕大小
//处理顶点坐标在多分辨率下的变化
//Feng中可独立包含的组件 与FengDef和Polygon紧密耦合
//迟早会重写的组件

namespace OneU
{
	template< class Contents >
	class LineBrk_t;
	template< class Contents >
	class Polygon_t;
	struct CVector4;
	struct CVector2;
	struct CLine2;
	namespace Feng
	{
		enum ALIGN{ LR_CENTER, LR_RIGHT, LR_LEFT, TB_CENTER, TB_TOP, TB_BOTTOM, STRETCH };

		//这个类其实相当于一个命名空间 用户不能使用其对象
		//只能调用它之中的静态函数
		class Screen
		{	
		private:
			unsigned int m_Width, m_Height;
			Screen( unsigned int Width, unsigned int Height )
				: m_Width( Width ), m_Height( Height )
			{
			}
			static Screen s_Work, s_Device;
			
		public:
			//设置程序员工作时假想出来的屏幕
			static void SetWorkScreen( unsigned int Width, unsigned int Height )
			{
				s_Work.m_Width = Width;
				s_Work.m_Height = Height;
	
			}
			//设置设备实际屏幕大小
			static void SetDeviceScreen( unsigned int Width, unsigned int Height )
			{
				s_Device.m_Width = Width;
				s_Device.m_Height = Height;
			}
			//由于宽屏等因素 导致不同对齐模式下在屏幕中的工作坐标的范围不同
			static unsigned int GetWholeWorkScreenWidth( ALIGN alignment )
			{
				return RMapX( s_Device.m_Width ) - RMapX( 0, alignment );
			}
			static unsigned int GetWholeWorkScreenHeight( ALIGN alignment )
			{
				return RMapY( s_Device.m_Height, alignment ) - RMapY( 0, alignment );
			}
			
			//为4：3与宽屏转换使用
			//不能合用的标志
			//LR表示宽屏与4：3互转换竖向视野保持 横向扩大或缩小
			//RB则表示横向视野保持 竖向缩小或扩大
			//将工作坐标映射成实际坐标
			static float MapX( float x, ALIGN Alignment )
			{
				switch( Alignment )
				{
				case STRETCH:
				case TB_CENTER:
				case TB_TOP:
				case TB_BOTTOM:
					return x / s_Work.m_Width * s_Device.m_Width;
				case LR_CENTER:
					return ( x / s_Work.m_Width - 0.5f ) * ( static_cast< float >( s_Work.m_Width ) / s_Work.m_Height )
						* s_Device.m_Height + s_Device.m_Width / 2;
				case LR_LEFT:
					return x / s_Work.m_Width * ( static_cast< float > ( s_Work.m_Width ) / s_Work.m_Height ) * s_Device.m_Height;
				case LR_RIGHT:
					return ( x / s_Work.m_Width - 1.0f ) * ( static_cast< float > ( s_Work.m_Width ) / s_Work.m_Height )
						* s_Device.m_Height + s_Device.m_Width;
				}
				return 0.0f;
			}
			static float MapY( float y, ALIGN Alignment = LR_CENTER )
			{
				switch( Alignment )
				{
				case STRETCH:
				case LR_CENTER:
				case LR_LEFT:
				case LR_RIGHT:
					return y / s_Work.m_Height * s_Device.m_Height;
				case TB_CENTER:
					return ( y / s_Work.m_Height - 0.5f ) * ( static_cast< float > ( s_Work.m_Height ) / s_Work.m_Width )
						* s_Device.m_Width + s_Device.m_Height / 2;
				case TB_TOP:
					return y / s_Work.m_Height * ( static_cast< float > ( s_Work.m_Height ) / s_Work.m_Width ) * s_Device.m_Width;
				case TB_BOTTOM:
					return ( y / s_Work.m_Height - 1.0f ) * ( static_cast< float > ( s_Work.m_Height ) / s_Work.m_Width ) * s_Device.m_Width + s_Device.m_Height;
				}
				return 0.0f;
			}
			//反过来
			static float RMapX( float x, ALIGN alignment = LR_CENTER )
			{
				switch( alignment )
				{
				case STRETCH:
				case TB_CENTER:
				case TB_TOP:
				case TB_BOTTOM:
					return x / s_Device.m_Width * s_Work.m_Width;
				case LR_CENTER:
					return ( ( x - s_Device.m_Width / 2 ) / s_Device.m_Height * s_Work.m_Height / s_Work.m_Width + 0.5f ) * s_Work.m_Width;
				case LR_LEFT:
					return x / s_Device.m_Height * s_Work.m_Height / s_Work.m_Width * s_Work.m_Width;
				case LR_RIGHT:
					return ( ( x - s_Device.m_Width ) / s_Device.m_Height * s_Work.m_Height / s_Work.m_Width + 1.0f ) * s_Work.m_Width;
				}
				return 0.0f;
			}
			static float RMapY( float y, ALIGN alignment = LR_CENTER )
			{
				switch( alignment )
				{
				case STRETCH:
				case LR_CENTER:
				case LR_LEFT:
				case LR_RIGHT:
					return y / s_Device.m_Height * s_Work.m_Height;
				case TB_CENTER:
					return ( ( y - s_Device.m_Height / 2 ) / s_Device.m_Width * s_Work.m_Width / s_Work.m_Height + 0.5f ) * s_Work.m_Height;
				case TB_TOP:
					return y / s_Device.m_Width * s_Work.m_Width / s_Work.m_Height * s_Work.m_Height;
				case TB_BOTTOM:
					return ( ( y - s_Device.m_Height ) / s_Device.m_Width * s_Work.m_Width / s_Work.m_Height + 1.0f ) * s_Work.m_Height;
				}
				return 0.0f;
			}
	
		};
	
		//可在程序中用作位置映射
		//除了用作位置映射外 FloatX<STRETCH>还可以用作速度、加速度映射
		//Screen float为ScrMapper所用
		template< ALIGN alignment >
		class floatX
		{
		private:
			float v;
		public:
			floatX()
			{}
			floatX( float rhs )
				: v( Screen::MapX( rhs, alignment ) )
			{}
			floatX( const floatX< alignment >& rhs )
				: v( rhs.v )
			{}
			floatX< alignment >& operator = ( float rhs )
			{
				v = Screen::MapX( rhs, alignment );
				return *this;
			}
			floatX< alignment >& operator = ( const floatX< alignment >& rhs )
			{
				v = rhs.v;
				return *this;
			}
			operator float()
			{
				return Screen::RMapX( v, alignment );
			}
			float GetContents()
			{
				return v;
			}
		};
		template< ALIGN alignment >
		class floatY
		{
		private:
			float v;
		public:
			floatY()
			{}
			floatY( float rhs )
				: v( Screen::MapY( rhs, alignment ) )
			{}
			floatY( const floatY< alignment >& rhs )
				: v( rhs.v )
			{}
			floatY< alignment >& operator = ( float rhs )
			{
				v = Screen::MapY( rhs, alignment );
				return *this;
			}
			floatY< alignment >& operator = ( const floatY< alignment >& rhs )
			{
				v = rhs.v;
				return *this;
			}
			operator float()
			{
				return Screen::RMapY( v, alignment );
			}
			float GetContents()
			{
				return v;
			}
		};
	
		namespace _Screen
		{
			//ComplexShape特指Polygon LineBrk这种类似的类
			template< class T >
			struct IsComplexShape
			{
				enum{ value = 0 };
			};
	
			template< class Contents >
			struct IsComplexShape< Polygon_t< Contents > >
			{
				enum{ value = 1 };
			};
			template< class Contents >
			struct IsComplexShape< LineBrk_t< Contents > >
			{
				enum{ value = 1 };
			};
		}
		//未知的T一般来说用起来会出错
		template< int bComplexShape, class T, ALIGN alignment >
		class ScrMapper_Base
			//: public T
		{
	
		};
		//为 LineBrk 和 Polygon的情况下
		template< class T, ALIGN alignment >
		class ScrMapper_Base< 1, T, alignment >
		{
		public:
			T v;
	
			T& GetContents()
			{
				return v;
			}
	
			//让外部保持在工作坐标之下
			//而内部是设备坐标
			const floatX< alignment > X( unsigned int i ) const
			{
				return *reinterpret_cast< floatX< alignment >* >( &v.X( i ) );
			}
			const floatY< alignment > Y( unsigned int i ) const
			{
				return *reinterpret_cast< floatX< alignment >* >( &v.Y( i ) );
			}
	
			floatX< alignment >& X( unsigned int i )
			{
				return *reinterpret_cast< floatX< alignment >* >( &v.X( i ) );
			}
			floatY< alignment >& Y( unsigned int i )
			{
				return *reinterpret_cast< floatY< alignment >* >( &v.Y( i ) );
			}
		};
		//CPoint2
		template< ALIGN alignment >
		class ScrMapper_Base< 0, CPoint2, alignment >
		{
		public:
			struct 
			{
				floatX< alignment > x;
				floatY< alignment > y;
			};
			CPoint2& GetContents()
			{
				return *reinterpret_cast< CPoint2* > ( this );
			}
		};
		//CRect2
		template< ALIGN alignment >
		class ScrMapper_Base< 0, CVector4, alignment >
		{
		public:
			floatX< alignment > left;
			floatY< alignment > top;
			floatX< alignment > right;
			floatY< alignment > bottom;
			CVector4& GetContents()
			{
				return *reinterpret_cast< CVector4* > ( this );
			}
	
		};
	
		//屏幕坐标映射 暂时什么也不做
		template< class T, ALIGN alignment = LR_CENTER >
		class ScrMapper
			: public ScrMapper_Base< _Screen::IsComplexShape< T >::value, T, alignment >
		{
			BOOST_STATIC_ASSERT( sizeof( T ) == sizeof( ScrMapper_Base< _Screen::IsComplexShape< T >::value, T, alignment > ) );
		};

		//GetScrMapperRef获得一个ScrMapper的接口
		//再调用GetContents获得的坐标不变
		//主要功能是能够使用Men里的组件来实现工作坐标的移动
		template< ALIGN alignment, class T >
		inline ScrMapper< T, alignment >& GetScrMapperRef( T &ct )
		{
			return reinterpret_cast< ScrMapper< T, alignment >& >( ct );
		}
	
		//CLine2
		template< ALIGN alignment >
		class ScrMapper_Base< 0, CLine2, alignment >
		{
		public:
			ScrMapper< CPoint2 > p1,p2;
			CLine2& GetContents()
			{
				return *reinterpret_cast< CLine2* >( this );
			}
		};


		template< ALIGN alignment >
		CMatrix& GetScrMapMatrix( CMatrix &M )
		{
			M.Transformation( Scale2( Screen::MapX( 1.0f, alignment ) - Screen::MapX( 0.0f, alignment ),//这个算出来是X轴的缩放倍数
				Screen::MapY( 1.0f, alignment ) - Screen::MapY( 0.0f, alignment ) )
				* Translate2( Screen::MapX( 0.0f, alignment ), Screen::MapY( 0.0f, alignment ) ) );
			return M;
		}
		//========================================================================
		//我是分割线
		//========================================================================

		template< class T >
		struct TexPaintTag;
		//关于ScrMapper< TexPaint, alignment >的使用 有点特殊一点 跟矢量图形不同
		//首先<<运算符定义的概念跟矢量的就不一样
		//所以关键函数是Map，而没有GetContents
		//保留
		template< ALIGN alignment, class TextureType >
		class ScrMapper_Base< 0, TexPaintTag< TextureType >, alignment >
		{
		private:
			ScrMapper_Base(){}
		public:
			const TextureType &Tex;
			CMatrix mat;

			TexPaintTag< TextureType >& Map()
			{
				GetScrMapMatrix< alignment >( internal::temp );
				mat = mat * internal::temp;
				return reinterpret_cast< TexPaintTag< TextureType >& >( *this );
			}
		};

		//========================================================================
		//我又是分割线
		//========================================================================
		//这部分为某些函数提供了一些简单的写法
		template< class T >
		inline ScrMapper< T, LR_LEFT >& SL( T &ct )
		{
			return GetScrMapperRef< LR_LEFT >( ct );
		}
		template< class T >
		inline ScrMapper< T, LR_RIGHT >& SR( T &ct )
		{
			return GetScrMapperRef< LR_RIGHT >( ct );
		}
		template< class T >
		inline ScrMapper< T, LR_CENTER >& SW( T &ct )
		{
			return GetScrMapperRef< LR_CENTER >( ct );
		}
		template< class T >
		inline ScrMapper< T, STRETCH >& SS( T &ct )
		{
			return GetScrMapperRef< STRETCH >( ct );
		}
		//仅供TexPaint使用
		class __MapMan
		{};
		extern __MapMan SEnd;
		template< class T, ALIGN alignment >
		inline T& operator << ( ScrMapper< T, alignment > &lhs, const __MapMan & )
		{
			return lhs.Map();
		}
	}
}