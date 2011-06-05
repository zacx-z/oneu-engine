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
#include "Atom_impl.h"
#include "../AtValue.h"
#include "LuaInterpreter.h"

namespace OneU
{
	static Atom_Impl* s_pAtom = NULL;

	ONEU_API void Atom_build(){
		s_pAtom = ONEU_NEW Atom_Impl;
		s_pAtom->init();
	}
	ONEU_API void Atom_destroy(){
		if(s_pAtom != NULL){ ONEU_DELETE s_pAtom; s_pAtom = NULL; }
	}
	ONEU_API IAtom& GetAtom(){ return *s_pAtom; }

	void Atom_Impl::init(){
		//初始化环境表变量
		global_env_v = atom::makeEnv();
		global_env_v->acquire();
		global_env = global_env_v->get<atom::env>();
		system_env_v = atom::makeEnv();
		system_env_v->acquire();
		system_env = system_env_v->get<atom::env>();

		//初始化解释器
		m_pInterpreter[L_LUA] = ONEU_NEW atom::LuaInterpreter;
	}

	Atom_Impl::~Atom_Impl(){
		ONEU_DELETE m_pInterpreter[L_LUA];

		global_env_v->unacquire();
		system_env_v->unacquire();
		unloadAllAtomLib();
		this->gc();
	}

	void Atom_Impl::gc(){
		for(gc_list_t::iterator it = gc_list.begin(); it != gc_list.end(); ++it){
			ONEU_DELETE_T(*it);
		}
		gc_list.clear();
	}

	bool Atom_Impl::acquireValue(atom::value* v){
		if(!__value_b_hold(v)) return false;
		gc_list.erase(__value_gcID(v));
		__value_b_hold(v) = false;
		return true;
	}

	void Atom_Impl::unacquireValue(atom::value* v){
		if(__value_b_hold(v)) return;
		gc_list.pushBack(v);
		__value_gcID(v) = gc_list.end().prev();
		__value_b_hold(v) = true;
	}

	//以下是输出value信息区域
	struct env_outputer
	{
		Atom_Impl& a;
		String& buffer;
		env_outputer(Atom_Impl& a, String& buffer) : a(a), buffer(buffer){}
		void operator()(pcwstr sym, atom::value* v){
			a.value_outputer_add_tab(buffer);
			buffer.append(sym);
			buffer.append(L"->\t");
			a.getDescription(v, buffer);
		}
	};
	void Atom_Impl::getDescription(atom::value* v, OneU::String& buffer){
		switch(v->type())
		{
		case atom::T_NIL:
			buffer.append(L"none\n");
			break;
		case atom::T_BOOL:
			if(*v->get<bool>())buffer.append(L"bool:true\n");
			else buffer.append(L"bool:false\n");
			break;
		case atom::T_INT:
			buffer.append(String().format(L"integer:%d\n", *v->get<int>()).c_str());
			break;
		case atom::T_FLOAT:
			buffer.append(String().format(L"float:%f\n", *v->get<float>()).c_str());
			break;
		case atom::T_STR:
			buffer.append(String().format(L"string:%s\n", v->get<String>()->c_str()).c_str());
			break;
		case atom::T_CPOINTER:
			buffer.append(String().format(L"cpointer:%p\n", *v->get<void*>()).c_str());
			break;
		case atom::T_TUPLE:
			buffer.append(L"tuple:\n");
			++value_outputer_tab_depth;
			{
				atom::tuple* t = v->get<atom::tuple>();
				for(uint32 i = 0; i < t->length(); ++i){
					value_outputer_add_tab(buffer);
					getDescription(t->get(i), buffer);
				}
				buffer.append(L'\n');
			}
			--value_outputer_tab_depth;
			break;
		case atom::T_FUNC:
			buffer.append(String().format(L"function:%p\n", v->get<atom::function>()->_getAddr()).c_str());
			break;
		case atom::T_ENV:
			buffer.append(L"environment:\n");
			++value_outputer_tab_depth;
			v->get<atom::env>()->traversal(env_outputer(*this, buffer));
			--value_outputer_tab_depth;
			break;
		case atom::T_KLS:
			buffer.append(String().format(L"klass: %p\n", v->get<atom::klass>()).c_str());
			++value_outputer_tab_depth;
			v->get<atom::klass>()->traversal(env_outputer(*this, buffer));
			--value_outputer_tab_depth;
			break;
		case atom::T_OBJ:
			buffer.append(String().format(L"object: (klass:%p)\n", v->get<atom::obj>()->getKlass()).c_str());
			break;
		case atom::T_OBJFUNC:
			buffer.append(L"object function:\n");
			break;
		default:
			ONEU_ASSERT(0);
		}
	}

	void Atom_Impl::value_outputer_add_tab(String& buffer)
	{
		for(int i = 0; i < value_outputer_tab_depth; ++i){
			buffer.append(L'\t');
		}
	}
	//输出区域结束

	void Atom_Impl::loadAtomLib(pcwstr path)
	{
		if(LibTable.insert(path) == NULL){//插入失败
			++LibTable.getLastItem().getValue().ref;
			return;
		}
		ModuleInfo& m = LibTable.getLastItem().getValue();
		m.H = ::LoadLibrary(path);
		m.ref = 1;
		if(m.H == NULL){
			LibTable.erase(LibTable.getLastItem());
			ONEU_RAISE(String().format(L"Can't find dll:%s", path).c_str());
		}

		typedef void (*InitProc_t)(LPCWSTR Version);
		InitProc_t InitProc = (InitProc_t)GetProcAddress(m.H, "AtomInit");
		if(InitProc != NULL) InitProc(L"Atom 1.0.0.0");
	}

	void Atom_Impl::unloadAllAtomLib()
	{
		for(Table<ModuleInfo>::iterator it = LibTable.begin(); it != LibTable.end(); ++it){
			typedef void (*DestroyProc_t)();
			DestroyProc_t DestroyProc = (DestroyProc_t)GetProcAddress(it.getValue().H, "AtomDestroy");
			if(DestroyProc != NULL) DestroyProc();
		}
		this->gc();
		for(Table<ModuleInfo>::iterator it = LibTable.begin(); it != LibTable.end(); ++it){
			FreeLibrary(it.getValue().H);
		}
		LibTable.clear();
	}

	atom::IInterpreter* Atom_Impl::createInterpreter( LANGUAGE language )
	{
		if(language == L_LUA){
			return ONEU_NEW atom::LuaInterpreter;
		}
		return NULL;
	}

}