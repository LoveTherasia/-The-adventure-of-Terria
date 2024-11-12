#include"Constant.h"
#include"Enemy.h"
#include"Player.h"
#include"Animation.h"
#include"Bullet.h"

extern std::vector<Enemy*> Enemy_list;
std::vector<Bullet*> bullet_list;
extern int Enemynum;

const int BUTTON_WIDTH = 257;
const int BUTTON_HEIGHT = 86;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

bool is_game_started = false;
bool running = true;

class Button
{
public :
	Button(RECT rect,LPCTSTR path_img_idle,LPCTSTR path_img_hovered,LPCTSTR path_img_pushed)
	{
		region = rect;

		loadimage(&img_idle, path_img_idle);
		loadimage(&img_hovered, path_img_hovered);
		loadimage(&img_pushed, path_img_pushed);
	}
	~Button() = default;

	void Draw()
	{
		switch (status)
		{
		case Status::Idle:
			putimage(region.left, region.top, &img_idle);
			break;
		case Status::Hovered:
			putimage(region.left, region.top, &img_hovered);
			break;
		case Status::Pushed:
			putimage(region.left, region.top, &img_pushed);
			break;
		}
	}
	//有限状态自动机判断当前状态
	void ProcessEvent(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:
			if (CheckCursorHit(msg.x, msg.y))
				status = Status::Pushed;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed)
				OnClick();
			break;
		default:
			break;
		}
	}

protected:
	virtual void OnClick() = 0;
private:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Pushed
	};
	//检测鼠标点击
	bool CheckCursorHit(int x, int y)
	{
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}

private:
	RECT region;
	IMAGE img_idle;
	IMAGE img_hovered;
	IMAGE img_pushed;
	Status status = Status::Idle;
};

//开始按钮
class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect,LPCTSTR path_img_idle,LPCTSTR path_img_hovered,LPCTSTR path_img_pushed)
		: Button(rect,path_img_idle,path_img_hovered,path_img_pushed) {}
	~StartGameButton() = default;
protected:
	void OnClick()
	{
		is_game_started = true;

		mciSendString(_T("open assert/m_bat_dsdevr_combine.mp3"), NULL, 0, NULL);
		mciSendString(_T("play assert/m_bat_dsdevr_combine.mp3 repeat from 0"), NULL, 0, NULL);
		mciSendString(_T("stop assert/storyteller.mp3"), NULL, 0, NULL);
	}
};

//退出按钮
class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed){}
	~QuitGameButton() = default;
protected:
	void OnClick()
	{
		running = false;
	}
};




