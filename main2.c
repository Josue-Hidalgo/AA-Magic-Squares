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
        while (b == 0 || a == order - 1 || a + b == order - 1) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // arriba - izquierda
    if (*stepRow == -1 && *stepCol == -1) {
        while (b == order - 1 || a == order - 1 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }
    
    // abajo - derecha
    if (*stepRow == 1 && *stepCol == 1) {
        while (b == 0 || a == 0 || a == b) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // abajo - izquierda
    if (*stepRow == 1 && *stepCol == -1) {
        while (b == order - 1 || a == 0 || a + b == order - 1) {
            a = rand() % order;
            b = rand() % order;
        }
    }

    // caballo
    if (*stepRow == 2 && *stepCol == 1) {
        while (b != order / 2 || a != 0) {
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