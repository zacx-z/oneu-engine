#ifndef __FLYARRAY_H
#define __FLYARRAY_H
/////////////////////////////////////////
/////////////////////////////////////////
//author ladace
//FlyArray管理轻量对象数组 提供便利操作 T可以为除了引用的任何东西
//ForEach可用来调用对象的成员函数或第一个参数为对象的函数（第一个参数一定为该对象这一点要修改 保留）
//LazyFunc同上，不过是可用返回值的。而且为了效率惰性执行的。可链式使用，使用完必须用Apply函数或在左侧加上Yi::A<<=
//二者参数都可用大小大于等于原数组大小的FlyArray，会将自动对应
//二者可显式指定函数类型 顺序为 返回值类型 第一个参数类型 第二个参数类型……
//为了能将上条功能人性化Yi::F函数能自动生成参数表那样的一个FlyArray数组。可显式指定类型。

//使用const_cast的地方是dirty trick 避免更麻烦的问题
//无法处理参数为引用的函数，因为引用和非引用调用的时候看起来一样 无法隐式推断出类型为引用 且__LazyFuncTag等没有考虑T为引用的情况
#include <boost\preprocessor\repetition\repeat.hpp>
#include <boost\preprocessor\punctuation\comma_if.hpp>
#include <boost\preprocessor\arithmetic\sub.hpp> 

#include <boost\type_traits\is_class.hpp>
#include <boost\type_traits\add_reference.hpp>
#include <boost\type_traits\add_pointer.hpp>
#include <boost\type_traits\remove_pointer.hpp>

#include <boost\static_assert.hpp>

#include "Array.h"
#define MAX_ARG_NUM  10 //最大支持参数数目 + 1

//为了预处理元编程而设立的宏 在文件末尾Undef
#define REPEAT_TEXT( z, n, dt ) BOOST_PP_COMMA_IF( n ) dt
#define REPEAT_TEXTN( z, n, dt ) BOOST_PP_COMMA_IF( n ) dt##n

#define REPEAT_ARG_TN( z, n, t ) BOOST_PP_COMMA_IF(n) T##n _arg##n
#define REPEAT_ARG_TN_CREF( z, n, t ) BOOST_PP_COMMA_IF( n ) const T##n &_arg##n

#define REPEAT_DEFAULT_CLASS_TN( z, n, t ) BOOST_PP_COMMA_IF( n ) class Arg##n = unused
#define REPEAT_UNUSED( z, n, t ) BOOST_PP_COMMA_IF( n ) unused

#define REPEAT_DEFINE_ARG_MEM_REF( z, n, t ) T##n &_arg##n;
#define REPEAT_INITIALIZE_ARG_MEM( z, n, t ) BOOST_PP_COMMA_IF( n ) _arg##n ( _arg##n )
#define REPEAT_INITIALIZE_ARG_MEM_REF( z, n, t ) BOOST_PP_COMMA_IF( n ) _arg##n ( const_cast< T##n& >( _arg##n ) )


namespace OneU
{

	template< class T, UINT N > class FlyArray;
	namespace Yi
	{
		struct __unusedTag;
		typedef __unusedTag* unused;

		//前置声明
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_DEFAULT_CLASS_TN, 0 ) >
		struct __LazyFuncTag;

		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_DEFAULT_CLASS_TN, 0 ) >
		struct __LazyMemFuncTag;


		//================================================================
		//处理某个参数也是FlyArray或Lazy函数或表达式模板结果的函数
		//__ExpandArg定义在文件最后
		//================================================================

		//==================================================
		//FlyArray特性萃取器
		//当T为FlyArray时 value_type为Array内含的类型
		//T为Lazy函数结果或表达式模板时 value_type为返回值类型
		//
		//注：T为表达式模板的特化在文件后面 因此以后写代码的时候表达式模板相关类的特化应在特性萃取器特化后写 否则会出现问题
		//其余情况为T
		//==================================================
		template< class T >
		struct __FlyArray_Traits
		{
			typedef T value_type;
			enum{ size = 1 };
		};
		template< class T, UINT N >
		struct __FlyArray_Traits< FlyArray< T, N > >
		{
			typedef T value_type;
			enum{ size = N };
		};
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		struct __FlyArray_Traits< __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > >
		{
			typedef Ret value_type;
			enum{ size = Left::size };
		};
		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		struct __FlyArray_Traits< __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > >
		{
			typedef Ret value_type;
			enum{ size = Left::size };
		};

		//获取多个类型的size属性 以第一个取得到非1的size为准
		template< BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		struct __GetMulti_T_Size
		{
#define REPEAT_GET_TN_SIZE( z, n, t ) __FlyArray_Traits< T##n >::size !=1 ? __FlyArray_Traits< T##n >::size :
			enum{ size = BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_GET_TN_SIZE, 0 ) 1 };
