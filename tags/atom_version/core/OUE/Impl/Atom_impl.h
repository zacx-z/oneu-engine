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


		//负责各种解释器的错误处理
		//一个解释器执行代码时将其压入栈
		//如果出错则将解释器栈里的解释器的栈信息弹出
		//目前最大支持10个
		atom::IInterpreter* m_pRunningIpSt[10];
		int m_RunningIpTop;

		atom::IInterpreter* m_pInterpreter[L_END];

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
			: value_outputer_tab_depth(0), m_RunningIpTop(-1)
		{
			memset(m_pInterpreter, 0, sizeof(m_pInterpreter));
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
		void _pushRunningIp(atom::IInterpreter* ip){
			m_pRunningIpSt[++m_RunningIpTop] = ip;
		}
		void _popRunningIp(){
			--m_RunningIpTop; 
		}
		void raise(pcwstr error_message){
			String out = error_message;
			for(int i = 0; i <= m_RunningIpTop; ++i)
			{
				out.append(L"\nStack:\n");
				m_pRunningIpSt[i]->_getStackInfo(out);
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
			return m_pInterpreter[language];
		}
		atom::IInterpreter* createInterpreter(LANGUAGE language);
	};
}