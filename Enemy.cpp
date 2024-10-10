#include"Constant.h"
#include"Animation.h"
#include"Enemy.h"
#include"Player.h"
#include"Bullet.h"

std::vector<Enemy*>Enemy_list;
int Enemynum = 0;

Enemy::Enemy()
{
	std::mt19937 myrand(clock());
	loadimage(&Enemy_shadow, _T("assert/shadow_player.png"), SHADOW_WIDTH);
	Enemy_move_left = new Animation(_T("assert/Enemy/soldies/Enemyleft%d.png"),7,45);
	Enemy_move_right = new Animation(_T("assert/Enemy/soldies/Enemyright%d.png"), 7, 45);

	enum SpawnEdge
	{
		UP = 0,
		Down,
		Left,
		Right

	};

	SpawnEdge Edge = (SpawnEdge)(myrand() % 4);
	
	switch (Edge)
	{
	case SpawnEdge::UP:
		Enemy_pos.x = myrand() % 1280;
		Enemy_pos.y = -Enemy_HEIGHT;
		break;
	case SpawnEdge::Down:
		Enemy_pos.x = myrand() % 1280;
		Enemy_pos.y = 720;
		break;
	case SpawnEdge::Left:
		Enemy_pos.x = -Enemy_WIDTH;
		Enemy_pos.y = myrand() % 720;
		break;
	case SpawnEdge::Right:
		Enemy_pos.x = 1280;
		Enemy_pos.y = myrand() % 720;
		break;
	}
}

Enemy::~Enemy()
{
	delete Enemy_move_left;
	delete Enemy_move_right;
}

void Enemy::Move(const Player& player)
{

	if (player.Player_pos.x > Enemy_pos.x) move_right = true,move_left=false;
	else if (player.Player_pos.x < Enemy_pos.x) move_left = true, move_right = false;
	else move_left = false, move_right = false;

	if (player.Player_pos.y > Enemy_pos.y) move_down = true,move_up=false;
	else if (player.Player_pos.y < Enemy_pos.y) move_up = true, move_down = false;
	else move_up = false, move_down = false;

	int dir_x = move_right - move_left;
	int dir_y = move_down - move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0) {
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		Enemy_pos.x += (int)(Enemy_speed * normalized_x);
		Enemy_pos.y += (int)(Enemy_speed * normalized_y);
	}
}

void EnemySpawn()
{
	//能不能实现时间越久难度越高?
	static int num = 0;
	static int counter = 100;
	if ((++num) % counter == 0)
	{
		Enemy_list.push_back(new Enemy);
		if (counter > 1) counter--;
		Enemynum++;
	}
	printf("%d %d\n", counter,Enemynum);
}

void Enemy::Draw(int delta)
{
	int Shadow_x = Enemy_pos.x + (Enemy_WIDTH - SHADOW_WIDTH) / 2;
	int Shadow_y = Enemy_pos.y + Enemy_HEIGHT - 50;
	putimage_alpha(Shadow_x, Shadow_y, &Enemy_shadow);

	static bool facing_left = false;
	int div_r = move_right - move_left;
	if (div_r < 0) facing_left = true;
	else if (div_r > 0) facing_left = false;

	if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
	else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

	putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
}

bool Enemy::CheckBulletCollsion(const Bullet &bullet)
{
	int bullet_x = bullet.Bullet_pos.x;
	int bullet_y = bullet.Bullet_pos.y;
	int bullet_r = bullet.radius;

	int enemy_leftx = Enemy_pos.x + 70;
	int enemy_lefty = Enemy_pos.y + 80;
	int enemy_rightx = Enemy_pos.x + 120;
	int enemy_righty = Enemy_pos.y + 160;

	if (bullet_x >= enemy_leftx && bullet_x <= enemy_rightx)
	{
		if (bullet_y >= enemy_lefty && bullet_y <= enemy_righty)
			return true;
	}

	return false;
}

bool Enemy::CheckPlayerCollsion(const Player& player)
{
	//人物宽大概为200，高大概为300，图片宽和高都为1000
	POINT check_postion = { Enemy_pos.x +  Enemy_WIDTH/2 , Enemy_pos.y + Enemy_HEIGHT/2 };
	

	//敌人宽大概为100，高大概为200
	if (check_postion.x >= player.Player_pos.x + 70 && check_postion.x <= player.Player_pos.x + 100)
	{
		if (check_postion.y >= player.Player_pos.y + 50 && check_postion.y <= player.Player_pos.y + 140)
			return true;
	}
	return false;
}

void Enemy::Hurt()
{
	alive = false;
}

bool Enemy::CheckAlive()
{
	return alive;
}
