#pragma once

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;
	
	POINT Bullet_pos{ 0,0 };

	const int radius = 10;

	//�ӵ��������߼�
	//���񲻺�ȥ������ֱ��ֱ�߷����ȥ���ӵ�(��Ҫ����double�����꣩
	void Draw();
};