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

#include "Error.h"
#include "Logger.h"
#include "LoggerDisk.h"
#include <winnls.h>

#pragma warning(disable : 4996)
//此类内不可抛出Exception样错误

#include <cstdlib>//未必一定要包含此头文件 保留

#define LOG_PATH    L"Log.LOG"

namespace OneU
{
	LoggerDisk::LoggerDisk(void) :  m_Ptr(0), m_bValid( true ){
		m_pLog = _wfopen(LOG_PATH , L"wt");
		ASSERT(m_pLog);
		if(!m_pLog)
		{
			m_bValid = false;
		}
	}

	LoggerDisk::~LoggerDisk(void){
		flush();
		fclose(m_pLog);
	}

	void LoggerDisk::write(pcwstr lpwstrMsg ){
		//缓冲处理的写信息
		ASSERT(this);
		if(!m_bValid) return;

		//转化
		char* lpstrMsg;
		if(wcslen( lpwstrMsg ) < 500){
			static char MsgBuf[500];
			::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, lpwstrMsg, -1, MsgBuf, 500, NULL, NULL);
			lpstrMsg = MsgBuf;
		}
		else{
			lpstrMsg = new char[wcslen(lpwstrMsg)];
			::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, lpwstrMsg, -1, lpstrMsg, 500, NULL, NULL);
			delete lpstrMsg;
		}
		//

		size_t len = strlen(lpstrMsg);
		if(m_Ptr + len <= LOGGER_BUFFER_SIZE){
			memcpy(&m_Buffer[m_Ptr], lpstrMsg, len);
			m_Ptr += len;
			if(m_Ptr == LOGGER_BUFFER_SIZE){
				fwrite( m_Buffer, m_Ptr, 1, m_pLog );
				m_Ptr = 0;
			}
		}
		else{
			//@todo（长期） 此方法效率未必高
			size_t dist = LOGGER_BUFFER_SIZE - m_Ptr;
			memcpy(&m_Buffer[m_Ptr], lpstrMsg, dist);

			fwrite(m_Buffer, LOGGER_BUFFER_SIZE, 1, m_pLog);
			len -= dist;

			if(len>= LOGGER_BUFFER_SIZE){//当其余部分超过缓冲区长度时
				fwrite( &lpstrMsg[ dist ], len, 1, m_pLog );
				m_Ptr = 0;
			}
			else{
				m_Ptr = len;
				memcpy(m_Buffer, &lpstrMsg[ dist ], m_Ptr);
			}
		}
	}

	void LoggerDisk::flush(){
		ASSERT( this );
		if(!m_bValid) return;
		if(m_Ptr){
			fwrite(m_Buffer, m_Ptr , 1, m_pLog);
			m_Ptr = 0;
		}
	}

	ILogger* LoggerDisk::__new()
	{
		return ONEU_NEW LoggerDisk();
	}

	extern "C" ONEU_BASE_API ILogger* LoggerDisk_Factory()
	{
		return LoggerDisk::__new();
	}
}