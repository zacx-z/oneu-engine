//==========================
//Observer_Adv.h
//监视类的实例 模板元升级库
//
//使用该库会覆盖掉OBSERVER_CLASS_OUTPUT对类的定制
//
//这个库未必输出正确的结果，但不会引起程序崩溃。
//因为会涉及到编译器对内存的对齐问题
//确保类的成员都为4字节可让结果准确
//              Ladace
//              2010.2.7
//==========================
#pragma once
//可能引起错误的地方(编译器相关)
//
//VC编译器的规律：
//有些类内存不会对齐。比如都是char的类，如果既有char又有4字节的成员，结果不好说。而且对齐后可能导致成员占用的内存不连续。对齐时该类大小分别（派生类成员和所有基类，如果多重继承的话，所有基类是作为一个整体对齐的）对齐为4的倍数。比如A大小为9，B派生自A声明了9个char，占用空间为24而不是20。
//
//任何带虚函数的类内存会被对齐。
//
//另：！！！
//
//A派生自空类，B派生自A和另一空类，那么sizeof(B)减去所有成员的大小，你会发现还剩下1。好像就是为空类留着的。
//
//一个对象的内容可能是离散的 保证成员都为4字节类型可避免此问题
//

#include <boost/mpl/vector.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include "Observer.h"

#define OBSERVER_MEM_ALIGN_1( N ) ( N / 4 + 1 ) * 4
namespace Observer
{

	//虚函数指针结构 如果类是多态类Vector第一个用vfptr做参数，或使用ClsWrapper_Adv_Plm
	class vfptr
	{
	private:
		void *_p;
	public:
		void * GetPointer() const{ return _p; }
	};
	//

