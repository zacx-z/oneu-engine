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
#pragma once
#include "../Event.h"
#include "../Atom.h"
#include "../Label.h"
#include "../Shape.h"

#include "Watch.h"

namespace OneU
{
	namespace tool
	{
		//shell
		class Shell//为什么INodeContainer放在第二位程序会崩溃。总之是不建议多重继承
			: public video::INodeContainer, public IInputReceiver
		{
			ILabel* m_pInput;
			IShape* m_pBack;
			String m_InputStr;
			TextField* m_pTextField;
			IShape* m_pTextBack;

			String m_CommandOut;
			int frame;
		public:
			Shell(){
				m_pInput = Label_create(800, 20, 15, L"Terminal");
				m_pInput->setX(0);
				m_pInput->setY(500);
				m_pInput->setText(L">");

				m_pBack = Shape_rect(800, 15);
				m_pBack->setColor(color_t(20, 20, 50, 127));
				m_pBack->setX(0);
				m_pBack->setY(500);

				m_pTextBack = Shape_rect(800, 450);
				m_pTextBack->setColor(color_t(20, 20, 50, 127));

				m_pTextField = ONEU_NEW(TextField(800, 450, 15, L"Terminal"));
				m_pTextField->setMaxRows(37);

				this->addChild(m_pBack);
				this->addChild(m_pInput);
				this->addChild(m_pTextBack);
				this->addChild(m_pTextField);
				frame = 0;
			}
			virtual void update(){
				if(frame++ == 20){
					m_pInput->setText((L">" + m_InputStr + L"_").c_str());
				}
				if(frame == 40){
					m_pInput->setText((L">" + m_InputStr).c_str());
					frame = 0;
				}
				video::INodeContainer::update();
			}
			void _refreshPos(){
				int l = m_pTextField->getLines();
				if(l > m_pTextField->getMaxRows()){
					m_pTextField->setPos(l - m_pTextField->getMaxRows());
				}
			}
			void output(pcwstr str){
				m_CommandOut.append(str);
				m_pTextField->setText(m_CommandOut.c_str());
				//在visible为false的情况下 这是冗余的
				if(this->visible){
					_refreshPos();
				}
			}
			bool _runCommand(pcwstr str){
				atom::value* v = GetAtom().getEnv()->getSymbol(L"shell");
				if(v == NULL) return false;
				atom::env* e = v->get<atom::env>();
				if(e == NULL) return false;

				int i = 0;
				String s;
				while(str[i] != L'\0'&& str[i] != L' '){
					s.append(str[i]);
					++i;
				}
				atom::value* t = e->getSymbol(s.c_str());
				if(!t) return false;

				//获取参数
				List<String> arg_list;
				while(str[i] != L'\0'){
					while(str[i] == L' ') ++i;
					if(str[i] == L'\0') break;
					s = L"";
					while(str[i] != L' ' && str[i] != L'\0'){
						s.append(str[i]);
						++i;
					}
					arg_list.pushBack(s);
				}

				atom::value* arg = atom::makeTuple(arg_list.size());
				List<String>::iterator it = arg_list.begin();
				for(uint i = 0; i < arg_list.size(); ++i, ++it){
					arg->get<atom::tuple>()->get(i)->eval(*it);
				}
				if(t)t->get<atom::function>()->call(arg);
				return true;
			}
			void runCommand(pcwstr str){
				if(!_runCommand(str))
					output(L"Can't find the Command.\n");
			}
			void onChar(CharEvent& event){
				wchar ch = event.getCode();
				switch(ch)
				{
				case L'\r':
				case L'\n':
					output((String(L">") + m_InputStr + L"\n").c_str());
					runCommand(m_InputStr.c_str());
					m_InputStr = L"";
					break;
				default:
					m_InputStr.append(ch);
					break;
				case L'\b':
					m_InputStr.cut(1);
					break;
				case 27://Escape键
					m_InputStr = L"";
				}
				m_pInput->setText((L">" + m_InputStr).c_str());
				frame = 20;//让光标立刻显现
			}

			void clearScreen(){
				m_CommandOut = L"";
				m_pTextField->setText(L"");
			}
		};
		//holds shell指针
		//监视键盘 并在按中特定按钮时显示shell或Watch
		class ShellController
			: public IEventListener
		{
			Watch* m_pWatch;

			//input
			IInputReceiver* m_pLastED;
			Shell* m_pShell;
		public:
			ShellController()
				: m_pWatch(NULL)
			{
				m_pWatch = ONEU_NEW(Watch);
				GetVideo().getRoot().addChild(m_pWatch, 100);
				m_pWatch->visible = false;

				//input
				m_pShell = ONEU_NEW(Shell);
				m_pShell->visible = false;
				GetVideo().getRoot().addChild(m_pShell, 101);
			}
			~ShellController(){
			}
			void onEvent(pcwstr event, EventArgs*, IEventDispatcher* source){
				if(!m_pShell->visible && GetControl().keyRelease(OIK_SLASH)){
					m_pWatch->visible = !m_pWatch->visible;
					if(m_pWatch->visible)
						m_pWatch->refreshContents();
				}
				//shell
				if(!m_pShell->visible && !m_pWatch->visible && GetControl().keyRelease(OIK_PERIOD)){
					m_pShell->visible = true;
					m_pShell->_refreshPos();

					m_pLastED = GetGame().setInputFocus(m_pShell);

				}
				if(m_pShell->visible && GetControl().keyRelease(OIK_ESCAPE)){
					m_pShell->visible = false;
					GetGame().setInputFocus(m_pLastED);
				}
			}
			Shell* getShell(){ return m_pShell; }
		};
		//在Game初始化时调用该函数来配置好该工具
		//返回值不需要delete
		Shell* Shell_config();
	}
}