void UpdateBullet(const Player &player)
{
	const double Radial_speed = 0.0035;//初始化径向波动速度
	const double Tangent_speed = 0.0045;//初始化切向波动速度

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
	enum RandomThing
	{
		Add_Attack = 0,
		Add_Health ,
		Add_speed ,
	};

	mciSendString(_T("open assert/storyteller/mp3"), NULL, 0, NULL);
	initgraph(1280, 720,EX_SHOWCONSOLE);

	IMAGE img_menu,img_background;
	ExMessage msg;
	Player * player = new Player();

	RECT region_btn_start_game, region_btn_quit_game;

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 570;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
		_T("assert/start_game_idle.png"), _T("assert/start_game_hovered.png"), _T("assert/start_game_pushed.png"));
	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
		_T("assert/quit_game_idle.png"), _T("assert/quit_game_hovered.png"), _T("assert/quit_game_pushed.png"));
	loadimage(&img_menu, _T("assert/BeginImg.jpg"));
	loadimage(&img_background, _T("assert/750px-SN-S-1-A.png"),1280,720);


	for (int i = 0; i < 3; i++)
	{
		Bullet *bullet;
		bullet = new Bullet;
		bullet_list.push_back(bullet);
	}


	int EnemyHealth = 1;
	int clocknum = 0;
	int clockcounter = 1000;

	mciSendString(_T("play assert/storyteller.mp3 repeat from 0"), NULL, 0, NULL);
	while (running)
	{	
		//事件处理
		while (peekmessage(&msg))
		{
			if (is_game_started)
				player->Process(msg);
			else
			{
				btn_start_game.ProcessEvent(msg);
				btn_quit_game.ProcessEvent(msg);
			}
		}

		//局内游戏运行
		if (is_game_started)
		{
			if (player->Player_experience >= player->Player_lever_counter) {
				TCHAR MESSAGE[100];
				_stprintf_s(MESSAGE, _T("选择YES增加1点攻击力\n选择NO增加2点生命力"));

				int choice;
				choice = MessageBox(GetHWnd(), MESSAGE, _T("请选择你要升级的属性"), MB_YESNO);
				player->Player_experience -= player->Player_lever_counter;
				player->Player_lever_counter += 10;
				if (choice == 6) player->Player_attack++;
				else player->Player_Health += 2, player->MAX_HEALTH += 2;

				player->Move_Clear();
			}	// 升级

			DWORD first_begin_time = GetTickCount();

			player->Move();		//移动
			UpdateBullet(*player);	//控制玩家子弹

			for (int i = 0; i < Enemy_list.size(); i++) {
				Enemy_list[i]->Move(*player);
			} //敌人移动

			for (Enemy* enemy : Enemy_list) {
				if (enemy->CheckPlayerCollsion(*player)) {
					DWORD HurtTime = GetTickCount();
					player->Hurt(HurtTime);
				}
			}	//检测敌人与玩家是否发生碰撞

			DWORD time = GetTickCount();
			player->CheckHurt(time);	//判断玩家是否受伤

			if (!player->Alive())
			{
				TCHAR MESSAGE[100];
				_stprintf_s(MESSAGE, _T("你总共的得分为%d"), player->score);
				MessageBox(GetHWnd(), MESSAGE, _T("艾雅法拉战败"), MB_OK);
				running = false;
				break;
			} //死亡

			for (int i = 0; i < Enemy_list.size(); i++)
			{
				DWORD time = GetTickCount();
				Enemy_list[i]->CheckHurt(time);
			} //判断是否无敌帧时间

			//std::cout << player->MAX_HEALTH << " " << player->Player_Health << "\n";

			DWORD start_time = GetTickCount();	//计时1

			if (Enemynum <= 30)
				EnemySpawn();//生成敌人
			DWORD end_time = GetTickCount();	//计时2

			

			for (Enemy* enemy : Enemy_list) {
				for (Bullet* bullet : bullet_list) {
					if (enemy->CheckBulletCollsion(*bullet)) {
						DWORD HurtTime = GetTickCount();
						enemy->Hurt(HurtTime, player->Player_attack);
					}
				}
			}	//检测敌人是否受到伤害

			for (size_t i = 0; i < Enemy_list.size(); i++) {
				Enemy* enemy = Enemy_list[i];
				if (!enemy->CheckAlive()) {
					Enemynum--;
					player->score++;
					player->Player_experience++;
					std::swap(Enemy_list[i], Enemy_list.back());
					Enemy_list.pop_back();
					delete enemy;
				}
			}	//判断敌人是否存活


			DWORD first_end_time = GetTickCount();

			DWORD delta_time = first_end_time - first_begin_time;
			if (delta_time < 1000 / 60) {
				Sleep(1000 / 60 - delta_time);
			}	//控制帧率
		}
		cleardevice();

		BeginBatchDraw();
		if (is_game_started)//绘制部分
		{
			setlinecolor(WHITE);
			putimage(0, 0, &img_background);
			//FlushBatchDraw();

			TCHAR SCORE[200];
			_stprintf_s(SCORE, _T("艾雅法拉当前得分为:%d"), player->score);
			setbkmode(TRANSPARENT);
			settextcolor(WHITE);
			outtextxy(1100, 0, SCORE);
			//FlushBatchDraw();

			for (int i = 0; i < bullet_list.size(); i++)
				bullet_list[i]->Draw();
			//FlushBatchDraw();

			//rectangle(player->Player_pos.x+70 , player->Player_pos.y + 50, player->Player_pos.x + 130, player->Player_pos.y + 140);
			player->Draw(45);
			for (int i = 0; i < Enemy_list.size(); i++)
				Enemy_list[i]->Draw();
			FlushBatchDraw();
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game.Draw();
			btn_quit_game.Draw();
			FlushBatchDraw();
		}
		EndBatchDraw();
	}
	return 0;
}
