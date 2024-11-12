#pragma once
//ÿ��ͷ�ļ���ͷ����д�����ɱ����ظ�����
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include"Constant.h"

class Animation {

private:
	int timer = 0;//������ʱ��
	int idx_frame = 0;//����֡����
	int interval_ms = 0;
	std::vector<IMAGE*>frame_list;

public:
	Animation(LPCTSTR path, int num, int interval,int width,int height);
	~Animation();

	void Play(int x, int y, int delta);

};

#endif