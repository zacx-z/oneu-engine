#pragma once
#include "OneUPreDef.h"
#include "Interface.h"
#include <new>
#include <map>
#include <string>

namespace OneU
{
	//@todo 重新写实现 没写拷贝函数等等
	//table相当于字符串映射的map

	template<typename T>
	class Table
	{
	public:
		typedef std::map<std::wstring, T> MapType;
	private:
		MapType m;
		typename MapType::iterator m_lastItem;
	public:
		Table(){}
		~Table(){}

		//新插入的迭代器可通过getLastItem获得
		//若插入失败 可通过getLastItem获得索引为index条目的迭代器
		T* insert(pcwstr index){
			//这里是调用拷贝构造函数 标准是调用无参构造函数
			std::pair<MapType::iterator, bool> ret = m.insert(std::pair<std::wstring, T>(index, T()));

			m_lastItem = ret.first;
			if(!ret.second)
				return NULL;

			return &ret.first->second;
		}
		T* find(pcwstr index){
			MapType::iterator iter = m.find(index);
			if(iter == m.end())
				return NULL;
			return &iter->second;
		}
		T& operator[](pcwstr index){
			return m[index];
		}

		class Iterator
		{
			friend class Table;
			typename MapType::iterator m_i;
		public:
			Iterator(typename MapType::iterator iter)
				: m_i(iter)
			{}
			Iterator(){}
			T& getValue(){ return m_i->second;}
		};
		Iterator getLastItem(){return m_lastItem; }
		void Erase(Iterator iter){
			m.erase(iter.m_i);
		}
	};
}
