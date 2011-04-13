#pragma once
#include "OneUPreDef.h"
#include "Interface.h"
#include <cstdarg>
namespace OneU
{
	//可以去掉该类 而把String什么的全内联
	class IString
		: public Interface
	{
	public:
		virtual ~IString(){}
		virtual void append(const IString& others) = 0;
		virtual void eval(const pcwstr str) = 0;
		virtual pcwstr c_str() const = 0;
		virtual pwstr getBuffer() = 0;
		virtual uint getLength() const = 0;
		virtual void vformat(pcwstr Format, va_list marker) = 0;
	};
	extern "C" ONEU_BASE_API IString* String_make(pcwstr others);
	extern "C" ONEU_BASE_API IString* String_create();
	//该String可以替代std::string作为接口
	class String
	{
		IString* pImp;
	public:
		String():pImp(String_create()){}
		String(pcwstr others): pImp(String_make(others)){}
		String(const String& others) : pImp(String_make(others.pImp->c_str())){}
		~String(){pImp->destroy();}
		String& operator+=(const String& others){pImp->append(*others.pImp);}
		String& operator=(const String& others){pImp->eval(others.pImp->c_str()); return *this;}
		String& operator=(pcwstr str){pImp->eval(str); return *this; }
		wchar& operator[](uint off){return pImp->getBuffer()[off];}
		pcwstr c_str(){return pImp->c_str();}
		pwstr getBuffer(){return pImp->getBuffer();}
		uint getLength(){return pImp->getLength();}
		String& format(pcwstr Format, ...)
		{
			va_list va_alist;
			va_start(va_alist, Format);
			pImp->vformat(Format, va_alist); return *this;
		}
	};

	//ANSI和Unicode的转换
	extern "C" ONEU_BASE_API pcwstr ANSI2Wide(pcstr str);
	extern "C" ONEU_BASE_API pcstr Wide2ANSI(pcwstr cstr);
}
