/** 
 * @file Polygon.h
 * @brief 多边形和折线的库
 *
 * 折线和多边形是容器图形类，既是图形也是点的容器。
 * 作为容器有模板参数表示实际存储类型和存储方式，可从预置中选择。
 * 决定了容器相关操作是否可使用。
 *
 * 多边形类有模板参数表示实现方式，有复杂多边形实现和简单多边形实现。
 * 复杂多边形只有最后一个多边形能进行容器操作。这是为了一旦形成不轻易改动顶点数目的多边形设计的。
 * 简单多边形不能创建多个多边形。
 *
 * 复杂多边形：由多个简单多边形组成，可用来表示有空洞的面。
 * 简单多边形：由闭合折线形成的多边形。
 * 以逆时针顶点顺序为正，法线向着屏幕方向。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2010-07-20
 */
#pragma once
#include <vector>
#include "..\..\DXGraphics\D3DDefs.h"
#include "..\..\DXGraphics\VertexUP.h"
#include "FengDef.h"

namespace OneU
{
	//前置声明
	template< class Contents >
	class MultiPolygon_Impl;
	template< class Contents >
	class SinglePolygon_Impl;
	//第一个参数为选择存储顶点的容器和内容（用Point方式或Vertex方式），第二个参数为实现方式
	//（单多边形实现或多重多边形实现，默认为多重多边形实现，在大多数函数中都会使用多重多边形）
	//对于Polygon_t还未处理所有与它有关的函数 保留
	template< class Contents, template< class > class Impl = SinglePolygon_Impl >
	class Polygon_t;

