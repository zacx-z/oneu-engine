//==========================
//Observer.h
//监视类的实例
//
//只支持单继承结构
//开启RTTI时认为基类和派生类指针是没有偏移的
//否则可能引起程序崩溃
//这个库做不到检查这一点，使用RTTI请慎重
//
//可配置Outputer，接口标准参见Output.h
//
//              Ladace
//              2010.2.7
//==========================
#pragma once
//可能引起错误：RTTI中默认原基类和派生类指针没有偏移
//
#include <list>
#include <map>
#include <cassert>
//#define OBSERVER_USE_RTTI

//OBSERVER_CLASS_OUTPUT用于定制类型打印 ClassName为类的名称，ClassNameStr字符串为输出名称，pClass为给出变量
#define OBSERVER_CLASS_OUTPUT_BEGIN( ClassName, ClassNameStr ) \
namespace Observer{\
	template< class Outputer >\
	class ClsGroup< ClassName, Outputer >\
		: public ClsGroup_Base< Outputer >\
	{\
	public:\
		void OutputClassName(){ Outputer()( ClassNameStr );}\
		void OutputElem( void * pElem )\
		{\
			ClassName *pClass = ( ClassName* ) pElem;
#define OBSERVER_CLASS_OUTPUT_END }};}

//定义该宏则使用RTTI机制
#ifdef OBSERVER_USE_RTTI
namespace Observer
{
	template< class T >
	struct Class_Use_RTTI
	{
		enum{ value = 0 };
	};
}
//当ClassName类使用RTTI时使用该宏，这个类必须有虚函数
//请写在声明ClassName的头文件的全局作用域里
#define OBSERVE_CLASS_RTTI( ClassName )\
namespace Observer{\
	template<>\
	struct Class_Use_RTTI< ClassName >\
	{\
		enum{ value = 1 };\
	};\
}
#endif

using namespace std;
namespace Observer
{
	//仅仅是声明 实现体在OneUEngine/Commander.h
	class DefautOutputer;
	//未知类
	class UnknownClass{};

	//前置声明
	template< class Outputer = DefaultOutputer >
	class ClsGroup_Base;

	//全局监视模块
	template< class Outputer = DefaultOutputer >
	class ClsObserver
	{
	private:
		typedef list< ClsGroup_Base< Outputer >* > ListType;
		ListType m_List;
		static ClsObserver *s_pInstance;
		ClsObserver(){}
	public:
		static ClsObserver* GetInstance()
		{
			if( !s_pInstance )
				s_pInstance = new ClsObserver();
			return s_pInstance;
		}
		void Insert( ClsGroup_Base< Outputer > * pW )
		{
			m_List.push_back( pW );
		}
		size_t GetN()
		{
			return m_List.size();
		}
		template< class T >
		void OutputClass()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Output();
		}
		void Output();

	};

	template< class Outputer >
	void ClsObserver< Outputer >::Output()
	{
		for( ListType::iterator i = m_List.begin(); i != m_List.end(); ++i )
			( *i )->Output();
	};

	//ClsGroup基类 保持接口
	template< class Outputer >
	class ClsGroup_Base
	{
	protected:
		typedef list< void * > ListType;
		ListType m_List;
		static unsigned int num;//类型的数量
		unsigned int ID;//类型唯一ID
	public:
		ClsGroup_Base()
			: ID ( num++ )
		{
		}
		static const int MAX_BUFFER_SIZE = 500;
		virtual void OutputClassName();
		virtual void OutputElem( void * Elem );
		virtual size_t GetSize(){ return 0;}
	#ifdef OBSERVER_USE_RTTI
		virtual//若使用RTTI该函数就为虚的
	#endif
		void Output();
		void Insert( void * pW )
		{
			m_List.push_back( pW );
		}
		//Remove方法比较朴素 效率较低
		//因为不知道List的实现方式，所以难以优化
		void Remove( void * pW )
		{
			ListType::iterator iter = m_List.begin();
			for( ; iter != m_List.end(); ++iter )
				if( *iter == pW ) break;
			if( iter == m_List.end() ) 
			{
				assert( 0 );
				return;
			}
			m_List.erase( iter );
		}

	};

	template< class Outputer >
	void ClsGroup_Base< Outputer >::OutputClassName()
	{
		Outputer()( L"UnknownClass#");
		Outputer()( ID );
	}

	template< class Outputer >
	void ClsGroup_Base< Outputer >::OutputElem( void *Elem )
	{
		Outputer()( Elem, GetSize() );
	}

	template< class Outputer >
	void ClsGroup_Base< Outputer >::Output()
	{
		for( ListType::iterator i = m_List.begin(); i != m_List.end(); ++i )
		{
			OutputClassName();
			Outputer()( L':' );
			OutputElem( *i );
			Outputer().NextLine();
		}
	}

	#ifdef OBSERVER_USE_RTTI

	//因为typeid返回的type_info为静态变量
	//通过type_info指针的地址找寻相应类的Group
	template< class Outputer >
	class RTTI_GetClsGroup
	{
	private:
		typedef map< const type_info *, ClsGroup_Base< Outputer > * > MapType;
		static MapType m_Map;
	public:
		template< class T >
		void AddMap( ClsGroup_Base< Outputer > * pGroup )
		{
			m_Map.insert( pair< const type_info *, ClsGroup_Base< Outputer > * >( &typeid( T ), pGroup ) );
		}
		ClsGroup_Base< Outputer > * operator()( const type_info & t )
		{
			MapType::iterator i = m_Map.find( &t );
			if( i == m_Map.end() ) return 0;
			return i->second;
		}
	};

	template< class Outputer >
	map< const type_info *, ClsGroup_Base< Outputer > * > RTTI_GetClsGroup< Outputer >::m_Map;

	//RTTI中间层
	template< class T, class Outputer, bool UseRTTI >
	class ClsGroup_RTTI
		: public ClsGroup_Base< Outputer >
	{
	};
	template< class T, class Outputer >
	class ClsGroup_RTTI< T, Outputer, true >
		: public ClsGroup_Base< Outputer >
	{
		void Output()
		{
			for( ListType::iterator i = m_List.begin(); i != m_List.end(); ++i )
			{
				ClsGroup_Base< Outputer > * pGroup = RTTI_GetClsGroup< Outputer >()( typeid( *( T * )( *i ) ) );
				if( pGroup )//
				{
					pGroup->OutputClassName();
					Outputer()( L':' );
					pGroup->OutputElem( *i );//这个默认基类指针跟派生类指针是没有偏移的 也就是必须为单继承
					Outputer().NextLine();
				}
				else
				{
					Outputer()( typeid( *( T * )( *i ) ).name() );
					Outputer()( L':' );
					Outputer()( L"未监视的类型-派生自" );
					Outputer()( typeid( T ).name() );
					Outputer().NextLine();
				}
			}
		}
	};
	#endif

	template< class T, class Outputer = DefaultOutputer >
	class ClsGroup
	#ifdef OBSERVER_USE_RTTI
		: public ClsGroup_RTTI< T, Outputer, Class_Use_RTTI< T >::value >
	#else
		: public ClsGroup_Base< Outputer >
	#endif
	{
	public:
		size_t GetSize();
		void OutputClassName();
	};

	template< class T, class Outputer >
	void ClsGroup< T, Outputer >::OutputClassName()
	{
		Outputer()( typeid( T ).name() );
	}

	template< class T, class Outputer >
	size_t ClsGroup< T, Outputer >::GetSize()
	{
		return sizeof( T );
	}


	//获得ClsGroup指针仿函数
	template< class T, class Outputer = DefaultOutputer >
	class ClsGroup_GetInstance
	{
	private:
		static ClsGroup< T, Outputer > *s_pInstance;
	public:
		ClsGroup< T, Outputer >* operator()()
		{
			if( !s_pInstance )
			{
				s_pInstance = new ClsGroup< T, Outputer >();
				ClsObserver< Outputer >::GetInstance()->Insert( s_pInstance );
	#ifdef OBSERVER_USE_RTTI
				RTTI_GetClsGroup< Outputer >().AddMap< T >( s_pInstance );
	#endif
			}
			return s_pInstance;
		}
		//针对模板元的特别版本
		//若无法获得 则创建ClsGroup_Adv< T, Declare, Outputer >
		template< class Declare >
		ClsGroup< T, Outputer >* Adv()
		{
			if( !s_pInstance )
			{
				s_pInstance = new ClsGroup_Adv< T, Declare, Outputer >();
				ClsObserver< Outputer >::GetInstance()->Insert( s_pInstance );
	#ifdef OBSERVER_USE_RTTI
				RTTI_GetClsGroup< Outputer >().AddMap< T >( s_pInstance );
	#endif
			}
			return s_pInstance;
		}
		bool Exist()
		{
			return s_pInstance != 0;
		}
	};

	//类型外覆器
	template< class T, class BaseClass = UnknownClass, class Outputer = DefaultOutputer >
	class ClsWrapper
	{
	public:
		ClsWrapper()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Insert( ( T * )this );
		}
		ClsWrapper( const ClsWrapper< T, BaseClass, Outputer >& )
		{
			ClsGroup_GetInstance< T, Outputer >()()->Insert( ( T * )this );
		}
		~ClsWrapper()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Remove( ( T * )this );
		}
	};
	template< class T, class Outputer >
	class ClsWrapper< T, UnknownClass, Outputer >
	{
	public:
		ClsWrapper()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Insert( ( T * )this );
		}
		ClsWrapper( const ClsWrapper< T, UnknownClass, Outputer >& )
		{
			ClsGroup_GetInstance< T, Outputer >()()->Insert( ( T * )this );
		}
		~ClsWrapper()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Remove( ( T * )this );
		}
	};


	template< class Outputer >
	ClsObserver< Outputer > * ClsObserver< Outputer >::s_pInstance = 0;
	template< class T, class Outputer >
	ClsGroup< T, Outputer > * ClsGroup_GetInstance< T, Outputer >::s_pInstance = 0;

	template< class Outputer >
	unsigned int ClsGroup_Base< Outputer >::num = 0;

}

using Observer::ClsWrapper;
using Observer::ClsObserver;