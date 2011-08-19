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
//for SetDllDirectory
#define _WIN32_WINNT 0x0502
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#include "RubyUtil.h"
#include "Game.h"

#include <sstream>

extern "C"{
#include <ruby.h>
};

#undef fopen
#undef fclose

//ruby dll的导出符号（但未写在头文件里，在此声明）
extern "C" VALUE rb_f_eval(int argc, VALUE* argv, VALUE self);

namespace OneU
{
	//用于存放Dll名字的表 用于判重
	static Table<byte>* s_MTable;

	//ruby的错误处理
	static void ThrowOnError(int error) {
		if(error == 0)
			return;

		using std::endl;
		std::ostringstream clog;

		VALUE lasterr = rb_errinfo();//rb_gv_get("$!");//$!貌似只有在rescue里用才有效

		// class
		VALUE klass = rb_class_path(CLASS_OF(lasterr));
		clog << StringValuePtr(klass) << endl; 

		// message
		VALUE message = rb_obj_as_string(lasterr);
		clog << StringValuePtr(message) << endl;

		//一下这两句功能与上面是一样的
		//VALUE info = rb_eval_string("\"class = #{$!.class}\nmessage = #{$!}\n\"");
		//clog << StringValuePtr(info) << endl;

		// backtrace
		VALUE errinfo = rb_errinfo();
		if(!NIL_P(errinfo)) {
			VALUE ary = rb_funcall(
				errinfo, rb_intern("backtrace"), 0);

			VALUE btstr = rb_funcall(ary, rb_intern("to_s"), 0);
			clog << "backtrace : " << StringValuePtr(btstr) << endl;
		}

		ErrorBox(Char2Wide(clog.str().c_str(), 65001), L"Ruby");
	}

	//-----------------------------------------------------------------
	//导出函数
	static VALUE _rb_prompt(int argc, VALUE* argv, VALUE self);
	static VALUE _rb_oload(VALUE self, VALUE fname);
	static VALUE _rb_olib(VALUE self, VALUE fname);
	static VALUE _init_rb_lib(VALUE){
		rb_define_global_function("prompt", (VALUE (*)(ANYARGS))_rb_prompt, -1);
		rb_define_global_function("oload", (VALUE (*)(ANYARGS))_rb_oload, 1);
		rb_define_global_function("olib", (VALUE (*)(ANYARGS))_rb_olib, 1);
		return Qnil;
	}


	//内部使用函数
	static void _getName(const char* fname, char* name){
		while(*fname != 0 && *fname != '.')
			*(name++) = *(fname++);
		*name = 0;
	}
	//导出函数实现
	static VALUE _rb_prompt(int argc, VALUE* argv, VALUE self){
		std::ostringstream o;
		for(int i = 0; i < argc; ++i){
			if(rb_type(argv[i]) != T_STRING)
				rb_raise(rb_eArgError, "String expected, but got %s.", rb_obj_classname(argv[i]));
			else
				o << StringValuePtr(argv[i]) << std::endl;
		}
		ONEU_PROMPT(Char2Wide(o.str().c_str(), 65001));
		return Qnil;
	}

	static VALUE _rb_oload(VALUE self, VALUE fname){
		FILE* f = _wfopen(Char2Wide(StringValuePtr(fname), 65001), L"rb");
		if(!f)
			rb_raise(rb_eArgError, "can't find the file - %s", StringValuePtr(fname));
		struct _stat stat;
		_fstat(fileno(f), &stat);
		char* buf = new char[stat.st_size + 1];
		fread(buf, stat.st_size, 1, f);
		fclose(f);
		buf[stat.st_size] = 0;

		VALUE arg[] = {rb_str_new2(buf), Qnil, fname};
		rb_f_eval(3, arg, Qnil);
		return Qnil;
	}
	static VALUE _rb_olib(VALUE self, VALUE fname){
		const char* fstr = StringValuePtr(fname);
		HMODULE hDll = LoadLibrary(Char2Wide(fstr));
		if(hDll == NULL)
			rb_raise(rb_eArgError, "cannot find library %s.", fstr);
		wchar dllname_buf[MAX_PATH];
		::GetModuleFileNameW(hDll, dllname_buf, MAX_PATH);
		if(s_MTable->insert(dllname_buf)){
			//run the init procedure
			char inprocname[256] = "Init_", name[256];
			_getName(fstr, name);
			strcat(inprocname, name);
			void (*proc)() = (void (*)())GetProcAddress(hDll, inprocname);
			if(proc == NULL)
				rb_raise(rb_eArgError, "library doesn't have %s function when loading %s!", inprocname, fstr);
			proc();
		}
		return Qnil;
	}

	//-----------------------------------------------------------------
	//用于异常的封装
	static VALUE LoadWrap(VALUE arg){
		_rb_oload(Qnil, rb_str_new2((const char*)arg));
		return Qnil;
	}
	static VALUE EvalStringWrap(VALUE arg){
		rb_eval_string((const char*)arg);
		return Qnil;
	}
	RUBY_GLOBAL_SETUP

	ONEU_API void RubyRun(){
		Game_build();

		if(!RubyInit())
			goto fail;

		RubyExecFile(L"script/main.rb");

		RubyFinalize();
fail:
		Game_destroy();
	}


	ONEU_API bool RubyInit(){
		s_MTable = new Table<byte>();

		char* a[] = {"a"};
		int n = 1;
		char** argv = a;
		ruby_sysinit(&n, &argv);
		RUBY_INIT_STACK;
		ruby_init();
		ruby_init_loadpath();
		ruby_script("oneu");
		//set load path
		rb_eval_string("$: << \"./\"");
#ifdef _DEBUG
		if(!SetDllDirectoryW(L"./../debug")){
			ErrorBox(L"cannot set dll directory", L"error!");
			ruby_finalize();
			return false;
		}
#endif

		int state;
		rb_protect(_init_rb_lib, Qnil, &state);
		if(state){
			ThrowOnError(state);
			ruby_finalize();
			//fail
			delete s_MTable;
			return false;
		}
		return true;
	}

	ONEU_API bool RubyExecFile(pcwstr filename){
		int state;
		rb_protect(LoadWrap,(VALUE)(const char*)Wide2Char(filename), &state);
		if(state){
			ThrowOnError(state);
			return false;
		}
		return true;
	}
	ONEU_API bool RubyExecCode(pcstr code){
		int state;
		rb_protect(EvalStringWrap,(VALUE)code, &state);
		if(state){
			ThrowOnError(state);
			return false;
		}
		return true;
	}

	ONEU_API void RubyFinalize(){
		ruby_finalize();
		delete s_MTable;
	}
}