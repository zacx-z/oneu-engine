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
#include "../Control.h"
#include "../Video.h"
#include "../Game.h"
namespace OneU
{
	namespace tool
	{
		class TextField
			: public video::INode
		{
			ILabel* m_pText;
			String text;
			int str_pos, pos;
			int maxrows;
		public:
			TextField(float width, float height, uint32 fontsize, pcwstr fontname = L"Arial"){
				str_pos = pos = 0;
				maxrows = 0;
				m_pText = Label_create(width, height, fontsize, fontname);
			}
			~TextField(){
				ONEU_DELETE m_pText;
			}
			void setPos(int i){
				pos = i;
				str_pos = 0;
				while(text[str_pos] != L'\0' && i > 0){
					++str_pos;
					while(text[str_pos] != L'\0' && text[str_pos] != L'\n')
						++str_pos;
					--i;
				}
				if(text[str_pos] == L'\n') ++str_pos;
				pos -= i;//如果已经到字符串末尾要对pos修正

				if(maxrows != 0){
					int m = maxrows;
					int end_pos = str_pos;
					while(text[end_pos] != L'\0' && m-- > 0){
						++end_pos;
						while(text[end_pos] != L'\0' && text[end_pos] != L'\n')
							++end_pos;
					}
					wchar ch = text[end_pos];
					text[end_pos] = L'\0';
					m_pText->setText(&text.c_str()[str_pos]);
					text[end_pos] = ch;
				}
				else
					m_pText->setText(&text.c_str()[str_pos]);
			}
			void setText(pcwstr t){
				text = t;
				setPos(0);
			}
			pcwstr getText(){ return text.c_str(); }
			void setX(float x){
				m_pText->setX(x);
			}
			void setY(float y){
				m_pText->setY(y);
			}
			void setMaxRows(int m){ maxrows = m;}
			int getMaxRows(){ return maxrows; }
			int getLines(){
				int i = 0, ret = 0;
				while(text[i] != L'\0')
					if(text[i++] == L'\n') ++ret;
				return ret + 1;
			}
			int getPos(){ return pos; }
			void paint(){
				m_pText->paint();
			}
		};
		//应看做Shell的一部分
		class Watch
			: public video::INodeContainer
		{
			IShape* m_pBack;
			TextField* m_pText;
			static const int TabNum = 3;
			ILabel* m_pTab[TabNum];
			int curTabIndex;
		public:
			Watch(){
				m_pBack = Shape_rect(rect(0, 0, 600, 500));
				m_pBack->setX(100);m_pBack->setY(50);
				m_pBack->setColor(color_t(127, 127, 127, 127));

				m_pText = ONEU_NEW TextField(500, 400, 15);
				m_pText->setX(150);m_pText->setY(80);
				m_pText->setMaxRows(30);

				m_pTab[0] = Label_create(100, 30, 20);
				m_pTab[0]->setX(150);m_pTab[0]->setY(55);
				m_pTab[0]->setText(L"Atom");

				m_pTab[1] = Label_create(100, 30, 20);
				m_pTab[1]->setX(250);m_pTab[1]->setY(55);
				m_pTab[1]->setText(L"AtomSys");

				m_pTab[2] = Label_create(100, 30, 20);
				m_pTab[2]->setX(350);m_pTab[2]->setY(55);
				m_pTab[2]->setText(L"RenderNode");

				curTabIndex = 0;
				m_pTab[curTabIndex]->setColor(color_t(50, 50, 127));


				this->addChild(m_pBack, 0);
				this->addChild(m_pText, 1);
				for(int i = 0; i < TabNum; ++i)
					this->addChild(m_pTab[i], 2 + i);
			}
			void refreshContents(){
				String str = L"";
				switch(curTabIndex)
				{
				case 0:
					GetAtom().getDescription(GetAtom().getEnvValue(), str);
					m_pText->setText(str.c_str());
					break;
				case 1:
					GetAtom().getDescription(GetAtom().getSystemEnvValue(), str);
					m_pText->setText(str.c_str());
					break;
				case 2:
					GetVideo().getRoot().getDescription(str);
					m_pText->setText(str.c_str());
					break;
				}
			}

			void nextTab(){
				m_pTab[curTabIndex]->setColor(color_t(255, 255, 255));
				if(++curTabIndex == TabNum) curTabIndex = 0;
				m_pTab[curTabIndex]->setColor(color_t(50, 50, 127));

				refreshContents();
			}
			void update(){
				bool key_event = false;

				if(GetControl().keyRelease(OIK_COMMA)){
					nextTab();
				}
				if(m_pText->visible){
					if(GetControl().keyIsDown(OIK_DOWN)){
						m_pText->setPos(m_pText->getPos() + 1);
					}
					if(GetControl().keyIsDown(OIK_UP)){
						m_pText->setPos(m_pText->getPos() - 1);
					}
				}
			}
		};
	}
}