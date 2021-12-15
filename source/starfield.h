#define NUM_STARS 40

typedef struct
{
    int x;
    int y;
    int speed;
    unsigned short color;

} Star;

// Function Prototypes
void setStar(Star *star, int xSpeed, int ySpeed, int xStart, int yStart);
void MoveStar(Star *star);
void ClearScreen(void);
void InitStars(Star *stars);
void DrawStar(Star *star);
void EraseStar(Star *star);