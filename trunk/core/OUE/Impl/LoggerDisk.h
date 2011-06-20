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
/**
 * @file LoggerDisk.h
 * @brief 磁盘日志
 *
 * 将日志记录到磁盘的日志实现。
 * @author Ladace
 * @version 1.0.0.1
 * @date 2011-04-09
 */
#pragma once

#include <windows.h>
#include "../Logger.h"

#define LOGGER_BUFFER_SIZE 1024

namespace OneU
{
	class LoggerDisk
		: public ILogger
	{
	private:

		FILE * m_pLog;
		char m_Buffer[ LOGGER_BUFFER_SIZE ];
		size_t m_Ptr;
		bool m_bValid;

	public:
		LoggerDisk(void);
		~LoggerDisk(void);
		void flush();
		void write( pcwstr lpstrLog );
	};
}
