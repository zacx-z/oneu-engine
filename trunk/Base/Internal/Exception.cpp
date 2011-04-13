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
#include <windows.h>
#include "../Error.h"
#include "Exception.h"
#include "../Logger.h"


namespace OneU
{
	Exception::Exception( wchar_t * name, bool MakeDump )
	{}

	pcwstr Exception::GetType() const
	{
		return L"错误";
	}
	pcwstr Exception::GetString() const
	{
		return L"程序发生了一个未知错误";
	}


	extern "C" ONEU_BASE_API int PreThrow(const char * FileName, const int Line, Exception& Exp)
	{
#ifdef _DEBUG
		wchar WFileName[ MAX_PATH ];
		MultiByteToWideChar( CP_ACP, 0, FileName, -1, WFileName, MAX_PATH );

		String str;

		str.format(L"错误：%s\n源代码：%s(%d)\n\n按“终止”结束程序，“重试”调试程序，“忽略”抛出错误。",
			Exp.GetString(), WFileName, Line);

		int result = MessageBox( NULL, str.c_str(), Exp.GetType(), MB_ABORTRETRYIGNORE | MB_ICONSTOP );
		switch( result )
		{
		case IDABORT:
			ONEU_RAISE(Exp);
		case IDRETRY:
			return 0;
		case IDIGNORE:
			return 1;
		}
#else
		_TerminateApp(FileName, Line, Exp);
#endif
		return 1;
	}
}