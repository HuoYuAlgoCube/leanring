#include <stdio.h>
#include "matrix.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define MAT_AT(m, row, col) ((m).p[(row)*(m).cols + (col)])

Matrix mat_alloc(int rows,int cols){
    assert(rows > 0 && cols > 0);
    Matrix m;
    m.cols = cols;
    m.rows = rows;
    m.p = (float *)calloc(rows*cols, sizeof(float));
    assert(m.p != NULL);
    return m;
}

Matrix mat_get(int rows,int cols,float *p){
    assert(p != NULL);
    Matrix m = mat_alloc(rows,cols);
    mat_set(m,p);
    return m;
}

Matrix mat_getrand(int rows,int cols,float min,float max){
    Matrix m = mat_alloc(rows,cols);
    mat_rand(m,min,max);
    return m;
}

Matrix mat_getfill(int rows,int cols,float f){
    Matrix m = mat_alloc(rows,cols);
    mat_fill(m,f);
    return m;
}

Matrix mat_getcopy(Matrix src) {
    assert(!isempty(src));
    Matrix dst = mat_alloc(src.rows, src.cols);
    mat_copy(dst, src);
    return dst;
}

void mat_set(Matrix m,float *p){
    assert(p != NULL);
    assert(! isempty(m));
    assert(m.p != p);
    memmove(m.p,p,mat_size(m)*sizeof(float));
}

void mat_free (Matrix *m){
    assert(m != NULL && m->p != NULL);
    m->rows = 0;
    m->cols = 0;
    free(m->p);
    m->p = NULL;
}

void mat_print(Matrix m){
    assert(!isempty(m));
    printf("\n");
    for (int row=0;row<m.rows;row++){
        for (int col=0;col<m.cols;col++){
            if (col == 0)printf("| ");
            printf("%f ",m.p[row*m.cols+col]);
            if (col == m.cols -1)printf("|\n");
        }
    }
    printf("\n");
}

bool isempty(Matrix m){
    return m.p == NULL || m.rows == 0 || m.cols == 0;
}

int mat_size(Matrix r){
    assert(!isempty(r));
    return r.rows*r.cols;
}

float my_rand(float min,float max){
    return (float)rand()/(float)RAND_MAX*(max-min)+min;
}

float sigmoidf(float x) {
    if (x >= 15.f) return 1.0;
    if (x <= -15.f) return 0.0;
    return 1.0 / (1.0 + exp(-x));
}

void mat_copy(Matrix dst,Matrix src){
    assert(!isempty(dst) && !isempty(src));
    assert(dst.rows == src.rows && dst.cols == src.cols);
    mat_set(dst,src.p);
}

void mat_dot(Matrix c,Matrix a,Matrix b){
    assert (!isempty(c) && !isempty(a) && !isempty(b));
    assert(a.cols == b.rows);
    assert(c.rows == a.rows && c.cols == b.cols);
    assert(c.p != a.p && c.p != b.p);
    for (int row=0;row<a.rows;row++){
        for (int col=0;col<b.cols;col++){
            int s = row*c.cols+col;
            c.p[s] = 0;
            for (int k=0;k<a.cols;k++){
                c.p[s] += a.p[row*a.cols+k]*b.p[k*b.cols+col];
            }
        }
    }
}

void mat_sum(Matrix c,Matrix a,Matrix b){
    assert(c.p != a.p && c.p != b.p);
    assert(a.rows == b.rows && a.cols == b.cols);
    assert(!isempty(c) && !isempty(a) && !isempty(b));
    assert(c.rows == b.rows && c.cols == b.cols);
    
    for (int i=0;i<mat_size(a);i++){
        c.p[i] = a.p[i] + b.p[i];
    }
}

