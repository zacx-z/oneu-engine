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
#include "../Atom.h"
#include "../Label.h"
namespace OneU
{
    namespace atom_export
    {
        using namespace atom;
        class e_Label
                :public exported
        {
            ILabel* _p;
        public:
            e_Label(value* arg){
                float width,heigth;
                uint fontSize;
                pcwstr fontName;
                int i = uArgN(arg);
                if(i == 3)
                    fontName = L"Arial";
                else
                    fontName = utoArg<String>(arg, 3).c_str();
                width = utoArg<float>(arg,0);
                heigth = utoArg<float>(arg,1);
                fontSize = utoArg<int>(arg,2);
                _p = OneU::Label_create(width, heigth, fontSize, fontName);

				IScene* s = GetLocalScene();
				int z = 0;
				if(uArgN(arg) > 4)
					z = utoArg<int>(arg, 4);
				if(s != NULL)
					s->getRenderScene()->addChild(_p, z);
				else
					GetVideo().getRoot().addChild(_p, z);
            }
            value* e_setX(value* arg){
				_p->setX(utoArg<float>(arg, 0));
				return NULL;
			}
			value* e_setY(value* arg){
				_p->setY(utoArg<float>(arg, 0));
				return NULL;
			}
			value* e_getX(value* arg){
			    return makeValue(_p->getX());
			}    
			value* e_getY(value* arg){
			    return makeValue(_p->getY());
			}
			value* e_setText(value* arg){
				_p->setText(utoArg<String>(arg, 0).c_str());
				return NULL;
			}
			value* e_getText(value* arg){
			    return makeValue(_p->getText());
			} 
			value* e_setColor(value* arg){
				color_t color;
				color.setRed(utoArg<int>(arg, 0));
				color.setBlue(utoArg<int>(arg, 1));
				color.setGreen(utoArg<int>(arg, 2));
                _p->setColor(color);
                return NULL;
            }
            value* e_getColor(value* arg){
                int r,g,b;
                color_t color = _p->getColor();
                r = int(color.getRed());
                g = int(color.getGreen());
                b = int(color.getBlue());
                value* re = makeTupleWithElem(3, makeValue(r), makeValue(g), makeValue(b)); 
                return re;
            }
			value* e_getAlpha(value* arg){
				return makeValue((int)_p->getAlpha());
			}
			value* e_setAlpha(value* arg){
				_p->setAlpha(utoArg<int>(arg, 0));
				return NULL;
			}
        };
        inline void export_Label(){
            value* arg = makeKlass<e_Label>();
			klass* k = arg->get<klass>();
			GetAtom().getEnv()->createSymbol(L"Label", arg);
			uexportMethod(k, L"setX", &e_Label::e_setX);
			uexportMethod(k, L"setY", &e_Label::e_setY);
			uexportMethod(k, L"getX", &e_Label::e_getX);
			uexportMethod(k, L"getY", &e_Label::e_getY);
            uexportMethod(k, L"setText", &e_Label::e_setText);
			uexportMethod(k, L"getText", &e_Label::e_getText); 
			uexportMethod(k, L"setColor", &e_Label::e_setColor);
			uexportMethod(k, L"getColor", &e_Label::e_getColor);   
			uexportMethod(k, L"setAlpha", &e_Label::e_setAlpha);
			uexportMethod(k, L"getAlpha", &e_Label::e_getAlpha);   
        }
    }
}            