#undef REPEAT_GET_TN_SIZE
		};

#define REPEAT_EXPAND_ARG( z, n, i ) BOOST_PP_COMMA_IF( n ) __ExpandArg( _arg##n, i )
#define REPEAT_TRAITS_VALUETYPE_TN( z, n, t ) BOOST_PP_COMMA_IF( n ) typename __FlyArray_Traits< T##n >::value_type

		//===================================================================
		//表达式模板系列
		//===================================================================

		template< class T >
		struct error_cant_deduce_template_argument{ typedef T type; };

		//二元运算符
		//如果推测运算符返回原来的类型 用如下宏定义
#define TEMPLATE_LAZY_OP_2( Name, Op )\
		template< class Left, class Right >\
		struct Name\
		{\
			/*出现此错误表示表达式模板不能判断该重载运算符返回值的类型，需要显示指定*/\
			typedef Name< Left, Right > ThisType;\
			typedef typename error_cant_deduce_template_argument< typename Left::value_type >::type value_type;\
			enum{ size = Left::size };\
			Left &lhs;\
			Right &rhs;\
			Name( const Left &lhs, const Right &rhs )\
				: lhs( const_cast< Left& >( lhs ) ), rhs( const_cast< Right& >( rhs ) )\
			{}\
			value_type operator[]( UINT index )\
			{\
				return lhs[index] Op __ExpandArg( rhs, index );\
			}\
			template< class T >\
			__LazyEqual< ThisType, T > operator=( const T &rhs )\
			{\
				return __LazyEqual< ThisType, T >( *this, rhs );\
			}\
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )\
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }\
			template< class Ret >\
			Ret operator[]( UINT index )\
			{\
				return lhs[index] Op __ExpandArg( rhs, index );\
			}\
			void Apply()\
			{\
				for( int i = 0; i < Left::size; ++i ) operator[]( i );\
			}\
		};
		//如果推测运算符内置情况返回引用 则用如下宏
#define TEMPLATE_LAZY_OPE_2( Name, Op )\
		template< class Left, class Right >\
		struct Name\
		{\
			typedef typename boost::add_reference<\
				typename error_cant_deduce_template_argument< typename Left::value_type >::type\
				>::type value_type;\
			typedef Name< Left, Right > ThisType;\
			enum{ size = Left::size };\
			Left &lhs;\
			Right &rhs;\
			Name( const Left &lhs, const Right &rhs )\
				: lhs( const_cast< Left& >( lhs ) ), rhs( const_cast< Right& >( rhs ) )\
			{}\
			/*出现此错误表示表达式模板不能判断该重载运算符返回值的类型，需要显示指定*/\
			\
			value_type operator[]( UINT index )\
			{\
				return lhs[index] Op __ExpandArg( rhs, index );\
			}\
			template< class T >\
			__LazyEqual< ThisType, T > operator=( T &rhs )\
			{\
				return __LazyEqual< ThisType, T >( *this, rhs );\
			}\
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )\
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }\
			template< class Ret >\
			Ret operator[]( UINT index )\
			{\
				return lhs[index] Op __ExpandArg( rhs, index );\
			}\
			void Apply()\
			{\
				for( int i = 0; i < Left::size; ++i ) operator[]( i );\
			}\
		};
		TEMPLATE_LAZY_OPE_2( __LazyEqual, = )
		TEMPLATE_LAZY_OP_2( __LazyPlus,+ )
		TEMPLATE_LAZY_OP_2( __LazyMinus, - )
		TEMPLATE_LAZY_OP_2( __LazyMul, * )
		TEMPLATE_LAZY_OP_2( __LazyDiv, / )
		TEMPLATE_LAZY_OPE_2( __LazyPlusE, += )
		TEMPLATE_LAZY_OPE_2( __LazyMinusE, -= )
		TEMPLATE_LAZY_OPE_2( __LazyMulE, *= )
		TEMPLATE_LAZY_OPE_2( __LazyDivE, /= )
		TEMPLATE_LAZY_OPE_2( __LazyIn, << )
		TEMPLATE_LAZY_OPE_2( __LazyOut, >> )
