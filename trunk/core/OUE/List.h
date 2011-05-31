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
 * @file List.h
 * @brief 链表
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "../Base/Error.h"
#include <new>
namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/**
	 * @brief 链表类
	 *
	 * @tparam T 链表元素类型
	 */
	/* ----------------------------------------------------------------------------*/
	template<typename T>
	class List
	{
		List(const List<T>&);
		List<T>& operator=(const List<T>&);

		struct node
		{
			node *prev, *next;
			T val;
		};
		IAllocator& allocator;
		node *head, *rail;
		uint32 m_Size;
	public:
		class iterator
		{
			node* ptr;
			friend class List;
			iterator(node* p)
				: ptr(p){}
		public:
			iterator()
				: ptr(NULL){}
			iterator(const iterator& other)
				: ptr(other.ptr){}
			iterator& operator=(const iterator& other){
				ptr = other.ptr;
				return *this;
			}
			iterator& operator++(){
				ONEU_ASSERT(ptr);
				ptr = ptr->next;
				ONEU_ASSERT(ptr);
				return *this;
			}
			iterator& operator--(){
				ONEU_ASSERT(ptr);
				ptr = ptr->prev;
				ONEU_ASSERT(ptr);
				return *this;
			}
			iterator operator++(int){
				iterator ret = *this;
				this->operator++();
				return ret;
			}
			iterator operator--(int){
				iterator ret = *this;
				this->operator--();
				return ret;
			}
			bool operator==(const iterator& other) const{
				return ptr == other.ptr;
			}
			bool operator!=(const iterator& other) const{
				return ptr != other.ptr;
			}
			iterator next(){
				ONEU_ASSERT(ptr);
				return ptr->next;
			}
			const iterator next() const{
				ONEU_ASSERT(ptr);
				return ptr->next;
			}
			iterator prev(){
				ONEU_ASSERT(ptr);
				return ptr->prev;
			}
			const iterator prev() const{
				ONEU_ASSERT(ptr);
				return ptr->prev;
			}

			T* operator->(){
				ONEU_ASSERT(ptr);
				return &ptr->val;
			}
			const T* operator->() const{
				ONEU_ASSERT(ptr);
				return &ptr->val;
			}
			T& operator*(){
				ONEU_ASSERT(ptr);
				return ptr->val;
			}
			const T& operator*() const{
				ONEU_ASSERT(ptr);
				return ptr->val;
			}
		};
		List() : allocator(GetAllocator()), m_Size(0){
			head = (node*)allocator.alloc(sizeof(node), __FILE__, __LINE__);
			rail = (node*)allocator.alloc(sizeof(node), __FILE__, __LINE__);

			if(head == NULL || rail == NULL) ONEU_RAISE(L"Allocating Error!");

			head->prev = rail->next = NULL;
			head->next = rail;
			rail->prev = head;
		}
		iterator begin(){ return iterator(head->next);}
		const iterator begin() const{ return iterator(head->next);}
		iterator end(){ return iterator(rail);}
		const iterator end() const{ return iterator(rail);}
		T& front(){ return *begin();}
		T& back(){ return *end().prev();}
	private:
		void _doInsert( node* new_p, iterator &it ) 
		{
			new_p->prev = it.ptr->prev;
			new_p->next = it.ptr;
			it.ptr->prev->next = new_p;
			it.ptr->prev = new_p;

			++m_Size;
		}
	public:
		uint32 size() const{ return m_Size; }
		//插到it前面
		iterator insert(iterator it, const T& val){
			ONEU_ASSERT(it.ptr != NULL);
			node* new_p = (node*)allocator.alloc(sizeof(node), __FILE__, __LINE__);
			if(new_p == NULL){ ONEU_RAISE(L"Allocating Error!");}
			new (&new_p->val) T(val);

			_doInsert(new_p, it);

			return iterator(new_p);
		}
		iterator insert(iterator it){
			ONEU_ASSERT(it.ptr != NULL);
			node* new_p = (node*)allocator.alloc(sizeof(node), __FILE__, __LINE__);
			if(new_p == NULL){ ONEU_RAISE(L"Allocating Error!");}
			new (&new_p->val) T;

			_doInsert(new_p, it);

			return iterator(new_p);
		}

		void erase(iterator it){
			ONEU_ASSERT(it.ptr != NULL && it.ptr != head && it.ptr != rail);
			it.ptr->prev->next = it.ptr->next;
			it.ptr->next->prev = it.ptr->prev;
			//析构
			it.ptr->val.~T();
			allocator.dealloc(it.ptr);

			--m_Size;
		}
		void pushBack(const T& val){
			insert(this->end(), val);
		}
		void pushFront(const T& val){
			insert(this->begin(), val);
		}
		void clear(){
			//析构掉内部所有元素
			for(iterator it = this->begin(); it != this->end();){
				it.ptr->val.~T();
				allocator.dealloc((it++).ptr);
			}
			head->next = rail;
			rail->prev = head;

			m_Size = 0;
		}
		~List(){
			clear();
			allocator.dealloc(head);
			allocator.dealloc(rail);
		}
	};
}
