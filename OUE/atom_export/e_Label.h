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
            ILabel* e_label;
        public:
            e_Label(value* v){
                float width,heigth;
                uint fontSize;
                pcwstr fontName;
                int i = v->get<tuple>()->length();
                if(i == 3)
                    fontName = L"Arial";
                else
                    fontName = v->get<tuple>()->get(3)->get<String>()->c_str();
                width = ugetArg<float>(v,0);
                heigth = ugetArg<float>(v,1);
                fontSize = ugetArg<int>(v,2);
                e_label = OneU::Label_create(width, heigth, fontSize, fontName);
            }
            value* e_setX(value* v){
				e_label->setX(ugetArg<float>(v, 0));
				return NULL;
			}
			value* e_setY(value* v){
				e_label->setY(ugetArg<float>(v, 0));
				return NULL;
			}
			value* e_getX(value* v){
			    return makeValue(e_label->getX());
			}    
			value* e_getY(value* v){
			    return makeValue(e_label->getY());
			}
			value* e_setText(value* v){
				e_label->setText(ugetArg<String>(v, 0).c_str());
				return NULL;
			}
			value* e_getText(value* v){
			    return makeValue(e_label->getText());
			} 
			value* e_setColor(value* v){
				color_t color;
				color.setRed(ugetArg<int>(v, 0));
				color.setBlue(ugetArg<int>(v, 1));
				color.setGreen(ugetArg<int>(v, 2));
                e_label->setColor(color);
                return NULL;
            }
            value* e_getColor(value* v){
                int r,g,b;
                color_t color = e_label->getColor();
                r = int(color.getRed());
                g = int(color.getGreen());
                b = int(color.getBlue());
                value* re = makeTupleWithElem(3, makeValue(r), makeValue(g), makeValue(b)); 
                return re;
            } 
        };
        inline void export_Label(){
            value* v = makeKlass<e_Label>();
			klass* k = v->get<klass>();
			GetAtom().getEnv()->createSymbol(L"Label", v);
			uexportMethod(k, L"setX", &e_Label::e_setX);
			uexportMethod(k, L"setY", &e_Label::e_setY);
			uexportMethod(k, L"getX", &e_Label::e_getX);
			uexportMethod(k, L"getY", &e_Label::e_getY);
            uexportMethod(k, L"setText", &e_Label::e_setText);
			uexportMethod(k, L"getText", &e_Label::e_getText); 
            uexportMethod(k, L"setColor", &e_Label::e_setColor);
			uexportMethod(k, L"getColor", &e_Label::e_getColor);   
        }
    }
}            


