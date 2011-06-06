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
			ISprite* _p;
		public:
			e_Sprite(value* arg){
				image_t a;
				if(arg->get<tuple>()->get(0)->type() == T_STR)
					a = GetVideo().loadImage(utoArg<String>(arg, 0).c_str());
				else {
					obj* o = &ugetArg<obj>(arg, 0);
					value* t = ucall(o, L"_getImagePtr", NULL);
					a = (video::IImage*)*t->get<void*>();
				}
				_p = Sprite_create(a);

				//可选参数z
				int z = 0;
				if(uArgN(arg) > 1)
					z = uto<int>(arg->get<tuple>()->get(1));

				IScene* s = GetLocalScene();
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
			value* e_setRotation(value* arg){
			    _p->setRotation(utoArg<float>(arg, 0));
			    return NULL;
			}
      	    value* e_getRotation(value* arg){
                return makeValue(_p->getRotation());
            }
            value* e_setScaleX(value* arg){
                _p->setScaleX(utoArg<float>(arg, 0));
                return NULL;
            }
            value* e_getScaleX(value* arg){
                return makeValue(_p->getScaleX());
			}
            value* e_setScaleY(value* arg){
                _p->setScaleY(utoArg<float>(arg, 0));
                return NULL;
            }
            value* e_getScaleY(value* arg){
                return makeValue(_p->getScaleY());
            }
            value* e_setCenterX(value* arg){
                _p->setCenterX(utoArg<float>(arg, 0));
                return NULL;
            }
            value* e_getCenterX(value* arg){
                return makeValue(_p->getCenterX());
            }
            value* e_setCenterY(value* arg){
                _p->setCenterY(utoArg<float>(arg, 0));
                return NULL;
            }
            value* e_getCenterY(value* arg){
                return makeValue(_p->getCenterY());
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
            value* e_setAlpha(value* arg){
                _p->setAlpha((ubyte)utoArg<int>(arg, 0));
                return NULL;
            }
            value* e_getAlpha(value* arg){
                return makeValue((int)_p->getAlpha());
            }
			value* e_setBlendMode(value* arg){
				_p->setBlendMode((video::BLENDMODE)utoArg<int>(arg, 0));
				return NULL;
			}
			value* e_getBlendMode(value* arg){
				return makeValue((int)_p->getBlendMode());
			}
			value* e_setColorBlendMode(value* arg){
				_p->setColorBlendMode(utoArg<int>(arg, 0));
				return NULL;
			}
			value* e_getColorBlendMode(value* arg){
				return makeValue((int)_p->getColorBlendMode());
			}
			~e_Sprite(){
				ONEU_DELETE _p;
			}
		};
		inline void export_Sprite(){
			value* arg = makeKlass<e_Sprite>();
			klass* k = arg->get<klass>();
			GetAtom().getEnv()->createSymbol(L"Sprite", arg);
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
			uexportMethod(k, L"setBlendMode", &e_Sprite::e_setBlendMode);
			uexportMethod(k, L"getBlendMode", &e_Sprite::e_getBlendMode);
			uexportMethod(k, L"setColorBlendMode", &e_Sprite::e_setColorBlendMode);
			uexportMethod(k, L"getColorBlendMode", &e_Sprite::e_getColorBlendMode);
		}
	}
}

