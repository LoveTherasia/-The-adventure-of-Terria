#include "Constant.h"

IMAGE Player_shadow;

void putimage_alpha(int x, int y, IMAGE* img)//ÓÃÓÚÈ¥³ýÍ¼Æ¬±³¾°
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
			   GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}