#include"Constant.h"
#include"Enemy.h"
#include"Player.h"
#include"Animation.h"
#include"Bullet.h"

extern std::vector<Enemy*> Enemy_list;
std::vector<Bullet*> bullet_list;
extern int Enemynum;

void UpdateBullet(const Player &player)
{
	const double Radial_speed = 0.0045;//初始化径向波动速度
	const double Tangent_speed = 0.0055;//初始化切向波动速度

	double radian_interval = 2 * 3.14159 / bullet_list.size();
	POINT player_postion = player.GetPostion();
	double radius = 100 + 25 * sin(GetTickCount() * Radial_speed);

	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		double radian = GetTickCount() * Tangent_speed + radian_interval * i;
		bullet_list[i]->Bullet_pos.x = player_postion.x + 100 + (int)(radius * sin(radian));
		bullet_list[i]->Bullet_pos.y = player_postion.y + 120 + (int)(radius * cos(radian));

	}
}

signed main()
{
	initgraph(1280, 720,EX_SHOWCONSOLE);

	mciSendString(_T("open assert/m_bat_dsdevr_combine.mp3"), NULL, 0, NULL);
	mciSendString(_T("play assert/m_bat_dsdevr_combine.mp3 repeat from 0"), NULL, 0, NULL);
	bool running = true;

	IMAGE img_background;
	loadimage(&img_background, _T("assert/750px-SN-S-1-A.png"),1280,720);
	ExMessage msg;
	Player * player = new Player();
	for (int i = 0; i < 3; i++)
	{
		Bullet *bullet;
		bullet = new Bullet;
		bullet_list.push_back(bullet);
	}

	BeginBatchDraw();


	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			player->Process(msg);
		}
		if(Enemynum <=30 )
			EnemySpawn();

		DWORD end_time = GetTickCount();

		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / 60)
		{
			Sleep(1000 / 60 - delta_time);
		}
		player->Move();
		UpdateBullet(*player);

		for (int i = 0; i < Enemy_list.size(); i++)
			Enemy_list[i]->Move(*player);

		for (Enemy* enemy : Enemy_list)//检测敌人与玩家是否发生碰撞
		{
			if (enemy->CheckPlayerCollsion(*player))
			{
				TCHAR MESSAGE[100];
				_stprintf_s(MESSAGE,_T("你总共的得分为%d"), player->score);
				MessageBox(GetHWnd(),MESSAGE, _T("艾雅法拉战败"), MB_OK);
				running = false;
				break;
			}
		}

		for (Enemy* enemy : Enemy_list)
		{
			for (Bullet* bullet : bullet_list)
			{
				if (enemy->CheckBulletCollsion(*bullet))
				{
					enemy->Hurt();
					Enemynum--;
				}
			}
		}

		for (size_t i = 0; i < Enemy_list.size(); i++)
		{
			Enemy* enemy = Enemy_list[i];
			if (!enemy->CheckAlive())
			{
				player->score++;
				std::swap(Enemy_list[i], Enemy_list.back());
				Enemy_list.pop_back();
				delete enemy;
			}
		}

		cleardevice();
		setlinecolor(WHITE);
		putimage(0, 0, &img_background);
		
		TCHAR SCORE[200];
		_stprintf_s(SCORE,_T("艾雅法拉当前得分为:%d"), player->score);
		setbkmode(TRANSPARENT);
		outtextxy(0, 0, SCORE);

		for (int i = 0; i < bullet_list.size(); i++)
			bullet_list[i]->Draw();
		rectangle(player->Player_pos.x+70 , player->Player_pos.y + 50, player->Player_pos.x + 130, player->Player_pos.y + 140);
		player->Draw(45);
		for (int i = 0; i < Enemy_list.size(); i++)
			Enemy_list[i]->Draw(45);
		FlushBatchDraw();
	}
	EndBatchDraw();
	return 0;
}