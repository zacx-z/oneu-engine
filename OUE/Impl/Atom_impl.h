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
#include "../Atom.h"
#include "../List.h"
namespace OneU
{
	class Atom_Impl
		: public IAtom
	{
		typedef List<atom::value*> gc_list_t;
		gc_list_t gc_list;

		atom::value* global_env_v, *system_env_v;
		atom::env* global_env, *system_env;

		//负责输出environment的临时变量
		int value_outputer_tab_depth;


		//负责各种语言错误处理
		//ehm某个位上有标志表示使用了该语言，错误时弹出该解释器的栈
		//如果出现多解释器，就更复杂了……
		int ehm[L_END];
		atom::IInterpreter* m_pInterpter[L_END];

		//已加载的AtomLib
		struct ModuleInfo{
			HMODULE H;
			int ref;
		};
		Table<ModuleInfo> LibTable;
	public:
		//非接口函数
		void init();//在Atom_build中被调用：如果在构造函数中创建环境表，最后会调用到自身的虚函数，出现问题。
		void value_outputer_add_tab(String& buffer);

		Atom_Impl()
			: value_outputer_tab_depth(0)
		{
			memset(m_pInterpter, 0, sizeof(m_pInterpter));
			memset(ehm, 0, sizeof(ehm));
		}

		//重写函数
		~Atom_Impl();
		void gc();
		bool acquireValue(atom::value* v);
		void unacquireValue(atom::value* v);

		atom::env* getEnv(){ return global_env; }
		virtual atom::value* getEnvValue(){ return global_env_v; }
		atom::env* getSystemEnv(){ return system_env; }
		virtual atom::value* getSystemEnvValue(){ return system_env_v; }

		void getDescription(atom::value* v, OneU::String& buffer);
	public:
		//错误处理部分
		//调用该语言的某函数时应压栈 这样当出错时会有该语言的堆栈信息
		//非接口，因为调用该函数的模块为解释器，属于该模块。
		void _pushErrorHook(LANGUAGE e){
			ehm[(int)e]++;
		}
		void _popErrorHook(LANGUAGE e){
			ehm[(int)e]--;
		}
		void raise(pcwstr error_message){
			String out = error_message;
			//通过判断是否给hook压栈来报错
			if(ehm[L_LUA])
			{
				out.append(L"\nLua Stack:\n");
				m_pInterpter[L_LUA]->_getStackInfo(out);
			}
			ONEU_RAISE(out.c_str());
		}
		void raise(pcwstr error_message, pcstr source_file, int source_line){
			raise(String().format(L"%s\nFile:%S(%d)", error_message, source_file, source_line).c_str());
		}
		//错误处理结束

		void loadAtomLib(pcwstr path);
		void unloadAllAtomLib();

		atom::IInterpreter* getInterpreter(LANGUAGE language){
			return m_pInterpter[language];
		}
		atom::IInterpreter* createInterpreter(LANGUAGE language);
	};
}