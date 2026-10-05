#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct{
    float* p;
    int r;
    int c;
}Matrix;

float rnd(float lower,float upper);
Matrix* matAlloc(int r,int c);
void matFree(Matrix* m);
int matAssert(Matrix* m);
void matPrint(Matrix* m);
void matInit(Matrix* m,float lower,float upper);

int main(void){
    printf("Hello Matrix!\n\n");
    srand(1234);
    Matrix* m = matAlloc(2,2);
    matInit(m,-1.f,1.f);
    matPrint(m);
    matFree(m);
    return 0;
}

float rnd(float lower,float upper){
    return (float)rand()/(float)RAND_MAX*(upper-lower)+lower;
}

Matrix* matAlloc(int r,int c){
    assert(r > 0 && c > 0);
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    float* p = (float*)malloc(r*c*sizeof(float));
    assert(m != NULL && p != NULL);
    m->r = r;
    m->c = c;
    m->p = p;
    return m;
}
void matFree(Matrix* m){
    free(m->p);
    free(m);
    m = NULL;
}
int matAssert(Matrix* m){
    return (m != NULL && m->p != NULL && m->r > 0 && m->c > 0) ? 1 : 0;
}
void matPrint(Matrix* m){
    assert(matAssert(m) == 1);
    int r = m->r;
    int c = m->c;
    printf("\n");
    for (int i=0;i<r;i++){
        for (int j=0;j<c;j++){
            if (j == 0)printf("| ");
            printf("%3.6f ",m->p[i*c+j]);
            if (j == c-1)printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

void matInit(Matrix* m,float lower,float upper){
    assert(matAssert(m) == 1);
    int r = m->r,c = m->c;
    for (int i=0;i<r*c;i++){
        m->p[i] = rnd(lower,upper);
    }
}
