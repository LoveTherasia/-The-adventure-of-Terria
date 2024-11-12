#pragma once
//每个头文件开头这样写，即可避免重复引用
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include"Constant.h"

class Animation {

private:
	int timer = 0;//动画计时器
	int idx_frame = 0;//动画帧索引
	int interval_ms = 0;
	std::vector<IMAGE*>frame_list;

public:
	Animation(LPCTSTR path, int num, int interval,int width,int height);
	~Animation();

	void Play(int x, int y, int delta);

};

#endif