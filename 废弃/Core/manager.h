/** 
 * @file Manager.h
 * @brief 对象管理器 函数管理器
 * @author Ladace
 * @version 1.0.0.1
 * @date 2009-10-24
 */

//*****************************************************
//                                             Ladace
// OneU Manager
//对象管理器
// 2009.10
//函数管理器 2009.10.14
//测试中…………保留
//
//*****************************************************

#include "OneULib.h"

#include <list> //保留 最好使用SGI版本

//*****************************************************
//错误处理部分 可写到一个头文件
//目前还没加入到代码中
//*****************************************************
#ifdef _DEBUG
#include <assert.h>
#ifndef ASSERT
#define ASSERT(exp) assert(exp)
#endif
#else
#define ASSERT(exp) void( 0 )
#endif
//*************************

//*************************
#include "memory.h"
//*************************

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 对象管理与函数管理
	 */
	/* ----------------------------------------------------------------------------*/
	namespace Manager
	{
		//****************************************************************************
		//!\defgroup Hook 钩子系统
		//!关于成员函数的钩子
		//!
		//!注：经汇编代码测试发现无论T是什么代码都是一样的
		//!MemCall是一个需要强制转换的基本类 ClassCall是一个模板的ADAPTER 会招致代码增多
		//!对于无参无返回值的 定义为Call
		//*****************************************************************************

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief  成员函数命令钩子【一般】
		 * \ingroup Hook
		 */
		/* ----------------------------------------------------------------------------*/
		//
		class MemCall
		{
		public:
			typedef void ( NullClass:: * FuncPointer) ();
			typedef NullClass ClassType;
		private:
			ClassType * const _pObj;
			FuncPointer const _pFunc;
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 构造函数
			 * 
			 * @param pFunc NullClass成员函数指针
			 * @param pObj NullClass对象指针
			 */
			/* ----------------------------------------------------------------------------*/
			//
			MemCall( FuncPointer pFunc, ClassType * pObj)
				:_pFunc( pFunc ), _pObj( pObj ){}
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief  执行钩子里的函数
			 */
			/* ----------------------------------------------------------------------------*/
			void operator() () { ( _pObj->*_pFunc )(); }
		};


		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 类成员函数钩子
		 *
		 * MemCall的Adapter
		 * \tparam T 成员函数所属类
		 * \ingroup Hook
		 */
		/* ----------------------------------------------------------------------------*/
		template< class T >
		class ClassCall
		{
		public:

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 函数指针类型
			 */
			/* ----------------------------------------------------------------------------*/
			typedef void (T::* FuncPointer)();

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 类的类型
			 */
			/* ----------------------------------------------------------------------------*/
			typedef T ClassType;
		private:
			MemCall _m;
		public:

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 构造函数
			 * 
			 * @param pFunc T的成员函数类型
			 * @param pObj T的对象类型
			 */
			/* ----------------------------------------------------------------------------*/
			ClassCall( FuncPointer pFunc, ClassType *pObj)
				:_m( reinterpret_cast< MemCall::FuncPointer > ( pFunc ), 
				reinterpret_cast< MemCall::ClassType * > ( pObj ) )
			{}
			void operator() () { _m(); }
		};

		//*****************************************************************************
		//!命令钩子管理类
		//!
		//!维护一个钩子链表
		//!\ingroup Hook
		//*****************************************************************************
		class CallManager
		{
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 链表类型
			 */
			/* ----------------------------------------------------------------------------*/
			typedef ::std::list< MemCall > ListType;
		private:
			ListType m_List;
		public:


			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 从最后加入钩子
			 * 
			 * @param pFunc 成员函数指针
			 * @param pObj 对象指针
			 * 
			 * @return 该类的链表迭代器，表示插入后的位置
			 */
			/* ----------------------------------------------------------------------------*/
			template < class T >
			ListType::iterator PushBack( void ( T::* pFunc)(), T * pObj )
			{
				m_List.push_back( 
					MemCall( reinterpret_cast< MemCall::FuncPointer > ( pFunc ), 
					reinterpret_cast< MemCall::ClassType * > ( pObj ) ) );
				return m_List.end();
			}
			

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 移除钩子
			 * 
			 * @param index 钩子对应迭代器
			 */
			/* ----------------------------------------------------------------------------*/
			void Remove( ListType::iterator index )
			{
				m_List.erase( index );
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 清除全部钩子函数
			 */
			/* ----------------------------------------------------------------------------*/
			void Clear()
			{
				m_List.clear();
			}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 依次执行各个钩子函数
			 */
			/* ----------------------------------------------------------------------------*/
			void Run()
			{
				for( ListType::iterator i=m_List.begin(); i != m_List.end(); i++)

					(*i)();
			}
		};

		//*****************************************************************************
		//管理对象 对象池系统
		//对象池总共有十个 
		//主要操作：向池中放入对象；将整个池放空
		//用模板实现配置空间配置器的要求
		//*****************************************************************************


		//!\defgroup ObjectWrapper 对象外覆器
		//!对象外覆器 用于实现正确的删除对象

		//!\defgroup ObjectPool 对象池系统

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 对象外覆器基类
		 *
		 * 为了实现多态产生的基类
		 * 并包含了配置空间配置器的功能
		 * \tparam Alloc 配置该类空间的空间配置器
		 * \ingroup ObjectWrapper 
		 * \ingroup ObjectPool
		 */
		/* ----------------------------------------------------------------------------*/
		//
		template< class Alloc = alloc >
		class ObjectWrapper_Base
		{
		public:
			virtual ~ObjectWrapper_Base() = 0 {}

			static void * operator new( size_t size )
			{
				return Alloc::allocate( size );
			}
			static void operator delete( void *p, size_t size )
			{
				return Alloc::deallocate( p, size );
			}
		};

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 对象外覆器
		 *
		 * 可支持按删除函数删除对象
		 * \tparam T 包裹的对象类型
		 * \tparam Alloc 配置该类空间的空间配置器
		 * \ingroup ObjectWrapper
		 * \ingroup ObjectPool
		 */
		/* ----------------------------------------------------------------------------*/
		template < class T , class Alloc = alloc >
		class ObjectWrapper
			: public ObjectWrapper_Base < Alloc >
		{
		public:
			typedef T ClassType;

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 删除器 
			 *
			 * 管理对包裹对象内存的卸载。
			 * 为标准空间配置器卸载内存的函数声明。
			 */
			/* ----------------------------------------------------------------------------*/
			typedef void ( *DeleterPointer )( void * , size_t );
		private:
			ClassType * const _p;
			DeleterPointer const _pDeleter;
		public:
			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 构造函数
			 * 
			 * @param pObj 包裹的对象指针
			 * @param pDeleter 删除器指针
			 */
			/* ----------------------------------------------------------------------------*/
			ObjectWrapper( ClassType * pObj, DeleterPointer pDeleter )
				:_p( pObj ), _pDeleter( pDeleter ) {}

			/* ----------------------------------------------------------------------------*/
			/** 
			 * @brief 析构函数
			 */
			/* ----------------------------------------------------------------------------*/
			~ObjectWrapper();
		};

		template < class T , class Alloc >
		ObjectWrapper< T, Alloc >::~ObjectWrapper()
		{
			if( !_pDeleter )
				delete _p;
			else
			{
				_p->~ClassType();
				( * _pDeleter )( reinterpret_cast < void * > ( _p ), sizeof( ClassType ) );
			}
		}

		//*****************************************************************************
		//!带有配置器信息的外覆器
		//!
		//!用模板元技术实现的 大小等于一个指针 但要增大生成代码量
		//!不过一般而言对于特定对象其分配器都是固定的 所以不会代码不会膨胀得很离谱
		//!
		//!\ingroup ObjectWrapper
		//!\ingroup ObjectPool
		//*****************************************************************************
		template< class T, class ObjAlloc, class Alloc = alloc >
		class ObjectWrapper_A
			:public ObjectWrapper_Base < Alloc >
		{
		public:
			ObjectWrapper_A( T * pObj )
				:_p( pObj ){}
			~ObjectWrapper_A();

			T * GetPointer(){ return _p }

			typedef T ClassType;
			typedef ObjAlloc AllocatorType;
		private:
			T * const _p;
		};

		template< class T, class ObjAlloc, class Alloc >
		ObjectWrapper_A< T, ObjAlloc, Alloc >::~ObjectWrapper_A()
		{
			_p->~ClassType();
			ObjAlloc::deallocate( _p, sizeof( ClassType ) );
		}

		//*****************************************************************************
		//!对象池
		//!
		//!注：这里的Alloc是Wrapper的分配器 而不是Object本身、Pool的分配器
		//!这个东西涉及到内存分配 应设计出默认内存分配器 保留
		//!\ingroup ObjectPool
		//*****************************************************************************
		template< class Alloc = alloc >
		class _ObjectPool
		{
		private:
			//删除器 内容是管理对内存的卸载  在那之前要先调用析构函数
			typedef void ( *DeleterPointer )( void * , size_t );
			typedef ::std::list< ObjectWrapper_Base< Alloc > * > ListType;

			ListType m_List;

		public:
			template < class T >
			void Attach( T * pObj,DeleterPointer pDeleter = 0 );

			template < class T, class ObjAlloc >
			void Attach( ObjectWrapper_A< T , ObjAlloc, Alloc > * pWrp )
			{
				m_List.push_back( pWrp );
			}
			void Clear();

			bool IsEmpty() const{ return m_List.empty(); }

			~_ObjectPool();
		};

		template< class Alloc >
		_ObjectPool< Alloc >::~_ObjectPool()
		{
			Clear();
		}

		template< class Alloc >
		template< class T >
		void _ObjectPool< Alloc >::Attach( T * pObj, DeleterPointer pDeleter )	
		{
			typedef ObjectWrapper< T, Alloc > Wrapper;

			Wrapper *p = new Wrapper( pObj, pDeleter );
			m_List.push_back( p );
		}

		template< class Alloc >
		void _ObjectPool< Alloc >::Clear()
		{
			for( ListType::iterator i = m_List.begin(); i != m_List.end(); i++)

				delete ( *i );

			m_List.clear();
		}

		//*****************************************************************************
		//!对象管理器
		//!
		//!封装了n个对象池 支持将对象插入到特定对象池中	
		//!注：这里的Alloc是Wrapper的分配器 而不是Object本身的分配器
		//!\ingroup ObjectPool
		//*****************************************************************************
		template< int n, class Alloc = alloc >
		class _ObjectManager
		{
		private:
			typedef void ( *DeleterPointer )( void * , size_t );
			_ObjectPool< Alloc > m_Pool[ n ];
		public:
			template< class T >
			T * Attach( int index, T * pObj, DeleterPointer pDeleter =0)
			{
				m_Pool[ index ].Attach( pObj, DeleterPointer );
				return pObj;
			}

			template< class T, class ObjAlloc >
			T * Attach( int index, ObjectWrapper_A< T, ObjAlloc, Alloc > * pWrp )
			{
				m_Pool[ index ].Attach( pWrp );
				return pWrp->GetPointer();
			}

			void Clear( int index )
			{
				m_Pool[ index ].Clear();
			}
		};

		//****************************************************************
		//!对象管理器增强
		//!
		//!提供函数管理
		//!\ingroup ObjectPool
		//****************************************************************
		template< int n, class Alloc = alloc >
		class _ObjectManagerW
			:public _ObjectManager< n, Alloc >
		{
		private:
			CallManager m_CallMgr[ n ];
		public:
			template< class T >
			void InsertFunction( int index, void (T::* pFunc) (), T * pObj)
			{
				m_CallMgr[ index ].PushBack( pFunc, pObj );
			}
			void Clear( int index )
			{
				m_Pool[ index ].Clear();
				m_CallMgr[ index ].Clear();
			}

			void Run()
			{
				for( int i = 0; i < n; i++ )
					m_CallMgr[ i ].Run();
			}

		};


		//*****************************************************************************
		//!对象创建仿函数
		//                           
		//!只要传入构造函数参数便可自动创造所需对象
		//!需要和对象管理器合作
		//!主要解决问题是实现对创建对象内存的管理
		//!\ingroup ObjectPool
		//                            2009.10.16
		//*****************************************************************************
		template< class T, class ObjAlloc = alloc, class Alloc = alloc >
		struct CreateObject
		{
			typedef Alloc AllocatorType;
			typedef T ClassType;
			typedef ObjectWrapper_A< T, ObjAlloc, Alloc > WrapperType;

			WrapperType * operator()( );

			template < class Arg1 >
			WrapperType * operator()( Arg1 & arg1 );

			template < class Arg1, class Arg2 >
			WrapperType * operator()( Arg1 & arg1, Arg2 & arg2 );
		};

		template< class T, class ObjAlloc, class Alloc >
		typename CreateObject< T, ObjAlloc, Alloc >::WrapperType *
			CreateObject< T, ObjAlloc, Alloc >::operator()( )
		{
			void * p = Alloc::allocate( sizeof( T ) );
			T * ret = new ( p ) T;
			return new WrapperType( ret );
		}

		template< class T, class ObjAlloc, class Alloc >
		template< class Arg1 >
		typename CreateObject< T, ObjAlloc, Alloc >::WrapperType *
			CreateObject< T, ObjAlloc, Alloc >::operator()( Arg1 & arg1 )
		{
			void * p = Alloc::allocate( sizeof( T ) );
			T * ret = new ( p ) T ( arg1 );
			return new WrapperType( ret );
		}

		template< class T, class ObjAlloc, class Alloc >
		template< class Arg1, class Arg2 >
		typename CreateObject< T, ObjAlloc, Alloc >::WrapperType *
			CreateObject< T, ObjAlloc, Alloc >::operator()( Arg1 & arg1, Arg2 & arg2 )
		{
			void * p = Alloc::allocate( sizeof( T ) );
			T * ret = new ( p ) T ( arg1, arg2 );
			return WrapperType( ret );
		}

		//*****************************************************************************
		//提供的typedef
		//保留 应定义到别处
		//*****************************************************************************


		//!默认的对象池
		typedef _ObjectPool<> ObjPool;
		//!\brief 默认的对象管理器
		//!
		//!带有7个对象池
		typedef _ObjectManager< 7 > ObjMgr;
		//!默认的增强对象管理器
		typedef _ObjectManagerW< 7 > ObjMgrW;
		//typedef ObjectWrapper_A ;
	}
}
