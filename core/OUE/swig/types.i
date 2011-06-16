%module(directors = "1") OUE

namespace OneU
{
	typedef unsigned long long uint64;
	typedef unsigned long uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef long long int64;
	typedef long int32;
	typedef short int16;
	typedef char int8;

	typedef wchar_t wchar;
	typedef wchar* pwstr;
	typedef const wchar* pcwstr;
	typedef char* pstr;
	typedef const char* pcstr;
	//typedef char byte;
	//typedef unsigned char ubyte;
}
	
#if defined(SWIGLUA)
namespace OneU
{
	%typemap(in) const wchar_t* (OneU::AutoPtr<wchar_t> temp) {
		if(lua_type(L, $input) != LUA_TSTRING)
			SWIG_fail_arg("$symname", $argnum, "$type");
		
		temp = OneU::Char2Wide(lua_tostring(L, $input));
		$1 = temp;
	}
	%typemap(out) const wchar_t* {
		lua_pushstring(L, OneU::Wide2Char($1));
	}
}
#elif defined(SWIGRUBY)
%fragment("SWIG_From_wchar_t", "header", fragment = "SWIG_FromCharPtr")
{	
VALUE SWIG_From_wchar_t(wchar_t value){
	static wchar_t s[2] = {0, 0};
	s[0] = value;
	return SWIG_FromCharPtr(OneU::Wide2Char(s));
}
}
%fragment("SWIG_AsVal_wchar_t", "header")
{	
int SWIG_AsVal_wchar_t(VALUE obj, wchar_t* val){
	if(TYPE(obj) == T_STRING){
		*val = OneU::Char2Wide(StringValuePtr(obj))[0];
		return SWIG_OK;
	}
	return SWIG_TypeError;
}
}
namespace OneU
{	
	%typemap(in) const wchar_t* (OneU::AutoPtr<wchar_t> temp){
		if(TYPE($input) != T_STRING)
			SWIG_exception_fail(SWIG_TypeError, Ruby_Format_TypeError( "$1_name", "$1_type","$symname", $argnum, $input ));
			
		temp = OneU::Char2Wide(StringValuePtr($input));
		$1 = temp;
	}
	%typemap(out) const wchar_t* {
		$result = rb_str_new2(OneU::Wide2Char($1));
	}
	%typemap(directorin) const wchar_t*{
		$result = rb_str_new2(OneU::Wide2Char($1));
	}
	%typemap(directorout) const wchar_t* (OneU::AutoPtr<wchar_t> temp){
		if(TYPE($input) != T_STRING)
			SWIG_exception_fail(SWIG_ArgError(res), Ruby_Format_TypeError( "$1_name", "$1_type","$symname", $argnum, $input ));
			
		temp = OneU::Char2Wide(StringValuePtr($input));
		$1 = temp;
	}
}
#endif

namespace OneU
{
	%typemap(typecheck) const wchar_t* = char*;
	%typemap(typecheck) pcwstr = const wchar_t*;
}

%include "Vector.i"
