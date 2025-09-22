#include <gtk/gtk.h>
#include <stdio.h>
#include "magic_square.h"

static StepState step_state = {0};

void fill_grid(GtkGrid *grid, int order, int **magicSquare, int sumasFilas[order], int sumasColumnas[order], int sumaDiagonalPrincipal, int sumaDiagonalSecundaria) {
    // Limpiar el grid
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Sumas de columnas
    for (int j = 0; j < order; j++) {
        char buf[16];
        if (sumasColumnas != NULL) {
            snprintf(buf, sizeof(buf), "%d", sumasColumnas[j]);
        } else {
            snprintf(buf, sizeof(buf), "-");
        }
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, j + 1, 0, 1, 1);
    }
    
    // Suma diagonal secundaria
    char buf_diag_sec[16];
    if (sumaDiagonalSecundaria != 0) {
        snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", sumaDiagonalSecundaria);
    } else {
        snprintf(buf_diag_sec, sizeof(buf_diag_sec), "-");
    }
    GtkWidget *label_diag_sec = gtk_label_new(buf_diag_sec);
    gtk_grid_attach(grid, label_diag_sec, order + 1, 0, 1, 1);

    // Sumas de filas
    for (int i = 0; i < order; i++) {
        char buf[16];
        if (sumasFilas != NULL) {
            snprintf(buf, sizeof(buf), "%d", sumasFilas[i]);
        } else {
            snprintf(buf, sizeof(buf), "-");
        }
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, 0, i + 1, 1, 1);
    }
    
    // Suma diagonal principal
    char buf_diag_prin[16];
    if (sumaDiagonalPrincipal != 0) {
        snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", sumaDiagonalPrincipal);
    } else {
        snprintf(buf_diag_prin, sizeof(buf_diag_prin), "-");
    }
    GtkWidget *label_diag_prin = gtk_label_new(buf_diag_prin);
    gtk_grid_attach(grid, label_diag_prin, 0, order + 1, 1, 1);

    // Celdas del cuadrado mágico
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", magicSquare[i][j]);
            GtkWidget *label = gtk_label_new(buf);
            gtk_grid_attach(grid, label, j + 1, i + 1, 1, 1);
        }
    }
    gtk_widget_show_all(GTK_WIDGET(grid));
}

void update_single_cell(GtkGrid *grid, int order, int **magicSquare, int row, int col) {
    GtkWidget *label = gtk_grid_get_child_at(grid, col + 1, row + 1);
    if (label != NULL) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", magicSquare[row][col]);
        gtk_label_set_text(GTK_LABEL(label), buf);
        
        // Opcional: resaltar la celda actual
        gtk_widget_set_name(label, "current-cell");
    }
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
    
    // Inicializar si es el primer paso
    if (!step_state.initialized) {
        printf("Inicializando paso a paso con orden %d\n", order);
        initStepState(order, stepRow, stepCol, &step_state);
        
        // Crear grid vacío
        fill_grid(grid, order, step_state.magic_square, NULL, NULL, 0, 0);
    }
    
    // Avanzar un paso
    int result = getNextStep(order, stepRow, stepCol, &step_state);
    
    if (result) {
        // Actualizar solo la celda actual
        update_single_cell(grid, order, step_state.magic_square, step_state.current_row, step_state.current_col);
        printf("Paso %d completado en (%d, %d)\n", 
               step_state.current_number, step_state.current_row, step_state.current_col);
    } else {
        printf("Cuadro mágico completado!\n");
        // Mostrar mensaje en la interfaz
        GtkLabel *status_label = GTK_LABEL(gtk_builder_get_object(builder, "IDStatusLabel"));
        if (status_label != NULL) {
            gtk_label_set_text(status_label, "Cuadro mágico completado!");
        }
    }
}

void on_finalize_clicked(GtkButton *button, gpointer user_data) {
    // Liberar memoria del modo paso a paso
    if (step_state.initialized) {
        freeStepState(&step_state);
    }
    
    GtkBuilder *builder = (GtkBuilder *)user_data;
    GtkSpinButton *spin_order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "IDOrder"));
    GtkComboBoxText *combo_form = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "IDForm"));
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "IDMatrix"));

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

    // Usar matriz local para el modo finalize
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
    
    fill_grid(grid, order, magicSquarePtr, sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria);
    free(magicSquarePtr);
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

    // Conectar señal del botón Finalize
    GtkButton *finalize_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDFinalize"));
    g_signal_connect(finalize_btn, "clicked", G_CALLBACK(on_finalize_clicked), builder);

    GtkButton *step_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDStep"));
    g_signal_connect(step_btn, "clicked", G_CALLBACK(on_step_clicked), builder);


    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    return 0;
}