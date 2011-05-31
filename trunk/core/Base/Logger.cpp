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
#include "LoggerDisk.h"

#include "String.h"
#include <cstdlib>
#include <memory>

namespace OneU
{
	//ILoggerµ•Ã¨÷∏’Î
	static ILogger * _Logger_pInstance = NULL;
	extern "C" ONEU_BASE_API void Logger_build(Factory<ILogger>::type lf){
		if(!_Logger_pInstance){
			_Logger_pInstance = lf();
		}
	}

	extern "C" ONEU_BASE_API ILogger& GetLogger()
	{
		ONEU_ASSERT( _Logger_pInstance );return *_Logger_pInstance;
	}

	extern "C" ONEU_BASE_API void Logger_destroy(){
		if(_Logger_pInstance != NULL){
			ONEU_DELETE _Logger_pInstance;
			_Logger_pInstance = NULL;
		}
	}
}
