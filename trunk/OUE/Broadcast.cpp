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
#include "Broadcast.h"
#include "String.h"

namespace OneU
{
	class Broadcast_Impl
		: public IBroadcast
	{
		//String msg_pool;
	public:
		//void message(pcwstr message){
		//	GetLogger().stream(ILogger::ML_TRIVIAL) << TimeInfo << message;
		//	msg_pool.append(message);msg_pool.append('\n');
		//}
		//pcwstr _getMessagePool(){ return msg_pool.c_str(); }
		void destroy();
	};
	void Broadcast_Impl::destroy(){
		ONEU_DELETE(this);
	}

	ONEU_API IBroadcast* BroadCast_create(){
		return ONEU_NEW(Broadcast_Impl);
	}
}