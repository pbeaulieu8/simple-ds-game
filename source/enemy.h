//number of barrels to be destroyed to spawn the boss
#define NUM_BARRELS 10;

typedef struct
{
    int x;
    int y;
} Enemy;

void barrelInit(Enemy *barrel);
void moveBarrel(Enemy *barrel);
