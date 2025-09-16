#ifndef MAGIC_SQUARE_H
#define MAGIC_SQUARE_H

void getOrder(int *order);
void getStep(int *stepRow, int *stepCol);
void getMagicSquareStatistics(int order, int stepRow, int stepCol, int (*magicSquare)[order][order], int *sumasFilas, int *sumasColumnas, int *sumaDiagonalPrincipal, int *sumaDiagonalSecundaria);
void printMatrix(int order, int (*magicSquare)[order][order]);
void printVector(int size, int vector[size]);
void getFinalPosition(const int *order, const int initialRow, const int initialCol, int *rowFinal, int *colFinal);
void getBreakMoveFunc(const int initialRow, const int initialCol, int *breakMoveRow, int *breakMoveCol, int order, int stepRow, int stepCol);
void edgeGuard(int *row, int *col, int order);
void getInitialPosition(int order, int stepRow, int stepCol, int *row, int *col);
int minimal_disp(int a, int b, int N);

#endif