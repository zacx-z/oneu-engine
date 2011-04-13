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
 * @file Atom.h
 * @brief Atom总头文件
 * 
 * 使用Atom时请包含该头文件。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-02-15
 */
#pragma once
#include "OUEDefs.h"
#include "List.h"
//层次在Base之上 其余之下

namespace OneU
{
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @addtogroup group_atom Atom表系统
	 */
	/* ----------------------------------------------------------------------------*/
	//@{
	namespace atom
	{
		class value;
		class tuple;
		class function;
		class env;
		class klass;
		class obj;
		class obj_function;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 导出类基类
		 *
		 * 任何想要导出的Atom类要派生自该类。
		 */
		/* ----------------------------------------------------------------------------*/
		class exported{};

		class IInterpreter;
	}
	class IAtom
	{
	public:
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 执行垃圾清理
		 *
		 * 需要定期执行垃圾清理回收无效的value。
		 * 在OUE框架中默认每帧执行一次。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void gc() = 0;
		virtual ~IAtom(){}

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 获取控制权
		 *
		 * 控制权只有一个。如果该变量已经被获取控制权，则会获取控制权失败。
		 * 获取该变量控制权的上下文负责控制变量的生存期，在必要的时机调用unacquire。
		 * 一般通过调用@ref atom::value::acquire实现相同功能。
		 * 
		 * @param v 变量
		 * 
		 * @returns 表示变量控制权获取成功与否
		 * @retval true 获取变量控制权成功
		 * @retval false 获取变量控制权失败
		 * 
		 * @sa atom::value::acquire
		 */
		/* ----------------------------------------------------------------------------*/
		virtual bool acquireValue(atom::value* v) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 丢弃控制权
		 *
		 * 丢弃变量的控制权会使其加入到GC列表里，在下次垃圾收集时被释放。
		 * 一般通过调用@ref atom::value::unacquire实现相同功能。
		 *
		 * @remarks 如果该变量控制权已经被丢弃了则没有任何行为。
		 *
		 * @param v 变量
		 * @sa atom::value::unacquire
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void unacquireValue(atom::value* v) = 0;

		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 得到当前环境表
		 * 
		 * @returns 当前环境表地址
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::env* getEnv() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 得到系统环境表
		 * 
		 * @returns 系统环境表地址
		 *
		 * @remarks 系统环境表里面存放一些涉及底层编程的元素。例如DXDevice的指针，慎用。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::env* getSystemEnv() = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 得到环境表变量
		 *
		 * @return 当前环境表变量
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::value* getEnvValue() = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 得到系统环境表
		 *
		 * @return 系统环境表变量
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::value* getSystemEnvValue() = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		* @brief 获取此变量的描述
		* 
		* @param v 变量
		* @param buffer 字符串缓冲区，描述会追加到该字符串上。
		*/
		/* ----------------------------------------------------------------------------*/
		virtual void getDescription(atom::value* v, OneU::String& buffer) = 0;
		/* ----------------------------------------------------------------------------*/
		/** 
		 * @brief 错误
		 * 
		 * 在使用Atom时请用该函数向Atom提交错误。
		 *
		 * @param error_message 错误信息。
		 * @remarks 永不返回。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void raise(pcwstr error_message) = 0;
		virtual void raise(pcwstr error_message, pcstr source_file, int source_line) = 0;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 加载Atom库文件
		 *
		 * @param path 库的相对路径或绝对路径。
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void loadAtomLib(pcwstr path) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 卸载所有Atom库文件 
		 */
		/* ----------------------------------------------------------------------------*/
		virtual void unloadAllAtomLib() = 0;

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 解释语言 
		 */
		/* ----------------------------------------------------------------------------*/
		enum LANGUAGE {L_LUA = 0, L_END};

		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 获取全局解释器
		 *
		 * @param language 语言名称 
		 *
		 * @return 解释器接口
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::IInterpreter* getInterpreter(LANGUAGE language) = 0;
		/* ----------------------------------------------------------------------------*/
		/**
		 * @brief 创建解释器
		 *
		 * @param language 语言名称
		 *
		 * @return 解释器接口
		 */
		/* ----------------------------------------------------------------------------*/
		virtual atom::IInterpreter* createInterpreter(LANGUAGE language) = 0;

