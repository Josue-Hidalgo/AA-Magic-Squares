#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void guessInitialBox(int order, int *row, int *col) {
    int a,b,mid;
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

int main() {
    int order;
    
    printf("Digite un numero impar mayor que 3 y menor que 21: ");
    scanf("%d", &order);

    if (order % 2 == 0 || order < 3 || order > 21) {
        printf("El numero debe ser impar, mayor que 3 y menor que 21\n");
        return 1;
    }

    int square[order][order]; // crea el cuadro (una matriz)

    // rellena el cuadro de 0s
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            square[i][j] = 0;
        }
    }

    // pone un 1 en el centro de la primera fila
    int row = 0;
    int col = order / 2;
    square[row][col] = 1;

    for (int i = 2; i <= pow(order, 2); i++) {
        // hace el movimiento diagonal hacia arriba
        row = row - 1;
        col = col + 1;

        edgeGuard(&row, &col, order);

        if (square[row][col] == 0) { // si la casilla es un 0, pone el siguiente número
            square[row][col] = i;
        } else { 
            // si ya hay un número, se regresa a la casilla original
            row = row + 1;
            col = col - 1;
            // y hace el break move (bajar 1)
            row = row + 1;
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