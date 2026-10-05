#include <raylib.h>
#include <stdlib.h>
#include "bfs.h"
#include <string.h>
#include <stdio.h>

int startRow = 0;
int startCol = 0;
int endRow = 10;
int endCol = 10;
int ROW = 20;
int COL = 20;
int WIDTH = 400;
int HEIGHT = 400;
int START_AREA = 3;
int END_AREA = 3;
float RATE = 0.2f;
Color color = (Color){0,0,0,255};

typedef struct Grid{
    int row,col;
    int isWall;
    struct Grid* neighbor[4];
}Grid;

int isWall(int row,int col){
    if (row < 0 || row >= ROW || col < 0 || col >= COL){
        return -1;
    }
    if (startRow - START_AREA <= row && row < startRow + START_AREA && startCol - START_AREA <= col && col < startCol + START_AREA){
        return 0;
    }
    if (endRow - END_AREA <= row && row < endRow + END_AREA && endCol - END_AREA <= col && col < endCol + END_AREA){
        return 0;
    }
    if ((float)rand() / RAND_MAX < RATE){
        return 1;
    }
    else {
        return 0;
    }
    return 0;
}

void initMaze(Grid* maze){
    int stackRow[ROW*COL];
    int stackCol[ROW*COL];
    int visited[ROW*COL];
    int top = -1;

    for (int i=0;i<ROW*COL;i++){
        visited[i] = 0;
    }

    stackRow[++top] = startRow;
    stackCol[top] = startCol;

    int dRow[4] = {-1, 1, 0, 0};
    int dCol[4] = {0, 0, -1, 1};

    while(top >= 0){
        int row = stackRow[top];
        int col = stackCol[top--];
        if (visited[row*COL+col] == 1)continue;
        visited[row*COL+col] = 1;

        Grid* grid = maze+(row*COL+col);
        grid->row = row;
        grid->col = col;
        grid->isWall = isWall(row,col);

        for (int i=0;i<4;i++){
            int nbRow = row+dRow[i];
            int nbCol = col+dCol[i];
            if (nbRow < 0 || nbRow >= ROW || nbCol < 0 || nbCol >= COL || visited[nbRow*COL+nbCol] == 1){
                grid->neighbor[i] = NULL;
            }
            else {
                stackRow[++top] = nbRow;
                stackCol[top] = nbCol;
                grid->neighbor[i] = maze+(nbRow*COL+nbCol);
            }
        }
    }
}

void bfs(Grid* maze,Grid* path[]){
    int queneRow[ROW*COL];
    int queneCol[ROW*COL];
    int visited[ROW*COL];
    int parentRow[ROW*COL];
    int parentCol[ROW*COL];

    memset(visited,0,sizeof(visited));
    memset(parentRow,-1,sizeof(parentRow));
    memset(parentCol,-1,sizeof(parentCol));
    memset(queneRow,-1,sizeof(queneRow));
    memset(queneCol,-1,sizeof(queneCol));

    for (int i=0;i<ROW*COL;i++){
        path[i] = NULL;
    }

    parentRow[startRow] = startRow;
    parentCol[startCol] = startCol;

    int front = 0;
    int rear = 0;

    queneRow[rear] = startRow;
    queneCol[rear++] = startCol;
    visited[startRow*COL+startCol] = 1;

    int dRow[4] = {-1,1,0,0};
    int dCol[4] = {0,0,-1,1};

    while(front < rear) {
        int row = queneRow[front];
        int col = queneCol[front++];

        for(int i=0;i<4;i++){
            int nbRow = row+dRow[i];
            int nbCol = col+dCol[i];
            if (nbRow < 0 || nbRow >= ROW || nbCol < 0 || nbCol >= COL || visited[nbRow*COL+nbCol] == 1){
                continue;
            }
            visited[nbRow*COL+nbCol] = 1;
            if (maze[nbRow*COL+nbCol].isWall != 1){
                queneRow[rear] = nbRow;
                queneCol[rear++] = nbCol;
                parentRow[nbRow*COL+nbCol] = row;
                parentCol[nbRow*COL+nbCol] = col;
            }
            if (nbRow == endRow && nbCol == endCol) {
                break;
            }
        }
    }

    int count = 0;
    Grid* flipPath[ROW*COL];
    for (int i=0;i<ROW*COL;i++){
        flipPath[i] = NULL;
    }
    for (int grid = endRow*COL+endCol;grid != startRow*COL+startCol;){
        flipPath[count++] = maze+grid;
        grid = parentRow[grid]*COL+parentCol[grid];
    }
    flipPath[count] = maze+startRow*COL+startCol;
    for (int i=count;i>=0;i--){
        if (flipPath[i] == NULL){
            break;
        }
        path[count-i] = flipPath[i];
    }
}

void drawMaze(Grid* maze){
    int w = WIDTH/COL;
    int h = HEIGHT/ROW;
    for (int i=0;i<ROW*COL;i++){
        if (maze[i].isWall == 1){
            DrawRectangle(maze[i].col*w,maze[i].row*h,w,h,color);
        }
    }
}
