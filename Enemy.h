#pragma once

#ifndef _ENEMY_H_
#define _ENEMY_H_


#include"Constant.h"
#include"Animation.h"
#include"Player.h"
#include"Bullet.h"


class Enemy {

private:
	const int Enemy_speed = 2;
	const int Enemy_WIDTH = 200;
	const int Enemy_HEIGHT = 200;
	const int SHADOW_WIDTH = 70;
	
	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool is_move = false;

	IMAGE Enemy_shadow;
	Animation* Enemy_move_left;
	Animation* Enemy_move_right;


public:
	POINT Enemy_pos = { 0,0 };

	bool alive = true;

	Enemy();

	~Enemy();

	void EnemySpawn();

	void Move(const Player &player);//��������

	void Draw(int delta);//�������

	bool CheckBulletCollsion(const Bullet &bullet);//��⵱ǰ�����Ƿ��ӵ�����

	bool CheckPlayerCollsion(const Player &player);

	void Hurt();

	bool CheckAlive();
};

void EnemySpawn();


#endif

