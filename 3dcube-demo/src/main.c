#include"raylib.h"
#include<stdio.h>
#include <math.h>
#include "render.h"

#define WIDTH 800
#define HEIGHT 800
#define FPS 60
#define BGCOLOR (Color){0,0,0,255}
#define TITLE "3D Cube"

int main(void){
    setbuf(stdout,NULL);
    InitWindow(WIDTH,HEIGHT,TITLE);
    
    SetTargetFPS(FPS);

    render_width = WIDTH;
    render_height = HEIGHT;
    rate = 1;
    color = (Color){255,0,0,255};

    Vector3 origin = (Vector3){0,0,0};

    float edge = 0.2;
    float x = 0.1,y = 0.1,z = 1;

    Vector3 m[] = {
        {x,y,z},
        {x+edge,y,z},
        {x,y+edge,z},
        {x,y,z+edge},
        {x+edge,y+edge,z},
        {x,y+edge,z+edge},
        {x+edge,y,z+edge},
        {x+edge,y+edge,z+edge},
    };
    
    while(!WindowShouldClose()){

        if (IsKeyDown(KEY_A)){
            origin.x -= 0.05;
        }
        if (IsKeyDown(KEY_D)){
            origin.x += 0.05;
        }

        if (IsKeyDown(KEY_W)){
            origin.z += 0.05;
        }
        if (IsKeyDown(KEY_S)){
            origin.z -= 0.05;
        }

        if (IsKeyDown(KEY_J)){
            origin.y += 0.05;
        }
        if (IsKeyDown(KEY_K)){
            origin.y -= 0.05;
        }
        
        BeginDrawing();
        ClearBackground(BGCOLOR);
        model(origin,m,l,8,0);
        EndDrawing();
    }
    
    CloseWindow();
}
