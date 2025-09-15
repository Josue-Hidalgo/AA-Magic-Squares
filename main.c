#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

// FUNCIONES AUXILIARES
/* Función para hacer el efecto cilindro, o sea, 
que si el movimiento hace que se salga del cuadro,
se regrese adentro en la posición equivalente
*/
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

// FUNCIONES GET
void getInitialPosition(int order, int *row, int *col) {
    int a = 0,b = 0,mid = 0;
    mid = (order - 1) / 2;

    while (
        (a == 0 && b == 0)          ||
        (a == 0 && b == order-1)    ||
        (a == order-1 && b == 0)    ||
        (a == mid && b == mid)      ||
        (a == order-1 && b == order-1)
    ) {
        a = rand() % order;
        b = rand() % order;
    }
    *row = a;
    *col = b;
}

void getFinalPosition(const int *order, const int row, const int col, int *rowFinal, int *colFinal) {
    int a,b,mid;
    mid = (*order - 1) / 2;

    a = 2 * mid - (row);
    b = 2 * mid - (col);

    *rowFinal = a;
    *colFinal = b;
}

void getOrder(int *order) {
    do {
        printf("Digite un numero impar mayor que 3 y menor que 21: ");
        scanf("%d", order);
    } while (*order % 2 == 0 || *order < 3 || *order > 21);
}

void getBreakMove(const int initialRow, const int initialCol, int *breakMoveRow, int *breakMoveCol, int order){
    int finalRow, finalCol;
    getFinalPosition(&order, initialRow, initialCol, &finalRow, &finalCol);

    *breakMoveRow = initialRow - finalRow;
    *breakMoveCol = initialCol - finalCol;
}

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

// MAIN
int main() {
    srand(time(0));

    int order, row, col, breakMoveRow, breakMoveCol, stepRow, stepCol;

    getOrder(&order);
    getStep(&stepRow, &stepCol);
    
    int square[order][order]; // crea el cuadro (una matriz)

    // rellena el cuadro de 0s
    for (int i = 0; i < order; i++) 
        for (int j = 0; j < order; j++)
            square[i][j] = 0;

    getInitialPosition(order, &row, &col);
    getBreakMove(row, col, &breakMoveRow, &breakMoveCol, order);

    // pone un 1 en una posición inicial aleatoria
    square[row][col] = 1;

    for (int i = 2; i <= pow(order, 2); i++) {

        //MOVIMIENTO SELECCINADO
        // hace el movimiento diagonal hacia arriba
        row += stepRow;
        col += stepCol;

        edgeGuard(&row, &col, order);

        if (square[row][col] == 0) { // si la casilla es un 0, pone el siguiente número
            square[row][col] = i;
        } else { 
            //-(MOVIMIENTO SELECCIONADO)
            // si ya hay un número, se regresa a la casilla original
            row -= stepRow;
            col -= stepCol;

            // y hace el break move (bajar 1)
            row += breakMoveRow;
            col += breakMoveCol;
            edgeGuard(&row, &col, order); // revisa de nuevo si se salió del cuadro

            square[row][col] = i;
        }
    }

    // imprimir el cuadro mágico
    printf("\nCuadro Magico de orden %d:\n", order);
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            printf("%3d ", square[i][j]);
        }
        printf("\n");
    }

    int suma = order * (1 + pow(order, 2)) / 2;
    printf("\nSuma que debe dar en todas: %d\n\n", suma);


    // *************************************************
    // SECCIÓN PARA VER LAS SUMAS DE LAS HORIZONTALES, VERTICALES Y DIAGONALES
    // *************************************************
    
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

    // diagonales
    int sumatoriaD = 0;
    for (int i = 0; i < order; i++) {    
        sumatoriaD += square[i][i];
        //printf("La sumatoria de la diagonal es: %d\n", sumatoriaD);//descomentar para ver el proceso
    }
    printf("Sumatoria de la diagonal izquierda: %d\n\n", sumatoriaD);

    sumatoriaD = 0;
    for (int i = 0; i < order; i++) {
        sumatoriaD += square[i][order - 1 - i];
        //printf("La sumatoria de la diagonal es: %d\n", sumatoriaD);//descomentar para ver el proceso
    }
    printf("Sumatoria de la diagonal derecha: %d\n", sumatoriaD);

    return 0;
}