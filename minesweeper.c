#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {

  int numCells = 1;


  for(int i = 0; i < dim; i++) {
    numCells = dim_sizes[i] * numCells;
  }

  for(int i = 0; i < numCells; i++){ // Sets the coordinates of each of the cells in ROW MAJOR ORDER!!!
    getCoord(i, dim_sizes, dim, &game[i]);
    game[i].mined = 0; // Also sets all the values of the mined to 0! :)
    game[i].num_adjacent = 0; // also sets amount of adjacent cells to 0. we will figure this out later.
    game[i].selected = 0;
    game[i].hint = 0;
  }

  for(int i = 0; i < num_mines; i++) {  // Goes through every cell and will set the mines where they are meant to be set.
    for(int j = 0; j < numCells; j++) {
      setMine(&game[j], mined_cells[i], dim);
    }
  }


  for(int i = 0; i < numCells; i++) { // Goes through every cell and will set its adjacent cells. for fuckks sake why isnt this working
    for(int j = 0; j < numCells; j++) {
      getAdj(&game[i], &game[j], dim, numCells);
    }
  }

  for(int i = 0; i < numCells; i++) {
    getHint(&game[i]);
  }
  return;
}

void getHint(struct cell * currentCell) {

  for(int i = 0; i < currentCell->num_adjacent; i++) {
    if(currentCell->adjacent[i]->mined == 1) {
      currentCell->hint++;
    }
  }

  return;
}

void getAdj(struct cell * currentCell, struct cell * nextCell, int dim, int numCells) {

  int arrayIndex = currentCell->num_adjacent;

  if(currentCell == nextCell) {
    return;
  }

  for(int i = 0; i < dim; i++) {
    if((currentCell->coords[i] - nextCell->coords[i]) < -1) {
      return;
    } else if ((currentCell->coords[i] - nextCell->coords[i]) > 1) {
      return;
    }
  }
  currentCell->adjacent[arrayIndex] = nextCell;
  currentCell->num_adjacent++;
  return;

}


void setMine(struct cell * currentCell, int * mineCoords, int dim) {

  for(int i = 0; i < dim; i++) { // if any of the coordinates do not equal that of the mines, instantly returns and then calls the next gamecell.
    if(currentCell->coords[i] != mineCoords[i]) {
      return;
    }
  }

  currentCell->mined = 1;
  return;

}



void getCoord(int offset, int * dim_sizes, int dim, struct cell * game) {
  for(int i = 0; i < dim; i++) {
    game->coords[i] = offset % dim_sizes[i];
    offset = offset / dim_sizes[i];
  }
  return;
}

int hasWon(struct cell * game, int dim, int * dim_sizes) {
  int numCells = 1;
  int numMines = 0;
  int numSelected = 0;

  for(int i = 0; i < dim; i++) {
    numCells = dim_sizes[i] * numCells;
  }

  for(int i = 0; i < numCells; i++) {
    if(game[i].mined == 1) {
      numMines++;
    }
  }

  for(int i = 0; i < numCells; i++) {
    if(game[i].selected == 1) {
      numSelected++;
    }
  }


  if((numCells - numMines) == numSelected) {
    return 1;
  }

  return 0;

}

void selectRec(struct cell * currentCell) {
  for(int i = 0; i < currentCell->num_adjacent; i++) {
    if(currentCell->adjacent[i]->mined == 1) {
      return;
    }
  }
  for(int i = 0; i < currentCell->num_adjacent; i++) {
    if(currentCell->adjacent[i]->selected == 0) {
      currentCell->adjacent[i]->selected = 1;
      selectRec(currentCell->adjacent[i]);
    }
  }
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {

    for(int i = 0; i < dim; i++) { // this is just testing if the coordinates inputted are valid.
      if(coords[i] > dim_sizes[i] || coords[i] < 0) {
        return 0;
      }
    }

    int numCells = 1;
    int selFlag = 0;

    for(int i = 0; i < dim; i++) {
      numCells = numCells * dim_sizes[i];
    }


    for(int i = 0; i < numCells; i++) {

      selFlag = 0;

      for(int j = 0; j < dim; j++) {

        if(game[i].coords[j] == coords[j]) {
          selFlag = 1;
        } else { // if ANY of the coordinates of the cell do not match those given, break.
          selFlag = 0;
          break;
        }
      }
      if(selFlag == 1) {
        if(game[i].selected == 1) {
          return 0;
        }
        game[i].selected = 1;
        if(game[i].mined == 1) {
          return 1;
        } else if(hasWon(game, dim, dim_sizes) == 1) { // Just checks if this is the last cell of the game.
          return 2;
        }

        selectRec(&game[i]); // Will call the recursive function once a cell is selected.
        if(hasWon(game, dim, dim_sizes) == 1) { // After the cells have been recursively called, once again check if won.
          return 2;
        }

        return 0;
      }
    }

    return 1;
}
