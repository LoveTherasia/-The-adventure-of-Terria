#include"Constant.h"
#include"Animation.h"
#include"Enemy.h"
#include"Player.h"
#include"Bullet.h"

std::vector<Enemy*>Enemy_list;
int Enemynum = 0;


void Enemy::Move(const Player& player)
{
	move_right = false;
	move_left = true;
	move_right = player.Player_pos.x > Enemy_pos.x;
	move_left = player.Player_pos.x < Enemy_pos.x;

	move_down = false;
	move_up = false;
	move_down = player.Player_pos.y > Enemy_pos.y;
	move_up = player.Player_pos.y < Enemy_pos.y;

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

void Enemy::Draw()
{
	if (status == Status::GetHurt)
	{
		HurtTime++;
		if (((HurtTime - 1) / 5) % 2 == 0) return;
	}

	static bool facing_left = false;
	int div_r = move_right - move_left;

	facing_left = div_r < 0;

	if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
	else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

	putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
	rectangle(Enemy_pos.x+30, Enemy_pos.y + Enemy_HEIGHT-40, Enemy_pos.x + Enemy_WIDTH-30, Enemy_pos.y + Enemy_HEIGHT-30);

	int health = (double)(Enemy_WIDTH - 60) * ((double)Enemy_Health / (double)MAX_HEALTH);
	setfillcolor(RED);
	fillrectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + 30 + health, Enemy_pos.y + Enemy_HEIGHT - 30);
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

	return	bullet_x >= enemy_leftx && 
			bullet_x <= enemy_rightx &&
			bullet_y >= enemy_lefty &&
			bullet_y <= enemy_righty;
}

bool Enemy::CheckPlayerCollsion(const Player & player)
{
	//人物宽大概为200，高大概为300，图片宽和高都为1000
	POINT check_postion = { Enemy_pos.x +  Enemy_WIDTH/2 , Enemy_pos.y + Enemy_HEIGHT/2 };
	
	//敌人宽大概为100，高大概为200
	return	check_postion.x >= player.Player_pos.x + 70 && 
			check_postion.x <= player.Player_pos.x + 100 &&
			check_postion.y >= player.Player_pos.y + 50 && 
			check_postion.y <= player.Player_pos.y + 140;
}

void Enemy::Hurt(DWORD HurtTime,int damage)
{
	if (status == Status::Clear)
	{
		last = HurtTime + eps;
		status = Status::GetHurt;
		Enemy_Health -= damage;
	}
	if (Enemy_Health <= 0) {
		alive = false;
	}
}

bool Enemy::CheckAlive()
{
	return alive;
}

void Enemy::CheckHurt(DWORD time)
{
	if (time >= last && status == Status::GetHurt)
	{
		status = Status::Clear;
		HurtTime = 0;
	}
}

Solider::Solider()
	{
		Enemy_speed = 2;
		Enemy_WIDTH = 200;
		Enemy_HEIGHT = 200;
		SHADOW_WIDTH = 70;
		eps = 500;//受到伤害后的无敌帧

		MAX_HEALTH = 2;
		last = 0;//标记下一次受到伤害的时候
		Enemy_Health = 2;

		std::mt19937 myrand(clock());
		Enemy_move_left = new Animation(_T("assert/Enemy/soldies/Enemyleft%d.png"), 7, 45,200,200);
		Enemy_move_right = new Animation(_T("assert/Enemy/soldies/Enemyright%d.png"), 7, 45,200,200);

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

Solider::~Solider()
	{
		delete Enemy_move_left;
		delete Enemy_move_right;
	}

void Solider::Move(const Player& player)
	{
		move_right = false;
		move_left = true;
		move_right = player.Player_pos.x > Enemy_pos.x;
		move_left = player.Player_pos.x < Enemy_pos.x;

		move_down = false;
		move_up = false;
		move_down = player.Player_pos.y > Enemy_pos.y;
		move_up = player.Player_pos.y < Enemy_pos.y;

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

void Solider::Draw()
	{
		if (status == Status::GetHurt)
		{
			HurtTime++;
			if (((HurtTime - 1) / 5) % 2 == 0) return;
		}

		static bool facing_left = false;
		int div_r = move_right - move_left;

		facing_left = div_r < 0;

		if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
		else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

		putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
		rectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + Enemy_WIDTH - 30, Enemy_pos.y + Enemy_HEIGHT - 30);

		int health = (double)(Enemy_WIDTH - 60) * ((double)Enemy_Health / (double)MAX_HEALTH);
		setfillcolor(RED);
		fillrectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + 30 + health, Enemy_pos.y + Enemy_HEIGHT - 30);
	}

