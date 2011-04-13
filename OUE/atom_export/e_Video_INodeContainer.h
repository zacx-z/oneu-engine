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
#include "../Video.h"
//·ÏÆú
namespace OneU
{
	namespace atom_export
	{
	    using namespace atom;
	    class e_NodeContainer
	         :public exported
        {
            video::INodeContainer* e_nodeContainer;
        public:
            e_NodeContainer(value* v)
            {
				e_nodeContainer = ONEU_NEW(video::INodeContainer);
            }
            value* e_addChild(value* v)
            {
				value* v1 = v->get<tuple>()->get(0);
				int i = v->get<tuple>()->length();
				video::INode* a;
				if(v1->type() == T_OBJ)
				{
					obj* o = v1->get<obj>();
					value* t = ucall(o, L"_getINodePtr", NULL);
					a = (video::INode*)*t->get<void*>();
				}
				if(i == 1)
					e_nodeContainer->addChild(a);
				else if(i == 2)
					e_nodeContainer->addChild(a, ugetArg<int>(v->get<tuple>()->get(1),0));
				else if(i == 3)
					e_nodeContainer->addChild(a,  ugetArg<int>(v->get<tuple>()->get(1),0),  ugetArg<String>(v->get<tuple>()->get(2),0).c_str());
				return NULL;
            }    
            value* e_paint(value* v)
            {
                e_nodeContainer->paint();
                return NULL;
            }
            value* e_describe(value* v)
            {
                String buffer;
                int depth;   
                buffer = v->get<tuple>()->get(0)->get<String>()->c_str();
                depth = *v->get<tuple>()->get(1)->get<int>();
                e_nodeContainer->describe(buffer, depth); 
                return NULL;
            }
            ~e_NodeContainer()
            {
                e_nodeContainer->~INodeContainer();
            }
        };
        inline void export_NodeContainer()
        {
            value* v = makeKlass<e_NodeContainer>();
			klass* k = v->get<klass>();
			GetAtom().getEnv()->createSymbol(L"NodeContainer", v);
            uexportMethod(k, L"paint", &e_NodeContainer::e_paint);    
            uexportMethod(k, L"describe", &e_NodeContainer::e_describe);
			uexportMethod(k, L"addChild", &e_NodeContainer::e_addChild);
        }
    }
}             
                    
