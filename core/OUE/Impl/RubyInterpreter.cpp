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
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#include "RubyInterpreter.h"
#include "../Game.h"

#include <sstream>
namespace OneU
{
	static void ThrowOnError(int error) {
		if(error == 0)
			return;

		using std::endl;
		std::ostringstream clog;

		VALUE lasterr = rb_errinfo();//rb_gv_get("$!");//$!貌似只有在rescue里用才有效

		// class
		VALUE klass = rb_class_path(CLASS_OF(lasterr));
		clog << "class = " << StringValuePtr(klass) << endl; 

		// message
		VALUE message = rb_obj_as_string(lasterr);
		clog << "message = " << StringValuePtr(message) << endl;

		//一下这两句功能与上面是一样的
		//VALUE info = rb_eval_string("\"class = #{$!.class}\nmessage = #{$!}\n\"");
		//clog << StringValuePtr(info);

		// backtrace
		VALUE errinfo = rb_errinfo();
		if(!NIL_P(errinfo)) {
			std::ostringstream o;
			VALUE ary = rb_funcall(
				errinfo, rb_intern("backtrace"), 0);
			int c;
			for (c=0; c<RARRAY(ary)->len; c++) {
				o << "\tfrom " << 
					StringValuePtr(RARRAY(ary)->ptr[c]) << 
					"\n";
			}
			clog << "backtrace = " << o.str() << endl;
		}

		MessageBoxA(NULL, clog.str().c_str(), "Ruby", MB_OK | MB_ICONERROR);
	}

	static VALUE LoadWrap(VALUE arg){
		rb_load(arg, false);
		return Qnil;
	}
	static VALUE EvalStringWrap(VALUE arg){
		rb_eval_string((const char*)arg);
		return Qnil;
	}

	ONEU_API void RubyRun(){
		Game_build(Game_create);

		char* a[] = {"a"};
		int n = 1;
		char** argv = a;
		ruby_sysinit(&n, &argv);
		RUBY_INIT_STACK;
		ruby_init();
		ruby_init_loadpath();
		ruby_script("oneu");
#ifdef _DEBUG
		rb_eval_string("$: << \"./../debug/\"");
#endif
		int state;
		rb_protect(LoadWrap, rb_str_new2("script/main.rb"), &state);

		if(state)
			ThrowOnError(state);

		ruby_finalize();
		
		Game_destroy();
	}



	ONEU_API IInterpreter* Interpreter_Ruby(){
		return ONEU_NEW RubyInterpreter();
	}

	RubyInterpreter::RubyInterpreter(){
		char* a[] = {"a"};
		int n = 1;
		char** argv = a;
		ruby_sysinit(&n, &argv);
		RUBY_INIT_STACK;
		ruby_init();
		ruby_init_loadpath();
		ruby_script("oneu");
	}
	
	void RubyInterpreter::execFile(pcwstr filename){
		int state;
		rb_protect(LoadWrap,rb_str_new2((const char*)Wide2Char(filename)), &state);
		if(state)
			ThrowOnError(state);
	}
	void RubyInterpreter::execCode(pcstr code){
		int state;
		rb_protect(EvalStringWrap,(VALUE)code, &state);
		if(state)
			ThrowOnError(state);
	}

	RubyInterpreter::~RubyInterpreter(){
		ruby_finalize();
	}
}