bool Solider::CheckBulletCollsion(const Bullet& bullet)
	{
		int bullet_x = bullet.Bullet_pos.x;
		int bullet_y = bullet.Bullet_pos.y;
		int bullet_r = bullet.radius;

		int enemy_leftx = Enemy_pos.x + 70;
		int enemy_lefty = Enemy_pos.y + 80;
		int enemy_rightx = Enemy_pos.x + 120;
		int enemy_righty = Enemy_pos.y + 160;

		return	bullet_x >= enemy_leftx &&
			bullet_x <= enemy_rightx &&
			bullet_y >= enemy_lefty &&
			bullet_y <= enemy_righty;
	}

bool Solider::CheckPlayerCollsion(const Player& player)
	{
		//人物宽大概为200，高大概为300，图片宽和高都为1000
		POINT check_postion = { Enemy_pos.x + Enemy_WIDTH / 2 , Enemy_pos.y + Enemy_HEIGHT / 2 };

		//敌人宽大概为100，高大概为200
		return	check_postion.x >= player.Player_pos.x + 70 &&
			check_postion.x <= player.Player_pos.x + 100 &&
			check_postion.y >= player.Player_pos.y + 50 &&
			check_postion.y <= player.Player_pos.y + 140;
	}

void Solider::Hurt(DWORD HurtTime, int damage)
	{
		if (status == Status::Clear)
		{
			last = HurtTime + eps;
			status = Status::GetHurt;
			Enemy_Health -= damage;
		}
		if (Enemy_Health <= 0) {
			alive = false;
		}
	}

bool Solider::CheckAlive()
	{
		return alive;
	}

void Solider::CheckHurt(DWORD time)
	{
		if (time >= last && status == Status::GetHurt)
		{
			status = Status::Clear;
			HurtTime = 0;
		}
	}

Airborne::Airborne()
	{
		Enemy_speed = 2;
		Enemy_WIDTH = 200;
		Enemy_HEIGHT = 200;
		SHADOW_WIDTH = 70;
		eps = 500;//受到伤害后的无敌帧

		MAX_HEALTH = 2;
		last = 0;//标记下一次受到伤害的时候
		Enemy_Health = 2;

		std::mt19937 myrand(clock());
		Enemy_move_left = new Animation(_T("assert/Enemy/Airborne/left%d.png"), 9, 45,200,200);
		Enemy_move_right = new Animation(_T("assert/Enemy/Airborne/right%d.png"),9, 45,200,200);

		Enemy_pos.x = myrand() % 1280;
		Enemy_pos.y = myrand() % 720;
	}

Airborne::~Airborne()
	{
		delete Enemy_move_left;
		delete Enemy_move_right;
		delete Enemy_Landing;
	}

void Airborne::Move(const Player& player)
	{
		if (landingstatus == Unlanding) return;

		move_right = false;
		move_left = true;
		move_right = player.Player_pos.x > Enemy_pos.x;
		move_left = player.Player_pos.x < Enemy_pos.x;

		move_down = false;
		move_up = false;
		move_down = player.Player_pos.y > Enemy_pos.y;
		move_up = player.Player_pos.y < Enemy_pos.y;

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

void Airborne::Draw()
	{
		if (landingstatus == Unlanding)
		{
			IMAGE* START = new IMAGE;
			loadimage(START,_T("assert/Enemy/Airborne/start1.png"),200,200);
			putimage_alpha(Enemy_pos.x, landingpos, START);
			landingpos += 20;
			landingpos = min(landingpos, Enemy_pos.y);
			if (landingpos == Enemy_pos.y) landingstatus = Landing;
			return;
		}

		if (status == Status::GetHurt)
		{
			HurtTime++;
			if (((HurtTime - 1) / 5) % 2 == 0) return;
		}

		static bool facing_left = false;
		int div_r = move_right - move_left;

		facing_left = div_r < 0;

		if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
		else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

		putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
		rectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + Enemy_WIDTH - 30, Enemy_pos.y + Enemy_HEIGHT - 30);

		int health = (double)(Enemy_WIDTH - 60) * ((double)Enemy_Health / (double)MAX_HEALTH);
		setfillcolor(RED);
		fillrectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + 30 + health, Enemy_pos.y + Enemy_HEIGHT - 30);
	}

