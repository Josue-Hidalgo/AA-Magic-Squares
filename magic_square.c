#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

typedef struct {
    int current_number;
    int current_row;
    int current_col;
    int break_move_row;
    int break_move_col;
    int initialized;
    int **magic_square;
    int order;
} StepState;

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
        else if (order == 9 || order == 15 || order == 21) {
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

void initStepState(int order, int stepRow, int stepCol, StepState *state) {
    state->current_number = 0;
    state->initialized = 1;
    state->order = order;
    
    // Crear matriz dinámica
    state->magic_square = (int **)malloc(order * sizeof(int *));
    for (int i = 0; i < order; i++) {
        state->magic_square[i] = (int *)calloc(order, sizeof(int));
    }
    
    // Obtener posición inicial y break move
    getInitialPosition(order, &state->current_row, &state->current_col, &stepRow, &stepCol);
    getBreakMoveFunc(state->current_row, state->current_col, &state->break_move_row, &state->break_move_col, order, stepRow, stepCol);
}

// Función para liberar memoria
void freeStepState(StepState *state) {
    if (state->magic_square != NULL) {
        for (int i = 0; i < state->order; i++) {
            free(state->magic_square[i]);
        }
        free(state->magic_square);
        state->magic_square = NULL;
    }
    state->initialized = 0;
    state->current_number = 0;
}

// Función para avanzar un paso
int getNextStep(StepState *state, int stepRow, int stepCol) {
    if (!state->initialized || state->magic_square == NULL) {
        return 0;
    }
    
    int order = state->order;
    
    // Si es el primer número (1)
    if (state->current_number == 0) {
        state->magic_square[state->current_row][state->current_col] = 1;
        state->current_number = 1;
        return 1;
    }
    
    // Si ya terminamos
    if (state->current_number >= order * order) {
        return 0;
    }
    
    int next_number = state->current_number + 1;
    int row = state->current_row;
    int col = state->current_col;
    
    // MOVIMIENTO PRINCIPAL
    int try_row = row + stepRow;
    int try_col = col + stepCol;
    edgeGuard(&try_row, &try_col, order);
    
    if (state->magic_square[try_row][try_col] == 0) {
        // Casilla libre
        state->magic_square[try_row][try_col] = next_number;
        state->current_row = try_row;
        state->current_col = try_col;
    } else {
        
        // Volver a posición original
        try_row = row;
        try_col = col;
        
        // Aplicar break move
        try_row += state->break_move_row;
        try_col += state->break_move_col;
        edgeGuard(&try_row, &try_col, order);
        
        state->magic_square[try_row][try_col] = next_number;
        state->current_row = try_row;
        state->current_col = try_col;
    }
    
    state->current_number = next_number;
    return 1;
}