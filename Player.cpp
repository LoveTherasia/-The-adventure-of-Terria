#include "Player.h"
#include"Constant.h"

extern IMAGE Player_shadow;

Player::Player() {
	loadimage(&Player_shadow, _T("assert/shadow_player.png"), SHADOW_WIDTH);
	player_relax_left = new Animation(_T("assert/relaxleft%d.png"), 40, 45);
	player_relax_right = new Animation(_T("assert/relax%d.jpg"), 39, 45);
	player_move_left = new Animation(_T("assert/moveleft%d.png"), 10, 45);
	player_move_right = new Animation(_T("assert/MOVE%d.jpg"), 10, 45);
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