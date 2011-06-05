#include "../OUE/OUE.h"

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
				ONEU_DELETE it->sp;
				spList.erase(it++);
			}
			else ++it;
		}
	}
	void _describe(String& buffer, int depth){
		buffer.append(L"<waterfall>\n");

	}
	void paint(){
		for(List<asprite>::iterator it = spList.begin(); it != spList.end(); ++it)
		{
			it->sp->visit_paint();
		}
	}
	~Waterfall(){
		for(List<asprite>::iterator it = spList.begin(); it != spList.end(); ++it)
			ONEU_DELETE it->sp;
	}
};