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

#include "LoggerDisk.h"

#pragma warning(disable : 4996)
//此类内不可Raise错误

#include <cstdlib>//未必一定要包含此头文件 保留
#include <io.h>


#define LOG_PATH    L"Log.LOG"

namespace OneU
{
	LoggerDisk::LoggerDisk(void) :  m_Ptr(0), m_bValid( true ){
		m_pLog = _wfopen(LOG_PATH , L"wt");
		ONEU_ASSERT(m_pLog);
		if(!m_pLog)
		{
			m_bValid = false;
		}
	}

	LoggerDisk::~LoggerDisk(void){
		flush();
		if(m_pLog) {
			fclose(m_pLog);//BUGS:官方ruby库会导出fclose符号，会使这句崩溃。因此使用的是自行编译的ruby。
		}
	}

	void LoggerDisk::write(pcwstr lpwstrMsg ){
		//缓冲处理的写信息
		ONEU_ASSERT(this);
		if(!m_bValid) return;

		AutoPtr<char> p = Wide2Char(lpwstrMsg, 65001);
		fwrite(&(*p), strlen(&(*p)) * sizeof(char), 1, m_pLog);
	}

	void LoggerDisk::flush(){
		ONEU_ASSERT( this );
		if(!m_bValid) return;
		fflush(m_pLog);
	}


	extern "C" ONEU_API ILogger* LoggerDisk_Factory()
	{
		return ONEU_NEW LoggerDisk;
	}
}