#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEED 1507
#define ROW 10
#define COL 10

#define S ROW*COL
#define A 5

#define ALPHA 0.1
#define BETA 0.1
#define GAMMA 0.9

const int actions[A][2] = {
    { 0,-1},
    { 0, 1},
    {-1, 0},
    { 1, 0},
    { 0, 0},
};

double rnd(float n){
    return (double)rand()/RAND_MAX*n;
}

int rcToState(int row,int col){
    return row*COL+col;
}
void stateToRc(int state,int* row,int* col){
    *row = state / COL;
    *col = state % COL;
}

void softmax(double* prefs,double* probs,int n){
    double max = prefs[0];
    for (int i=1;i<n;i++){
        if (prefs[i] > max)max = prefs[i];
    }
    double sum = 0.0;
    for (int i=0;i<n;i++){
        probs[i] = exp(prefs[i] - max);
        sum += probs[i];
    }

    for (int i=0;i<n;i++){
        probs[i] /= sum;
    }
}

int sample_action(double *probs, int n) {
    double r = rnd(1.0);
    double cum = 0.0;
    for (int i = 0; i < n; i++) {
        cum += probs[i];
        if (r < cum) return i;
    }
    return n - 1;
}

void step_env(int s,int a,double* r,int* next_s,int* done){
    *done = 0;
    int row,col,nRow,nCol;
    stateToRc(s,&row,&col);
    nRow = row + actions[a][0];
    nCol = col + actions[a][1];
    if (nRow < 0){
        nRow = 0;
    }
    if (nRow >= ROW){
        nRow = ROW - 1;
    }
    if (nCol < 0){
        nCol = 0;
    }
    if (nCol >= COL){
        nCol = COL - 1;
    }
    *next_s = rcToState(nRow,nCol);
    if (nRow == ROW -1 && nCol == COL -1){
        *done = 1;
        *r = 10.0;
    }
    else if (nRow < 0 || nCol < 0 || nRow >= ROW || nCol >= COL){
        *r = -100.0;
    }
    else if (fabs(nRow - 9)+fabs(nCol - 9) < fabs(row - 9)+fabs(col - 9)){
        *r = -0.1;
    }
    else {
        *r = -0.5;
    }
}

int main(int agrc,char* argv[]){
    printf("Hello Actor-Critic!\n\n");
    srand(SEED);

    double h[S][A];
    double V[S];
    for (int i=0;i<S;i++){
        V[S] = 0.0;
        for (int j=0;j<A;j++){
            h[i][j] = 0.0;
        }
    }

    int ep;
    int done = 0;
    const int EPISODES = 1e5;
    const int MAX_STEP = ROW * COL;
    for (ep = 0;ep < EPISODES;ep++){
        int s = rcToState(0,0);
        int step = 0;
        while (step < MAX_STEP){
            double probs[A];
            softmax(h[s],probs,A);
            int a = sample_action(probs,A);

            int ns;
            double r;
            step_env(s,a,&r,&ns,&done);

            double nV = done == 1 ? 0.0 : V[ns];
            double delta = r + GAMMA * nV - V[s];
            V[s] += BETA * delta;

            for (int act = 0; act < A; act++) {
                if (act == a) {
                    h[s][act] += ALPHA * delta * (1.0 - probs[act]);
                } else {
                    h[s][act] -= ALPHA * delta * probs[act];
                }
            }

            s = ns;

            step ++;
            if (done == 1)break;
        }
    }

    for (int i=0;i<ROW;i++){
        for (int j=0;j<COL;j++){
            int s = rcToState(i,j);
            double probs[A];
            softmax(h[s],probs,A);
            int a = sample_action(probs,A);
            printf("(%+d %+d %+6.3f) ",actions[a][0],actions[a][1],V[s]);
        }
        printf("\n");
    }
    return 0;
}
