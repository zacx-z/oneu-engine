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
#include "Shell.h"
#include "../AtomLocal.h"
#include "../Broadcast.h"
#include "../Video.h"
#include "../Game.h"
namespace OneU
{
	namespace tool
	{
		//commands
		//前置定义
		namespace shell
		{
			using namespace atom;
			value* cls(value*);
			value* echo(value* arg);
			value* info(value* arg);
			value* fps(value* arg);
			value* quit(value* arg);
		}

		static Shell* s_pShell;

		Shell* Shell_config(){
			//添加Watch
			ShellController* p = ONEU_NEW(ShellController);
			s_pShell = p->getShell();
			GetBroadcast().addEventListener(event::ENTER_FRAME, p);

			atom::value* v = atom::makeEnv();
			ONEU_ENSURE(GetAtom().getEnv()->createSymbol(L"shell", v));
			//注册Command
			atom::ExportShellCommand(L"cls", shell::cls);
			atom::ExportShellCommand(L"echo", shell::echo);
			atom::ExportShellCommand(L"info", shell::info);
			atom::ExportShellCommand(L"fps", shell::fps);
			atom::ExportShellCommand(L"quit", shell::quit);
			atom::ExportShellCommand(L"exit", shell::quit);
			return p->getShell();
		}

		//commands
		namespace shell
		{
			value* cls(value*){
				s_pShell->clearScreen();
				return NULL;
			}
			value* echo(value* arg){
				if(uArgN(arg) < 1){
					s_pShell->output(L"Too few arguments.\n");
					return NULL;
				}
				s_pShell->output(ugetArg<String>(arg, 0).c_str());
				return NULL;
			}
			value* info(value* arg){
				if(uArgN(arg) < 1){
					GetGame().showInfo();
					return NULL;
				}
				if(ugetArg<String>(arg, 0) == L"video"){
					GetVideo().showInfo();
				}
				return NULL;
			}
			value* fps(value* arg){
				GetGame().stream() << L"FPS: " << GetGame().getFPS() << endl;
				return NULL;
			}
			value* quit(value* arg){
				GetGame().quit();
				return NULL;
			}
		}
	}

}