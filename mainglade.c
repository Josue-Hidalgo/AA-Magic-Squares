#include <gtk/gtk.h>
#include <stdio.h>
#include "magic_square.h"

static StepState step_state = {0};

// Función para calcular sumas de la matriz actual
void calculateCurrentSums(int order, int **magicSquare, int *sumasFilas, int *sumasColumnas, 
                         int *sumaDiagonalPrincipal, int *sumaDiagonalSecundaria) {
    // Inicializar sumas
    for (int i = 0; i < order; i++) {
        sumasFilas[i] = 0;
        sumasColumnas[i] = 0;
    }
    *sumaDiagonalPrincipal = 0;
    *sumaDiagonalSecundaria = 0;
    
    // Calcular sumas de filas y columnas
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            sumasFilas[i] += magicSquare[i][j];
            sumasColumnas[j] += magicSquare[i][j];
        }
    }
    
    // Calcular diagonales
    for (int i = 0; i < order; i++) {
        *sumaDiagonalPrincipal += magicSquare[i][i];
        *sumaDiagonalSecundaria += magicSquare[i][order - 1 - i];
    }
}

void fill_grid(GtkGrid *grid, GtkBuilder *builder, int order, int **magicSquare, 
               int sumasFilas[order], int sumasColumnas[order], 
               int sumaDiagonalPrincipal, int sumaDiagonalSecundaria,
               gboolean show_all_numbers) {  // ← AÑADIR este parámetro
    
    // Limpiar el grid
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Sumas de columnas (primera fila)
    for (int j = 0; j < order; j++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasColumnas[j]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, j + 1, 0, 1, 1);
    }

    // Sumas de filas (primera columna)
    for (int i = 0; i < order; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasFilas[i]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, 0, i + 1, 1, 1);
    }

    // Celdas del cuadrado mágico
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            char buf[16];
            // Si show_all_numbers es FALSE, mostrar solo números colocados (≠0)
            if (!show_all_numbers && magicSquare[i][j] == 0) {
                snprintf(buf, sizeof(buf), " ");  // Celda vacía
            } else {
                snprintf(buf, sizeof(buf), "%d", magicSquare[i][j]);
            }
            GtkWidget *label = gtk_label_new(buf);
            gtk_grid_attach(grid, label, j + 1, i + 1, 1, 1);
        }
    }
    gtk_widget_show_all(GTK_WIDGET(grid));

    // Actualizar labels de las diagonales
    GtkLabel *label_diag_prin = GTK_LABEL(gtk_builder_get_object(builder, "IDPDiag"));
    GtkLabel *label_diag_sec = GTK_LABEL(gtk_builder_get_object(builder, "IDSDiag"));
    char buf_diag_prin[16], buf_diag_sec[16];
    
    snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", sumaDiagonalPrincipal);
    snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", sumaDiagonalSecundaria);
    
    gtk_label_set_text(label_diag_prin, buf_diag_prin);
    gtk_label_set_text(label_diag_sec, buf_diag_sec);
}

void update_single_cell(GtkGrid *grid, GtkBuilder *builder, int order, int **magicSquare, 
                       int row, int col, int *sumasFilas, int *sumasColumnas,
                       int *sumaDiagonalPrincipal, int *sumaDiagonalSecundaria) {
    // Actualizar la celda específica
    GtkWidget *label = gtk_grid_get_child_at(grid, col + 1, row + 1);
    if (label != NULL) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", magicSquare[row][col]);
        gtk_label_set_text(GTK_LABEL(label), buf);
    }
    
    // Recalcular y actualizar todas las sumas
    calculateCurrentSums(order, magicSquare, sumasFilas, sumasColumnas, 
                        sumaDiagonalPrincipal, sumaDiagonalSecundaria);
    
    // Actualizar sumas de filas
    for (int i = 0; i < order; i++) {
        GtkWidget *label_fila = gtk_grid_get_child_at(grid, 0, i + 1);
        if (label_fila != NULL) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", sumasFilas[i]);
            gtk_label_set_text(GTK_LABEL(label_fila), buf);
        }
    }
    
    // Actualizar sumas de columnas
    for (int j = 0; j < order; j++) {
        GtkWidget *label_columna = gtk_grid_get_child_at(grid, j + 1, 0);
        if (label_columna != NULL) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", sumasColumnas[j]);
            gtk_label_set_text(GTK_LABEL(label_columna), buf);
        }
    }
    
    // Actualizar diagonales
    GtkLabel *label_diag_prin = GTK_LABEL(gtk_builder_get_object(builder, "IDPDiag"));
    GtkLabel *label_diag_sec = GTK_LABEL(gtk_builder_get_object(builder, "IDSDiag"));
    char buf_diag_prin[16], buf_diag_sec[16];
    
    snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", *sumaDiagonalPrincipal);
    snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", *sumaDiagonalSecundaria);
    
    gtk_label_set_text(label_diag_prin, buf_diag_prin);
    gtk_label_set_text(label_diag_sec, buf_diag_sec);
}

