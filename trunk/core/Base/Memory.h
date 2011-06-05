/*
This source file is part of OneU Engine.
Copyright (c) 2011 Ladace

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
/**
 * @file Memory.h
 * @brief 内存管理
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
//level 0
#include "OneUPreDef.h"
#include <new>
namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 内存分配器接口
	 *
	 * @remarks 该类是实现Interface类等的基石，因此无法从Interface派生
	 */
	/* ----------------------------------------------------------------------------*/
	class IAllocator
		//: public Interface
	{
	private:
		//禁止拷贝
		IAllocator(const IAllocator&);
		IAllocator& operator=(const IAllocator&);
	public:
		IAllocator(){}
		virtual ~IAllocator(){}
	public:
		virtual void* alloc(size_t count) = 0;
		virtual void* alloc(size_t count, const char* filename, const int Line, const wchar* comment = NULL) = 0;
		virtual void dealloc(void* _ptr) = 0;
		virtual void destroy() = 0;
		//调试版本具备检查内存泄露功能
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 检查是否有内存泄露
		 *
		 * 一般来说只有调试版本才有作用。
		 *
		 * @todo 目前该函数时针对默认内存分配器实现的。应该将分配器与检验内存泄露二者代码分离，以能够自定义内存分配器。（不过这个功能是否需要有待考证）
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void checkLeaks() = 0;
	};
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 默认内存分配器工厂
	 *
	 * @return 内存分配器接口
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API IAllocator* Allocator_create();
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 创建内存分配器单件
	 *
	 * @param af 内存分配器工厂
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API void Allocator_build(IAllocator* (*af)());
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 获取内存分配器单件
	 *
	 * @return 单件
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API IAllocator& GetAllocator();
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 销毁内存分配器单件
	 */
	/* ----------------------------------------------------------------------------*/
	extern "C" ONEU_BASE_API void Allocator_destroy();
}

//comment会被记录到内存块中
inline void* operator new(size_t count, OneU::IAllocator* allocator, int/*目前用以区分AllocatedObject的new的原型 避免笔误*/, const char* filename, const int Line, const OneU::wchar* comment = NULL){
	return allocator->alloc(count, filename, Line, comment);
}
//comment会被记录到内存块中
inline void* operator new[](size_t count, OneU::IAllocator* allocator, const char* filename, const int Line, int/*防止系统选择operator new函数而不是operator new[]*/, const OneU::wchar* comment = NULL){
	size_t* p = (size_t*)allocator->alloc(count + sizeof(size_t), filename, Line, comment);

	*p = count;
	return ++p;
}

//见effective c++ #52
inline void operator delete(void* _ptr, OneU::IAllocator* allocator, int, const char* filename, const int Line, const OneU::wchar* comment){
	allocator->dealloc(_ptr);
}
//见effective c++ #52
inline void operator delete[](void* _ptr, OneU::IAllocator* allocator, const char* filename, const int Line, int, const OneU::wchar* comment){
	allocator->dealloc((size_t*)_ptr - 1);
}

namespace OneU
{
	template<class T>
	void _doDelete(T* _ptr, IAllocator* allocator, const char* filename, const int Line){
		_ptr->~T();
		allocator->dealloc(_ptr);
	}

	//@todo 可以判断是否为内置类型来优化
	template<class T>
	void _doDeleteArray(T* _ptr, IAllocator* allocator, const char* filename, const int Line){
		T* p = _ptr;
		size_t* s = (size_t*)_ptr - 1;
		ONEU_ASSERT(*s % sizeof(T) == 0);
		for(uint32 i = 0; i < *s / sizeof(T); ++i, ++p)
			p->~T();
		allocator->dealloc(s);
	}
}

/* ----------------------------------------------------------------------------*/
/**
 * @brief new宏
 *
 * 创建AllocatedObject（包括Interface）时，使用该宏而非new可以在Debug模式下追踪其内存。
 */
/* ----------------------------------------------------------------------------*/
//使用ONEU内存分配器单件创建对象。
//使用该宏创建的对象可以跨dll销毁。
#define ONEU_NEW new(&OneU::GetAllocator(), __FILE__, __LINE__)
#define ONEU_DELETE delete


/* ----------------------------------------------------------------------------*/
/**
 * @brief 类型new宏
 *
 * 创建原生类型，非多继承类型可用该宏。该宏使用内存分配器，保证内存分配在同一个DLL内。
 * 使用ONEU_DELETE_T删除。
 */
