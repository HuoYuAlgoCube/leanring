#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define ROW 10
#define COL 10
#define SEED 1240
#define EPSILON 0.1f
#define ALPHA 0.1f
#define GAMMA 0.9f

float rnd(float n);
void initQValues(float qValues[ROW][COL],float n);
void printResult(float qValues[ROW][COL]);
void select(float qValues[ROW][COL],int curRow,int curCol,
    int* nextRow,int* nextCol,int flag);
void reward(float qValues[ROW][COL],float maze[ROW][COL],int curRow,int curCol,int tRow,int tCol);

int main(int argc,char* argv[]){
    printf("Hello Q-Learning Search!\n\n");
    srand(SEED);
    float qValues[ROW][COL];
    initQValues(qValues,100);
    printResult(qValues);
    int row0,col0,row1,col1;
    row0 = 0;
    col0 = 0;
    int targetRow = 9;
    int targetCol = 9;
    float maze[ROW][COL] = {
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0}
    };
    for (int i=0;i<100;i++){
        row0 = 0;
        col0 = 0;
        while (row0 != targetRow || col0 != targetCol){
            reward(qValues,maze,row0,col0,targetRow,targetCol);
            select(qValues,row0,col0,&row1,&col1,1);
            int tempRow,tempCol;
            tempRow = row0;
            row0 = row1;
            row1 = tempRow;
            tempCol = col0;
            col0 = col1;
            col1 = tempCol;
        }
    }
    row0 = 0;
    col0 = 0;
    while (row0 != targetRow || col0 != targetCol){
        reward(qValues,maze,row0,col0,targetRow,targetCol);
        select(qValues,row0,col0,&row1,&col1,1);
        printf("%d %d\n",row1,col1);
        int tempRow,tempCol;
        tempRow = row0;
        row0 = row1;
        row1 = tempRow;
        tempCol = col0;
        col0 = col1;
        col1 = tempCol;
    }
    printf("\n");
    printf("maze:\n");
    printResult(maze);
    printf("q:\n");
    printResult(qValues);
    return 0;
}

float rnd(float n){
    return (float)rand()/RAND_MAX*n;
}
void initQValues(float qValues[ROW][COL],float n){
    for (int i=0;i<ROW;i++){
        for (int j=0;j<COL;j++){
            qValues[i][j] = rnd(n);
        }
    }
}
void printResult(float qValues[ROW][COL]){
    for (int i=0;i<ROW;i++){
        for (int j=0;j<COL;j++){
            printf("%10.3f ",qValues[i][j]);
        }
        printf("\n");
    }
}

void select(float qValues[ROW][COL],int curRow,int curCol,
    int* nextRow,int* nextCol,int flag){
    int dRows[4] = {-1, 1, 0, 0};
    int dCols[4] = { 0, 0,-1, 1};
    if (rnd(1.0f) < EPSILON && flag == 1){
        int row2,col2;
        do{
            int i = (int)rnd(3.9f);
            row2 = curRow + dRows[i];
            col2 = curCol + dCols[i];
        }while(row2 < 0 || col2 < 0 || row2 >= ROW || col2 >= COL);
        nextRow[0] = row2;
        nextCol[0] = col2;
    }
    else {
        int maxQ = -FLT_MAX;
        int row2,col2;
        int row,col;
        for (int i=0;i<4;i++){
            row2 = curRow + dRows[i];
            col2 = curCol + dCols[i];
            if (row2 < 0 || col2 < 0 || row2 >= ROW || col2 >= COL)continue;
            if (qValues[row2][col2] > maxQ){
                maxQ = qValues[row2][col2];
                row = row2;
                col = col2;
            }
        }
        nextRow[0] = row;
        nextCol[0] = col;
    }
}

void reward(float qValues[ROW][COL],float maze[ROW][COL],int curRow,int curCol,int tRow,int tCol){
    if (curCol == tCol && curRow == tRow)return;
    if (maze[curRow][curCol] == 1.f)return;
    int dRows[4] = {-1, 1, 0, 0};
    int dCols[4] = { 0, 0,-1, 1};
    float maxQ = -FLT_MAX;
    int r,c;
    for (int i=0;i<4;i++){
        r = curRow + dRows[i];
        c = curCol + dCols[i];
        if (r < 0 || c < 0 || r >= ROW || c >= COL)continue;
        if (qValues[r][c] > maxQ){
            maxQ = qValues[r][c];
        }
    }
    for (int i=0;i<4;i++){
        int row2,col2;
        row2 = curRow + dRows[i];
        col2 = curCol + dCols[i];
        if (row2 < 0 || col2 < 0 || row2 >= ROW || col2 >= COL)continue;
        int r;
        if (maze[row2][col2] == 1.f){
            r = -500;
        }
        else if (row2 == tRow && col2 == tCol){
            r = 100;
        }
        else {
            r = -0.1;
        }
        qValues[row2][col2] = qValues[row2][col2] + ALPHA * (r + GAMMA*maxQ - qValues[row2][col2]);
    }
}
