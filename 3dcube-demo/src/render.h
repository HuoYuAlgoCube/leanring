#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"

extern Color color;
extern int render_width;
extern int render_height;
extern float rate;
extern int point_size;

void point(Vector2 p,Color color);

void printVector2(Vector2 p2);

void printVector3(Vector3 p3);

Vector2 toScreen(Vector2 p);

Vector2 to2D(Vector3 p3);

Vector3 plus(Vector3 p1,Vector3 p2);

void model(Vector3 origin,Vector3 m[],int line[][2],int len,int edge);

#endif
