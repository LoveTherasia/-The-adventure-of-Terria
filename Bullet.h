#pragma once

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;
	
	POINT Bullet_pos{ 0,0 };

	const int radius = 10;

	//子弹的生成逻辑
	//好像不好去做成能直接直线发射出去的子弹(需要处理double型坐标）
	void Draw();
};