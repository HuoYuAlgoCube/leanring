#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"

void updateFrame(Player* player,float frameTime);
void drawAnim(Player player,Color color);
void setFrames(Rectangle* frames,int fw,int fh,int count,int row);
void fillf(float* ptr,int count,float f);
void fillr(Rectangle* ptr,int count,int x,int y,int w,int h);

void gravity(Player* player);
void fly(Player* player);
void checkAttack(Player* player,Player* enermy,float frameTime);
void checkBullet();

void initTco();

void initBullet();
void startBullet(Player player,float frameTime);
void drawBullets();
void updateBullets(float frameTime);
void checkBulletAttack(Player* player,float frameTime);
void freeBullet();

void ai(Player player,Player* enermy);

Player getTco();
void printTco(Player player);

#endif
