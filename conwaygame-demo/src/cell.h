#ifndef CELL_H
#define CELL_H

#define MAX_VERTEX 16
#define MAX_NEIGHBOR 48
#define ALIVE_VERTEX 0
#define ALIVE 1
#define DEAD 0

typedef struct {
    int weight[MAX_VERTEX];
    int vertex;
} Cell;

typedef struct {
    int dtRow[MAX_NEIGHBOR];
    int dtCol[MAX_NEIGHBOR];
    int maxRow;
    int maxCol;
    int neighbor;
    float rate[MAX_VERTEX];
    float duration;
} Settings;

typedef Cell (*Rule)(Cell* cur, Cell* nb, Settings* set);

Cell conway(Cell* cur, Cell* nb, Settings* set);
void initCell(Cell* cells, int v, Settings* set);
void update(Cell* cells, Cell* temp, Rule rule, Settings* set);

#endif
