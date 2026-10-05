#include <stdio.h>
#include <stdlib.h>

const int batch_size = 3;
const int data_size = 3;
const int seed = 1234;

int getScore(int weight[data_size],int inp[batch_size][data_size],
    int pred[batch_size]);
int myRand();

int main(void){
    printf("Hello!\n\n");
    srand(seed);

    int weight[data_size];
    int inp[batch_size][data_size];
    int pred[batch_size];

    for (int i=0;i<batch_size;i++){
        for (int j=0;j<data_size;j++){
            printf("data(%d %d):",i,j);
            scanf("%d",&inp[i][j]);
        }
        printf("pred(%d):",i);
        scanf("%d",&pred[i]);
    }
    for (int i=0;i<data_size;i++){
        weight[i] = myRand();
    }

    printf("\ndata pred\n");
    for (int i=0;i<batch_size;i++){
        for (int j=0;j<data_size;j++){
            printf("%d ",inp[i][j]);
        }
        printf("%d\n",pred[i]);
    }

    printf("\nweight\n");
    for (int i=0;i<data_size;i++){
        printf("%d ",weight[i]);
    }
    printf("\n");

    int score = getScore(weight,inp,pred);
    printf("score:%d\n",score);
    return 0;
}


int myRand(){
    float r = (float)rand()/RAND_MAX*3;
    if (r < 1){
        return 0;
    }
    else if (r >= 1 && r < 2){
        return 1;
    }
    else {
        return 2;
    }
}

int getScore(int weight[data_size],int inp[batch_size][data_size],
    int pred[batch_size]){
    int score = 0;
    for (int i=0;i<batch_size;i++){
        int point = 0;
        for (int j=0;j<data_size;j++){
            if (weight[j] == 2){
                point++;
            }
            else if (weight[j] == inp[i][j]){
                point++;
            }
        }
        if (point == data_size && pred[i] == 1){
            score ++;
        }
        else if(point != data_size && pred[i] == 0){
            score ++;
        }
    }
    return score;
}
