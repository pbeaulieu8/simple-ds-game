#include <nds.h>
#include <stdio.h>

//import sprite data
#include "ship.h"
#include "barrel.h"
#include "bullet.h"

//import game object data
#include "player.h"
#include "enemy.h"

//import background
#include "starfield.h"

//initialize star array
Star stars[NUM_STARS];

//define game objects
Player ship;
Enemy barrel;
Bullet bullets[BULLETS_MAX];
int barrels_remaining = NUM_BARRELS;

//called when player destroys all barrels
void victory()
{
	bool retry = false;
	//clear background
	oamClear(&oamMain, 0, 0);
	oamUpdate(&oamMain);

	//clear stars
	ClearScreen();

	//clear console
	consoleClear();
	while (!retry)
	{
		//print message to bottom screen
		iprintf("\x1b[1;1HCongratulations! You win!");
		iprintf("\x1b[2;1HPress A to restart");
		scanKeys();

		//restart game when player presses A
		if (keysDown() == KEY_A)
		{
			consoleClear();

			//reset positions of all game objects
			barrelInit(&barrel);
			bulletsInit(bullets);
			playerInit(&ship);

			return;
		}
	}
}

//called when player is hit by a barrel
void gameOver()
{
	bool retry = false;
	//clear background
	oamClear(&oamMain, 0, 0);
	oamUpdate(&oamMain);

	//clear stars
	ClearScreen();

	//clear console
	consoleClear();
	while (!retry)
	{
		//print message to bottom screen
		iprintf("\x1b[1;1HGame Over!");
		iprintf("\x1b[2;1HPress A to restart");
		scanKeys();
		if (keysDown() == KEY_A)
		{
			consoleClear();

			//reset positions of all game objects
			barrelInit(&barrel);
			bulletsInit(bullets);
			playerInit(&ship);

			return;
		}
	}
}

//check if one of the player's bullets hits the barrel
void checkBulletBarrelCollision()
{
	//iterate through all bullets
	for (int i = 0; i < BULLETS_MAX; ++i)
	{
		//check for collision with active bullets
		if (bullets[i].isActive &&
			bullets[i].x + 8 > barrel.x &&
			bullets[i].x < barrel.x + 32 &&
			bullets[i].y + 8 > barrel.y &&
			bullets[i].y < barrel.y + 32)
		{
			//remove bullet sprite (by moving it offscreen)
			bullets[i].x = -8;
			//set bullet to inactive
			bullets[i].isActive = false;

			//reset barrel position
			barrelInit(&barrel);

			//decrement remaining barrel count
			if (barrels_remaining > 1)
				--barrels_remaining;
			//go to victory screen when player destroys the last barrel
			else
				victory();
		}
	}
}

//check if collision between the barrel and the ship
void checkShipBarrelCollision()
{
	//check for collision
	if ((barrel.y + 32) > ship.y &&
		barrel.y < (ship.y + 32) &&
		barrel.x < (ship.x + 32) &&
		(barrel.x + 32) > ship.x)
	{
		//if collision detected, game over
		gameOver();
	}
}

int main()
{
	//set starting positions for game objects
	barrelInit(&barrel);
	playerInit(&ship);
	bulletsInit(bullets);

	//set video mode to mode 5
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	//map primary vram banks; bank A is set to main screen background
	vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_LCD, VRAM_C_LCD, VRAM_D_LCD);
	//map vram bank E to main screen sprite graphics
	vramSetBankE(VRAM_E_MAIN_SPRITE);

	//initialize 2d sprite engine
	oamInit(&oamMain, SpriteMapping_Bmp_1D_256, false);
	//initialize background layer 3 on main screen; will be used to draw star backdrop
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	//initialize console
	consoleDemoInit();

	//allocate memory for ship sprote
	u16 *ship_gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
	//copy ship sprite into allocated memory
	dmaCopy(shipTiles, ship_gfx, shipTilesLen);
	//copy ship palette into palette slot 0
	dmaCopy(shipPal, SPRITE_PALETTE, shipPalLen);

	//allocate memory for barrel sprite
	u16 *barrel_gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_16Color);
	//copy barrel sprite into allocated memory
	dmaCopy(barrelTiles, barrel_gfx, barrelTilesLen);
	//copy barrel palette into palette slot 1
	dmaCopy(barrelPal, SPRITE_PALETTE + 16, barrelPalLen);

	//allocate memory for bullet sprite
	u16 *bullet_gfx = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_16Color);
	//copy bullet sprite into allocated memory
	dmaCopy(bulletTiles, bullet_gfx, bulletTilesLen);
	//copy bullet palette into palette slot 2
	dmaCopy(bulletPal, SPRITE_PALETTE + 32, bulletPalLen);

	//initialize stars
	InitStars(stars);

	while (1)
	{
		//clear console; used to reset console and print updated barrels_remaining value
		consoleClear();
		//print number of barrels remaining
		iprintf("\x1b[1;1HEnemies remaining: %d", barrels_remaining);

		//check current keypad state
		scanKeys();
		//move ship if player is holding up or down
		if (keysHeld() == KEY_UP || keysHeld() == KEY_DOWN)
		{
			moveShip(&ship, keysHeld());
		}
		//shoot if player is pressing A
		if (keysDown() == KEY_A)
		{
			shootBullet(&ship, bullets);
		}

		//update bullet position
		moveBullets(bullets);
		//check of any bullets collide with the barrel
		checkBulletBarrelCollision();
		//update barrel position
		moveBarrel(&barrel);
		//check if ship collides with barrel
		checkShipBarrelCollision();

		//display ship sprite on main screen
		oamSet(&oamMain, 0, barrel.x, barrel.y, 0, 1, SpriteSize_32x32, SpriteColorFormat_16Color, barrel_gfx, 0, false, false, false, false, false);
		//display barrel sprite on main screen
		oamSet(&oamMain, 1, ship.x, ship.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_16Color, ship_gfx, 0, false, false, false, false, false);

		//display all active bullet sprites on main screen
		for (int i = 0; i < BULLETS_MAX; i++)
		{
			oamSet(&oamMain, i + 2, bullets[i].x, bullets[i].y, 0, 2, SpriteSize_8x8, SpriteColorFormat_16Color, bullet_gfx, 0, false, false, false, false, false);
		}

		//update star positions
		for (int i = 0; i < NUM_STARS; i++)
		{
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}

		//wait for entire frame to draw
		swiWaitForVBlank();

		//update sprite engine; i.e. redraw sprites with update positions
		oamUpdate(&oamMain);
	}

	return 0;
}