#undef TEMPLATE_LAZY_OP_2
		//====================================
		//一元运算符 只提供2个
		//解指针
		template< class Left >
		struct __LazyDef
		{
			typedef typename error_cant_deduce_template_argument< typename boost::add_reference<
				typename boost::remove_pointer< typename Left::value_type >::type
					>::type >::type value_type;
			typedef __LazyDef< Left > ThisType;
			enum{ size = Left::size };
			Left &lhs;
			__LazyDef( const Left &lhs )
				: lhs( const_cast< Left& >( lhs ) )
			{}
			value_type operator[]( UINT index )
			{
				return *lhs[index];
			}
			template< class T >
			__LazyEqual< ThisType, T > operator=( const T &rhs )
			{
				return __LazyEqual< ThisType, T >( *this, rhs );
			}
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }
			//似乎没用 保留
			template< class Ret >
			Ret operator[]( UINT index )
			{
				return *lhs[index];
			}
			void Apply()
			{
				for( int i = 0; i < Left::size; ++i ) operator[]( i );\
			}
		};
		//取地址
		template< class Left >
		struct __LazyAdr
		{
			typedef typename error_cant_deduce_template_argument< typename boost::add_pointer< typename Left::value_type >::type >::type value_type;
			enum{ size = Left::size };
			typedef __LazyAdr< Left > ThisType;
			Left &lhs;
			__LazyAdr( const Left &lhs )
				: lhs( const_cast< Left& >( lhs ) )
			{}
			value_type operator[]( UINT index )
			{
				return &lhs[index];
			}
			template< class T >
			__LazyEqual< ThisType, T > operator=( const T &rhs )
			{
				return __LazyEqual< ThisType, T >( *this, rhs );
			}
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }
			//似乎没用 保留
			template< class Ret >
			Ret operator[]( UINT index )
			{
				return &lhs[index];
			}
			void Apply()
			{
				for( int i = 0; i < Left::size; ++i ) operator[]( i );\
			}
		};
		//为一系列结构体重载一元运算符
#define OPERATOR_1( Left, Name, Op )\
		template< class A >\
		Name< Left< A > > operator Op( const Left< A > &lhs )\
		{ return Name< Left< A > >( lhs );	}
#define OPERATOR_2_1( Left, Name, Op )\
		template< class A, class B >\
		Name< Left< A, B > > operator Op( const Left< A, B > &lhs )\
		{ return Name< Left< A, B > >( lhs ); }
#define OPERATOR_1_SINGLE( Name, Op )\
		OPERATOR_1( __LazyDef, Name,  Op )\
		OPERATOR_1( __LazyAdr, Name,  Op )\
		OPERATOR_2_1( __LazyEqual, Name,  Op )\
		OPERATOR_2_1( __LazyPlus,  Name,  Op )\
		OPERATOR_2_1( __LazyMinus, Name,  Op )\
		OPERATOR_2_1( __LazyMul,   Name,  Op )\
		OPERATOR_2_1( __LazyDiv,   Name,  Op )\
		OPERATOR_2_1( __LazyPlusE, Name,  Op )\
		OPERATOR_2_1( __LazyMinusE,Name,  Op )\
		OPERATOR_2_1( __LazyMulE,  Name,  Op )\
		OPERATOR_2_1( __LazyDivE,  Name,  Op )\
		OPERATOR_2_1( __LazyIn,    Name,  Op )\
		OPERATOR_2_1( __LazyOut,   Name,  Op )

		OPERATOR_1_SINGLE( __LazyDef, * )
		OPERATOR_1_SINGLE( __LazyAdr, & )