		//typedef void (*ut_df_t)(void*);
		//typedef void (*ut_of_t)(void*, String&);
		//virtual dword registerUserType(ut_df_t deleteFunction = NULL, ut_of_t outputFunction = NULL) = 0;

		//virtual ut_df_t _getUserTypeDeleteFunction(dword userType) = 0;
		//virtual ut_of_t _getUserTypeOutputFunction(dword userType) = 0;

		//in AtValue.h
		/*@cond INTERNAL*/
		inline atom::value* _make_value();
		/*@endcond*/
	protected:
		//仅用于Atom的实现访问value私有成员
		//in AtValue.h
		inline List<atom::value*>::iterator& __value_gcID(atom::value*);
		inline bool& __value_b_hold(atom::value*);
	};
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 建立Atom单态
	 *
	 * @remarks 要在对Atom组件做任何操作之前调用。调用Atom_destroy销毁单态。
	 * @sa Atom_destroy
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API void Atom_build();
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 销毁Atom单态
	 *
	 * @remarks 要在对Atom组件做任何操作之后调用。
	 * @sa Atom_build
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API void Atom_destroy();
	/* ----------------------------------------------------------------------------*/
	/** 
	 * @brief 获取Atom单态引用
	 * 
	 * @returns Atom单态的引用
	 * @remarks 必须调用完Atom_build之后调用，否则结果不可预料。
	 */
	/* ----------------------------------------------------------------------------*/
	ONEU_API IAtom& GetAtom();

#define ATOM_RAISE(error_msg) GetAtom().raise(error_msg, __FILE__, __LINE__)