	//以此结构体为接口标准
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 用vector存储Point结构
	 *
	 * 可Resize及PushBack。
	 * 将此类作为PolyLine_t的参数或Polygon_t的第一个参数
	 * 	 
	 * @sa PolyLine_t
	 * @sa Polygon_t
	 */
	/* ----------------------------------------------------------------------------*/
	class PointCt
	{
	public:
		typedef CPoint2 value_type;
		typedef std::size_t size_type;
		enum{
			b_resize = 1, /*表示此类顶点数目可变*/
			b_push_front = 0, /*表示此类可从头部插入*/
			b_push_back = 1, /*表示此类可从尾部插入*/
			b_insert = 0 /*表示此类可以随机插入*/
		};
	private:
		std::vector< CPoint2 > _v;
		size_type _n;
	public:
		PointCt()
			: _n( 0 )
		{}
		value_type* Data()
		{
			return &_v[0];
		}
		float& X( size_type index )
		{
			ASSERT( index < _n );
			return _v[ index ].x;
		}
		float& Y( size_type index )
		{
			ASSERT( index < _n );
			return _v[ index ].y;
		}
		float X( size_type index ) const
		{
			ASSERT( index < _n );
			return _v[ index ].x;
		}
		float Y( size_type index ) const
		{
			ASSERT( index < _n );
			return _v[ index ].y;
		}
		size_type Size() const
		{
			return _n;
		}
		void Resize( size_type N )
		{
			_v.resize( N );
			_n = N;
		}
		//附加属性：可从尾部加入
		void PushBack( float x, float y )
		{
			_v.push_back( CPoint2( x, y ) );
			_n++;
		}
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 用vector存储VertexUP< FVF >结构
	 *
	 * 可Resize及PushBack。
	 * 将此类作为PolyLine_t的参数或Polygon_t的第一个参数
	 *
	 * @sa PolyLine_t
	 * @sa Polygon_t
	 */
	/* ----------------------------------------------------------------------------*/
	template< dword FVF >
	class VertexCt
	{
	public:
		typedef DX::VertexUP< FVF > value_type;
		typedef std::size_t size_type;
		enum{
			b_resize = 1, /*表示此类顶点数目可变*/
			b_push_front = 0, /*表示此类可从头部插入*/
			b_push_back = 1, /*表示此类可从尾部插入*/
			b_insert = 0 /*表示此类可以随机插入*/
		};
	private:
		std::vector< DX::VertexUP< FVF > > _v;
		size_type _n;
	public:
		VertexCt()
			: _n( 0 )
		{}
		value_type* Data()
		{
			return &_v[0];
		}
		float& X( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].X();
		}
		float& Y( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].Y();
		}
		float X( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].X();
		}
		float Y( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].Y();
		}
		size_type Size() const
		{
			return _n;
		}
		void Resize( size_type N )
		{
			_v.resize( N );
			_n = N;
		}
		void PushBack( float x, float y )
		{
			_v.push_back( VertexType() );
			_v[ _n ].x() = x;
			_v[ _n ].y() = y;
			_n++;
		}
	};
	//固定大小的点容器
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 用C式数组存储Point结构体
	 *
	 * 不可Resize及PushBack。
	 * 将此类作为PolyLine_t的参数或Polygon_t的第一个参数
	 *
	 * @sa PolyLine_t
	 * @sa Polygon_t
	 */
	/* ----------------------------------------------------------------------------*/
	template< std::size_t N >
	class PointStaticCt
	{
	public:
		typedef CPoint2 value_type;
		typedef std::size_t size_type;
		enum{
			b_resize = 1, /*表示此类顶点数目可变*/
			b_push_front = 0, /*表示此类可从头部插入*/
			b_push_back = 1, /*表示此类可从尾部插入*/
			b_insert = 0 /*表示此类可以随机插入*/
		};
		size_type _n;
	private:
		CPoint2 _v[N];
	public:
		PointStaticCt()
			: _n( 0 )
		{}
		value_type* Data()
		{
			return &_v[0];
		}
		float& X( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].x;
		}
		float& Y( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].y;
		}
		float X( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].x;
		}
		float Y( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].y;
		}
		size_type Size() const
		{
			return _n;
		}
		void Resize( size_type NewN )
		{
			ASSERT( NewN <= N );

			_n = NewN;
		}
		void PushBack( float x, float y )
		{
			Resize( _n + 1 );
			_v[ _n - 1 ].x() = x;
			_v[ _n - 1 ].y() = y;
		}
	};
	//固定大小的顶点容器
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 用C式数组存储VertexUP< FVF >结构体
	 *
	 * 不可Resize及PushBack。
	 * 将此类作为PolyLine_t的参数或Polygon_t的第一个参数
	 *
	 * @sa PolyLine_t
	 * @sa Polygon_t
	 */
	/* ----------------------------------------------------------------------------*/
	template< dword FVF, std::size_t N >
	class VertexStaticCt
	{
	public:
		typedef DX::VertexUP< FVF > value_type;
		typedef std::size_t size_type;
		enum{
			b_resize = 1, /*表示此类顶点数目可变*/
			b_push_front = 0, /*表示此类可从头部插入*/
			b_push_back = 1, /*表示此类可从尾部插入*/
			b_insert = 0 /*表示此类可以随机插入*/
		};
		size_type _n;
	private:
		DX::VertexUP< FVF > _v[N];
	public:
		VertexStaticCt()
			: _n( 0 )
		{}
		value_type* Data()
		{
			return &_v[0];
		}
		float& X( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].X();
		}
		float& Y( size_type index )
		{
			ASSERT( index < _n );

			return _v[ index ].Y();
		}
		float X( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].X();
		}
		float Y( size_type index ) const
		{
			ASSERT( index < _n );

			return _v[ index ].Y();
		}
		size_type Size() const
		{
			return _n;
		}
		void Resize( size_type NewN )
		{
			ASSERT( NewN <= N );

			_n = NewN;
		}
		void PushBack( float x, float y )
		{
			Resize( _n + 1 );
			_v[ _n - 1 ].x() = x;
			_v[ _n - 1 ].y() = y;
		}
	};


	//Line
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 折线类
	 *
	 * @tparam Contents 容器类，可从预置的类中选择。
	 *
	 * @sa PointCt
	 * @sa VertexCt
	 * @sa PointStaticCt
	 * @sa VertexStaticCt
	 */
	/* ----------------------------------------------------------------------------*/
	template< class Contents >
	class PolyLine_t
	{
	private:
		Contents _r;
	public:
		typedef typename Contents::value_type value_type;
		typedef std::size_t size_type;

		enum{
			b_resize = Contents::b_resize, /*表示此类顶点数目可变*/
			b_push_front = Contents::b_push_front, /*表示此类可从头部插入*/
			b_push_back = Contents::b_push_back, /*表示此类可从尾部插入*/
			b_insert = Contents::b_insert /*表示此类可以随机插入*/
		};

		value_type* Data()
		{
			 return _r.Data();
		}
	public:
		PolyLine_t()
			: _r()
		{}
		const float X( size_type index ) const
		{
			return _r.X( index );
		}
		const float Y( size_type index ) const
		{
			return _r.Y( index );
		}
		float& X( size_type index )
		{
			return _r.X( index );
		}
		float& Y( size_type index )
		{
			return _r.Y( index );
		}
		size_type Size() const
		{
			return _r.Size();
		}
		void Clear( size_type N )
		{
			_r.Resize( N );
		}
		void PushBack( float x, float y )
		{
			_r.PushBack( x, y );
		}
	};

	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 复杂多边形实现
	 *
	 * 作为Polygon_t的第二个参数
	 *
	 * @sa Polygon_t
	 */
	/* ----------------------------------------------------------------------------*/
	template< class Contents >
	class MultiPolygon_Impl
	{
	public:
		typedef typename Contents::size_type size_type;
		typedef typename Contents::value_type value_type;

		enum{
			b_resize = Contents::b_resize, /*表示此类顶点数目可变*/
			b_push_front = Contents::b_push_front, /*表示此类可从头部插入*/
			b_push_back = Contents::b_push_back, /*表示此类可从尾部插入*/
			b_insert = Contents::b_insert /*表示此类可以随机插入*/
		};
	private:
		std::vector< size_type > m_Seg;
		Contents _r;

		size_type _PolyBegin( uint index ) const
		{
			if( index == 0 )return 0;
			ASSERT( index < NoP() );
			return m_Seg[ index - 1 ];
		}
		size_type _PolyEnd( uint index ) const
		{
			if( index == NoP() - 1 ) return _r.Size();
			ASSERT( index < NoP() );
			return m_Seg[ index ];
		}
	public:
		DEPRECATED size_type PolyBegin( uint index ) const
		{
			if( index == 0 )return 0;
			ASSERT( index < NoP() );
			return m_Seg[ index - 1 ];
		}
		DEPRECATED size_type PolyEnd( uint index ) const
		{
			if( index == NoP() - 1 ) return _r.Size();
			ASSERT( index < NoP() );
			return m_Seg[ index ];
		}

	public:
		MultiPolygon_Impl()
			: m_Seg()
		{}
		MultiPolygon_Impl( const VECTOR4 &rc )
		{
			operator=( rc );
		}
		template< class U, template< class > class Impl >
		explicit MultiPolygon_Impl( const Polygon_t< U, Impl > &rhs)
		{
			operator=(rhs);
		}
		template< class U, template< class > class Impl >
		MultiPolygon_Impl& operator=(const Polygon_t<U, Impl> &rhs)
		{
			for( uint i = 0; i < rhs.NoP(); ++i )
			{
				size_type n = Size();
				NewPoly( rhs[i].Size() );
				for( size_type j = 0; j < rhs[i].Size(); ++j )
				{
					_r.X( j + n ) = rhs[i].X( j );
					_r.Y( j + n ) = rhs[i].Y( j );
				}
			}
			return *this;
		}
		MultiPolygon_Impl< Contents >& operator= ( const VECTOR4 &rc )
		{
			Clear( 4 );
			_r.X( 0 ) = ( rc.left ); _r.Y( 0 ) = ( rc.top );
			_r.X( 1 ) = ( rc.left ); _r.Y( 1 ) =( rc.bottom );
			_r.X( 2 ) = ( rc.right ); _r.Y( 2 ) =( rc.bottom );
			_r.X( 3 ) = ( rc.right ); _r.Y( 3 ) = ( rc.top );

			return *this;
		}
		template< class U, template< class > class Impl >
		void Append( const Polygon_t< U, Impl > &rhs )
		{
			for( uint i = 0; i < rhs.NoP(); ++i )
			{
				size_type n = Size();
				NewPoly( rhs[i].Size() );
				for( size_type j = 0; j < rhs[i].Size(); ++j )
				{
					_r.X( j + n ) = rhs[i].X( j );
					_r.Y( j + n ) = rhs[i].Y( j );
				}
			}
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 返回顶点所在的内存段
		 * 
		 * @return 内存段的首地址
		 */
		/* ----------------------------------------------------------------------------*/
		value_type* Data()
		{
			return _r.Data();
		}
		/*废弃*/
		DEPRECATED float& X( size_type index )
		{
			return _r.X( index );
		}
		/*废弃*/
		DEPRECATED float& Y( size_type index )
		{
			return _r.Y( index );
		}
		/*废弃*/
		DEPRECATED const float X( size_type index ) const
		{
			return _r.X( index );
		}
		/*废弃*/
		DEPRECATED const float Y( size_type index ) const
		{
			return _r.Y( index );
		}
		//在最后一个简单多边形上添加顶点
		DEPRECATED void PushBack( float x, float y )
		{
			_r.PushBack( x, y );
		}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取所有顶点的数目
		 * 
		 * @return 所有顶点的数目
		 */
		/* ----------------------------------------------------------------------------*/
		size_type Size() const
		{
			return _r.Size();
		}
		//把整个变作一个段 大小为N
		void Clear( size_type N = 0 )
		{
			_r.Resize( N );
			m_Seg.resize( 0 );
		}
		class PolyElem
		{
		private:
			Contents &_r;
			size_type _begin, _end;
			uint index;
			const MultiPolygon_Impl< Contents > &p;
		public:
			PolyElem( const MultiPolygon_Impl< Contents > &p, const Contents &contents, size_type _begin, size_type _end, uint index )
				: p( p ), _r( const_cast< Contents& >( contents ) ), _begin( _begin ), _end( _end ), index( index )
			{}
			float& X( size_type index )
			{
				return _r.X( _begin + index );
			}
			float& Y( size_type index )
			{
				return _r.Y( _begin + index );
			}
			const float X( size_type index ) const
			{
				return _r.X( _begin + index );
			}
			const float Y( size_type index ) const
			{
				return _r.Y( _begin + index );
			}
			size_type Size() const{ return _end - _begin; }

			//仅有最后一个简单多边形可调用PushBack Clear函数
			void PushBack( float x, float y )
			{
				ASSERT( index == p.NoP() - 1 );
				_r.PushBack( x, y );
				_end++;
			}
			void Clear()
			{
				ASSERT( index == p.NoP() - 1 );
				_r.Resize( _begin );
				_end = _begin;
			}
			CPoint2* Data()
			{
				return _r.Data() + _begin;
			}
			const CPoint2* Data() const
			{
				return _r.Data() + _begin;
			}
		};
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取复杂多边形中的简单多边形
		 * 
		 * @param index 简单多边形的ID
		 *
		 * 调用NoP()来获得简单多边形数目。index允许范围为[ 0, Nop() )。
		 * 
		 * @return 第index个简单多边形
		 *
		 * @sa NoP()
		 */
		/* ----------------------------------------------------------------------------*/
		PolyElem operator[]( uint index )
		{
			return PolyElem( *this, _r, _PolyBegin( index ), _PolyEnd( index ), index );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取复杂多边形中的简单多边形
		 * 
		 * @param index 简单多边形的ID
		 *
		 * 调用NoP()来获得简单多边形数目。index允许范围为[ 0, Nop() )。
		 * 
		 * @return 第index个简单多边形
		 *
		 * @sa NoP()
		 */
		/* ----------------------------------------------------------------------------*/
		const PolyElem operator[]( uint index ) const
		{
			return PolyElem( *this, _r, _PolyBegin( index ), _PolyEnd( index ), index );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 添加简单多边形
		 *
		 * 若最后一个多边形为空，则不会添加多边形，而是将最后一个多边形的顶点数调整到PolySize。
		 * 
		 * @param PolySize 简单多边形顶点的数目
		 */
		/* ----------------------------------------------------------------------------*/
		void NewPoly( size_type PolySize )
		{
			if(Size() == 0 || m_Seg.size() != 0 && m_Seg.back() == Size());
			else
				m_Seg.push_back( _r.Size() );
			_r.Resize( _r.Size() + PolySize );
		}
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 简单多边形的数目（Number of Polygons）
		 * 
		 * @return 返回简单多边形的数目 
		 */
		/* ----------------------------------------------------------------------------*/
		uint NoP() const
		{
			return m_Seg.size() + 1;
		}
	};
	template< class Contents >
	class SinglePolygon_Impl
	{	
	public:
		typedef typename Contents::size_type size_type;
		typedef typename Contents::value_type value_type;

	enum{
		b_resize = Contents::b_resize, /*表示此类顶点数目可变*/
		b_push_front = Contents::b_push_front, /*表示此类可从头部插入*/
		b_push_back = Contents::b_push_back, /*表示此类可从尾部插入*/
		b_insert = Contents::b_insert /*表示此类可以随机插入*/
	};
	private:
		Contents _r;

		size_type _PolyBegin( uint index ) const
		{
			ASSERT( index == 0 );
			return 0;
		}
		size_type _PolyEnd( uint index ) const
		{
			ASSERT( index == 0 );
			return _r.Size();
		}
	public:
		DEPRECATED size_type PolyBegin( uint index ) const
		{
			ASSERT( index == 0 );
			return 0;
		}
		DEPRECATED size_type PolyEnd( uint index ) const
		{
			ASSERT( index == 0 );
			return _r.Size();
		}

	public:
		SinglePolygon_Impl()
		{}
		SinglePolygon_Impl( const VECTOR4 &rc )
		{
			operator=( rc );
		}
		template< class U >
		explicit SinglePolygon_Impl( const Polygon_t< U > &other )
		{
			operator=(other);
		}
		template< class U >
		SinglePolygon_Impl& operator =( const Polygon_t< U > &other )
		{
			_r.Resize( other.Size() );
			for( size_type i = 0; i < _r.Size(); i++ )
			{
				_r.X( i ) = other._r.X( i );
				_r.Y( i ) = other._r.Y( i );
			}
			return *this;
		}
		SinglePolygon_Impl< Contents >& operator= ( const VECTOR4 &rc )
		{
			Clear( 4 );
			_r.X( 0 ) = ( rc.left ); _r.Y( 0 ) = ( rc.top );
			_r.X( 1 ) = ( rc.left ); _r.Y( 1 ) =( rc.bottom );
			_r.X( 2 ) = ( rc.right ); _r.Y( 2 ) =( rc.bottom );
			_r.X( 3 ) = ( rc.right ); _r.Y( 3 ) = ( rc.top );

			return *this;
		}
		template< class U >
		void Append( const Polygon_t< U > &rhs )
		{
			unsigned int n = Size();
			NewPoly( rhs.Size() );
			for( unsigned int i = 0; i < rhs.Size(); ++i )
			{
				_r.X( i + n ) = rhs.X( i );
				_r.Y( i + n ) = rhs.Y( i );
			}
		}
		value_type* Data()
		{
			return _r.Data();
		}
		/*废弃*/
		DEPRECATED float& X( size_type index )
		{
			return _r.X( index );
		}
		/*废弃*/
		DEPRECATED float& Y( size_type index )
		{
			return _r.Y( index );
		}
		/*废弃*/
		DEPRECATED const float X( size_type index ) const
		{
			return _r.X( index );
		}
		/*废弃*/
		DEPRECATED const float Y( size_type index ) const
		{
			return _r.Y( index );
		}
		//在最后一个简单多边形上添加顶点
		DEPRECATED void PushBack( float x, float y )
		{
			_r.PushBack( x, y );
		}
		size_type Size() const
		{
			return _r.Size();
		}
		//把整个变作一个段 大小为N
		void Clear( size_type N = 0 )
		{
			_r.Resize( N );
		}
		class PolyElem
		{
		private:
			Contents &_r;
			size_type _begin, _end;
			uint index;
			const SinglePolygon_Impl< Contents > &p;
		public:
			PolyElem( const SinglePolygon_Impl< Contents > &p, const Contents &contents, size_type _begin, size_type _end, uint index )
				: p( p ), _r( const_cast< Contents& >( contents ) ), _begin( _begin ), _end( _end ), index( index )
			{}
			float& X( size_type index )
			{
				return _r.X( _begin + index );
			}
			float& Y( size_type index )
			{
				return _r.Y( _begin + index );
			}
			const float X( size_type index ) const
			{
				return _r.X( _begin + index );
			}
			const float Y( size_type index ) const
			{
				return _r.Y( _begin + index );
			}
			size_type Size() const{ return _end - _begin; }

			//最后一个简单多边形可调用PushBack Clear函数
			void PushBack( float x, float y )
			{
				ASSERT( index == p.NoP() - 1 );
				_r.PushBack( x, y );
				_end++;
			}
			void Clear()
			{
				ASSERT( index == p.NoP() - 1 );
				_r.Resize( _begin );
				_end = _begin;
			}
			CPoint2* Data()
			{
				return _r.Data() + _begin;
			}
			const CPoint2* Data() const
			{
				return _r.Data() + _begin;
			}
		};
		//返回第index个多边形
		//调用NoP成员函数可获得多边形数目
		PolyElem operator[]( uint index )
		{
			return PolyElem( *this, _r, _PolyBegin( index ), _PolyEnd( index ), index );
		}
		const PolyElem operator[]( uint index ) const
		{
			return PolyElem( *this, _r, _PolyBegin( index ), _PolyEnd( index ), index );
		}
		//为此多重多边形类添加一个简单多边形
		//对于单个多边形来说 这不允许调用
		void NewPoly( size_type PolySize )
		{
			ASSERT( FALSE );
		}
		//获得简单多边形数目(Number of Polygons)
		uint NoP() const
		{
			return 1;
		}
	};

	//声明此类是为了让泛型函数能够识别出多边形
	template< class Contents, template< class > class Impl >
	class Polygon_t
		: public Impl< Contents >
	{
	public:
		Polygon_t()
		{}
		Polygon_t(const Polygon_t<Contents, Impl>& rhs)
			: Impl<Contents>((Impl<Contents>&)(rhs)){}//万一更改基类 此处修改
		Polygon_t< Contents, Impl >& operator=(const Polygon_t<Contents, Impl>& rhs)
		{
			Impl< Contents >::operator=((Impl<Contents>&)(rhs));return *this;
		}
		template< class T >
		Polygon_t( const T &rhs )
			: Impl< Contents >( rhs )
		{}
		template< class T >
		Polygon_t< Contents, Impl >& operator=( const T &rhs )
		{
			Impl< Contents >::operator=( rhs );return *this;
		}
		CPoint2 P(typename Impl<Contents>::size_type index)
		{
			return CPoint2(X(index), Y(index));
		}
	};

	typedef PolyLine_t< PointCt > PolyLine;
	typedef Polygon_t< PointCt, SinglePolygon_Impl > Polygon;//Polygon默认为多重多边形
	typedef Polygon_t< PointCt, MultiPolygon_Impl > MPolygon;
}//namespace OneU
