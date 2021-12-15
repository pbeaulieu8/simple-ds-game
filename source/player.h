#define BULLETS_MAX 5

typedef struct
{
    int x;
    int y;
} Player;

typedef struct
{
    int x;
    int y;

    //true if being shot
    bool isActive;
} Bullet;

void playerInit(Player *ship);
void bulletsInit(Bullet *bullets);
void moveShip(Player *ship, int key);
void shootBullet(Player *ship, Bullet *bullets);
void moveBullets(Bullet *bullets);