#include "params.h"

struct cell {
    int mined;
    int selected;
    int num_adjacent;
    struct cell * adjacent[MAX_ADJACENT];
    int coords[MAX_DIM];
    int hint;
};

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells);

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords);

void getCoord(int offset, int * dim_sizes, int dim, struct cell * game);

void getHint(struct cell * currentCell);

void setMine(struct cell * currentCell, int * mineCoords, int dim);

//void setMine(int * mineCoords, struct cell * game, int dim, int numCells);

void selectRec(struct cell * currentCell);

void getAdj(struct cell * currentCell, struct cell * nextCell, int dim, int numCells);
