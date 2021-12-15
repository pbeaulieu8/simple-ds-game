#include "enemy.h"
#include <stdlib.h>

//initialize barrel position
void barrelInit(Enemy *barrel)
{
    //set x so barrel sprite is offscreen
    barrel->x = 256;
    //set y to random position
    //use mod (192-32) to ensure entire barrel sprite doesn't end up partially offscreen
    barrel->y = rand() % (192 - 32);
}

//update barrel position
void moveBarrel(Enemy *barrel)
{
    //move barrel left 2 pixels every frame
    barrel->x -= 2;

    //reset barrel position if offscreen
    if (barrel->x < -64)
    {
        barrelInit(barrel);
    }
}