/* ----------------------------------------------------------------------------*/
#define ONEU_NEW_T(v) new(&OneU::GetAllocator(), int(), __FILE__, __LINE__) v

/* ----------------------------------------------------------------------------*/
/**
 * @brief OneU new宏（附加注释）
 *
 * @todo 附加了注释。未实现。
 */
/* ----------------------------------------------------------------------------*/
#define ONEU_NEW_C(v, comment) new(&OneU::GetAllocator(), __FILE__, __LINE__, c) v
//使用ONEU内置的内存池创建数组
//不能直接用内存池函数释放 必须调用ONEU_DELETE宏
#define ONEU_NEW_ARRAY_T(v) new(&OneU::GetAllocator(), __FILE__, __LINE__, int(), NULL) v
#define ONEU_NEW_ARRAY_T_C(v) new(&OneU::GetAllocator(), __FILE__, __LINE__, int(), c) v

/* ----------------------------------------------------------------------------*/
/**
 * @brief delete宏
 */
/* ----------------------------------------------------------------------------*/
#define ONEU_DELETE_T(v) _doDelete(v, &OneU::GetAllocator(), __FILE__, __LINE__)
#define ONEU_DELETE_ARRAY_T(v) _doDeleteArray(v, &OneU::GetAllocator(), __FILE__, __LINE__)

/* ----------------------------------------------------------------------------*/
/**
 * @brief 分配内存宏
 *
 * 使用ONEU内存非配器单件分配内存。
 * 被分配的内存可跨dll销毁。
 */
/* ----------------------------------------------------------------------------*/
#define ONEU_ALLOC(size) OneU::GetAllocator().alloc(size, __FILE__, __LINE__)
#define ONEU_ALLOC_C(size,comment) OneU::GetAllocator().alloc(size, __FILE__, __LINE__, c)

/* ----------------------------------------------------------------------------*/
/**
 * @brief 销毁内存宏
 */
/* ----------------------------------------------------------------------------*/
#define ONEU_DEALLOC(p) OneU::GetAllocator().dealloc(p)


namespace OneU
{

	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 可分配对象类
	 *
	 * 直接被引擎内存分配器管理的对象类。
	 * @remarks 对其调用new、delete均会使用内存分配器管理。支持ONEU_NEW等用于调试。
	 */
	/* ----------------------------------------------------------------------------*/
	class AllocatedObject{
	public:
		void* operator new(size_t count, OneU::IAllocator* allocator, const char* filename, const int Line, const OneU::wchar* comment = NULL){
			return allocator->alloc(count, filename, Line, comment);
		}
		void* operator new(size_t count){
			return GetAllocator().alloc(count);
		}
		void operator delete(void* _ptr, OneU::IAllocator* allocator, const char* filename, const int Line, const OneU::wchar* comment){
			GetAllocator().dealloc(_ptr);
		}
		void operator delete(void* _ptr){
			GetAllocator().dealloc(_ptr);
		}
		void* operator new[](size_t count, OneU::IAllocator* allocator, const char* filename, const int Line, const OneU::wchar* comment = NULL){
			return allocator->alloc(count, filename, Line, comment);
		}
		void* operator new[](size_t count){
			return GetAllocator().alloc(count);
		}
		void operator delete[](void* _ptr){
			GetAllocator().dealloc(_ptr);
		}
		void operator delete[](void* _ptr, OneU::IAllocator* allocator, const char* filename, const int Line, const OneU::wchar* comment){
			GetAllocator().dealloc(_ptr);
		}
	};
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 自动指针
	 * 支持ONEU_NEW_T
	 *
	 * @remarks reserved.可能会在未来的版本删除。
	 */
	/* ----------------------------------------------------------------------------*/
	template<class T>
	class AutoPtr
	{
	private:
		mutable T* _p;
	public:
		typedef T* pointer;
		explicit AutoPtr(T* p)
			: _p(p)
		{}
		AutoPtr(const AutoPtr& rhs)
			: _p(rhs._p)
		{
			rhs._p = NULL;
		}
		AutoPtr& operator=(const AutoPtr& rhs){
			_p = rhs._p;
			rhs._p = NULL;
		}
		~AutoPtr(){
			if(_p != NULL) ONEU_DELETE_T(_p);
		}
		T* operator->(){ return _p; }
		T& operator*(){ return *_p; }
		operator pointer(){ return _p;}
	};
}