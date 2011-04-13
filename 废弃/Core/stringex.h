#pragma once
#include <cstring>
#include <string>

class stringex : public ::std::string
{
public:
	stringex(void){}
	~stringex(void){}
	__CLR_OR_THIS_CALL stringex(const string& _Right)
		: string(_Right){}

	__CLR_OR_THIS_CALL stringex(const string& _Right, size_type _Roff,
		size_type _Count = npos)
		: string(_Right, _Roff, _Count){}

	__CLR_OR_THIS_CALL stringex(const string& _Right, size_type _Roff, size_type _Count,
		const _Alloc& _Al)
		: string(_Right, _Roff, _Count, _Al){}

	__CLR_OR_THIS_CALL stringex(const char *_Ptr, size_type _Count)
		: string(_Ptr, _Count){}

	__CLR_OR_THIS_CALL stringex(const char *_Ptr, size_type _Count, const _Alloc& _Al)
		: string(_Ptr, _Count, _Al){}

	__CLR_OR_THIS_CALL stringex(const char *_Ptr)
		: string(_Ptr){}

	__CLR_OR_THIS_CALL stringex(const char *_Ptr, const _Alloc& _Al)
		: string(_Ptr, _Al){}

	__CLR_OR_THIS_CALL stringex(size_type _Count, char _Ch)
		: string(_Count, _Ch){}

	__CLR_OR_THIS_CALL stringex(size_type _Count, char _Ch, const _Alloc& _Al)
		: string(_Count, _Ch, _Al){}

public:
	stringex& operator<<(int integer)
	{
		//临时办法 保留
		//
		size_type _Num;
		if( _Grow( _Num = _Mysize + 12 ) )
		{
			_itoa_s( integer, _Myptr() + _Mysize, 12, 10 );
			_Eos( _Mysize + strlen( _Myptr() + _Mysize ) );
		}
		return *this;
	}

	stringex& operator<<(LPCSTR lpstr)
	{
		strcpy( _Myptr() + _Mysize, lpstr);
		return *this;
	}
};