	/* ----------------------------------------------------------------------------*/
	/** 
	 * @page page_atom Atom组件
	 *
	 * Atom组件动态类型变量所组成的表系统。\n
	 * Atom以表为核心，表内存储着符号（字符串）对变量的映射。表可以配置系统参数，制作导出类供脚本调用等。\n
	 * \n
	 * 
	 * @section section_value 变量
	 * 变量类型
	 *  - nil
	 *  - bool
	 *  - int
	 *  - float
	 *  - string
	 *  - cpointer
	 *  - tuple
	 *  - function
	 *  - environment
	 *  - klass
	 *  - object
	 *  - object function
	 *  .
	 * 其中从bool到cpointer属于C内置类型。\n
	 * \n
	 * 对于C内置类型：可以调用@ref atom::value::eval将任何变量赋值成C内置类型，这样变量类型会动态改变。\n
	 * 对于非C内置类型，使用makeXXX函数创建其变量。目前不支持将变量赋值成非C内置类型。\n
	 * cpointer是供函数传递底层信息使用的，例如存储DXDevice的指针。这种类型多存于System表。\n
	 *
	 * atom::value的工厂函数：
	 *  - @ref atom::makeValue 可用于C内置类型。
	 *  - @ref atom::makeRef
	 *  - @ref atom::makeTuple
	 *  - @ref atom::makeTupleWithElem
	 *  - @ref atom::makeFunc
	 *  - @ref atom::makeEnv
	 *  - @ref atom::makeKlass
	 *  - @ref atom::makeObj
	 *  - @ref atom::makeObjFunc
	 *  .
	 * 通过工厂函数可以创建各种类型的变量。\n
	 * \n
	 * 为了获取变量的内容，调用@ref atom::value::get函数，在模板参数显示指定类型，即可获得所对应类型的值的指针（注意是指针）。如果类型不被支持，则会编译出错。\n
	 * C内置类型为C++语言所对应的类型。其中cpointer对应void*。\n
	 * 非C内置类型对应的类：
	 *  - tuple @ref atom::tuple
	 *  - environment @ref atom::env
	 *  - klass @ref atom::klass
	 *  - object @ref atom::obj
	 *  - object function @ref atom::obj_function
	 *  .
	 * C内置类型的使用：
	 * @code
	 * atom::value* v = atom::makeNil();
	 * v->eval(L"This is a String");//v->type() = atom::T_STRING
	 * v->get<String>();//返回String*指针，其内容为L"This is a String"
	 * v->eval(20.0f);//v->type() = atom::T_FLOAT（注意，如果不加f的话会编译出错，因为atom不支持double）
	 * v->get<float>();//返回float*指针，所指浮点数为20.0f
	 * @endcode
	 * Tuple的使用：
	 * @code
	 * atom::value* v = atom::makeTuple(3);//创建一个3元素的Tuple v = (nil, nil, nil)
	 * v->get(0)->eval(10.0f);
	 * v->get(1)->eval(true);
	 * v->get(2)->eval(3);//v = (10.0f, true, 3)
	 * @endcode
	 * @code
	 * atom::value* v= atom::makeTupleWithElem(3, atom::makeValue(10.0f), atom::makeValue(true), atom::makeValue(3));//v = (10.0f, true, 3)
	 * @endcode
	 * function的使用：略\n
	 * klass和obj的使用：
	 * @code
	 * class exported_klass
	 * 	:public atom::exported//对于要导出的成员函数，一定要派生自这个基类
	 * {
	 * public:
	 * 	exported_klass(atom::value* arg){
	 *		//arg是一个Tuple（长度可为0），表示初始化参数。
	 * 	}
	 * };
	 * //...
	 * 	atom::value* k = atom::makeKlass<exported_klass>();
	 * 	atom::value* o = atom::makeObj(k->get<atom::klass>(), NULL);//NULL会自动转为长度为0的Tuple
	 * //...
	 * @endcode
	 * object function的使用：
	 * @code
	 * class exported_klass
	 * 	:public atom::exported//对于要导出的成员函数，一定要派生自这个基类
	 * {
	 * public:
	 * 	exported_klass(atom::value*);
	 * 	atom::value* exported_method(atom::value*){ return NULL; }
	 * };
	 * //...
	 * 	atom::value* v = atom::makeObjFunc(klass_v, &exported_klass::exported_method);//atom::klass* klass_v
	 * //...
	 * @endcode
	 * @subsection section_acquire 控制权和GC
	 * Atom具有半自动垃圾回收系统。\n
	 * 每个变量具有唯一控制权。如果没有被获取控制权会被视为临时变量，会在下一次GC中释放。如果获取两次控制权，第二次会失败。\n
	 * 如果要保留一个变量，应该获取该变量的控制权，然后在合适的地方释放控制权。\n
	 * 其中有些操作会尝试获取控制权，如果失败则会造成操作失败。\n
	 * 控制权的使用：
	 * @code
	 * class hold_control
	 * {
	 * 	atom::value* v;
	 * public:
	 * 	hold_control(){
	 * 		v = atom::makeValue(10.0f);
	 * 		v->acquire();
	 * 	}
	 *	~hold_control(){
	 *		v->unacquire();
	 *	}
	 * };
	 * @endcode
	 *
	 *
	 * @section section_list 表系统
	 *
	 * environment环境是一张Atom表，相当于以字符串为键值，以atom::value*为值的map。\n
	 * atom::klass派生自atom::env，所以klass也可以支持environment所有特性。但是将atom::T_KLASS和atom::T_ENV视为两种不同类型。\n
	 * \n
	 * 可以向表中添加变量，删除变量。表会向变量获取控制权。如果获取控制权失败，则添加变量失败。\n
	 * 表可以有（单）继承关系。调用@ref atom::env::setFather使一张表继承另一张表。
	 * 当一张表继承自另一张时，在查找符号时会首先在当前表查找，再在父表中查找。
	 *
	 * @section section_exporting 导出类
	 * @code
	 * class exported_class
	 * 	: public atom::exported
	 * {
	 * 	float m_f;
	 * public:
	 * 	exported_class(atom::value* v){
	 * 		m_f = *getArg<float>(v, 0);
	 * 	}
	 * 	atom::value* exported_method(atom::value* arg){
	 * 		return arg;
	 * 	}
	 * };
	 * ...
	 * 	atom::value* v = atom::makeKlass<exported_class>();
	 * 	GetAtom()->getEnv()->createSymbol(L"new_class", v);
	 * 	v->get<atom::klass>()->exportMethod(L"new_method", &exported_class::exported_method);
	 * ...
	 * @endcode
	 * 同样也可以封装一个类，将其导出：
	 * @code
	 * class exported_class
	 * 	: public atom::exported, private aClass
	 * {
	 * public:
	 * 	exported_class(atom::value* v)
	 * 		: aClass(*getArg<float>(v, 0))
	 * 	{}
	 * };
	 * @endcode
	 * @subsection section_export_cooperation 互相有协作关系的类的导出
	 * 有时候一个类的成员函数可能需要另一个类的对象做参数。例如：
	 * @code
	 * class IImage
	 * {
	 * 	//...
	 * };
	 * class Sprite
	 * {
	 * public:
	 * 	Sprite(IImage*);
	 * };
	 * @endcode
	 * 导出Sprite类的时候，要在其导出类构造函数中验证传入参数是否为IImage*，要安全地转换为IImage*才能供Sprite类使用。\n
	 * 实现方法：\n
	 * 头文件：
	 * @code
	 * IImage* SafeToImage(atom::obj* v);
	 * class Image_exp
	 * 	: public atom::exported
	 * {
	 * 	IImage* m_pImage;
	 * public:
	 * 	Image(atom::value* arg){ m_pImage = GetVideo().loadImage(atom::getArg<String>().c_str()); }
	 * 	IImage* getImage(){ return m_pImage; }
	 * };
	 * class Sprite_exp
	 * 	: public atom::exported, public Sprite
	 * {
	 * public:
	 * 	Sprite_exp(atom::value* arg)
	 * 		: Sprite(SafeToImage(atom::getArg<atom::obj>(arg, 0)))
	 * 	{}
	 * };
	 * @endcode
	 * 实现文件：
	 * @code
	 * static atom::klass* image_kls = NULL;
	 * bool RegisterImage(){
	 * 	atom::value* image_kls_v = atom::makeKlass<Image_exp>();
	 * 	image_kls = image_kls_v->get<atom::klass>();
	 * 	return GetAtom().getEnv()->createSymbol(L"image", image_kls_v);
	 * }
	 * IImage* SafeToImage(atom::obj* v)
	 * {
	 * 	if(v->getKlass() != image_kls) GetAtom().error(L"not a image object!");
	 * 	return ((IImage_exp*)v->getAddr())->getImage();
	 * }
	 * @endcode
	 * 这种方法能够保证绝对安全，但是存在一个缺陷：代码中IImage是一个接口，如果在别的插件中写了一个实现，是没有办法被Sprite_exp所利用的。\n
	 * 解决方法是修改上面代码，加一个间接层，将Image_exp实现成一个指针持有类，改名为Image_holder，它只接收一个参数cpointer类型，并把它强转成IImage*存在里面。这样能保证使用Image_holder的时候是类型安全的。
	 * 同时我们写一个工厂函数，返回一个Image_holder。这样就能实现刚才的功能。如果在别的插件写了一个实现，可以再写一个工厂函数，实现了多态。\n
	 * 类型不安全的地方只存在于Image_holder的构造函数，只要书写工厂函数时注意即可。由于类型不安全性，Image_holder属于底层类，因此可以在Image导出名字前加一个下划线。\n
	 * \n
	 * \n
	 * @remarks 还有另一种解决办法，抛弃掉SafeToImage，为Image_exp导出一个获取IImage*的函数，返回类型为cpointer，名字要起得复杂一点（避免重名），在Sprite中调用该函数，强转成IImage*。该方法存在小几率不安全性。
	 * 如果有更好的办法通知我。
	 *
	 * @todo 如果实现了Atom用户自定义类型的话，这种类型不安全性可以解决。
	 *
	 * @todo 实现自定义类型。
	 */
	/* ----------------------------------------------------------------------------*/

	//@}
}



#include "AtValue.h"
#include "AtConvert.h"
#include "AtInterpreter.h"
#include "AtUtility.h"
