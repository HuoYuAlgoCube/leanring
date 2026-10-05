#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bfs.h"

#define W 800
#define H 800
#define FPS 60
#define BGCOLOR WHITE
#define TITLE "BFS"

const Color startColor = (Color){0,0,255,255};
const Color endColor = (Color){0,255,0,255};
const Color pathColor = (Color){255,0,0,255};

int main(void){
    setbuf(stdout,NULL);

    srand(time(NULL));

    WIDTH = W;
    HEIGHT = H;
    RATE = 0.3f;

    int playerRow = 10;
    int playerCol = 10;

    startRow = 18;
    startCol = 18;
    endRow = playerRow;
    endCol = playerCol;
    ROW = 20;
    COL = 20;

    Grid* maze = (Grid*)calloc(ROW*COL,sizeof(Grid));
    Grid* path[ROW*COL];
    int step = 0;

    initMaze(maze);
    bfs(maze,path);

    InitWindow(WIDTH,HEIGHT,TITLE);

    SetTargetFPS(FPS);

    float timer = 0;
    float moveTimer = 0;

    int w = WIDTH/COL;
    int h = HEIGHT/ROW;

    int prevRow = endRow;
    int prevCol = endCol;

    while (!WindowShouldClose()){

        timer += GetFrameTime();
        moveTimer += GetFrameTime();

        if (timer > 0.23){
            timer = 0;
            step++;
        }

        if (step >= ROW*COL){
            step = ROW*COL-1;
        }

        if (IsKeyDown(KEY_E)){
            initMaze(maze);
        }
        if (IsKeyPressed(KEY_Q)){
            break;
        }

        if (moveTimer > 0.2){
            moveTimer = 0;
            if (IsKeyDown(KEY_W) && maze[(endRow-1)*COL+endCol].isWall != 1){
                endRow -= 1;
            }
            if (IsKeyDown(KEY_S) && maze[(endRow+1)*COL+endCol].isWall != 1){
                endRow += 1;
            }
            if (IsKeyDown(KEY_A) && maze[(endRow)*COL+endCol-1].isWall != 1){
                endCol -= 1;
            }
            if (IsKeyDown(KEY_D) && maze[(endRow)*COL+endCol+1].isWall != 1){
                endCol += 1;
            }
        }

        if (endRow < 0){
            endRow = 0;
        }
        if (endCol < 0){
            endCol = 0;
        }
        if (endRow >= ROW){
            endRow = ROW-1;
        }
        if (endCol >= COL){
            endCol = COL-1;
        }

        if (endRow != prevRow || endCol != prevCol){
            prevRow = endRow;
            prevCol = endCol;
            step = 0;
            bfs(maze,path);
        }

        if (endRow == startRow && endCol == startCol){
            break;
        }

        if (path[step] != NULL){
            startRow = path[step]->row;
            startCol = path[step]->col;
        }

        BeginDrawing();
        ClearBackground(BGCOLOR);
        drawMaze(maze);
        DrawRectangle(startCol*w,startRow*h,w,h,startColor);
        DrawRectangle(endCol*w,endRow*h,w,h,endColor);
        EndDrawing();
    }
    CloseWindow();

    free(maze);
}
