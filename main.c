#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRINCIPALES
////////////////////////////////////////////////////////////////////////////

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
// Casilla Inicial
void getInitialPosition(int order, int *row, int *col, int *stepRow, int *stepCol) {
    int a = 0, b = 0;
    // arriba - derecha
    if (*stepRow == -1 && *stepCol == 1) {
        // inválidas: fila orden - 1, columna 0, diagonal secundaria
        while (a == order - 1 || b == 0 || a + b == order - 1) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // arriba - izquierda
    if (*stepRow == -1 && *stepCol == -1) {
        // inválidas: fila y columna orden - 1, diagonal principal
        while (a == order - 1 || b == order - 1 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }
    
    // abajo - derecha
    if (*stepRow == 1 && *stepCol == 1) {
        // inválidas: fila y columna 0, diagonal principal
        while (a == 0 || b == 0 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // abajo - izquierda
    if (*stepRow == 1 && *stepCol == -1) {
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
// Movimiento de Ruptura (Break Move) - VemagicSquarersión modificada para obtener los valores
void getBreakMoveFunc(const int initialRow, const int initialCol, int *breakMoveRow, int *breakMoveCol, int order, int stepRow, int stepCol){
    if (stepRow == 2 && stepCol == 1) {
        *breakMoveRow = 4;
        *breakMoveCol = 0;
    } else {
        int finalRow, finalCol;
        getFinalPosition(&order, initialRow, initialCol, &finalRow, &finalCol);

        int dx = minimal_disp(finalRow, initialRow, order);
        int dy = minimal_disp(finalCol, initialCol, order);

        *breakMoveRow = dx;
        *breakMoveCol = dy;
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
// Obtener el Cuadro Mágico y estadísticas
void getMagicSquareStatistics(int order, int stepRow, int stepCol, int (*magicSquare)[order][order], int *sumasFilas, int *sumasColumnas, int *sumaDiagonalPrincipal, int *sumaDiagonalSecundaria){
    // Inicializar variables
    int row, col, initialRow, initialCol, breakMoveRow, breakMoveCol;
    getInitialPosition(order, &initialRow, &initialCol, &stepRow, &stepCol);

    // Inicializar la matriz a 0
    for (int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            (*magicSquare)[i][j] = 0;

    // Obtener Break Move
    getBreakMoveFunc(initialRow, initialCol, &breakMoveRow, &breakMoveCol, order, stepRow, stepCol);

    // Primer valor
    row = initialRow;
    col = initialCol;

    (*magicSquare)[row][col] = 1;

    // Llenar el cuadrado mágico
    for (int i = 2; i <= order * order; i++) {
        // Paso
        row += stepRow;
        col += stepCol;
        // Efecto cilíndrico
        edgeGuard(&row, &col, order);
        // Colocar número 
        if ((*magicSquare)[row][col] == 0) {
            (*magicSquare)[row][col] = i;
        } else {
        // Movimiento de ruptura
            row -= stepRow;
            col -= stepCol;
            row += breakMoveRow;
            col += breakMoveCol;
            edgeGuard(&row, &col, order);
            (*magicSquare)[row][col] = i;
        }
    }

    // Calcular sumas de filas 
    for (int i = 0; i < order; i++) {
        int sumatoria = 0;
        for (int j = 0; j < order; j++)
            sumatoria += (*magicSquare)[i][j];
        sumasFilas[i] = sumatoria;
    }

    // Calcular sumas de columnas
    for (int i = 0; i < order; i++) {
        int sumatoria = 0;
        for (int j = 0; j < order; j++)
            sumatoria += (*magicSquare)[j][i];
        sumasColumnas[i] = sumatoria;
    }

    // Calcular suma diagonal principal y secundaria
    *sumaDiagonalPrincipal = 0;
    *sumaDiagonalSecundaria = 0;
    
    for (int i = 0; i < order; i++) {
        *sumaDiagonalPrincipal += (*magicSquare)[i][i];
        *sumaDiagonalSecundaria += (*magicSquare)[i][order - 1 - i];
    }
}
// Imprimir matriz cuadrada
void printMatrix(int order, int (*magicSquare)[order][order]) {
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            printf("%3d ", (*magicSquare)[i][j]);
        }
        printf("\n");
    }
}
// Imprimir vector
void printVector(int size, int vector[size]) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

// MAIN //
int main() {
    srand(time(0));
    
    int order, stepRow, stepCol;

    getOrder(&order);
    getStep(&stepRow, &stepCol);

    int magicSquare[order][order];
    int sumasFilas[order];
    int sumasColumnas[order];
    int sumaDiagonalPrincipal = 0;
    int sumaDiagonalSecundaria = 0;

    getMagicSquareStatistics(order, stepRow, stepCol, &magicSquare, sumasFilas, sumasColumnas, &sumaDiagonalPrincipal, &sumaDiagonalSecundaria);

    printf("Matriz generada:\n");
    printMatrix(order, &magicSquare);
    printf("Sumas de filas:\n");
    printVector(order, sumasFilas);
    printf("Sumas de columnas:\n");
    printVector(order, sumasColumnas);
    printf("Suma diagonal principal: %d\n", sumaDiagonalPrincipal);
    printf("Suma diagonal secundaria: %d\n", sumaDiagonalSecundaria);

    return 0;
}