bool Airborne::CheckBulletCollsion(const Bullet& bullet)
	{
		if (landingstatus == Unlanding) return false;

		int bullet_x = bullet.Bullet_pos.x;
		int bullet_y = bullet.Bullet_pos.y;
		int bullet_r = bullet.radius;

		int enemy_leftx = Enemy_pos.x + 70;
		int enemy_lefty = Enemy_pos.y + 80;
		int enemy_rightx = Enemy_pos.x + 120;
		int enemy_righty = Enemy_pos.y + 160;

		return	bullet_x >= enemy_leftx &&
			bullet_x <= enemy_rightx &&
			bullet_y >= enemy_lefty &&
			bullet_y <= enemy_righty;
	}

bool Airborne::CheckPlayerCollsion(const Player& player)
	{
		if (landingstatus == Unlanding) return false;

		//人物宽大概为200，高大概为300，图片宽和高都为1000
		POINT check_postion = { Enemy_pos.x + Enemy_WIDTH / 2 , Enemy_pos.y + Enemy_HEIGHT / 2 };

		//敌人宽大概为100，高大概为200
		return	check_postion.x >= player.Player_pos.x + 70 &&
			check_postion.x <= player.Player_pos.x + 100 &&
			check_postion.y >= player.Player_pos.y + 50 &&
			check_postion.y <= player.Player_pos.y + 140;
	}

void Airborne::Hurt(DWORD HurtTime, int damage)
	{
		if (status == Status::Clear)
		{
			last = HurtTime + eps;
			status = Status::GetHurt;
			Enemy_Health -= damage;
		}
		if (Enemy_Health <= 0) {
			alive = false;
		}
	}

bool Airborne::CheckAlive()
	{
		return alive;
	}

void Airborne::CheckHurt(DWORD time)
	{
		if (time >= last && status == Status::GetHurt)
		{
			status = Status::Clear;
			HurtTime = 0;
		}
	}

Hound::Hound()
{
	Enemy_speed = 4;
	Enemy_WIDTH = 200;
	Enemy_HEIGHT = 200;
	SHADOW_WIDTH = 70;
	eps = 500;//受到伤害后的无敌帧

	MAX_HEALTH = 1;
	last = 0;//标记下一次受到伤害的时候
	Enemy_Health = 1;

	std::mt19937 myrand(clock());
	Enemy_move_left = new Animation(_T("assert/Enemy/Hound/left%d.png"), 4, 90,200,200);
	Enemy_move_right = new Animation(_T("assert/Enemy/Hound/right%d.png"), 4, 90,200,200);

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

Hound::~Hound()
{
	delete Enemy_move_left;
	delete Enemy_move_right;
}

void Hound::Move(const Player& player)
{
	move_right = false;
	move_left = true;
	move_right = player.Player_pos.x > Enemy_pos.x;
	move_left = player.Player_pos.x < Enemy_pos.x;

	move_down = false;
	move_up = false;
	move_down = player.Player_pos.y > Enemy_pos.y;
	move_up = player.Player_pos.y < Enemy_pos.y;

	double dir_x = move_right - move_left;
	double dir_y = move_down - move_up;
	
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0) {
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		Enemy_pos.x +=  (int)((double)this->Enemy_speed * normalized_x);
		Enemy_pos.y +=  (int)((double)this->Enemy_speed * normalized_y);
	}
}

