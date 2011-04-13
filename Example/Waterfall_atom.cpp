#include <OUE/OUE.h>

using namespace OneU;

class Waterfall
	: public video::INode
{
	image_t img;
	color_t c;
	struct asprite
	{
		float v;
		ISprite* sp;
	};
	List<asprite> spList;
public:
	Waterfall(color_t c)
		: c(c)
	{
		img = GetVideo().loadImage(L"Resource/Waterfall/unit.png");
		IScene* s = atom::GetLocalScene();
		if(s)
			s->getRenderScene()->addChild(this);
	}
	void update(){
		if(os::Rand() < 0.3f){
			ISprite* sp = Sprite_create(img);
			sp->setCenterX(0.5f);sp->setCenterY(1.0f);
			sp->setX(os::Rand() * GetVideo().getDeviceSize().x);
			sp->setBlendMode(video::BM_ADD);
			sp->setColorBlendMode(video::CBM_MODULATE);
			sp->setColor(c);

			asprite x;
			x.v = 0.0f; x.sp = sp;
			spList.pushBack(x);
		}
		for(List<asprite>::iterator it = spList.begin(); it != spList.end();){
			asprite& x = *it;
			ISprite* sp = x.sp;
			x.v += 0.01f;
			sp->setY(sp->getY() + x.v);
			sp->setAlpha(sp->getAlpha() - 1);
			if(sp->getAlpha() <= 0){
				it->sp->destroy();
				spList.erase(it++);
			}
			else ++it;
		}
	}
	void describe(String& buffer, int depth){
		buffer.append(L"<waterfall>\n");

	}
	void paint(){
		for(List<asprite>::iterator it = spList.begin(); it != spList.end(); ++it)
		{
			it->sp->paint();
		}
	}
	~Waterfall(){
		for(List<asprite>::iterator it = spList.begin(); it != spList.end(); ++it)
			it->sp->destroy();
	}
};


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
};

ONEU_API void AtomInit(pcwstr){
	ucreateSymbol(GetAtom().getEnv(), L"Waterfall", makeKlass<e_Waterfall>());
}
ONEU_API void AtomDestroy(){
	;//nothing
}