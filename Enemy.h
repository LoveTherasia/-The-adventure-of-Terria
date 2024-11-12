#pragma once

#include"Constant.h"
#include"Animation.h"
#include"Player.h"
#include"Bullet.h"
#include"vector"


class Enemy {
public:
	int Enemy_speed;
	int Enemy_WIDTH;
	int Enemy_HEIGHT;
	int SHADOW_WIDTH ;
	int eps ;//受到伤害后的无敌帧

	int MAX_HEALTH;
	int last = 0;//标记下一次受到伤害的时候
	int Enemy_Health;

	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool is_move = false;
	int HurtTime = 0;

	IMAGE Enemy_shadow;
	Animation* Enemy_move_left;
	Animation* Enemy_move_right;

public:
	enum Status
	{
		Clear = 0,
		GetHurt
	};

	Status status = Status::Clear;

public:
	POINT Enemy_pos = { 0,0 };

	bool alive = true;


	virtual void Move(const Player &player);//处理数据

	virtual void Draw();//输出画面

	virtual bool CheckBulletCollsion(const Bullet &bullet);//检测当前敌人是否被子弹击中

	virtual bool CheckPlayerCollsion(const Player &player);

	virtual void Hurt(DWORD HurtTime,int damage);

	virtual bool CheckAlive();

	virtual void CheckHurt(DWORD time);
};

class Solider : public Enemy
{
public:
	Solider();

	~Solider();

	void Move(const Player& player);

	void Draw();

	bool CheckBulletCollsion(const Bullet& bullet);

	bool CheckPlayerCollsion(const Player& player);

	void Hurt(DWORD HurtTime, int damage);

	bool CheckAlive();

	void CheckHurt(DWORD time);
};

class Airborne : public Enemy
{
public:
	int landingpos = -Enemy_HEIGHT;

	enum StatusStart
	{
		Unlanding = 0,
		Landing
	};

	StatusStart landingstatus = Unlanding;

public:

	Airborne();

	~Airborne();

	void Move(const Player& player);

	void Draw();

	bool CheckBulletCollsion(const Bullet& bullet);

	bool CheckPlayerCollsion(const Player& player);

	void Hurt(DWORD HurtTime, int damage);

	bool CheckAlive();

	void CheckHurt(DWORD time);

private:
	Animation* Enemy_Landing;
};

class Hound : public Enemy
{
public:

public:
	Hound();

	~Hound();

	void Move(const Player& player);

	void Draw();

	bool CheckBulletCollsion(const Bullet& bullet);

	bool CheckPlayerCollsion(const Player& player);

	void Hurt(DWORD HurtTime, int damage);

	bool CheckAlive();

	void CheckHurt(DWORD time);
};

class Spine : public Enemy
{
public:
	Spine();

	~Spine();

	void Move(const Player& player);

	void Draw();

	bool CheckBulletCollsion(const Bullet& bullet);

	bool CheckPlayerCollsion(const Player& player);

	void Hurt(DWORD HurtTime, int damage);

	bool CheckAlive();

	void CheckHurt(DWORD time);
};

void EnemySpawn();