#undef OPERATOR_1_SINGLE
#undef OPERATOR_2_1
#undef OPERATOR_1
		//为一系列结构体重载二元运算符
#define OPERATOR_1_2( Left, Name, Op )\
		template< class T, class A >\
		Name< Left< A >, T > operator Op( const Left< A > &lhs, const T& rhs )\
		{ return Name< Left< A >, T >( lhs, rhs ); }
#define OPERATOR_2( Left, Name, Op )\
		template< class T, class A, class B >\
		Name< Left< A, B >, T > operator Op( const Left< A, B > &lhs, const T& rhs )\
		{ return Name< Left< A, B >, T >( lhs, rhs ); }
#define OPERATOR_2_SINGLE( Name, Op )\
		OPERATOR_1_2( __LazyDef, Name,  Op )\
		OPERATOR_1_2( __LazyAdr, Name,  Op )\
		OPERATOR_2( __LazyEqual, Name,  Op )\
		OPERATOR_2( __LazyPlus,  Name,  Op )\
		OPERATOR_2( __LazyMinus, Name,  Op )\
		OPERATOR_2( __LazyMul,   Name,  Op )\
		OPERATOR_2( __LazyDiv,   Name,  Op )\
		OPERATOR_2( __LazyPlusE, Name,  Op )\
		OPERATOR_2( __LazyMinusE,Name,  Op )\
		OPERATOR_2( __LazyMulE,  Name,  Op )\
		OPERATOR_2( __LazyDivE,  Name,  Op )\
		OPERATOR_2( __LazyIn,    Name,  Op )\
		OPERATOR_2( __LazyOut,   Name,  Op )

		OPERATOR_2_SINGLE( __LazyPlus,  + )
		OPERATOR_2_SINGLE( __LazyMinus, - )
		OPERATOR_2_SINGLE( __LazyMul,   * )
		OPERATOR_2_SINGLE( __LazyDiv,   / )
		OPERATOR_2_SINGLE( __LazyPlusE, += )
		OPERATOR_2_SINGLE( __LazyMinusE,-= )
		OPERATOR_2_SINGLE( __LazyMulE,  *= )
		OPERATOR_2_SINGLE( __LazyDivE,  /= )
		OPERATOR_2_SINGLE( __LazyIn,    << )
		OPERATOR_2_SINGLE( __LazyOut,   >> )
#undef OPERATOR_2_SINGLE
#undef OPERATOR_1_2
#undef OPERATOR_2

		//================================================================
		//负责Lazy函数的类
		//处理类似表达式模板的事务
		//=======================================================
		//=========================================
		//Lazy自由函数类的定义
		//为了降低编程复杂度 最后一个TN永远是UnUsed 因此支持参数数目为MAX_ARG_NUM - 1
		//需要根据类的参数数目不同而进行偏特化
		//=================================================
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		struct __LazyFuncTag
		{};	


#define REPEAT_LAZYFUNC_SPECIALIZE( z, n, t )\
		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
		struct __LazyFuncTag< Ret, BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) >\
		{\
			typedef __LazyFuncTag< Ret, BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) > ThisType;\
			enum{ size = __GetMulti_T_Size< BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) >::size };\
			typedef Ret value_type;\
			\
			typedef Ret (*FuncPointer)( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) );\
			FuncPointer _pFunc;\
			BOOST_PP_REPEAT_##z( n, REPEAT_DEFINE_ARG_MEM_REF, 0 );\
			\
			__LazyFuncTag( FuncPointer pF BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
				: _pFunc( pF ) BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_INITIALIZE_ARG_MEM_REF, 0 )\
			{}\
			Ret operator[]( UINT index )\
			{\
				return ( *_pFunc )(\
					BOOST_PP_REPEAT_##z( n, REPEAT_EXPAND_ARG, index ) );\
			}\
			template< class U >\
			__LazyEqual< ThisType, U > operator=( const U& rhs )\
			{\
				return __LazyEqual< ThisType, U >( *this, rhs );\
			}\
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )\
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }\
			void Apply()/*让Lazy函数执行*/\
			{\
				for( int i = 0; i < size; ++i )\
					operator[]( i );\
			}\
		};/*根据参数数目偏特化*/
		BOOST_PP_REPEAT( MAX_ARG_NUM , REPEAT_LAZYFUNC_SPECIALIZE, 0 )
