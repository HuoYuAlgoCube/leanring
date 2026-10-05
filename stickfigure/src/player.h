#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define MAX_FRAMES 10
#define MAX_ANIMS 10
#define MAX_BULLETS 1000

typedef struct {
    int frameWidth,frameHeight;
    Rectangle frames[MAX_FRAMES];
    float duration[MAX_FRAMES];
    Rectangle translation[MAX_FRAMES];
    float attack[MAX_FRAMES];
    Rectangle hit[MAX_FRAMES];
    int cancelAnim;
    int defaultAnim;
    
    int frameCount;
    int curFrame;
    float timer;
    int loop;
}AnimFlip;

typedef enum{
    STATE_STAND,STATE_RUN,STATE_FLY,STATE_ATTACK,STATE_HIT,STATE_LASER
}States;

typedef struct{
    Texture2D image;
    AnimFlip anims[MAX_ANIMS];
    int animCount;
    int curAnim;
    Rectangle box;
    int direction;
    States state;

    float GRAVITY;
    float FLY;
    float MAX_GRAVITY;
    float MAX_FLY;
    int ground;

    int gravity;
    int fly;
    
    float attackTimer;
    float attackDuration;

    int health;

    int visible;
    int isPlayer;
}Player;

#endif
