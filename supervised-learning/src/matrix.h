#ifndef MATRIX_H
#define MATRIX_H
#include <stdbool.h>

typedef struct{
    int rows;
    int cols;
    float *p;
}Matrix;

Matrix mat_alloc(int rows,int cols);
Matrix mat_get(int rows,int cols,float *p);
Matrix mat_getrand(int rows,int cols,float min,float max);
Matrix mat_getfill(int rows,int cols,float f);
Matrix mat_getcopy(Matrix src);
void mat_set(Matrix m,float *p);
void mat_free (Matrix *m);
void mat_print(Matrix m);
bool isempty(Matrix m);
int mat_size(Matrix m);
float my_rand(float min,float max);
float sigmoidf(float x);

void mat_copy(Matrix dst,Matrix src);
void mat_dot(Matrix c,Matrix a,Matrix b);
void mat_sum(Matrix c,Matrix a,Matrix b);
void mat_minus(Matrix c,Matrix a,Matrix b);
void mat_mul(Matrix c,Matrix a,Matrix b);
void mat_sumcols(Matrix dst,Matrix src);
void mat_scale(Matrix m,float s);
void mat_bias(Matrix m,Matrix b);
void mat_sigmoid(Matrix dst,Matrix src);
void mat_sigmoid_deriv(Matrix dst,Matrix act);
void mat_tsp(Matrix m,Matrix m0);
void mat_sub(Matrix subm,Matrix m,int fromrow,int fromcol);
void mat_rand(Matrix m,float min,float max);
void mat_fill(Matrix m,float f);
void mat_row(Matrix m,Matrix m0,int row);

#endif