#undef REPEAT_LAZYFUNC_SPECIALIZE

		//=========================================
		//Lazy成员函数类的定义
		//需要根据类的参数数目不同而进行偏特化
		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		struct __LazyMemFuncTag
		{};	
		
	
#define REPEAT_LAZYMEMFUNC_SPECIALIZE( z, n, t )\
		template< class Left, class T, class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
		struct __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) >\
		{\
			typedef __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
					BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) > ThisType;\
			enum{ size = Left::size };\
			typedef Ret value_type;\
			Left &_v;\
			typedef Ret (T::*FuncPointer)( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) );\
			FuncPointer _pFunc;\
			BOOST_PP_REPEAT_##z( n, REPEAT_DEFINE_ARG_MEM_REF, 0 );\
			\
			__LazyMemFuncTag( Left &v, FuncPointer pF BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
				: _v( v ), _pFunc( pF ) BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_INITIALIZE_ARG_MEM_REF, 0 )\
			{}\
			Ret operator[]( UINT index )\
			{\
				return ( (&(_v[index])/*获得类型为T的对象*/)->*_pFunc )(\
				BOOST_PP_REPEAT_##z( n, REPEAT_EXPAND_ARG, index ) );\
			}\
			template< class U >\
			__LazyEqual< ThisType, U > operator=( const U& rhs )\
			{\
				return __LazyEqual< ThisType, U >( *this, rhs );\
			}\
			__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )\
			{ return __LazyEqual< ThisType, ThisType >( *this, rhs ); }\
			void Apply()/*让Lazy函数执行*/\
			{\
				for( int i = 0; i < size; ++i )\
					operator[]( i );\
			}\
		};/*根据参数数目偏特化*/
		BOOST_PP_REPEAT( MAX_ARG_NUM , REPEAT_LAZYMEMFUNC_SPECIALIZE, 0 )
#undef REPEAT_LAZYMEMFUNC_SPECIALIZE


		//===================================================================
		//
		//Lazy函数的表达式模板
		//需要为Lazy函数的Tag重载符号
		//===================================================================
		//一元运算符

#define LAZYM_OPERATOR_1( Name, Op )\
		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >\
		Name< __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > > operator Op(\
			const __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > &lhs )\
		{\
			return Name<  __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > >( lhs );\
		}
#define LAZY_OPERATOR_1( Name, Op )\
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >\
		Name< __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > > operator Op(\
			const __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > &lhs )\
		{\
			return Name<  __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > >( lhs );\
		}
		LAZY_OPERATOR_1( __LazyDef, * )
		LAZY_OPERATOR_1( __LazyAdr, & )
		LAZYM_OPERATOR_1( __LazyDef, * )
		LAZYM_OPERATOR_1( __LazyAdr, & )
#undef LAZYM_OPERATOR_1
#undef LAZY_OPERATOR_1
		//二元运算符
#define LAZYM_OPERATOR_2( Name, Op )\
		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ), class U >\
		Name< __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >, U > operator Op(\
			const __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > &lhs, const U &rhs )\
		{\
			return Name<  __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >, U >( lhs, rhs );\
		}

#define LAZY_OPERATOR_2( Name, Op )\
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ), class U >\
		Name< __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >, U > operator Op(\
			const __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) > &lhs, const U &rhs )\
		{\
			return Name<  __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >, U >( lhs, rhs );\
		}
		LAZY_OPERATOR_2( __LazyPlus,  + )
		LAZY_OPERATOR_2( __LazyMinus, - )
		LAZY_OPERATOR_2( __LazyMul,   * )
		LAZY_OPERATOR_2( __LazyDiv,   / )
		LAZY_OPERATOR_2( __LazyPlusE, += )
		LAZY_OPERATOR_2( __LazyMinusE,-= )
		LAZY_OPERATOR_2( __LazyMulE,  *= )
		LAZY_OPERATOR_2( __LazyDivE,  /= )
		LAZY_OPERATOR_2( __LazyIn,    << )
		LAZY_OPERATOR_2( __LazyOut,   >> )
		LAZYM_OPERATOR_2( __LazyPlus,  + )
		LAZYM_OPERATOR_2( __LazyMinus, - )
		LAZYM_OPERATOR_2( __LazyMul,   * )
		LAZYM_OPERATOR_2( __LazyDiv,   / )
		LAZYM_OPERATOR_2( __LazyPlusE, += )
		LAZYM_OPERATOR_2( __LazyMinusE,-= )
		LAZYM_OPERATOR_2( __LazyMulE,  *= )
		LAZYM_OPERATOR_2( __LazyDivE,  /= )
		LAZYM_OPERATOR_2( __LazyIn,    << )
		LAZYM_OPERATOR_2( __LazyOut,   >> )
