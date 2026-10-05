#ifndef TRAINLIB_H
#define TRAINLIB_H

#include "matrix.h"

void forward(Matrix X, Matrix Y, Matrix *W, Matrix *b, int num_layers, Matrix *Z, Matrix *A);
void backward(Matrix X, Matrix Y, Matrix *W, Matrix *b, int num_layers,Matrix *Z, Matrix *A, Matrix *dW, Matrix *db);
void update(Matrix *W, Matrix *b, Matrix *dW, Matrix *db, int num_layers, float lr);
float mat_mse(Matrix pred, Matrix target);
void matp_free(Matrix *p,int size);
void matp_print(Matrix *p,int size);
void mat_leaky_relu(Matrix dst, Matrix src);
void mat_leaky_relu_deriv(Matrix dst, Matrix src);

#endif
