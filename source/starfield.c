#include <nds.h>
#include <stdlib.h>
#include "starfield.h"

void MoveStar(Star *star)
{
	setStar(star, -star->speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
}

void setStar(Star *star, int xSpeed, int ySpeed, int xStart, int yStart)
{
	star->x += xSpeed;
	star->y += ySpeed;

	if (star->y < 0 || star->y > SCREEN_HEIGHT)
	{
		star->x = xStart;
		star->y = yStart;
		star->speed = rand() % 4 + 1;
	}
	else if (star->x < 0 || star->x > SCREEN_WIDTH)
	{
		star->x = xStart;
		star->y = yStart;
		star->speed = rand() % 4 + 1;
	}
}

void ClearScreen(void)
{
	int i;
	for (i = 0; i < 256 * 192; i++)
		BG_GFX[i] = RGB15(0, 0, 0) | BIT(15);
}

void InitStars(Star *stars)
{
	int i;
	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].color = rand(); // both color & rand() are a 16bit value
		stars[i].x = rand() % 256;
		stars[i].y = rand() % 192;
		stars[i].speed = rand() % 4 + 1;
	}
}
void DrawStar(Star *star)
{
	BG_GFX[star->x + star->y * SCREEN_WIDTH] = star->color;
}

void EraseStar(Star *star)
{
	BG_GFX[star->x + star->y * SCREEN_WIDTH] = RGB15(0, 0, 0) | BIT(15);
}

/*int main(void)
{
	int i;

	irqInit();
	irqEnable(IRQ_VBLANK);

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	//initialize the background
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	ClearScreen();
	InitStars();

	for (i = 0; i < 256 * 256; i++)
	{
		BG_GFX_SUB[i] = RGB15(0, 21, 11) | BIT(15);
	}

	while (1)
	{
		scanKeys();
		if (keysDown())
			lastKey = keysHeld();

		swiWaitForVBlank();

		for (i = 0; i < NUM_STARS; i++)
		{
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}
	}
	return 0;
}*/