#undef LAZY_OPERATOR_2

		//===================================================================
		//ForEach的外部函数版本
		//===================================================================
#define REPEAT_YI_FOREACH( z, n, t )\
		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ), class T, UINT N >\
		inline void ForEach( FlyArray< T, N >& _v, Ret( T::*pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) ) BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
		{\
			for( int i = 0; i < N; ++i )\
			{\
				( ( &_v[i] )->*pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_EXPAND_ARG, i ) );\
			}\
		}\
		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
		inline void ForEach( Ret( *pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) ) BOOST_PP_COMMA_IF( n )\
			BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
		{\
			for( int i = 0; i < __GetMulti_T_Size< BOOST_PP_REPEAT( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_TEXT, unused ) >::size; ++i )\
			{\
				( *pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_EXPAND_ARG, i ) );\
			}\
		}
		BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_YI_FOREACH, 0 )
		//如果这里出现错误 可能是
		//1.你的函数有重载版本（有重载版本需显式指定返回值和参数类型）
		//2.参数的数目或类型错误
		//3.显式指定的类型错误
#undef REPEAT_YI_FOREACH

		//LazyFunc函数
#define REPEAT_YI_LAZYFUNC( z, n, t )\
		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ),  class T, class Left >\
		inline __LazyMemFuncTag< Left, T, Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) >\
			LazyFunc( Left& _v,\
				Ret( T::*pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) ) BOOST_PP_COMMA_IF( n )\
				BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
		{\
			return __LazyMemFuncTag< Left, T,\
				Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) >\
				( _v, pFunc BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, _arg ) );\
		}\
		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
		inline __LazyFuncTag< Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) >\
			LazyFunc(\
				Ret( *pFunc )( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) ) BOOST_PP_COMMA_IF( n )/*这里只支持函数参数为FlyArray里的T而不是T的引用的*/\
				BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
		{\
			return __LazyFuncTag<\
				Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) >\
				( pFunc BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, _arg ) );\
		}
		BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_YI_LAZYFUNC, 0 )
		//如果这里出现错误 可能是
		//1.你的函数有重载版本（有重载版本需显式指定返回值和参数类型）
		//2.参数的数目或类型错误
		//3.显式指定的类型错误
#undef REPEAT_YI_LAZYFUNC

		//外部Apply函数
		//让Lazy函数执行
		template< class LazyFuncType >
		inline void Apply( LazyFuncType v )
		{
			v.Apply();
		}
		//可以在语句前面加上Yi::A<<=来让惰性函数或表达式执行
		struct __ATag
		{
			template< class LazyFuncType >
			void operator <<=( LazyFuncType v )
			{
				v.Apply();
			}
		};
		__ATag A;

		//======================================================
		//成员函数的缓式评估
		template< class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_DEFAULT_CLASS_TN, 0 ) >
		struct MemFunc
		{};
#define REPEAT_MEMFUNC_SPECIALIZE( z, n, t )\
		template< class T, class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
		struct MemFunc< T, Ret, BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T ) BOOST_PP_COMMA_IF( n )\
			BOOST_PP_REPEAT_##z( BOOST_PP_SUB( MAX_ARG_NUM, n ), REPEAT_UNUSED, 0 ) >\
		{\
			typedef Ret( T::*type )( BOOST_PP_REPEAT_##z( n, REPEAT_TRAITS_VALUETYPE_TN, 0 ) );\
		};
		BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_MEMFUNC_SPECIALIZE, 0 )
