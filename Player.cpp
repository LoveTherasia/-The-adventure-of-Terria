#include "Player.h"
#include"Constant.h"

extern IMAGE Player_shadow;

Player::Player() {
	loadimage(&Player_shadow, _T("assert/Player/Eyjafalla/shadow_player.png"), SHADOW_WIDTH);
	loadimage(&player_head, _T("assert/head_Eyjafalla.png"),70,70);

	player_relax_left = new Animation(_T("assert/Player/Eyjafalla/relaxleft%d.png"), 40, 45,200,200);
	player_relax_right = new Animation(_T("assert/Player/Eyjafalla/relax%d.jpg"), 39, 45,200,200);
	player_move_left = new Animation(_T("assert/Player/Eyjafalla/moveleft%d.png"), 10, 45,200,200);
	player_move_right = new Animation(_T("assert/Player/Eyjafalla/MOVE%d.jpg"), 10, 45,200,200);
}

Player::~Player() {
	delete player_relax_left;
	delete player_relax_left;
	delete player_move_left;
	delete player_move_right;
}

void Player::Process(const ExMessage& msg) {
	switch (msg.message) {
		case WM_KEYDOWN:
			switch (msg.vkcode) {
				case 'W':
					move_up = true;
					break;
				case 'S':
					move_down = true;
					break;
				case 'A':
					move_left = true;
					break;
				case 'D':
					move_right = true;
					break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode) {
				case 'W':
					move_up = false;
					break;
				case 'S':
					move_down = false;
					break;
				case 'A':
					move_left = false;
					break;
				case 'D':
					move_right = false;
					break;
			}
			break;
	}
}

void Player::Move() {
	int dir_x = move_right - move_left;
	int dir_y = move_down - move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0) {
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		Player_pos.x += (int)(Player_speed * normalized_x);
		Player_pos.y += (int)(Player_speed * normalized_y);
	}
	if (Player_pos.x < -100) Player_pos.x = -100;
	if (Player_pos.y < -100) Player_pos.y = -100;
	if (Player_pos.x + PLAYER_WIDTH > 1280) Player_pos.x = 1280 - PLAYER_WIDTH;
	if (Player_pos.y + PLAYER_HEIGHT > 720) Player_pos.y = 720 - PLAYER_HEIGHT;
}

void Player::Draw(int delta) {

	rectangle(0, 0, 70, 70);
	setfillcolor(RGB(235, 19, 169));
	fillrectangle(0, 0, 70, 70);
	putimage_alpha(0, 0, &player_head);

	rectangle(90, 0, 1000, 30);
	rectangle(90, 40, 1000, 70);
	setfillcolor(RED);
	int health = ((double)(1000 - 90) * ((double)Player_Health / (double)MAX_HEALTH));
	fillrectangle(90, 0, 90 + health, 30);
	setfillcolor(BLUE);
	int experience = ((double)(1000 - 90) * (double)Player_experience / (double)Player_lever_counter);
	fillrectangle(90, 40, 90 + experience, 70);

	if (status == Status::GetHurt)
	{
		HurtTime++;
		if (((HurtTime - 1) / 5) % 2 == 0) return;
	}

	int Shadow_x = Player_pos.x + (PLAYER_WIDTH - SHADOW_WIDTH) / 2;
	int Shadow_y = Player_pos.y + PLAYER_HEIGHT - 50;
	putimage_alpha(Shadow_x, Shadow_y, &Player_shadow);


	static bool facing_left = false;
	int div_r = move_right - move_left;
	if (div_r < 0) facing_left = true;
	else if (div_r > 0) facing_left = false;
	is_move = ((abs(move_right-move_left)) || (abs(move_up-move_down)));

	if (is_move && facing_left) player_move_left->Play(Player_pos.x, Player_pos.y, delta);
	else if (is_move && !facing_left) player_move_right->Play(Player_pos.x, Player_pos.y, delta);
	else if (!is_move && facing_left) player_relax_left->Play(Player_pos.x, Player_pos.y, delta);
	else if (!is_move && !facing_left) player_relax_right->Play(Player_pos.x, Player_pos.y, delta);
}

void Player::Hurt(DWORD HurtTime)
{
	if (status == Status::Clear)
	{
		last = HurtTime + eps;
		status = Status::GetHurt;
		Player_Health--;
	}
	if (Player_Health <= 0) alive = false;
}

bool Player::Alive()
{
	return alive;
}

void Player::CheckHurt(DWORD time)
{
	if (time >= last && status == Status::GetHurt)
	{
		status = Status::Clear;
		HurtTime = 0;
	}
}

void Player::Move_Clear() {
	move_up = move_down = move_left = move_right = false;
}