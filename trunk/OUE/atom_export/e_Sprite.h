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
#include "../Scene.h"
#include "../Sprite.h"
#include "../AtomLocal.h"
namespace OneU
{
	namespace atom_export
	{
		using namespace atom;
		class e_Sprite
			: public exported
		{
			ISprite* e_sprite;
		public:
			e_Sprite(value* arg){
				value* v = arg->get<tuple>()->get(0);
				image_t a;
				if(v->type() == T_STR)
					a = GetVideo().loadImage(v->get<String>()->c_str());
				else if(v->type() == T_OBJ)
				{
					obj* o = v->get<obj>();
					value* t = ucall(o, L"_getNodePtr", NULL);
					a = (video::IImage*)*t->get<void*>();
				}
				e_sprite = Sprite_create(a);

				//可选参数z
				int z = 0;
				if(arg->get<tuple>()->length() > 1)
					z = uto<int>(arg->get<tuple>()->get(1));

				IScene* s = GetLocalScene();
				if(s != NULL)
					s->getRenderScene()->addChild(e_sprite, z);
				else
					GetVideo().getRoot().addChild(e_sprite, z);
			}
			value* e_setX(value* v){
				e_sprite->setX(ugetArg<float>(v, 0));
				return NULL;
			}
			value* e_setY(value* v){
				e_sprite->setY(ugetArg<float>(v, 0));
				return NULL;
			}
			value* e_getX(value* v){
			    return makeValue(e_sprite->getX());
			}    
			value* e_getY(value* v){
			    return makeValue(e_sprite->getY());
			}    
			value* e_setRotation(value* v){
			    e_sprite->setRotation(ugetArg<float>(v, 0));
			    return NULL;
			}
      	    value* e_getRotation(value* v){
                return makeValue(e_sprite->getRotation());
            }
            value* e_setScaleX(value* v){
                e_sprite->setScaleX(ugetArg<float>(v, 0));
                return NULL;
            }
            value* e_getScaleX(value* v){
                return makeValue(e_sprite->getScaleX());
            }   
            value* e_setScaleY(value* v){
                e_sprite->setScaleY(ugetArg<float>(v, 0));
                return NULL;
            }
            value* e_getScaleY(value* v){
                return makeValue(e_sprite->getScaleY());
            }           
            value* e_setCenterX(value* v){
                e_sprite->setCenterX(ugetArg<float>(v, 0));
                return NULL;
            }
            value* e_getCenterX(value* v){
                return makeValue(e_sprite->getCenterX());
            }  
            value* e_setCenterY(value* v){
                e_sprite->setCenterY(ugetArg<float>(v, 0));
                return NULL;
            }
            value* e_getCenterY(value* v){
                return makeValue(e_sprite->getCenterY());
            } 
            value* e_setColor(value* v){
				color_t color;
				color.setRed(ugetArg<int>(v, 0));
				color.setBlue(ugetArg<int>(v, 1));
				color.setGreen(ugetArg<int>(v, 2));
                e_sprite->setColor(color);
                return NULL;
            }
            value* e_getColor(value* v){
                int r,g,b;
                color_t color = e_sprite->getColor();
                r = int(color.getRed());
                g = int(color.getGreen());
                b = int(color.getBlue());
                value* re = makeTupleWithElem(3, makeValue(r), makeValue(g), makeValue(b)); 
                return re;
            }       
            value* e_setAlpha(value* v){
                e_sprite->setAlpha((ubyte)ugetArg<int>(v, 0));
                return NULL;
            }
            value* e_getAlpha(value* v){
                return makeValue((int)e_sprite->getAlpha());
            }   
			~e_Sprite(){
				e_sprite->destroy();
			}
		};
		inline void export_Sprite(){
			value* v = makeKlass<e_Sprite>();
			klass* k = v->get<klass>();
			GetAtom().getEnv()->createSymbol(L"Sprite", v);
			uexportMethod(k, L"setX", &e_Sprite::e_setX);
			uexportMethod(k, L"setY", &e_Sprite::e_setY);
			uexportMethod(k, L"getX", &e_Sprite::e_getX);
			uexportMethod(k, L"getY", &e_Sprite::e_getY);
			uexportMethod(k, L"setRotation", &e_Sprite::e_setRotation);
			uexportMethod(k, L"getRotation", &e_Sprite::e_getRotation);
			uexportMethod(k, L"setScaleX", &e_Sprite::e_setScaleX);
			uexportMethod(k, L"getScaleX", &e_Sprite::e_getScaleX);
			uexportMethod(k, L"setScaleY", &e_Sprite::e_setScaleY);
			uexportMethod(k, L"getScaleY", &e_Sprite::e_getScaleY);
			uexportMethod(k, L"setCenterX", &e_Sprite::e_setCenterX);
			uexportMethod(k, L"getCenterX", &e_Sprite::e_getCenterX);
			uexportMethod(k, L"setCenterY", &e_Sprite::e_setCenterY);
			uexportMethod(k, L"getCenterY", &e_Sprite::e_getCenterY);
			uexportMethod(k, L"setColor", &e_Sprite::e_setColor);
			uexportMethod(k, L"getColor", &e_Sprite::e_getColor);
			uexportMethod(k, L"setAlpha", &e_Sprite::e_setAlpha);
			uexportMethod(k, L"getAlpha", &e_Sprite::e_getAlpha);
		}
	}
}

