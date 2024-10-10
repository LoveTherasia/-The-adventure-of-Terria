#include "Animation.h"

Animation::Animation(LPCTSTR path, int num, int interval) {
	interval_ms = interval;

	TCHAR path_file[2000];
	for (size_t i = 0; i < num; i++) {
		_stprintf_s(path_file, path, i);
		IMAGE* frame = new IMAGE;
		loadimage(frame, path_file, 200, 200);
		frame_list.push_back(frame);
	}
}

Animation::~Animation() {
	for (size_t i = 0; i < frame_list.size(); i++)
		delete(frame_list[i]);
}

void Animation::Play(int x, int y, int delta) {
	timer += delta;
	if (timer > interval_ms) {
		idx_frame = (idx_frame + 1) % frame_list.size();
		timer = 0;
	}
	putimage_alpha(x, y, frame_list[idx_frame]);
}

