#include "cell.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void initCell(Cell* cells, int v, Settings* set) {
    Cell* cell;
    int i, j, k;
    for (i = 0; i < set->maxRow; i++) {
        for (j = 0; j < set->maxCol; j++) {
            cell = cells + i * set->maxCol + j;
            // 先将整个结构体置零，避免未初始化的垃圾值
            memset(cell, 0, sizeof(Cell));
            cell->vertex = v;
            for (k = ALIVE_VERTEX; k < cell->vertex; k++) {
                // 使用更安全的随机整数生成，范围 [0, rate[k]]
                int maxVal = (int)(set->rate[k]);
                cell->weight[k] = (int)((float)rand() / (RAND_MAX + 1.0f) * (maxVal + 1));
                // 保证不超出范围（防御）
                if (cell->weight[k] > maxVal) cell->weight[k] = maxVal;
            }
        }
    }
}

void update(Cell* cells, Cell* temp, Rule rule, Settings* set) {
    if (set->duration <= 0) return;
    if (set->neighbor > MAX_NEIGHBOR) return;

    Cell nb[MAX_NEIGHBOR];
    int i, j, k;
    int nbRow, nbCol;

    for (i = 0; i < set->maxRow; i++) {
        for (j = 0; j < set->maxCol; j++) {
            for (k = 0; k < set->neighbor; k++) {
                nbRow = i + set->dtRow[k];
                nbCol = j + set->dtCol[k];
                if (nbRow < 0 || nbRow >= set->maxRow || nbCol < 0 || nbCol >= set->maxCol) {
                    // 越界邻居全部置零，避免读取垃圾数据
                    memset(&nb[k], 0, sizeof(Cell));
                    continue;
                }
                nb[k] = cells[nbRow * set->maxCol + nbCol];
            }
            temp[i * set->maxCol + j] = rule(cells + i * set->maxCol + j, nb, set);
        }
    }
    memcpy(cells, temp, set->maxRow * set->maxCol * sizeof(Cell));
}

Cell conway(Cell* cur, Cell* nb, Settings* set) {
    Cell out = *cur;
    int alive = 0;
    for (int i = 0; i < set->neighbor; i++) {
        if (nb[i].weight[ALIVE_VERTEX] == ALIVE) alive++;
    }
    if (cur->weight[ALIVE_VERTEX] == ALIVE) {
        if (alive < 2 || alive > 3)
            out.weight[ALIVE_VERTEX] = DEAD;
    } else {
        if (alive == 3)
            out.weight[ALIVE_VERTEX] = ALIVE;
    }
    return out;
}
