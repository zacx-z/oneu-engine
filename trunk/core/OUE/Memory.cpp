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
#include <cassert>
#include "Memory.h"
#include <windows.h>

//stl
#include <list>
#include <string>

#ifdef _DEBUG
#define USE_CHECK_LEAKS 
#endif
namespace OneU
{
	class Allocator
		: public IAllocator
	{
#ifdef USE_CHECK_LEAKS
		struct block_info{
			std::string filename;
			int line;
			void* ptr;
		};
		typedef std::list<block_info> block_list_t;
		block_list_t block_list;
#endif
	public:
		void* alloc(size_t count){
#ifdef USE_CHECK_LEAKS
			void* p = ::operator new(count + sizeof(block_list_t::iterator));
			if(p == NULL)
				return NULL;
			block_info newb;
			newb.ptr = p;
			newb.line = -1;
			block_list.push_front(newb);

			new (p) block_list_t::iterator();
			*(block_list_t::iterator*)p = block_list.begin();
			return ((block_list_t::iterator*)p + 1);
#else
			return ::operator new(count);
#endif
		}
		void* alloc(size_t count, const char* filename, const int Line){
#ifdef USE_CHECK_LEAKS
			void* p = ::operator new(count + sizeof(block_list_t::iterator));
			if(p == NULL)
				return NULL;
			block_info newb;
			newb.ptr = p;
			newb.line = Line;
			newb.filename = filename;
			block_list.push_front(newb);

			new (p) block_list_t::iterator();
			*(block_list_t::iterator*)p = block_list.begin();
			return (block_list_t::iterator*)p + 1;
#else
			return ::operator new(count);
#endif
		}
		void dealloc(void* _ptr){
#ifdef USE_CHECK_LEAKS
			block_list_t::iterator it = *((block_list_t::iterator*)_ptr - 1);
			block_list.erase(it);
			::operator delete((block_list_t::iterator*)_ptr - 1);
#else
			::operator delete(_ptr);
#endif
		}
		void destroy(){
			//不能使用ONEU_DELETE
			delete this;
		}

		void checkLeaks(){
#ifdef USE_CHECK_LEAKS
			if(block_list.size() != 0){
#ifdef _MSC_VER
				OutputDebugString(L"MemoryLeaks!\n\n\n\n");
#elif defined __GNUC__
				didn't write
#else
				wtf?
#endif
			}
#endif
			//非Debug模式是无效的
		}
	};


	extern "C" ONEU_API IAllocator& GetAllocator(){
		static Allocator s_AllocatorInstance;
		return s_AllocatorInstance;
	}
}