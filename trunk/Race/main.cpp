#include "../OUE/Sprite.h"
#include "../OUE/Label.h"
#include "../OUE/Video.h"
#include "../OUE/Game.h"
#include "../OUE/Scene.h"
#include "../OUE/Control.h"
#include "../OUE/Stereo.h"

using namespace OneU;

class Car;

class RaceScene
	: public IScene
{
	Car* player1, *player2;

	ILabel* Text;
	enum STATUS{ BEGIN, GAMING, END } m_Status;
	int frame;
public:
	RaceScene();
	inline void update();
	~RaceScene();
};

class Car
{
	ISprite* _p;
	ILabel* _pLabel;
	static const float resist;
	float v, rot;
	double last_at;
	int laps;
public:
	Car(ISprite* sp, ILabel* l)
		: v(0.0f), rot(0.0f), last_at(0.1f), laps(0)
	{
		_p = sp;
		_p->setCenterX(0.5f);
		_p->setCenterY(0.5f);
		_pLabel = l;

		rot = 90;
		update();
	}
	ISprite* sprite(){ return _p; }
	ILabel* label(){ return _pLabel; }
	int getLaps(){ return (int)floor(laps / 2.0f); }
	void accelerate(){
		v += 0.15f;
	}
	void brake(){
		if(v > 0)v -= 0.4f;
		else v -= 0.1f;
	}
	void wheel_left(){
		rot -= 2.0f;
	}
	void wheel_right(){
		rot += 2.0f;
	}
	void update(){
		v *= resist;
		float x = _p->getX(), y = _p->getY();
		_p->setX(x + sin(rot / 180 * 3.1415926f) * v);
		_p->setY(y - cos(rot / 180 * 3.1415926f) * v);
		_p->setRotation(rot);

		float dx = _p->getX() - 400, dy = _p->getY() - 300;
		const float l = vector2(dx, dy).length();
		if(l > 290 || l < 210){
			v = -v;
			_p->setX(x + sin(rot / 180 * 3.1415926f) * v);
			_p->setY(y - cos(rot / 180 * 3.1415926f) * v);
			v *= 0.5f;
		}

		double at = atan2(dx, dy);
		if(at < 0) at += 3.1415926;
		if(at < 1.0f && last_at > 2.0f) ++laps;
		if(at > 2.0f && last_at < 1.0f) --laps;

		last_at = at;

		_pLabel->setText(String().format(L"LAPS: %d", getLaps()).c_str());
	}
};

const float Car::resist = 0.96f;

RaceScene::RaceScene()
{
	GetStereo().playMusic(L"back.mp3");
	image_t img = GetVideo().loadImage(L"car.png");

	player1 = ONEU_NEW(Car(Sprite_create(img), Label_create(200, 100, 20)));
	player2 = ONEU_NEW(Car(Sprite_create(img), Label_create(200, 100, 20)));
	player1->label()->setY(20);
	player2->label()->setY(75);
	player1->sprite()->setX(400);
	player1->sprite()->setY(550);
	player2->sprite()->setX(400);
	player2->sprite()->setY(550);
	player1->sprite()->setColor(color_t(200, 50, 50));
	player2->sprite()->setColor(color_t(50, 50, 200));
	getRenderScene()->addChild(Sprite_create(GetVideo().loadImage(L"track.png")));
	getRenderScene()->addChild(player1->sprite());
	getRenderScene()->addChild(player2->sprite());
	getRenderScene()->addChild(player1->label());
	getRenderScene()->addChild(player2->label());

	ILabel* l1 = Label_create(200, 100, 20);
	l1->setText(L"Player1");
	l1->setColor(color_t(200, 50, 50));
	ILabel* l2 = Label_create(200, 100, 20);
	l2->setText(L"Player2");
	l2->setY(50);
	l2->setColor(color_t(50, 50, 200));
	getRenderScene()->addChild(l1);
	getRenderScene()->addChild(l2);

	Text = Label_create(400, 200, 50);
	getRenderScene()->addChild(Text);

	m_Status = BEGIN;
	frame = 0;
}

void RaceScene::update()
{
	if(m_Status == GAMING){
		if(GetControl().keyIsDown(OIK_W))
			player1->accelerate();
		if(GetControl().keyIsDown(OIK_S))
			player1->brake();
		if(GetControl().keyIsDown(OIK_D))
			player1->wheel_right();
		if(GetControl().keyIsDown(OIK_A))
			player1->wheel_left();
		if(GetControl().keyIsDown(OIK_UP))
			player2->accelerate();
		if(GetControl().keyIsDown(OIK_DOWN))
			player2->brake();
		if(GetControl().keyIsDown(OIK_RIGHT))
			player2->wheel_right();
		if(GetControl().keyIsDown(OIK_LEFT))
			player2->wheel_left();
	
		player1->update();
		player2->update();

		if(frame <= 51){
			Text->setColor(color_t(255, 50, 50, 255 - frame * 5));
			++frame;
		}
		if(player1->getLaps() > 3){
			Text->setColor(color_t(255, 50, 50));
			Text->setText(L"Player1 won!!");
			Text->setX(280);
			m_Status = END;
		}
		if(player2->getLaps() > 3){
			Text->setColor(color_t(255, 50, 50));
			Text->setText(L"Player2 won!!");
			Text->setX(280);
			m_Status = END;
		}
	}else if(m_Status == BEGIN){
		if(frame == 0){
			Text->setText(L"3");
			Text->setX(390);Text->setY(200);
		}
		if(frame == 51){
			Text->setText(L"2");
		}
		if(frame == 102){
			Text->setText(L"1");
		}
		if(frame == 153){
			Text->setText(L"Go");
			Text->setX(375);
			m_Status = GAMING;
			frame = 0;
		}
		Text->setColor(color_t(255, 50, 50, 255 - (frame % 51) * 5));
		++frame;
	}
}

RaceScene::~RaceScene()
{
	ONEU_DELETE(player1);
	ONEU_DELETE(player2);
}

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ::LPSTR lpCmdLine, int nShowCmd){
	Game_build(Game_create);
	IGame& game = GetGame();
	game.init(L"赛车-张光远", 800, 600, true);

	game.replaceScene(ONEU_NEW(RaceScene));
	return game.run();
}