	//Declare为mpl::vector的一个实例 元素为类的成员类型
	template< class T, class Declare, class BaseClass = UnknownClass, class Outputer = DefaultOutputer >
	class ClsWrapper_Adv
	{
	private:
		enum{ M_Size = OBSERVER_MEM_ALIGN_1( sizeof( BaseClass ) ) - sizeof( BaseClass ) };
		char __holder[ M_Size ];//只占位 把多重继承基类大小之和对齐到4的倍数 看最上面的说明
	public:
		ClsWrapper_Adv()
		{
			ClsGroup_GetInstance< T, Outputer >().Adv<
				typename boost::mpl::push_front<
					typename boost::mpl::push_front<
						Declare, TMP::PlaceHolder< M_Size > //占位
					>::type, BaseClass 
				>::type >()->Insert( ( T * )this );
		}
		~ClsWrapper_Adv()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Remove( ( T * )this );
		}
	};
	template< class T, class Declare, class Outputer >
	class ClsWrapper_Adv< T, Declare, UnknownClass, Outputer >
	{
	public:
		ClsWrapper_Adv()
		{
			ClsGroup_GetInstance< T, Outputer >().Adv< Declare >()->Insert( ( T * )this );
		}
		~ClsWrapper_Adv()
		{
			ClsGroup_GetInstance< T, Outputer >()()->Remove( ( T * )this );
		}
	};

	template< class T, class Declare, class BaseClass = UnknownClass, class Outputer = DefaultOutputer >
	class ClsWrapper_Adv_Plm
		: public ClsWrapper_Adv< T, typename boost::mpl::push_front< Declare, vfptr >::type, BaseClass, Outputer >
	{};

	//
	template< class T, class Declare, class Outputer = DefaultOutputer >
	class ClsGroup_Base_Adv
		:public ClsGroup< T, Outputer >
	{
	public:
		void OutputElem( void * Elem )
		{
			BOOST_STATIC_ASSERT( TMP::SizeSum< Declare >::value <= sizeof( T ) );//会有编译器 内存对齐问题 所以断言小于等于
			TMP::DeclareOutputer< Declare, Outputer >()( Elem );
			//Outputer()( sizeof( T ) );
		}
	};
	template< class T, class Declare, class Outputer = DefaultOutputer >
	class ClsGroup_Adv
		: public ClsGroup_Base_Adv< T, Declare, Outputer >
	{

	};




	//////////////////////////////
	//模板元部分
	//
	namespace TMP
	{
		template< int size >
		class PlaceHolder//占size个字节的类
		{
			char a[ size ];
		};
		template<>
		class PlaceHolder< 0 >
		{};
		template< class Vector, class Iter > struct _ForEachSum;
		template< class Vector, class Iter > struct _SumOp;
		template< class Vector >
		struct SizeSum
		{
			typedef typename _ForEachSum< Vector, typename boost::mpl::begin< Vector >::type >::type type;
			enum{ value = type::value };
		};
		template< class Vector, class Iter >
		struct _ForEachSum
		{
			typedef typename boost::mpl::eval_if< boost::is_same< Iter, typename boost::mpl::end< Vector >::type >,
				boost::mpl::int_< 0 >,
				_SumOp< Vector, Iter > >::type type;
		};
		template< class Vector, class Iter >
		struct _SumOp
		{
			typedef boost::mpl::plus< boost::mpl::int_< sizeof( boost::mpl::deref< Iter >::type ) >,
				typename _ForEachSum< Vector, typename boost::mpl::next< Iter >::type >::type > type;
		};

		template< class Vector, class Outputer >
		struct DeclareOutputer
		{
			void operator()( void * Elem )
			{
				Outputer().NextLine();
				_ForEachOut< Vector, boost::mpl::begin< Vector >::type, Outputer >()( ( char * )Elem );
			}
		};

		struct _None
		{
			void operator()( void* ){}
		};

		template< class Vector, class Iter, class Outputer >
		struct _ForEachOut
		{
			void operator()( char * Elem )
			{
				typedef boost::mpl::deref< Iter >::type T;

				_Outputer< Outputer >()( ( T* )Elem );

				boost::mpl::eval_if< boost::is_same< typename boost::mpl::next< Iter >::type, typename boost::mpl::end< Vector >::type >,
					_None,
					_ForEachOut< Vector, typename boost::mpl::next< Iter >::type, Outputer > >()( Elem + sizeof( T ) );
			}
		};

		template< class Outputer >
		struct _Outputer
		{
			template< class T >
			void operator()( T * Elem )
			{
				Outputer().IncTab();
				boost::mpl::eval_if< boost::is_scalar< T >,
					_ScalarOutputer< Outputer >,
					_ObjOutputer< Outputer > >()( *Elem );
				Outputer().DecTab();
			}
		};
		template< class Outputer >
		struct _ScalarOutputer
		{
			template< class T >
			void operator()( const T & Elem )
			{
				Outputer()( typeid( T ).name() );
				Outputer()( L':' );
				Outputer()( Elem );
				Outputer().NextLine();
			}
		};
		template< class Outputer >
		struct _ObjOutputer
		{
			template< class T >
			void operator()( const T & Elem )
			{
				if( !ClsGroup_GetInstance< T, Outputer >().Exist() )
				{
					Outputer()( typeid( T ).name() );
					Outputer()( L':' );
					Outputer()( sizeof( T ) );
					Outputer()( "字节。" );
					Outputer().NextLine();
				}
				else
				{
					//由于此时为对象的输出 不具备多态（不是指针或引用） 因此不必要使用RTTI
					ClsGroup_GetInstance< T, Outputer >()()->OutputClassName();
					Outputer()( ":" );
					ClsGroup_GetInstance< T, Outputer >()()->OutputElem( ( void* )&Elem );
				}
			}
			template< int Size >
			void operator()( const PlaceHolder< Size > & Elem ){}
			template<>
			void operator()< vfptr >( const vfptr & Elem )
			{
				Outputer()( "vfptr" );
				Outputer()( L':' );
				Outputer()( Elem.GetPointer() );
				Outputer().NextLine();
			}
		};
	}
}

using Observer::ClsWrapper_Adv;