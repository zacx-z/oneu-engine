#include "../OUE/OUE.h"

#include "Waterfall.h"

using namespace OneU;
using namespace atom;

class e_Waterfall
	: public exported
{
	Waterfall* v;
public:
	e_Waterfall(value* arg){
		color_t c(255, 255, 255);
		if(uArgN(arg) == 3)
			c = color_t(utoArg<int>(arg, 0), utoArg<int>(arg, 1), utoArg<int>(arg, 2));
		else if(uArgN(arg) == 4)
			c = color_t(utoArg<int>(arg, 0), utoArg<int>(arg, 1), utoArg<int>(arg, 2), utoArg<int>(arg, 3));

		v = ONEU_NEW(Waterfall(c));
	}
	~e_Waterfall(){
		v->destroy();
	}
};

ONEU_API void AtomInit(pcwstr){
	ucreateSymbol(GetAtom().getEnv(), L"Waterfall", makeKlass<e_Waterfall>());
}
ONEU_API void AtomDestroy(){
	;//nothing
}