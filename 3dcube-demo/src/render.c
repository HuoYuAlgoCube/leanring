#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define POINT_SIZE 5

Color color = (Color){255,0,0,255};
int render_width = 100;
int render_height = 100;
int point_size = 5;
float rate = 1;

void point(Vector2 p){
    DrawRectangle(round(p.x-point_size/2),round(p.y-point_size/2),point_size,point_size,color);
}

void printVector2(Vector2 p2){
    printf("x %f y %f\n\n",p2.x,p2.y);
}

void printVector3(Vector3 p3){
    printf("x %f y %f z %f\n\n",p3.x,p3.y,p3.z);
}

Vector2 toScreen(Vector2 p){
    p.x = (p.x+1)/2*render_width*rate;
    p.y = (1-(p.y+1)/2)*render_height*rate;
    return p;
}

Vector2 to2D(Vector3 p3){
    Vector2 p2;
    if (p3.z == 0){
        p3.z = 1e-3;
    }

    p2.x = (p3.x)/(p3.z);
    p2.y = (p3.y)/(p3.z);
    return p2;
}

Vector3 plus(Vector3 p1,Vector3 p2){
    return (Vector3){p1.x+p2.x,p1.y+p2.y,p1.z+p2.z};
}

void model(Vector3 origin,Vector3 m[],int line[][2],int len,int edge){
    int i;
    for (i=0;i<len;i++){
        Vector2 p = toScreen(to2D(plus(origin,m[i])));
        point(p);
    }
    for (i=0;i<edge;i++){
        Vector2 p1 = toScreen(to2D(plus(origin,m[line[i][0]])));
        Vector2 p2 = toScreen(to2D(plus(origin,m[line[i][1]])));
        DrawLine(p1.x,p1.y,p2.x,p2.y,color);
    }
}
