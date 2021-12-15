#include <nds.h>
#include <stdlib.h>

#include "player.h"

//initialize player position
void playerInit(Player *ship)
{
    //set player on right side of screen
    ship->x = 0;
    //set player position in middle of the screen
    ship->y = 128 - 16; //128 is half of screen height, and 16 is half of ship sprite height
}

//move player up or down depending on the current key being held
void moveShip(Player *ship, int key)
{
    //move player up 2 pixels per frame if up key held
    if (key == KEY_UP)
    {
        ship->y -= 2;
    }
    //move player down 2 pixels per frame if down key held
    if (key == KEY_DOWN)
    {
        ship->y += 2;
    }
    //prevent player from moving above top of screen
    if (ship->y < 0)
    {
        ship->y = 0;
    }
    //prevent player from moving below bottom of screen
    else if (ship->y > 192 - 32)
    {
        ship->y = 192 - 32;
    }
}

//initialize bullet positions
void bulletsInit(Bullet *bullets)
{
    for (int i = 0; i < BULLETS_MAX; ++i)
    {
        //initialize bullet sprites offscreen so they won't be visible before shooting
        bullets[i].x = -8;
        bullets[i].y = 0;
        bullets[i].isActive = false;
    }
}

//shoot bullet infrom of player
void shootBullet(Player *ship, Bullet *bullets)
{
    for (int i = 0; i < BULLETS_MAX; ++i)
    {
        if (!bullets[i].isActive)
        {
            //set first inactive bullet to active
            bullets[i].isActive = true;
            //set x position to directly in front of player sprite
            bullets[i].x = ship->x + 32;
            //set y position to mid point of player sprite
            bullets[i].y = ship->y + 16 - 4;
            return;
        }
    }
}

//update all active bullet positions
void moveBullets(Bullet *bullets)
{
    for (int i = 0; i < BULLETS_MAX; ++i)
    {
        if (bullets[i].isActive)
        {
            //move bullet right 3 pixels per frame if active
            bullets[i].x += 3;
        }
        if (bullets[i].x >= 260)
        {
            //set bullet to inactive upon moving offscreen
            bullets[i].isActive = false;
        }
    }
}