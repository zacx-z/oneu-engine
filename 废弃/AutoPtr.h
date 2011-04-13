#pragma once
#include "OneUPreDef.h"

namespace OneU
{
	//逻辑不对 不用这个
	template<typename T, IAllocator& alloc = GetAllocator() >
	class AutoPtr
	{
	private:
		T* m_p;
	public:
		AutoPtr<T>& operator=(AutoPtr<T>& other)
		{
			if(m_p != NULL) delete m_p;
			m_p = other.m_p;
			other.m_p = NULL;
			return *this;
		}
		AutoPtr<T>& operator=(T* p)
		{
			if(m_p != NULL) delete m_p;
			m_p = p;
			return *this;
		}
		AutoPtr(AutoPtr<T>& other)
		{
			operator=(other);
		}
		AutoPtr(T* p)
		{
			operator=(p);
		}
		//@todo 如果是接口就要调用destroy
		~AutoPtr(){m_p->~T(); alloc.dealloc(m_p);}

		T* operator->(){return m_p;}
		const T* operator->() const{return m_p;}
	};
}