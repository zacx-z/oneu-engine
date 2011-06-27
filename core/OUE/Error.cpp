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

#include "OneUPreDef.h"
#include "Error.h"
#include <windows.h>
#include <cstdlib>
#include <errno.h>
#include "String.h"
#include "Logger.h"

#ifdef __GNUC__
typedef int errno_t;
#endif
#pragma warning( disable : 4311 )

namespace OneU
{

#ifdef ONEU_USE_ASSERT

	void DumpString( pcwstr str )
	{
		OutputDebugString( str );
	}

#endif
}

#pragma warning( default : 4311 )

namespace OneU
{

	extern "C" ONEU_API pcwstr GetErrnoString( errno_t Errno )
	{
		switch( Errno )
		{
		case EPERM:
			return L"Operation not permitted";
		case ENOENT:
			return L"No such file or directory";
		case ESRCH:
			return L"No such process";
		case EINTR:
			return L"Interrupted system call";
		case EIO:
			return L"I/O error";
		case ENXIO:
			return L"No such device or address";
		case E2BIG:
			return L"Arg list too long";
		case ENOEXEC:
			return L"Exec format error";
		case EBADF:
			return L"Bad file number";
		case ECHILD:
			return L"No child processes";
		case EAGAIN:
			return L"Try again";
		case ENOMEM:
			return L"Out of memory";
		case EACCES:
			return L"Permission denied";
		case EFAULT:
			return L"Bad address";
		case EBUSY:
			return L"Device or resource busy";
		case EEXIST:
			return L"File exists";
		case EXDEV:
			return L"Cross-device link";
		case ENODEV:
			return L"No such device";
		case ENOTDIR:
			return L"Not a directory";
		case EISDIR:
			return L"Is a directory";
		case EINVAL:
			return L"Invalid argument";
		case ENFILE:
			return L"File table overflow";
		case EMFILE:
			return L"Too many open files";
		case ENOTTY:
			return L"Not a typewriter";
		case EFBIG:
			return L"File too large";
		case ENOSPC:
			return L"No space left on device";
		case ESPIPE:
			return L"Illegal seek";
		case EROFS:
			return L"Read-only file system";
		case EMLINK:
			return L"Too many links";
		case EPIPE:
			return L"Broken pipe";
		case EDOM:
			return L"Math argument out of domain of func";
		case ERANGE:
			return L"Math result not representable";
		case EDEADLK:
			return L"Resource deadlock would occur";
		case ENAMETOOLONG:
			return L"File name too long";
		case ENOLCK:
			return L"No record locks available";
		case ENOSYS:
			return L"Function not implemented";
		case ENOTEMPTY:
			return L"Directory not empty";
		case EILSEQ:
			return L"Illegal byte sequence";
#ifdef _MSC_VER_
		case STRUNCATE:
			return L"A string copy or concatenation resulted in a truncated string";
#endif
		default:
			return L"An unknown error occurs";
		}//switch
	}//GetErrorString

	static TerminateHandler s_eh = NULL;

	void _TerminateApp(const char * FileName, const int Line, const char* Function, pcwstr str ){
		GetLogger().stream(ILogger::ML_CRITICAL) << FileName << L':' << Line << L"\nFunction:" << Function << L"\n发生异常，程序终止执行。\n异常信息：" << str;
		::MessageBoxW(NULL, str, L"错误", MB_OK | MB_ICONERROR);

		//调用ExitHandler
		if(s_eh != NULL)s_eh();

		//Logger和Allocator应该在Game析构函数（在s_eh中）销毁 为了顾及不安全情况 在这里再调用销毁函数一次
		Logger_destroy();
		Allocator_destroy();
		exit(-1);
	}

	TerminateHandler SetTerminateHandler(TerminateHandler eh)
	{
		TerminateHandler ret = s_eh;
		s_eh = eh;
		return ret;
	}

	void Prompt( pcwstr message )
	{
		::MessageBoxW(NULL, message, L"Message", MB_OK);
	}
}
