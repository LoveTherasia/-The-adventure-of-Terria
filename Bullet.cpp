#include"Constant.h"
#include"Bullet.h"


void Bullet::Draw()
{
	setfillcolor(RGB(112, 227, 240));
	setlinecolor(RGB(206, 255, 247));
	fillcircle(Bullet_pos.x, Bullet_pos.y, radius);
}

