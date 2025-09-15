#include <stdio.h>
#include <stdlib.h>

int abs_int(int x){ return x < 0 ? -x : x; }

int minimal_disp(int a, int b, int N) {
    int raw = b - a;
    int cand1 = raw;
    int cand2 = raw - N;
    int cand3 = raw + N;
    int best = cand1;
    if (abs_int(cand2) < abs_int(best)) best = cand2;
    if (abs_int(cand3) < abs_int(best)) best = cand3;
    return best;
}

void print_route(int x1, int y1, int x2, int y2, int N) {
    int dx = minimal_disp(x1, x2, N); // vertical: + -> bajar, - -> subir
    int dy = minimal_disp(y1, y2, N); // horizontal: + -> derecha, - -> izquierda
    int steps = abs_int(dx) + abs_int(dy);

    printf("Distancia mínima (torus Manhattan): %d pasos\n", steps);
    printf("Desplazamiento vertical: %d (positivo = bajar)\n", dx);
    printf("Desplazamiento horizontal: %d (positivo = derecha)\n", dy);

    // Empezamos en (x1,y1)
    int x = x1, y = y1;
    // Mover verticalmente
    int k;
    for (k = 0; k < abs_int(dx); ++k) {
        if (dx > 0) {
            // bajar (x+1 con wrap)
            x = (x + 1) % N;
            printf("%2d: bajar -> (%d,%d)\n", k+1, x, y);
        } else {
            // subir (x-1 con wrap)
            x = (x - 1 + N) % N;
            printf("%2d: subir  -> (%d,%d)\n", k+1, x, y);
        }
    }
    // Mover horizontalmente (continúa numeración)
    for (k = 0; k < abs_int(dy); ++k) {
        int step_no = abs_int(dx) + k + 1;
        if (dy > 0) {
            // derecha (y+1 con wrap)
            y = (y + 1) % N;
            printf("%2d: derecha -> (%d,%d)\n", step_no, x, y);
        } else {
            // izquierda (y-1 con wrap)
            y = (y - 1 + N) % N;
            printf("%2d: izquierda-> (%d,%d)\n", step_no, x, y);
        }
    }
}

int main(void) {
    int N = 5;              // ejemplo
    int x1 = 1, y1 = 0;     // A
    int x2 = 4, y2 = 3;     // B
    // Si son diametralmente opuestos respecto al centro (0-based):
    // (x2,y2) = (N-1-x1, N-1-y1)

    printf("A (%d,%d) -> B (%d,%d) en N=%d\n", x1, y1, x2, y2, N);
    print_route(x1, y1, x2, y2, N);
    return 0;
}
