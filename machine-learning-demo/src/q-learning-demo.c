#include <stdio.h>
#include <stdlib.h>

#define Q_SIZE 100
#define SEED 2026
#define SUB_SIZE 4
#define EPSILON 0.3f
#define REWARD 1000
#define COUNT 1000
#define ALPHA 0.1
#define GAMMA 0.9

float rnd(float n);
void initQValues(int qValues[]);
void readData(int actions[],int subNodes[][SUB_SIZE]);
void printData(int qValues[],int actions[],int subNodes[][SUB_SIZE]);
int switchState(int state,int qValues[],int subNodes[][SUB_SIZE],int epsilon);
void reward(int state,int qValues[],int subNodes[][SUB_SIZE],int actions[]);
int maxQ(int state,int qValues[],int subNodes[][SUB_SIZE]);

int main(){
    printf("Hello Q Learning!\n\n");
    srand(SEED);

    int qValues[Q_SIZE];
    int actions[Q_SIZE];
    int subNodes[Q_SIZE][SUB_SIZE];

    initQValues(qValues);
    readData(actions,subNodes);

    printData(qValues,actions,subNodes);
    int state = 0;

    for (int i=0;i<COUNT;i++){
        state = 0;
        while (state != 99){
            reward(state,qValues,subNodes,actions);
            state = switchState(state,qValues,subNodes,1);
        }
    }

    printData(qValues,actions,subNodes);

    for (state=0;state!=99;state=switchState(state,qValues,subNodes,1)){
        int x = state / 10;
        int y = state % 10;
        printf("state:%d x:%d y:%d action:%d q:%d\n",state,x,y,actions[state],qValues[state]);
    }
    printf("state:%d action:%d\n",state,actions[state]);
}

float rnd(float n){
    return (float)rand()/RAND_MAX*n;
}
void initQValues(int qValues[]){
    for (int i=0;i<Q_SIZE;i++){
        qValues[i] = rnd(100.f);
    }
}
void readData(int actions[],int subNodes[][SUB_SIZE]){
    for (int i=0;i<Q_SIZE;i++){
        scanf("%d",&actions[i]);
        for (int j=0;j<SUB_SIZE;j++){
            scanf("%d",&subNodes[i][j]);
        }
    }
}

void printData(int qValues[],int actions[],int subNodes[][SUB_SIZE]){
    for (int i=0;i<Q_SIZE;i++){
        printf("state:%d q:%d action:%d\n",i,qValues[i],actions[i]);
        printf("sub nodes:");
        for (int j=0;j<SUB_SIZE;j++){
            printf("%d ",subNodes[i][j]);
        }
        printf("\n\n");
    }
}

int switchState(int state,int qValues[],int subNodes[][SUB_SIZE],int epsilon){
    int maxQ = INT_MIN;
    int nstate;
    if (rnd(1) < EPSILON && epsilon == 1){
        int r = (int)rnd(SUB_SIZE);
        nstate = subNodes[state][r];
    }
    else {
        for (int i=0;i<SUB_SIZE;i++){
            if (subNodes[state][i] == state)continue;
            if (qValues[subNodes[state][i]] > maxQ){
                maxQ = qValues[subNodes[state][i]];
                nstate = subNodes[state][i];
            }
        }
    }
    return nstate;
}

void reward(int state,int qValues[],int subNodes[][SUB_SIZE],int actions[]){
    int x = state / 10;
    int y = state % 10;
    int qVs[Q_SIZE];
    for (int i=0;i<Q_SIZE;i++){
        qVs[i] = qValues[i];
    }
    for (int i=0;i<SUB_SIZE;i++){
        int sub = subNodes[state][i];
        int sx = sub / 10;
        int sy = sub % 10;
        if (sx > x || sy > y){
            qVs[sub] = qValues[sub] + ALPHA*(REWARD - qValues[sub]);
        }
        else {
            qVs[sub] = qValues[sub] + ALPHA*(GAMMA*maxQ(state,qValues,subNodes)-
                qValues[sub]);
        }
    }
    for (int i=0;i<Q_SIZE;i++){
        qValues[i] = qVs[i];
    }
}

int maxQ(int state,int qValues[],int subNodes[][SUB_SIZE]){
    int maxQ = INT_MIN;
    for (int i=0;i<SUB_SIZE;i++){
        if (subNodes[state][i] == state)continue;
        if (qValues[subNodes[state][i]] > maxQ){
            maxQ = qValues[subNodes[state][i]];
        }
    }
    return maxQ;
}