void Hound::Draw()
{
	if (status == Status::GetHurt)
	{
		HurtTime++;
		if (((HurtTime - 1) / 5) % 2 == 0) return;
	}

	static bool facing_left = false;
	int div_r = move_right - move_left;

	facing_left = div_r < 0;

	if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
	else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

	putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
	rectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + Enemy_WIDTH - 30, Enemy_pos.y + Enemy_HEIGHT - 30);

	int health = (double)(Enemy_WIDTH - 60) * ((double)Enemy_Health / (double)MAX_HEALTH);
	setfillcolor(RED);
	fillrectangle(Enemy_pos.x + 30, Enemy_pos.y + Enemy_HEIGHT - 40, Enemy_pos.x + 30 + health, Enemy_pos.y + Enemy_HEIGHT - 30);
}

bool Hound::CheckBulletCollsion(const Bullet& bullet)
{
	int bullet_x = bullet.Bullet_pos.x;
	int bullet_y = bullet.Bullet_pos.y;
	int bullet_r = bullet.radius;

	int enemy_leftx = Enemy_pos.x + 70;
	int enemy_lefty = Enemy_pos.y + 80;
	int enemy_rightx = Enemy_pos.x + 120;
	int enemy_righty = Enemy_pos.y + 160;

	return	bullet_x >= enemy_leftx &&
		bullet_x <= enemy_rightx &&
		bullet_y >= enemy_lefty &&
		bullet_y <= enemy_righty;
}

bool Hound::CheckPlayerCollsion(const Player& player)
{
	//人物宽大概为200，高大概为300，图片宽和高都为1000
	POINT check_postion = { Enemy_pos.x + Enemy_WIDTH / 2 , Enemy_pos.y + Enemy_HEIGHT / 2 };

	//敌人宽大概为100，高大概为200
	return	check_postion.x >= player.Player_pos.x + 70 &&
		check_postion.x <= player.Player_pos.x + 100 &&
		check_postion.y >= player.Player_pos.y + 50 &&
		check_postion.y <= player.Player_pos.y + 140;
}

void Hound::Hurt(DWORD HurtTime, int damage)
{
	if (status == Status::Clear)
	{
		last = HurtTime + eps;
		status = Status::GetHurt;
		Enemy_Health -= damage;
	}
	if (Enemy_Health <= 0) {
		alive = false;
	}
}

bool Hound::CheckAlive()
{
	return alive;
}

void Hound::CheckHurt(DWORD time)
{
	if (time >= last && status == Status::GetHurt)
	{
		status = Status::Clear;
		HurtTime = 0;
	}
}

