//处理内存的库
//                  2009.10
#pragma once
#include<memory.h>
#include<new>
//VC STL空间分配器
#include<xmemory>

//*****************************
//CRT库 内存检测工具
//在DEBUG模式下使用
//关于 CRT与 alloc的兼容问题 解决中 …… 保留
//*****************************
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>

#include<crtdbg.h>
#endif

//保留 已在DXEnc/Segment.h
//以下未测试
//引入这个主要作为3D顶点初始化
template < typename T >
inline T* mem_init( T dst[], size_t num, const T& x)//以X为初值初始化内存 不支持构造函数
{
	for(unsigned int i=0; i<num; i++)//朴素方法 另一种保留方法是每次把已初始化好的拷贝到后面 不断增大拷贝的长度
		dst[i]=x;
	return dst;
}

template < typename T >
inline T* mem_init( T dst[], size_t num)//以第一个元素为初值初始化内存
{
	for( size_t i=1;i<num;i++)
		dst[i]=dst[0];
	return dst;
}

inline void* mem_init( void* dst, size_t size_x, size_t offset, size_t step, size_t size_all )//按步长跳跃初始化 采用未必效率会更高 保留
{
	for( size_t i=offset + step; i<size_all; i+=step)
		memcpy( (char*)dst + i, (char*)dst + offset, size_x);
	return dst;
}

//VC的STL库空间配置器太次
//暂用

//adapter for VC's Allocator
template < int inst >
class vc_alloc
{
private:
	static std::allocator<char> _r;
public:
	static void * allocate( size_t size )
	{
#ifdef _DEBUG
		char * ret = _r.allocate( size + 8 );

		ret[ 0 ] = ret[ 1 ]=ret [ 2 ] = '*';

		return ret + 8;
#elif
		return _r.allocate( size );
#endif
	}

	static void deallocate( void * p, size_t size )
	{
#ifdef _DEBUG
		_r.deallocate( reinterpret_cast< char * >( p ) - 8, size );
#elif
		_r.deallocate( p, size );
#endif
	}
};

template < int inst >
std::allocator< char > vc_alloc< inst >::_r;

typedef vc_alloc< 0 > alloc;

///////////////////////////////////////
// 采用修改后的SGI空间配置器
// 貌似有bug
//////////////////////////////////////
/*#include "sgi_alloc.h"

class sgi_debug_alloc
{
public:
	static void * allocate( size_t size )
	{
		char * ret = reinterpret_cast < char * > ( SGI::alloc::allocate( size + 8) );
		ret[ 0 ] = ret [ 1 ] = ret [ 2 ];
		return ret + 8;

	}
	static void deallocate( void * p, size_t size )
	{
		SGI::alloc::deallocate( reinterpret_cast< char * >( p ) - 8, size );
	}
};*/

#ifdef _DEBUG
//typedef sgi_debug_alloc alloc;
#elif
//typedef SGI::alloc alloc;
#endif
