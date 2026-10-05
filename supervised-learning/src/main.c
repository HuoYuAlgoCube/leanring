#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "matrix.h"
#include "trainlib.h"

int main(){
    setbuf(stdout,NULL);
    int batch_size = 100;
    int inp_size = 2;
    int out_size = 1;
    int hidden_size = 20;
    int layers = 2;

    float x_data[] = {
        0,0,
        0,1,
        0,2,
        0,3,
        0,4,
        0,5,
        0,6,
        0,7,
        0,8,
        0,9,
        1,0,
        1,1,
        1,2,
        1,3,
        1,4,
        1,5,
        1,6,
        1,7,
        1,8,
        1,9,
        2,0,
        2,1,
        2,2,
        2,3,
        2,4,
        2,5,
        2,6,
        2,7,
        2,8,
        2,9,
        3,0,
        3,1,
        3,2,
        3,3,
        3,4,
        3,5,
        3,6,
        3,7,
        3,8,
        3,9,
        4,0,
        4,1,
        4,2,
        4,3,
        4,4,
        4,5,
        4,6,
        4,7,
        4,8,
        4,9,
        5,0,
        5,1,
        5,2,
        5,3,
        5,4,
        5,5,
        5,6,
        5,7,
        5,8,
        5,9,
        6,0,
        6,1,
        6,2,
        6,3,
        6,4,
        6,5,
        6,6,
        6,7,
        6,8,
        6,9,
        7,0,
        7,1,
        7,2,
        7,3,
        7,4,
        7,5,
        7,6,
        7,7,
        7,8,
        7,9,
        8,0,
        8,1,
        8,2,
        8,3,
        8,4,
        8,5,
        8,6,
        8,7,
        8,8,
        8,9,
        9,0,
        9,1,
        9,2,
        9,3,
        9,4,
        9,5,
        9,6,
        9,7,
        9,8,
        9,9,
    };
    float y_data[] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        0,
        2,
        4,
        6,
        8,
        10,
        12,
        14,
        16,
        18,
        0,
        3,
        6,
        9,
        12,
        15,
        18,
        21,
        24,
        27,
        0,
        4,
        8,
        12,
        16,
        20,
        24,
        28,
        32,
        36,
        0,
        5,
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        0,
        6,
        12,
        18,
        24,
        30,
        36,
        42,
        48,
        54,
        0,
        7,
        14,
        21,
        28,
        35,
        42,
        49,
        56,
        63,
        0,
        8,
        16,
        24,
        32,
        40,
        48,
        56,
        64,
        72,
        0,
        9,
        18,
        27,
        36,
        45,
        54,
        63,
        72,
        81,
    };
    
    Matrix X = mat_get(batch_size,inp_size,x_data);
    Matrix Y_pred = mat_alloc(batch_size,out_size);
    Matrix Y_true = mat_get(batch_size,out_size,y_data);

    Matrix* W = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* b = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* Z = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* A = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* dW = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* db = (Matrix*)calloc(layers,sizeof(Matrix));

    for (int i=0;i<layers;i++){
        float limit;
        if (i==0){
            limit = sqrtf(2.0f / X.cols);
        }else{
            limit = sqrtf(2.0f / W[i-1].cols);
        }
        if (i==0){
            W[i] = mat_getrand(inp_size,hidden_size,-limit,limit);
            b[i] = mat_getrand(1,hidden_size,-limit,limit);
        }
        else if(i == layers-1){
            W[i] = mat_getrand(hidden_size,out_size,-limit,limit);
            b[i] = mat_getrand(1,out_size,-limit,limit);
        }
        else{
            W[i] = mat_getrand(hidden_size,hidden_size,-limit,limit);
            b[i] = mat_getrand(1,hidden_size,-limit,limit);
        }
    }


    for (int i=0;i<layers;i++){
        dW[i] = mat_alloc(W[i].rows,W[i].cols);
        db[i] = mat_alloc(1,W[i].cols);
        Z[i] = mat_alloc(batch_size,W[i].cols);
        A[i] = mat_alloc(batch_size,W[i].cols);
    }
    
    int count = 10000000;
    float rate = 1e-4;
    int out = 1000;
    for (int i=0;i<count;i++){
        forward(X,Y_pred,W,b,layers,Z,A);
        backward(X,Y_true,W,b,layers,Z,A,dW,db);
        update(W,b,dW,db,layers,rate);
        if(i%(count/out)==0){
            printf("count:%d cost:%f\n",i,mat_mse(Y_pred,Y_true));
        }
    }

    printf("cost:%f\n",mat_mse(Y_pred,Y_true));
    printf("Y_pred:\n");
    mat_print(Y_pred);
    printf("W:\n");
    matp_print(W,layers);
    printf("b:\n");
    matp_print(b,layers);

    //--------------------

    int batch_size2 = 1;
    float x_data2[] = {
        5,6,
    };
    Matrix X2 = mat_get(batch_size2,inp_size,x_data2);
    Matrix Y_pred2 = mat_alloc(batch_size2,out_size);
    Matrix* Z2 = (Matrix*)calloc(layers,sizeof(Matrix));
    Matrix* A2 = (Matrix*)calloc(layers,sizeof(Matrix));
    for (int i=0;i<layers;i++){
        Z2[i] = mat_alloc(batch_size2,W[i].cols);
        A2[i] = mat_alloc(batch_size2,W[i].cols);
    }

    forward(X2,Y_pred2,W,b,layers,Z2,A2);
    printf("Y_pred2:\n");
    mat_print(Y_pred2);
    

    mat_free(&X);
    mat_free(&Y_pred);
    mat_free(&Y_true);
    mat_free(&X2);
    mat_free(&Y_pred2);

    matp_free(W,layers);
    matp_free(b,layers);
    matp_free(Z,layers);
    matp_free(A,layers);
    matp_free(dW,layers);
    matp_free(db,layers);
    matp_free(Z2,layers);
    matp_free(A2,layers);
    free(W);free(b);free(dW);free(db);free(Z);free(A);free(Z2);free(A2);
}
