#ifndef BFS_H
#define BFS_H

extern int startRow;
extern int startCol;
extern int endRow;
extern int endCol;
extern int ROW;
extern int COL;
extern int WIDTH;
extern int HEIGHT;
extern int START_AREA;
extern int END_AREA;
extern float RATE;
extern Color color;

typedef struct Grid{
    int row,col;
    int isWall;
    struct Grid* neighbor[4];
}Grid;

int isWall(int row,int col);
void initMaze(Grid* maze);
void bfs(Grid* maze,Grid* path[]);
void drawMaze(Grid* maze);

#endif
