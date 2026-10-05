#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW 10
#define COL 10
#define SEED 1240
#define EPSILON 0.2f
#define REWARD 100.f
#define ALPHA 0.2f
#define GAMMA 0.8f

float rnd(float n);
void initQValues(float qValues[ROW][COL],float n);
void printQValues(float qValues[ROW][COL]);
void select(float qValues[ROW][COL],int curRow,int curCol,
    int* nextRow,int* nextCol,int flag);
int maxQValue(float qValues[ROW][COL],int row,int col);
void reward(float qValues[ROW][COL],int curRow,int curCol,int tRow,int tCol);

int main(int argc,char* argv[]){
    printf("Hello Q-Learning Search!\n\n");
    srand(SEED);
    float qValues[ROW][COL];
    initQValues(qValues,10);
    printQValues(qValues);
    int row0,col0,row1,col1;
    row0 = 0;
    col0 = 0;
    int targetRow = 9;
    int targetCol = 9;
    for (int i=0;i<20;i++){
        row0 = 0;
        col0 = 0;
        while (row0 != targetRow || col0 != targetCol){
            reward(qValues,row0,col0,targetRow,targetCol);
            select(qValues,row0,col0,&row1,&col1,1);
            // printf("%d %d\n",row1,col1);
            int tempRow,tempCol;
            tempRow = row0;
            row0 = row1;
            row1 = tempRow;
            tempCol = col0;
            col0 = col1;
            col1 = tempCol;
        }
    }
    printf("\n");
    printQValues(qValues);
    row0 = 0;
    col0 = 0;
    while (row0 != targetRow || col0 != targetCol){
        reward(qValues,row0,col0,targetRow,targetCol);
        select(qValues,row0,col0,&row1,&col1,0);
        printf("%d %d\n",row1,col1);
        int tempRow,tempCol;
        tempRow = row0;
        row0 = row1;
        row1 = tempRow;
        tempCol = col0;
        col0 = col1;
        col1 = tempCol;
    }
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
void printQValues(float qValues[ROW][COL]){
    for (int i=0;i<ROW;i++){
        for (int j=0;j<COL;j++){
            printf("%10.3f ",qValues[i][j]);
        }
        printf("\n");
    }
}

int maxQValue(float qValues[ROW][COL],int row,int col){
    int maxQ = 0.001f;
    int row2,col2;
    int dRows[4] = {-1, 1, 0, 0};
    int dCols[4] = { 0, 0,-1, 1};
    for (int i=0;i<4;i++){
        row2 = row + dRows[i];
        col2 = col + dCols[i];
        if (row2 < 0 || col2 < 0 || row2 >= ROW || col2 >= COL)continue;
        if (qValues[row2][col2] > maxQ){
            maxQ = qValues[row2][col2];
        }
    }
    return maxQ;
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
        int maxQ = 0.001f;
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

void reward(float qValues[ROW][COL],int curRow,int curCol,int tRow,int tCol){
    int dRows[4] = {-1, 1, 0, 0};
    int dCols[4] = { 0, 0,-1, 1};
    float maxQ = maxQValue(qValues,curRow,curCol);
    for (int i=0;i<4;i++){
        int row2,col2;
        row2 = curRow + dRows[i];
        col2 = curCol + dCols[i];
        if (row2 < 0 || col2 < 0 || row2 >= ROW || col2 >= COL)continue;
        int r;
        if (fabs(row2 - tRow)+fabs(col2 - tCol) <
            fabs(curRow - tRow)+fabs(curCol - tCol)){
            r = REWARD;
        }
        else {
            r = 0;
        }
        qValues[row2][col2] = qValues[row2][col2] + ALPHA * (r + maxQ - qValues[row2][col2]);
    }
}