#undef REPEAT_MEMFUNC_SPECIALIZE

	}

	//轻量数组
	//提供一些便利计算
	//T为非引用的类型
	template< class T, UINT N >
	class FlyArray
		: public Array< T, N >
	{
	public:
		typedef FlyArray< T, N > ThisType;
	public:
		FlyArray()
		{
			//FlyArray一个标准是它要跟数组一样大 这样以便于它跟数组的无缝操作
			BOOST_STATIC_ASSERT( sizeof( ThisType ) == N * sizeof( T ) );
		}
//#define REPEAT_FOREACH( z, n, t )\
//		template< class Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
//		void ForEach( typename Yi::MemFunc< T, Ret BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT( n, REPEAT_TEXTN, T ) >::type pFunc\
//				BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
//		{\
//			Yi::ForEach( *this, pFunc BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, _arg ) );\
//		}/*这一版保证了你能显示指定返回值类型和参数类型，由于T不一定为Class，因此需要MemFunc来缓式评估*/\
//		template< class FuncPointer BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, class T ) >\
//		void ForEach( FuncPointer pFunc\
//				BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_ARG_TN_CREF, 0 ) )\
//		{\
//			Yi::ForEach( *this, pFunc BOOST_PP_COMMA_IF( n ) BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, _arg ) );\
//		}/*当不为重载函数时，这一版保证了你不需指定返回值和参数类型就能使用*/
//		BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_FOREACH, 0 )
//		//如果这里出现错误 可能是
//		//1.你的函数有重载版本（有重载版本需显式指定返回值和参数类型）
//		//2.参数的数目或类型错误
//		//3.显式指定的类型错误
//#undef REPEAT_FOREACH

		template< class U >
		Yi::__LazyEqual< ThisType, U > operator=( const U &rhs )
		{
			return Yi::__LazyEqual< ThisType, U >( *this, rhs );
		}
		Yi::__LazyEqual< ThisType, ThisType > operator=( const ThisType &rhs )//防止编译器的默认定义
		{
			return Yi::__LazyEqual< ThisType, ThisType >( *this, rhs );
		}
	};

	//====================================================
	//FlyArray对表达式的支持
	//为FlyArray重载各种运算符
	//====================================================

#define FLYARRAY_OPERATOR_1( Name, Op )\
	template< class T, UINT N >\
	Name< FlyArray< T, N > > operator Op( const FlyArray< T, N > &lhs )\
	{ return Name< FlyArray< T, N > >( lhs ); }

	FLYARRAY_OPERATOR_1( Yi::__LazyDef, * )
	FLYARRAY_OPERATOR_1( Yi::__LazyAdr, & )
#undef FLYARRAY_OPERATOR_1

#define FLYARRAY_OPERATOR_2( Name, Op )\
	template< class T, UINT N, class U >\
	Name< FlyArray< T, N >, U > operator Op( const FlyArray< T, N > &lhs, const U &rhs )\
	{ return Name< FlyArray< T, N >, U >( lhs, rhs ); }

	FLYARRAY_OPERATOR_2( Yi::__LazyPlus,  + )
	FLYARRAY_OPERATOR_2( Yi::__LazyMinus, - )
	FLYARRAY_OPERATOR_2( Yi::__LazyMul,   * )
	FLYARRAY_OPERATOR_2( Yi::__LazyDiv,   / )
	FLYARRAY_OPERATOR_2( Yi::__LazyPlusE, += )
	FLYARRAY_OPERATOR_2( Yi::__LazyMinusE,-= )
	FLYARRAY_OPERATOR_2( Yi::__LazyMulE,  *= )
	FLYARRAY_OPERATOR_2( Yi::__LazyDivE,  /= )
	FLYARRAY_OPERATOR_2( Yi::__LazyIn,    << )
	FLYARRAY_OPERATOR_2( Yi::__LazyOut,   >> )
