#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"Animation.h"
#include"Constant.h"

class Player {

private:
	const int Player_speed = 5;
	const int SHADOW_WIDTH = 70;
	const int PLAYER_WIDTH = 200;
	const int PLAYER_HEIGHT = 200;

	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool is_move = false;

	Animation* player_relax_left;
	Animation* player_relax_right;
	Animation* player_move_left;
	Animation* player_move_right;

public:
	POINT Player_pos = { 50,50 };

	int score = 0;

	Player();	//在构造的时候进行图片加载
	
	~Player();

	void Process(const ExMessage& msg);

	void Move();

	void Draw(int delta);

	const POINT& GetPostion() const
	{
		return Player_pos;
	}
};

#endif

