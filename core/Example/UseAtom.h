#pragma once
#include "../Atom.h"
#include "../Sprite.h"
namespace OneU
{
	namespace atom_export
	{
		using namespace atom;
		class Sprite
			: public exported
		{
			ISprite* m_sp;
		public:
			Sprite(value* v){
				v = v->get<tuple>()->get(0);
				image_t a;
				if(v->type() == T_STR)
					a = GetVideo().loadImage(v->get<String>()->c_str());
				else if(v->type() == T_OBJ)
				{
					obj* o = v->get<obj>();
					value* t = ucall(o, L"_getNodePtr", NULL);
					a = (video::IImage*)*t->get<void*>();
				}
				m_sp = Sprite_create(a);
				GetVideo().getRoot().addChild(m_sp);
			}
			value* set_x(value* v){
				m_sp->setX(ugetArg<float>(v, 0));
				return NULL;
			}
			value* set_y(value* v){
				m_sp->setY(ugetArg<float>(v, 0));
				return NULL;
			}
			~Sprite(){
				m_sp->destroy();
			}
		};
		inline void export_Sprite(){
			value* v = makeKlass<Sprite>();
			GetAtom().getEnv()->createSymbol(L"sprite", v);
			klass* k = v->get<klass>();
			uexportMethod(k, L"set_x", &Sprite::set_x);
			uexportMethod(k, L"set_y", &Sprite::set_y);
		}
	}
}