Spine::Spine()
{
	Enemy_speed = 1 ;
	Enemy_WIDTH = 200;
	Enemy_HEIGHT = 200;
	SHADOW_WIDTH = 70;
	eps = 500;//受到伤害后的无敌帧

	MAX_HEALTH = 10;
	last = 0;//标记下一次受到伤害的时候
	Enemy_Health = 10;

	std::mt19937 myrand(clock());
	Enemy_move_left = new Animation(_T("assert/Enemy/Spine/left%d.png"), 11, 45,300,300);
	Enemy_move_right = new Animation(_T("assert/Enemy/Spine/right%d.png"), 11, 45,300,300);

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

Spine::~Spine()
{
	delete Enemy_move_left;
	delete Enemy_move_right;
}

void Spine::Move(const Player& player)
{
	move_right = false;
	move_left = true;
	move_right = player.Player_pos.x > Enemy_pos.x + Enemy_WIDTH / 2 + 20;
	move_left = player.Player_pos.x < Enemy_pos.x + Enemy_WIDTH / 2 + 20;

	move_down = false;
	move_up = false;
	move_down = player.Player_pos.y > Enemy_pos.y + Enemy_HEIGHT / 2 - 20;
	move_up = player.Player_pos.y < Enemy_pos.y + Enemy_HEIGHT / 2 - 20;

	int dir_x = move_right - move_left;
	int dir_y = move_down - move_up;

	Enemy_pos.x += dir_x;
	Enemy_pos.y += dir_y;

}

void Spine::Draw()
{
	if (status == Status::GetHurt)
	{
		HurtTime++;
		if (((HurtTime - 1) / 5) % 2 == 0) return;
	}

	static bool facing_left = false;
	int div_r = move_right - move_left;

	facing_left = div_r < 0;

	if (!facing_left) Enemy_move_left->Play(Enemy_pos.x, Enemy_pos.y, 45);
	else Enemy_move_right->Play(Enemy_pos.x, Enemy_pos.y, 45);

	putpixel(Enemy_pos.x + Enemy_WIDTH / 2, Enemy_pos.y + Enemy_HEIGHT / 2, BLUE);
	rectangle(Enemy_pos.x + 80, Enemy_pos.y + Enemy_HEIGHT + 50 , Enemy_pos.x + Enemy_WIDTH + 20, Enemy_pos.y + Enemy_HEIGHT +60);

	int health = (double)(Enemy_WIDTH - 60) * ((double)Enemy_Health / (double)MAX_HEALTH);
	setfillcolor(RED);
	fillrectangle(Enemy_pos.x + 80, Enemy_pos.y +Enemy_HEIGHT + 50, Enemy_pos.x + 80 + health, Enemy_pos.y + 60 + Enemy_HEIGHT );
}

bool Spine::CheckBulletCollsion(const Bullet& bullet)
{
	int bullet_x = bullet.Bullet_pos.x;
	int bullet_y = bullet.Bullet_pos.y;
	int bullet_r = bullet.radius;

	int enemy_leftx = Enemy_pos.x + 90;
	int enemy_lefty = Enemy_pos.y + 140;
	int enemy_rightx = Enemy_pos.x + 200;
	int enemy_righty = Enemy_pos.y + 240;


	return	bullet_x >= enemy_leftx &&
		bullet_x <= enemy_rightx &&
		bullet_y >= enemy_lefty &&
		bullet_y <= enemy_righty;
}

bool Spine::CheckPlayerCollsion(const Player& player)
{
	//人物宽大概为200，高大概为300，图片宽和高都为1000
	POINT check_postion = { Enemy_pos.x + Enemy_WIDTH / 2 +20, Enemy_pos.y + Enemy_HEIGHT / 2 +60};
	
	//putpixel(check_postion.x, check_postion.y, WHITE);
	//rectangle(player.Player_pos.x+60, player.Player_pos.y+80, player.Player_pos.x+140, player.Player_pos.y+180);
	
	//敌人宽大概为100，高大概为200
	return	check_postion.x >= player.Player_pos.x + 60 &&
		check_postion.x <= player.Player_pos.x  + 120 &&
		check_postion.y >= player.Player_pos.y + 80 &&
		check_postion.y <= player.Player_pos.y + 150;
}

void Spine::Hurt(DWORD HurtTime, int damage)
{
	if (status == Status::Clear)
	{
		last = HurtTime + eps;
		status = Status::GetHurt;
		Enemy_Health -= damage;
	}
	if (Enemy_Health <= 0) {
		alive = false;
	}
}

bool Spine::CheckAlive()
{
	return alive;
}

void Spine::CheckHurt(DWORD time)
{
	if (time >= last && status == Status::GetHurt)
	{
		status = Status::Clear;
		HurtTime = 0;
	}
}

void EnemySpawn()
{
	//能不能实现时间越久难度越高?
	static int num = 0;
	static int counter = 100;

	if ((++num) % counter) return;
	enum Enemy_Sort
	{
		SOLIDER = 0,
		AIRBORNE,
		HOUND,
		SPINE
	};

	std::mt19937 myrand(clock());

	Enemy_Sort EnemySort = Enemy_Sort(myrand() % 4);
	Enemynum++;

	int Houndcounter = 1050;
	int Spinecounter = 2050;

	switch (EnemySort)
	{
	case 0:
		Enemy_list.push_back(new Solider());
		break;
	case 1:
		Enemy_list.push_back(new Airborne());
		break;
	case 2:
		if (num >= Houndcounter)
		{
			Enemy_list.push_back(new Hound());
		}
		break;
	case 3:
		if (num >= Spinecounter)
		{
			Enemy_list.push_back(new Spine());
		}
		break;
	}
	//printf("%d %d\n", counter,Enemynum);
}