void mat_minus(Matrix c,Matrix a,Matrix b){
    assert(c.p != a.p && c.p != b.p);
    assert(a.rows == b.rows && a.cols == b.cols);
    assert(!isempty(c) && !isempty(a) && !isempty(b));
    assert(c.rows == b.rows && c.cols == b.cols);
    
    for (int i=0;i<mat_size(a);i++){
        c.p[i] = a.p[i] - b.p[i];
    }
}

void mat_mul(Matrix c,Matrix a,Matrix b){
    assert(c.p != a.p && c.p != b.p);
    assert(!isempty(c) && !isempty(a) && !isempty(b));
    assert(a. rows == b.rows && a.cols == b.cols);
    assert(c.rows == a.rows && c.cols == a.cols);

    for (int i=0;i<mat_size(a);i++){
        c.p[i] = a.p[i] * b.p[i];
    }
}

void mat_sumcols(Matrix dst, Matrix src) {
    assert(dst.p != src.p);
    assert(!isempty(dst) && !isempty(src));
    assert(dst.rows == 1 && dst.cols == src.cols);
    mat_fill(dst, 0.0f);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dst.p[j] += src.p[i * src.cols + j];
        }
    }
}

void mat_scale(Matrix m,float s){
    assert(!isempty(m));
    for (int i=0;i<mat_size(m);i++){
        m.p[i] *= s;
    }
}

void mat_bias(Matrix m,Matrix b){
    assert(m.p != b.p);
    assert(!isempty(m) && !isempty(b));
    assert(b.rows == 1 && b.cols == m.cols);

    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            m.p[i * m.cols + j] += b.p[j];
        }
    }
}

void mat_sigmoid(Matrix dst, Matrix src) {
    assert(!isempty(dst) && !isempty(src));
    assert(dst.rows == src.rows && dst.cols == src.cols);
    for (int i = 0; i < mat_size(src); i++) {
        dst.p[i] = sigmoidf(src.p[i]);
    }
}

void mat_sigmoid_deriv(Matrix dst, Matrix act) {
    assert(!isempty(dst) && !isempty(act));
    assert(dst.rows == act.rows && dst.cols == act.cols);
    for (int i = 0; i < mat_size(act); i++) {
        float a = act.p[i];
        dst.p[i] = a * (1.0f - a);
    }
}

void mat_tsp(Matrix m,Matrix m0){
    assert(!isempty(m) && !isempty(m0));
    assert(m.rows == m0.cols && m.cols == m0.rows);
    assert(m.p != m0.p);

    for (int row=0;row < m0.rows;row++){
        for (int col=0;col < m0.cols;col++){
            m.p[col*m.cols+row] = m0.p[row*m0.cols+col];
        }
    }
}

void mat_sub(Matrix subm,Matrix m,int fromrow,int fromcol){
    assert(!isempty(subm) && !isempty(m));
    assert(subm.p != m.p);
    assert(fromrow >= 0 && fromcol >= 0);
    assert(fromrow+subm.rows <= m.rows && fromcol+subm.cols <= m.cols);
    for (int row = fromrow;row<fromrow+subm.rows;row++){
        for (int col = fromcol;col<fromcol+subm.cols;col++){
            int subrow = row-fromrow;
            int subcol = col-fromcol;
            subm.p[subrow*subm.cols+subcol] = m.p[row*m.cols+col];
        }
    }
}

void mat_rand(Matrix m,float min,float max){
    assert(!isempty(m));

    for (int i=0;i<mat_size(m);i++){
        m.p[i] = my_rand(min,max);
    }
}

void mat_fill(Matrix m,float f){
    assert(!isempty(m));

    for (int i=0;i<mat_size(m);i++){
        m.p[i] = f;
    }
}

void mat_row(Matrix m,Matrix m0,int row){
    assert(!isempty(m) && !isempty(m0));
    assert(m.p != m0.p);
    assert(m.cols == m0.cols);
    assert(m.rows == 1);
    assert(row < m0.rows);
    assert(row >= 0);

    for (int col=0;col<m0.cols;col++){
        m.p[col] = m0.p[row*m0.cols+col];
    }
}