#undef FLYARRAY_OPERATOR_2


	namespace Yi
	{	
		//==========================================
		//Yi::F生成FlyArray数组
		//==========================================
#define REPEAT_ASSIGN_V( z, n, t ) v[n] = v##n;
#define REPEAT_F( z, n, t )\
		template< class T >\
		inline FlyArray< T, n > F( BOOST_PP_REPEAT_##z( n, REPEAT_TEXTN, T v ) )\
		{\
			FlyArray< T, n > v;\
			BOOST_PP_REPEAT_##z( n, REPEAT_ASSIGN_V, 0 )\
			return v;\
		}
		//最大数组数目为20
		BOOST_PP_REPEAT( 20, REPEAT_F, 0 )
#undef REPEAT_F
#undef REPEAT_ASSIGN_V

		//================================================================
		//处理某个参数也是FlyArray或Lazy函数或表达式模板结果的函数
		//当参数是普通量 展开后为原来
		//当参数是Array 展开后为Array[index]
		//================================================================
		template< class T >
		inline T __ExpandArg( const T &arg, UINT index ){ return arg; }

		template< class T, UINT N >
		inline T __ExpandArg( const FlyArray< T, N >& arg, UINT index ){ return arg[index]; }
		template< class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		inline Ret __ExpandArg( const __LazyFuncTag< Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >& arg, UINT index ){ return arg[index]; }
		template< class Left, class T, class Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, class T ) >
		inline Ret __ExpandArg( const __LazyMemFuncTag< Left, T, Ret, BOOST_PP_REPEAT( MAX_ARG_NUM, REPEAT_TEXTN, T ) >& arg, UINT index ){ return arg[index]; }

#define EXPAND_OPERATOR_1( Name )\
		template< class A >\
		inline typename Name< A >::value_type __ExpandArg( Name< A > &arg, UINT index ){ return arg[index]; }
		EXPAND_OPERATOR_1( __LazyDef )
		EXPAND_OPERATOR_1( __LazyAdr )
#undef EXPAND_OPERATOR_1

#define EXPAND_OPERATOR_2( Name )\
	template< class A, class B >\
	inline typename Name< A, B >::value_type __ExpandArg( Name< A, B > &arg, UINT index ){ return arg[index]; }

		EXPAND_OPERATOR_2( __LazyEqual  )
		EXPAND_OPERATOR_2( __LazyPlus   )
		EXPAND_OPERATOR_2( __LazyMinus  )
		EXPAND_OPERATOR_2( __LazyMul    )
		EXPAND_OPERATOR_2( __LazyDiv    )
		EXPAND_OPERATOR_2( __LazyPlusE  )
		EXPAND_OPERATOR_2( __LazyMinusE )
		EXPAND_OPERATOR_2( __LazyMulE   )
		EXPAND_OPERATOR_2( __LazyDivE   )
		EXPAND_OPERATOR_2( __LazyIn     )
		EXPAND_OPERATOR_2( __LazyOut    )
#undef EXPAND_OPERATOR_2

		//特性萃取器对表达式的特化

#define VALUETYPE_TRAITS_OPERATOR_1( Name )\
		template< class A >\
		struct __FlyArray_Traits< Name< A > >\
		{\
			typedef typename Name< A >::value_type value_type;\
			enum{ size = Name< A >::size };\
		};
		VALUETYPE_TRAITS_OPERATOR_1( __LazyDef )
		VALUETYPE_TRAITS_OPERATOR_1( __LazyAdr )
#undef VALUETYPE_TRAITS_OPERATOR_1
#define VALUETYPE_TRAITS_OPERATOR_2( Name )\
		template< class A, class B >\
		struct __FlyArray_Traits< Name< A, B > >\
		{\
			typedef typename Name< A, B >::value_type value_type;\
			enum{ size = Name< A, B >::size };\
		};

		VALUETYPE_TRAITS_OPERATOR_2( __LazyEqual   )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyPlus    )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyMinus   )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyMul     )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyDiv     )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyPlusE   )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyMinusE  )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyMulE    )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyDivE    )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyIn      )
		VALUETYPE_TRAITS_OPERATOR_2( __LazyOut     )
#undef VALUETYPE_TRAITS_OPERATOR_2
	}
}

#undef REPEAT_DEFINE_ARG_MEM_REF
#undef REPEAT_INITIALIZE_ARG_MEM
#undef REPEAT_INITIALIZE_ARG_MEM_REF

#undef REPEAT_DEFAULT_CLASS_TN
#undef REPEAT_UNUSED

#undef REPEAT_ARG_TN
#undef REPEAT_ARG_TN_CREF

#undef REPEAT_TEXTN
#undef REPEAT_TEXT

#endif