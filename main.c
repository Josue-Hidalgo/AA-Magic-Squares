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
void getInitialPosition(int order, int *row, int *col) {
    int a = 0, b = 0;
    while (
    b == 0 ||                // primera columna
    a == order-1 ||          // última fila
    a == b ||                // diagonal principal
    (a == 0 && b == 0) ||    // esquina superior izquierda
    (a == 0 && b == order-1) || // esquina superior derecha
    (a == order-1 && b == 0) || // esquina inferior izquierda
    (a == order-1 && b == order-1) // esquina inferior derecha
    ) {
        a = rand() % order;
        b = rand() % order;
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

    printf("Initial Position: (%d, %d)\n", initialRow, initialCol); // Debug print
    printf("Final Position: (%d, %d)\n", finalRow, finalCol); // Debug print

    // Usar desplazamiento mínimo en toro desde la final a la inicial
    int dx = minimal_disp(finalRow, initialRow, order);
    int dy = minimal_disp(finalCol, initialCol, order);

    printf("Break Move (before adjustment): (%d, %d)\n", dx, dy); // Debug print

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
        printf("5. izquierda-arriba (left-up)\n");
        printf("6. izquierda-abajo (left-down)\n");
        printf("7. derecha-arriba (right-up)\n");
        printf("8. derecha-abajo (right-down)\n");
        printf("9. Caballo (abajo-abajo-derecha)\n");

        scanf("%d", &option);
    } while (option < 1 || option > 9);

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
        case 5: // izquierda-arriba
            *stepRow = -1;
            *stepCol = 0;
            break;
        case 6: // izquierda-abajo
            *stepRow = 1;
            *stepCol = 0;
            break;
        case 7: // derecha-arriba
            *stepRow = -1;
            *stepCol = 0;
            break;
        case 8: // derecha-abajo
            *stepRow = 1;
            *stepCol = 0;
            break;
        case 9: // caballo (abajo-abajo-derecha)
            *stepRow = 2;
            *stepCol = 1;
            break;
    }
}

// MAIN //
int main() {
    // Inicializaciones
    srand(time(0));
    int order, row, col, breakMoveRow, breakMoveCol, stepRow, stepCol;
    getOrder(&order);
    getStep(&stepRow, &stepCol);

    printf("Order chosen: %d\n", order); // Debug print
    printf("Step chosen: (%d, %d)\n", stepRow, stepCol); // Debug print

    int square[order][order];
    for (int i = 0; i < order; i++) 
        for (int j = 0; j < order; j++)
            square[i][j] = 0;
    getInitialPosition(order, &row, &col);
    getBreakMove(row, col, &breakMoveRow, &breakMoveCol, order);

    // ALGORITMO: CUADRO MAGICO

    // 1 SE COLOCA EN POSICION INICIAL
    printf("Initial Position chosen: (%d, %d)\n", row, col); // Debug print
    square[row][col] = 1;

    // SE LLENA EL CUADRO A PARTIR DEL 2 HASTA order^2
    for (int i = 2; i <= pow(order, 2); i++) {
        //printMatrix(order, square);
        //printf("\n");

        // HACE EL MOVIMIENTO SELECCIONADO
        row += stepRow;
        col += stepCol;

        edgeGuard(&row, &col, order);

        // SI LA CASILLA ESTÁ VACÍA, SE COLOCA EL NÚMERO
        if (square[row][col] == 0) { 
            square[row][col] = i;
        } else { 
            // SI LA CASILLA YA TIENE UN NÚMERO

            // SE DEVUELVE A LA CASILLA ANTERIOR
            row -= stepRow;
            col -= stepCol;

            // SE HACE EL MOVIMIENTO DE RUPTURA
            row += breakMoveRow;
            col += breakMoveCol;
            edgeGuard(&row, &col, order);

            // SE COLOCA EL NÚMERO
            square[row][col] = i;
        }
    }

    // PRINT DEL CUADRO MAGICO
    printf("\nCuadro Magico de orden %d:\n", order);
    printMatrix(order, square);

    // FÓRMULA DE LA SUMA
    int suma = order * (1 + pow(order, 2)) / 2;
    printf("\nSuma que debe dar en todas: %d\n\n", suma);

    // VERIFICACIÓN DE SUMAS

    // filas
    for (int i = 0; i < order; i++) { // filas
        int sumatoria = 0;
        for (int j = 0; j < order; j++) { // columnas
            sumatoria += square[i][j]; 
            //printf("La sumatoria de la fila %d es: %d\n", i, sumatoria); //descomentar para ver el proceso
        }
        printf("Sumatoria de la fila %d: %d\n", i, sumatoria);
        printf("\n");
    }

    // columnas
    for (int i = 0; i < order; i++) { // filas
        int sumatoria = 0;
        for (int j = 0; j < order; j++) { // columnas
            sumatoria += square[j][i];
            //printf("La sumatoria de la columna %d es: %d\n", i, sumatoria);//descomentar para ver el proceso
        }
        printf("Sumatoria de la columna %d: %d\n", i, sumatoria);
        printf("\n");
    }

    // diagonal izquierda
    int sumatoriaD = 0;
    for (int i = 0; i < order; i++) {    
        sumatoriaD += square[i][i];
        //printf("La sumatoria de la diagonal es: %d\n", sumatoriaD);//descomentar para ver el proceso
    }
    printf("Sumatoria de la diagonal izquierda: %d\n\n", sumatoriaD);

    // diagonal derecha
    sumatoriaD = 0;
    for (int i = 0; i < order; i++) {
        sumatoriaD += square[i][order - 1 - i];
        //printf("La sumatoria de la diagonal es: %d\n", sumatoriaD);//descomentar para ver el proceso
    }
    printf("Sumatoria de la diagonal derecha: %d\n", sumatoriaD);

    return 0;
}