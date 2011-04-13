#include "BaseLayer.h"
//待定

//使用边界检查时就用断言
#ifdef __ONEU_USE_BOUND_CHECK
#define _ASSERT( exp ) ASSERT( exp )
#else
#define _ASSERT( exp ) void( 0 )
#endif
namespace OneU
{

	//以下未测试
	//引入这个主要作为3D顶点初始化
	template < typename T >
	T* mem_fill( T dst[], size_t num, const T& x)//以X为初值初始化内存 不支持构造函数
	{
		for( size_t i=0; i<num; i++)//朴素方法 另一种保留方法是每次把已初始化好的拷贝到后面 不断增大拷贝的长度
			dst[i]=x;
		return dst;
	}

	template < typename T >
	T* mem_fill( T dst[], size_t num)//以第一个元素为初值初始化内存
	{
		for( size_t i=1;i<num;i++)
			dst[i]=dst[0];
		return dst;
	}

	void* mem_fill( void* dst, size_t size_x, size_t offset, size_t step, size_t size_all )//按步长跳跃初始化 采用未必效率会更高 保留
	{
		for( size_t i=offset + step; i<size_all; i+=step)
			memcpy( (char*)dst + i, (char*)dst + offset, size_x);
		return dst;
	}

	//一种数组的封装方式
	template< class Element, size_t size >
	class Array
	{
	private:
		Element _v[ size ];
	public:
		typedef Element ElementType;
		Array(){}
		Array( Element & x )
		{
			Fill( x );
		}
		void Fill( Element & x)
		{
			mem_init( _v, size, x );
		}

		Element & operator[]( int index )
		{
			_ASSERT( index >= 0 && index < size )
			return _v[ index ];
		}
		~Array(){}
	};

	template< class Type, class Alloc = alloc >
	class AllocPtr//名字 位置保留
	{
	private:
		Type* _p;
	public:
		AllocPtr()
		{
			_p = alloc::allocate( sizeof( Type ) );
		}
		~AllocPtr()
		{
			alloc::deallocate( _p, sizeof( type ) );
		}
		Type* operator ->()
		{
			return _p;
		}
		operator Type*()
		{
			return _p;
		}
	};
}
	/*

	//动态数组
	template< class Element, class Alloc = alloc >
	class Array_d
	{
	private:
		Element * _p;
		size_t _size;
	public:
		typedef Element ElementType;
		typedef Alloc AllocatorType;

		Array_d()
			:_p( 0 ), size( 0 ){}
		Array_d( size_t size )
			: _size( size )
		{
			Alloc( size );
		}
		void Alloc( size_t size );
		void Release();


		size_t & Size() const
		{
			return size;
		}

		Element & operator[]( int index )
		{
			_ASSERT( _p )
			_ASSERT( index >= 0 && index < size )
			return _v[ index ];
		}

		~Array()
		{
			delete [] _p;
		}
	};
	template< class Element, class Alloc >
	void Array_d< Element, Alloc >::Alloc( int size )
	{
		if( _p )Release();
		_p = Alloc::allocate( size );
	}

	template< class Element, class Alloc >
	void Array_d< Element, Alloc >::Release()
	{
		ASSERT( _p )
		Alloc::deallocate( _p, size );

		_p = 0;
	}*/
}

#undef _ASSERT