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
 * @file TplTypes.h
 * @brief ƒ£∞Â¿‡–Õ
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once
namespace OneU
{
	template<class T>
	class RefWrapper
	{
		T* m_pI;
	public:
		RefWrapper(): m_pI(NULL){}
		RefWrapper(T* i): m_pI(i){
			if(m_pI != NULL)
				m_pI->addRef();
		}
		RefWrapper(const RefWrapper<T>& rhs): m_pI(rhs.m_pI){
			if(m_pI != NULL)
				m_pI->addRef();
		}
		~RefWrapper(){
			if(m_pI != NULL)
				m_pI->release();
		}
		RefWrapper<T>& operator=(const RefWrapper<T>& rhs){
			if(m_pI != NULL)
				m_pI->release();
			m_pI = rhs.m_pI;
			if(m_pI) m_pI->addRef();
			return *this;
		}
		RefWrapper& operator=(T* i){
			if(m_pI != NULL)
				m_pI->release();
			m_pI = i;
			if(m_pI) m_pI->addRef();
			return *this;
		}
		T* operator->(){ return m_pI; }
		T* get(){ return m_pI; }
	};
}