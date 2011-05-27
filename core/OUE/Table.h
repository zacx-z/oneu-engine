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
 * @file Table.h
 * @brief 表
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
#include "../Base/Error.h"
#include "String.h"

namespace OneU
{
	//字符串映射表（支持空串映射）
	template<typename T>
	class Table
	{
		Table(const Table<T>&);
		Table<T>& operator=(const Table<T>&);
	private:
		struct node
		{
			node *child, *brother, *parent, *rbrother;//儿子 兄弟 父亲 反方向兄弟指针

			wchar c;//Key
			T* val;
		};
		IAllocator& allocator;
		node root;

		node* last_item;
		//寻找p的key为c的孩子，如果寻找不到则返回NULL。
		node* _find_child(node* p, wchar c){
			node* child = p->child;
			while(child != NULL){
				if(child->c == c) return child;
				child = child->brother;
			}
			return NULL;
		}
		//获取p的key为c的孩子，如果寻找不到则创建一个。
		//如果分配内存失败，会结束进程。
		node* _get_child(node* p, wchar c){
			node** child = &p->child;
			node* last_child = NULL;//遍历孩子中的上一个孩子
			while(*child != NULL && (*child)->c <= c){
				if((*child)->c == c) return *child;
				last_child = *child;
				child = &(*child)->brother;
			}

			//没有找到 创建新node
			node* old = *child;
			node* newp = *child = (node*)allocator.alloc(sizeof(node), __FILE__, __LINE__);

			if(newp == NULL) ONEU_RAISE(L"Allocating Error!");

			newp->brother = old;
			newp->c = c;
			newp->val = NULL;
			newp->child = NULL;

			newp->parent = p;

			if(newp->brother != NULL)
				newp->brother->rbrother = newp;
			newp->rbrother = last_child;

			return newp;
		}
		//一个节点为多余的条件是：
		//1.儿子节点全部为多余的或者没有儿子节点
		//2.节点不携带有效的value。
		//该函数递归的判断p的孩子是否为多余的然后删除该节点的孩子并判断该节点是否为多余的。
		//如果该节点是多余的，返回true，否则返回false。
		//bool _recursive_tidy(node* p){
		//	node** ch = &p->child;
		//	bool ret = (p->val == NULL);
		//	while(*ch != NULL){
		//		node** t = &(*ch)->brother;

		//		bool r = _recursive_tidy(*ch);
		//		if(r){
		//			node* temp = *t;
		//			allocator.dealloc(*ch);
		//			*ch = temp;
		//		}
		//		else ch = t;

		//		ret &= r;
		//	}
		//	return ret;
		//}
		void _recursive_delete_child(node* p){
			node* ch = p->child;
			while(ch != NULL){
				node* t = ch->brother;
				_recursive_delete_child(ch);
				
				if(ch->val != NULL){
					//析构删除
					ch->val->~T();
					allocator.dealloc(ch->val);
				}

				//删除节点
				allocator.dealloc(ch);

				ch = t;
			}
		}
	public:
		class iterator
		{
			friend class Table;
			node* ptr;
			iterator(node* p) : ptr(p){}
			void _next_ptr(){
				if(ptr->child != NULL) ptr = ptr->child;
				else if(ptr->brother != NULL) ptr = ptr->brother;
				else{
					while(ptr->parent->brother == NULL)
						ptr = ptr->parent;
					ptr = ptr->parent->brother;
				}
			}
		public:
			iterator() : ptr(NULL){}
			iterator(const iterator& other) : ptr(other.ptr){}
			iterator& operator=(const iterator& other){ ptr = other.ptr; return *this;}
			bool operator==(const iterator& other) const{ return ptr == other.ptr; }
			bool operator!=(const iterator& other) const{ return ptr != other.ptr; }

			iterator& operator++(){
				if(ptr == NULL) return *this;

				_next_ptr();
				if(ptr->parent == NULL) return *this;//is root
				while(ptr->val == NULL){
					_next_ptr();
					if(ptr->parent == NULL) return *this;
				}
				return *this;
			}
			iterator operator++(int){
				iterator ret = *this;
				operator++();
				return ret;
			}
			iterator next(){
				iterator ret = *this;
				return ++ret;
			}

			T& getValue(){ ASSERT(ptr != NULL); return *ptr->val;}
			const T& getValue() const{ if(ptr != NULL) return ptr->val; else return NULL;}
			String _getKey() const{
				String ret;
				node* p = ptr;
				while(p->parent != NULL){
					ret.append(p->c);
					p = p->parent;
				}
				ret.reverse();
				return ret;
			}//temp
		};
		Table()
			:allocator(GetAllocator()), last_item(NULL){
			root.child = root.parent = NULL;
			root.brother = root.rbrother = &root;//为iterator的operator++和--提供极限条件 使其在调用_next_ptr和_prev_ptr可恰好循环遍历
			root.val = NULL;
		}
		iterator begin(){ iterator ret = &root; ++ret; return ret;}
		iterator end(){ return &root; }
		iterator getLastItem(){ return last_item; }
		//如果找到index的节点，那么插入失败返回NULL
		//如果没有找到 插入成功 返回新的T的地址
		//该函数会修改last_item为新加入的节点或找到的节点
		//如果分配内存失败，会结束进程。
		T* insert(pcwstr index){
			node* p = &root;
			for(const wchar* pc = index; *pc != 0; ++pc)
			{
				p = _get_child(p, *pc);//分配内存失败该函数会结束进程
				//if(p == NULL){last_item = NULL; return NULL;}//分配内存失败
			}

			last_item = p;

			if(p->val) return NULL;

			//空串就插到root
			p->val = (T*)allocator.alloc(sizeof(T), __FILE__, __LINE__);
			if(p->val == NULL)ONEU_RAISE(L"Allocating Error!");//{ last_item = NULL; return NULL;}//分配内存失败
			new(p->val) T;

			return p->val;
		}
		//如果找到index的节点，返回T的地址。否则返回NULL。
		//如果找到节点，该函数会修改last_item为找到的节点。否则修改last_item为NULL。
		T* find(pcwstr index){
			node* p = &root;
			for(const wchar* pc = index; p != NULL && *pc != 0; ++pc)
				p = _find_child(p, *pc);

			last_item = p;

			if(p == NULL) return NULL;
			return p->val;
		}

		//该函数会让last_item为NULL。
		void erase(iterator it){
			node* p = it.ptr;
			if(p == NULL) return;

			if(p->val != NULL){
				//析构
				p->val->~T();
				allocator.dealloc(p->val);
				p->val = NULL;
			}

			while(p->child == NULL && p->val == NULL && p != &root){
				node* parent = p->parent;

				if(p->brother != NULL)
					p->brother->rbrother = p->rbrother;
				if(p->rbrother != NULL)
					p->rbrother->brother = p->brother;
				else
					p->parent->child = p->brother;

				allocator.dealloc(p);
				p = parent;
			}
			last_item = NULL;
		}

		void remove(pcwstr index){
			find(index);
			erase(last_item);
		}
		//清除trie树多余的枝节。多余的枝节会在remove中产生。
		//如果频繁的调用remove，应定期调用该函数。
		//void tidy(){
		//	_recursive_tidy(&root);
		//}
		T& operator[](pcwstr index){
			insert(index);
			return *last_item->val;
		}
		void clear(){
			_recursive_delete_child(&root);

			if(root.val != NULL){
				root.val->~T();
				allocator.dealloc(root.val);
				root.val = NULL;
			}
			root.child = NULL;
		}
		~Table(){
			clear();
		}
	};
}
