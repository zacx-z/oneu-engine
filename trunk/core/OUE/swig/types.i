%module OUE

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
	typedef char byte;
	typedef unsigned char ubyte;
	
	
	%typemap(in) const wchar_t* (OneU::AutoPtr<wchar_t> temp) {
		temp = OneU::ANSI2Wide(lua_tostring(L, $input));
		$1 = temp;
	}

	%typemap(out) const wchar_t* {
		lua_pushstring(L, OneU::Wide2ANSI($1));
	}
}

%import "../Vector.h"