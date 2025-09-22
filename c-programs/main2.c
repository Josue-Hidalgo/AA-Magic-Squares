#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

// FUNCIONES AUXILIARES

// Imprimir matriz cuadrada
void printMatrix(int order, int square[order][order]) {
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            printf("%3d ", square[i][j]);
        }
        printf("\n");
    }
}

// Efecto Cilindrico
void edgeGuard(int *row, int *col, int order) {
    if (*row < 0)
        *row = *row + order;
    if (*row >= order)
        *row = *row - order;
    if (*col < 0)
        *col = *col + order;
    if (*col >= order)
        *col = *col - order;
}

// Casilla Inicial
void getInitialPosition(int order, int *row, int *col, int *stepRow, int *stepCol) {
    int a = 0, b = 0;
    // arriba - derecha
    if (*stepRow == -1 && *stepCol == 1) {
        // inválidas: fila orden - 1, columna 0, diagonal secundaria
        if (order == 21) {
            int validas[][2] = {{0,1},{0,4},{0,10},{0,16},{0,19},
            {1,2},{1,8},{1,11},{1,17},{1,20},
            {3,1},{3,4},{3,13},{3,16},{3,19},
            {4,5},{4,8},{4,11},{4,17},{4,20},
            {7,2},{7,5},{7,8},{7,11},{7,17},
            {9,1},{9,10},{9,13},{9,16},{9,19},
            {10,2},{10,5},{10,8},{10,11},{10,20},
            {12,4},{12,10},{12,13},{12,16},{12,19},
            {15,1},{15,4},{15,10},{15,13},{15,16},
            {16,2},{16,5},{16,8},{16,17},{16,20},
            {18,1},{18,4},{18,10},{18,13},{18,19},
            {19,2},{19,5},{19,11},{19,17},{19,20}
        };
        int idx = rand() % 60;
        a = validas[idx][0];
        b = validas[idx][1];
        }
        while (a == order - 1 || b == 0 || a + b == order - 1) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // arriba - izquierda
    if (*stepRow == -1 && *stepCol == -1) {
        if (order == 21) {
            int validas[][2] = {{0, 1},{0, 4},{0, 10},{0, 16},{0, 19},
            {1, 0},{1, 3},{1, 9},{1, 12},{1, 18},
            {3, 1},{3, 4},{3, 7},{3, 16},{3, 19},
            {4, 0},{4, 3},{4, 9},{4, 12},{4, 15},
            {7, 3},{7, 9},{7, 12},{7, 15},{7, 18},
            {9, 1},{9, 4},{9, 7},{9, 10},{9, 19},
            {10, 0},{10, 9},{10, 12},{10, 15},{10, 18},
            {12, 1},{12, 4},{12, 7},{12, 10},{12, 16},
            {15, 4},{15, 7},{15, 10},{15, 16},{15, 19},
            {16, 0},{16, 3},{16, 12},{16, 15},{16, 18},
            {18, 1},{18, 7},{18, 10},{18, 16},{18, 19},
            {19, 0},{19, 3},{19, 9},{19, 15},{19, 18}
        };
        int idx = rand() % 60;
        a = validas[idx][0];
        b = validas[idx][1];
        }
        // inválidas: fila y columna orden - 1, diagonal principal
        while (a == order - 1 || b == order - 1 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }
    
    // abajo - derecha
    if (*stepRow == 1 && *stepCol == 1) {
        if (order == 21) {
            int validas[][2] = {{1, 2},{1, 5},{1, 11},{1, 17},{1, 20},
            {2, 1},{2, 4},{2, 10},{2, 13},{2, 19},
            {4, 2},{4, 5},{4, 8},{4, 17},{4, 20},
            {5, 1},{5, 4},{5, 10},{5, 13},{5, 16},
            {8, 4},{8, 10},{8, 13},{8, 16},{8, 19},
            {10, 2},{10, 5},{10, 8},{10, 11},{10, 20},
            {11, 1},{11, 10},{11, 13},{11, 16},{11, 19},
            {13, 2},{13, 5},{13, 8},{13, 11},{13, 17},
            {16, 5},{16, 8},{16, 11},{16, 17},{16, 20},
            {17, 1},{17, 4},{17, 13},{17, 16},{17, 19},
            {19, 2},{19, 8},{19, 11},{19, 17},{19, 20},
            {20, 1},{20, 4},{20, 10},{20, 16},{20, 19}
        };
        int idx = rand() % 60;
        a = validas[idx][0];
        b = validas[idx][1];
        }

        // inválidas: fila y columna 0, diagonal principal
        while (a == 0 || b == 0 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // abajo - izquierda
    if (*stepRow == 1 && *stepCol == -1) {
        if (order == 21) {
            int validas[][2] = {{1, 0},{1, 3},{1, 9},{1, 15},{1, 18},
            {2, 1},{2, 7},{2, 10},{2, 16},{2, 19},
            {4, 0},{4, 3},{4, 12},{4, 15},{4, 18},
            {5, 4},{5, 7},{5, 10},{5, 16},{5, 19},
            {8, 1},{8, 4},{8, 7},{8, 10},{8, 16},
            {10, 0},{10, 9},{10, 12},{10, 15},{10, 18},
            {11, 1},{11, 4},{11, 7},{11, 10},{11, 19},
            {13, 3},{13, 9},{13, 12},{13, 15},{13, 18},
            {16, 0},{16, 3},{16, 9},{16, 12},{16, 15},
            {17, 1},{17, 4},{17, 7},{17, 16},{17, 19},
            {19, 0},{19, 3},{19, 9},{19, 12},{19, 18},
            {20, 1},{20, 4},{20, 10},{20, 16},{20, 19}
        };
        int idx = rand() % 60;
        a = validas[idx][0];
        b = validas[idx][1];
        }
        // inválidas: fila 0, columna orden - 1, diagonal secundaria
        while (a == 0 || b == order - 1 || a + b == order - 1) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // caballo
    if (*stepRow == 2 && *stepCol == 1) {
        // inválidas: todas menos la fila 0 en el medio (SOLO ORDEN 3)
        if (order == 3) {
            while (a != 0 || b != order / 2 ) {
                a = rand() % order;
                b = rand() % order;
            }
        }
        else if (order == 9 || order == 15 || order || 21) {
            while (a %3 != 0 || b % 3 != 1) {
                a = rand() % order;
                b = rand() % order;
            }
        } else {
            a = rand() % order;
            b = rand() % order;
        }
    }
    *row = a;
    *col = b;
}

// Casilla Final
void getFinalPosition(const int *order, const int initialRow, const int initialCol, int *rowFinal, int *colFinal) {
    int a,b,mid;
    mid = (*order - 1) / 2;

    a = 2 * mid - (initialRow);
    b = 2 * mid - (initialCol);

    *rowFinal = a;
    *colFinal = b;
}

// Movimiento de Ruptura (Break Move)
int minimal_disp(int a, int b, int N) {
    int raw = b - a;
    int cand1 = raw;
    int cand2 = raw - N;
    int cand3 = raw + N;
    int best = cand1;
    if (abs(cand2) < abs(best)) best = cand2;
    if (abs(cand3) < abs(best)) best = cand3;
    return best;
}

// Movimiento de Ruptura (Break Move)
void getBreakMove(const int initialRow, const int initialCol, int *breakMoveRow, int *breakMoveCol, int order){
    int finalRow, finalCol;
    getFinalPosition(&order, initialRow, initialCol, &finalRow, &finalCol);

    // Usar desplazamiento mínimo en toro desde la final a la inicial
    int dx = minimal_disp(finalRow, initialRow, order);
    int dy = minimal_disp(finalCol, initialCol, order);

    *breakMoveRow = dx;
    *breakMoveCol = dy;
}

// Orden (Luego se cambiará por inpun del GTKGlade)
void getOrder(int *order) {
    do {
        printf("Digite un numero impar mayor que 3 y menor que 21: ");
        scanf("%d", order);
    } while (*order % 2 == 0 || *order < 3 || *order > 21);
}

// Paso (Luego se cambiará por input del GTKGlade)
void getStep(int *stepRow, int *stepCol) {
    int option;
    do {
        printf("Introduzca una forma que se utilizará para construir el cuadrado mágico: \n");
        printf("1. arriba-derecha (up-right)\n");
        printf("2. arriba-izquierda (up-left)\n");
        printf("3. abajo-derecha (down-right)\n");
        printf("4. abajo-izquierda (down-left)\n");
        printf("5. Caballo (abajo-abajo-derecha)\n");

        scanf("%d", &option);
    } while (option < 1 || option > 5);

    switch (option) {
        case 1: // arriba-derecha
            *stepRow = -1;
            *stepCol = 1;
            break;
        case 2: // arriba-izquierda
            *stepRow = -1;
            *stepCol = -1;
            break;
        case 3: // abajo-derecha
            *stepRow = 1;
            *stepCol = 1;
            break;
        case 4: // abajo-izquierda
            *stepRow = 1;
            *stepCol = -1;
            break;
        case 5: // caballo (abajo-abajo-derecha)
            *stepRow = 2;
            *stepCol = 1;
            break;
        default:
            printf("Opción inválida. Usando movimiento por defecto (arriba-derecha).\n");
            break;
    }
}

// Algoritmo completo de cuadro mágico
void magicSquareAlgorithm(int order, int stepRow, int stepCol, int initialRow, int initialCol) {
    int row = initialRow, col = initialCol, breakMoveRow, breakMoveCol;
    int square[order][order];
    for (int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            square[i][j] = 0;
    if (stepRow == 2 && stepCol == 1) {
        breakMoveRow = 4;
        breakMoveCol = 0;
    } else {
        getBreakMove(row, col, &breakMoveRow, &breakMoveCol, order);
    }
    square[row][col] = 1;
    for (int i = 2; i <= order * order; i++) {
        row += stepRow;
        col += stepCol;
        edgeGuard(&row, &col, order);
        if (square[row][col] == 0) {
            square[row][col] = i;
        } else {
            row -= stepRow;
            col -= stepCol;
            row += breakMoveRow;
            col += breakMoveCol;
            edgeGuard(&row, &col, order);
            square[row][col] = i;
        }
    }
    int suma = order * (1 + order * order) / 2;
    
    // En lugar de setear valido tiene que devolver los valores de las sumas.
    int valido = 1;
    
    // Verificar filas
    for (int i = 0; i < order; i++) {
        int sumatoria = 0;
        for (int j = 0; j < order; j++)
            sumatoria += square[i][j];
        if (sumatoria != suma) valido = 0;
    }
    // Verificar columnas
    for (int i = 0; i < order; i++) {
        int sumatoria = 0;
        for (int j = 0; j < order; j++)
            sumatoria += square[j][i];
        if (sumatoria != suma) valido = 0;
    }
    // Verificar diagonales
    int sumatoriaD = 0;
    for (int i = 0; i < order; i++)
        sumatoriaD += square[i][i];
    if (sumatoriaD != suma) valido = 0;
    sumatoriaD = 0;
    for (int i = 0; i < order; i++)
        sumatoriaD += square[i][order - 1 - i];
    if (sumatoriaD != suma) valido = 0;
    
    
    /*if (valido) {
        printf("{%d, %d},", initialRow, initialCol);
    }*/

    if (valido) {
        printf("Cuadro válido\n");
        printMatrix(order, square);
    } else {
        printf("Cuadro no válido\n");
        printf("Inicial: (%d, %d)\n", initialRow, initialCol);
        int finalRow, finalCol;
        getFinalPosition(&order, initialRow, initialCol, &finalRow, &finalCol);
        printf("Final: (%d, %d)\n", finalRow, finalCol);
        printf("BreakMove: (%d, %d)\n", breakMoveRow, breakMoveCol);
        printMatrix(order, square);
    }
}

// Recorrer todas las posiciones iniciales válidas
void recorrerTodasLasIniciales(int order, int stepRow, int stepCol) {
    for (int a = 0; a < order; a++) {
        for (int b = 0; b < order; b++) {
            magicSquareAlgorithm(order, stepRow, stepCol, a, b);
        }
    }
}

// MAIN //
int main() {
    // Inicializaciones
    srand(time(0));
    int order, stepRow, stepCol, initialRow, initialCol;
    getOrder(&order);
    getStep(&stepRow, &stepCol);
    getInitialPosition(order, &initialRow, &initialCol, &stepRow, &stepCol);
    magicSquareAlgorithm(order, stepRow, stepCol, initialRow, initialCol);
    //recorrerTodasLasIniciales(order, stepRow, stepCol);
    //getAllValidInitialPositions(order, stepRow, stepCol);
    return 0;
}