void on_step_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = (GtkBuilder *)user_data;
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "IDMatrix"));
    GtkSpinButton *spin_order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "IDOrder"));
    GtkComboBoxText *combo_form = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "IDForm"));
    
    int order = gtk_spin_button_get_value_as_int(spin_order);
    int active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_form));
    int stepRow = -1, stepCol = 1;
    
    switch (active_index) {
        case 0: stepRow = -1; stepCol = 1; break;
        case 1: stepRow = -1; stepCol = -1; break;
        case 2: stepRow = 1; stepCol = 1; break;
        case 3: stepRow = 1; stepCol = -1; break;
        case 4: stepRow = 2; stepCol = 1; break;
        default: break;
    }
    
    // Arrays para almacenar sumas actuales
    static int *sumasFilas = NULL;
    static int *sumasColumnas = NULL;
    static int sumaDiagonalPrincipal = 0;
    static int sumaDiagonalSecundaria = 0;
    static int current_order = 0;
    
    // Si el cuadro está completo, reiniciar para generar uno nuevo
    if (step_state.initialized && step_state.current_number >= order * order) {
        freeStepState(&step_state);
        step_state.initialized = 0; // Forzar reinicio
        
        // También liberar arrays de sumas si el orden cambió
        if (sumasFilas) free(sumasFilas);
        if (sumasColumnas) free(sumasColumnas);
        sumasFilas = NULL;
        sumasColumnas = NULL;
    }

    // Inicializar arrays de sumas si es el primer paso o cambió el orden
    if (!step_state.initialized || current_order != order) {
        if (sumasFilas) free(sumasFilas);
        if (sumasColumnas) free(sumasColumnas);
        
        sumasFilas = (int *)malloc(order * sizeof(int));
        sumasColumnas = (int *)malloc(order * sizeof(int));
        current_order = order;
        
        // Inicializar sumas a 0
        for (int i = 0; i < order; i++) {
            sumasFilas[i] = 0;
            sumasColumnas[i] = 0;
        }
        sumaDiagonalPrincipal = 0;
        sumaDiagonalSecundaria = 0;
    }
    
    // Inicializar si es el primer paso
    if (!step_state.initialized) {
        initStepState(order, stepRow, stepCol, &step_state);
        
        // Crear grid vacío (mostrar solo números colocados)
        fill_grid(grid, builder, order, step_state.magic_square, 
                  sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria,
                  FALSE);  // ← FALSE = mostrar solo números ≠ 0
    }
    
    // Avanzar un paso
    int result = getNextStep(&step_state, stepRow, stepCol);
    
    if (result) {
        // Actualizar solo la celda actual y todas las sumas
        update_single_cell(grid, builder, order, step_state.magic_square, 
                          step_state.current_row, step_state.current_col,
                          sumasFilas, sumasColumnas, &sumaDiagonalPrincipal, &sumaDiagonalSecundaria);
    } else {
        // Cuando se completa, mostrar todos los números (por si hay ceros)
        fill_grid(grid, builder, order, step_state.magic_square, 
                  sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria,
                  TRUE);  // ← TRUE = mostrar todos los números
    }
}

void on_finalize_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = (GtkBuilder *)user_data;
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "IDMatrix"));
    GtkSpinButton *spin_order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "IDOrder"));
    GtkComboBoxText *combo_form = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "IDForm"));
    
    int order = gtk_spin_button_get_value_as_int(spin_order);
    int active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_form));
    int stepRow = -1, stepCol = 1;
    
    switch (active_index) {
        case 0: stepRow = -1; stepCol = 1; break;
        case 1: stepRow = -1; stepCol = -1; break;
        case 2: stepRow = 1; stepCol = 1; break;
        case 3: stepRow = 1; stepCol = -1; break;
        case 4: stepRow = 2; stepCol = 1; break;
        default: break;
    }

    // Si hay un cuadro en progreso, COMPLETARLO en lugar de generar uno nuevo
    if (step_state.initialized && step_state.current_number < order * order) {
        
        // Completar los números faltantes
        while (getNextStep(&step_state, stepRow, stepCol)) {
            // Continuar hasta completar
        }
        
        // Calcular sumas finales
        int *sumasFilas = (int *)malloc(order * sizeof(int));
        int *sumasColumnas = (int *)malloc(order * sizeof(int));
        int sumaDiagonalPrincipal = 0, sumaDiagonalSecundaria = 0;
        
        calculateCurrentSums(order, step_state.magic_square, sumasFilas, sumasColumnas, 
                            &sumaDiagonalPrincipal, &sumaDiagonalSecundaria);
        
        // Mostrar cuadro completo
        fill_grid(grid, builder, order, step_state.magic_square, 
                  sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria,
                  TRUE);
        
        free(sumasFilas);
        free(sumasColumnas);
    } else {
        // Si no hay cuadro en progreso, generar uno nuevo
        int magicSquare[order][order];
        int sumasFilas[order];
        int sumasColumnas[order];
        int sumaDiagonalPrincipal = 0;
        int sumaDiagonalSecundaria = 0;

        getMagicSquareStatistics(order, stepRow, stepCol, &magicSquare, sumasFilas, sumasColumnas, &sumaDiagonalPrincipal, &sumaDiagonalSecundaria);

        // Convertir matriz local a int** para fill_grid
        int **magicSquarePtr = (int **)malloc(order * sizeof(int *));
        for (int i = 0; i < order; i++) {
            magicSquarePtr[i] = magicSquare[i];
        }
        
        fill_grid(grid, builder, order, magicSquarePtr, sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria, TRUE);
        free(magicSquarePtr);
    }
}

void exit_app() {
    printf("Exit app\n");
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "magic_square.css", NULL);
    GtkStyleContext *context = gtk_style_context_new();
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    builder = gtk_builder_new_from_file("GTK_Glade/GUI_MagicSquare.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "IDWindow"));

    // Conectar señales de los botones
    GtkButton *finalize_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDFinalize"));
    g_signal_connect(finalize_btn, "clicked", G_CALLBACK(on_finalize_clicked), builder);

    GtkButton *step_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDStep"));
    g_signal_connect(step_btn, "clicked", G_CALLBACK(on_step_clicked), builder);

    g_signal_connect(window, "destroy", G_CALLBACK(exit_app), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    // Liberar memoria al salir
    if (step_state.initialized) {
        freeStepState(&step_state);
    }

    g_object_unref(builder);
    return 0;
}