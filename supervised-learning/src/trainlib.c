#include "trainlib.h"
#include "matrix.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void forward(Matrix X, Matrix Y, Matrix *W, Matrix *b, int num_layers, Matrix *Z, Matrix *A) {
    assert(Y.rows == X.rows);
    assert(num_layers > 0);
    assert(W != NULL && b != NULL && Z != NULL && A != NULL);
    assert(!isempty(X) && !isempty(Y));
    assert(Y.cols == W[num_layers - 1].cols);

    Matrix prev = X;

    for (int i = 0; i < num_layers; i++) {
        assert(!isempty(Z[i]) && !isempty(A[i]));
        assert(Z[i].rows == prev.rows && Z[i].cols == W[i].cols);
        assert(A[i].rows == prev.rows && A[i].cols == W[i].cols);

        
        mat_dot(Z[i], prev, W[i]);
        mat_bias(Z[i], b[i]);

        
       if (i == num_layers - 1) {
           mat_copy(A[i], Z[i]);        // 输出层无激活
       } else {
           mat_leaky_relu(A[i], Z[i]);  // 隐藏层使用 Leaky ReLU
       }

        prev = A[i];
    }

    mat_copy(Y, prev);  
}

void backward(Matrix X, Matrix Y, Matrix *W, Matrix *b, int num_layers,
              Matrix *Z, Matrix *A, Matrix *dW, Matrix *db) {
    assert(Y.rows == X.rows);
    assert(num_layers > 0);
    assert(W != NULL && b != NULL && Z != NULL && A != NULL);
    assert(dW != NULL && db != NULL);
    assert(!isempty(X) && !isempty(Y));
    assert(Y.cols == W[num_layers - 1].cols);

    int batch_size = X.rows;
    Matrix delta_next;
    bool has_delta_next = false;

    for (int i = num_layers - 1; i >= 0; i--) {
        int cols_i = W[i].cols;
        Matrix delta;

        if (i == num_layers - 1) {
            
            
            delta = mat_alloc(batch_size, cols_i);
            mat_minus(delta, A[i], Y);   
        } else {
            
            Matrix deriv = mat_alloc(batch_size, cols_i);
            mat_leaky_relu_deriv(deriv, Z[i]);

            
            Matrix W_next_T = mat_alloc(W[i+1].cols, W[i+1].rows);
            mat_tsp(W_next_T, W[i+1]);

            Matrix temp = mat_alloc(batch_size, cols_i);
            mat_dot(temp, delta_next, W_next_T);

            delta = mat_alloc(batch_size, cols_i);
            mat_mul(delta, temp, deriv);

            mat_free(&temp);
            mat_free(&W_next_T);
            mat_free(&deriv);
        }

        
        Matrix prev_act = (i == 0) ? X : A[i - 1];
        Matrix prev_act_T = mat_alloc(prev_act.cols, prev_act.rows);
        mat_tsp(prev_act_T, prev_act);
        mat_dot(dW[i], prev_act_T, delta);
        mat_free(&prev_act_T);

        
        mat_sumcols(db[i], delta);

        
        if (has_delta_next) {
            mat_free(&delta_next);
        }
        delta_next = delta;
        has_delta_next = true;
    }

    
    if (has_delta_next) {
        mat_free(&delta_next);
    }

    
    float inv_batch = 1.0f / batch_size;
    for (int i = 0; i < num_layers; i++) {
        mat_scale(dW[i], inv_batch);
        mat_scale(db[i], inv_batch);
    }
}

void update(Matrix *W, Matrix *b, Matrix *dW, Matrix *db, int num_layers, float lr){
    assert(num_layers > 0);
    assert(W != NULL && b != NULL && dW != NULL && db != NULL);

    for (int i = 0; i < num_layers; i++) {
        
        assert(W[i].rows == dW[i].rows && W[i].cols == dW[i].cols);
        assert(b[i].rows == db[i].rows && b[i].cols == db[i].cols);

        
        int w_size = mat_size(W[i]);
        for (int j = 0; j < w_size; j++) {
            W[i].p[j] -= lr * dW[i].p[j];
        }

        
        int b_size = mat_size(b[i]);
        for (int j = 0; j < b_size; j++) {
            b[i].p[j] -= lr * db[i].p[j];
        }
    }
}

float mat_mse(Matrix pred, Matrix target) {
    assert(!isempty(pred) && !isempty(target));
    assert(pred.rows == target.rows && pred.cols == target.cols);
    int n = mat_size(pred);
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = pred.p[i] - target.p[i];
        sum += diff * diff;
    }
    return sum / n;
}

void matp_free(Matrix *p,int size){
    assert(p != NULL);
    assert(size > 0);

    for (int i=0;i<size;i++){
        mat_free(&p[i]);
    }
}

void matp_print(Matrix* p,int size){
    assert(p != NULL);
    assert(size > 0);

    for (int i=0;i<size;i++){
        mat_print(p[i]);
    }
}

void mat_leaky_relu(Matrix dst, Matrix src) {
    assert(!isempty(dst) && !isempty(src));
    assert(dst.rows == src.rows && dst.cols == src.cols);
    const float alpha = 0.01f;
    for (int i = 0; i < mat_size(src); i++) {
        float x = src.p[i];
        dst.p[i] = (x > 0.0f) ? x : alpha * x;
    }
}

void mat_leaky_relu_deriv(Matrix dst, Matrix src) {
    // 注意：这里 src 是激活前的输入 Z（不是激活后的 A）
    assert(!isempty(dst) && !isempty(src));
    assert(dst.rows == src.rows && dst.cols == src.cols);
    const float alpha = 0.01f;
    for (int i = 0; i < mat_size(src); i++) {
        float x = src.p[i];
        dst.p[i] = (x > 0.0f) ? 1.0f : alpha;
    }
}
