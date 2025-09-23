#include <gtk/gtk.h>
#include <stdio.h>
#include "magic_square.h"

static StepState step_state = {0};

void fill_grid(GtkGrid *grid, GtkBuilder *builder, int order, int **magicSquare, int sumasFilas[order], int sumasColumnas[order], int sumaDiagonalPrincipal, int sumaDiagonalSecundaria) {
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

    // Actualizar labels de las diagonales
    GtkLabel *label_diag_prin = GTK_LABEL(gtk_builder_get_object(builder, "IDPDiag"));
    GtkLabel *label_diag_sec = GTK_LABEL(gtk_builder_get_object(builder, "IDSDiag"));
    char buf_diag_prin[16], buf_diag_sec[16];
    
    if (sumaDiagonalPrincipal != 0) {
        snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", sumaDiagonalPrincipal);
    } else {
        snprintf(buf_diag_prin, sizeof(buf_diag_prin), "-");
    }
    
    if (sumaDiagonalSecundaria != 0) {
        snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", sumaDiagonalSecundaria);
    } else {
        snprintf(buf_diag_sec, sizeof(buf_diag_sec), "-");
    }
    
    gtk_label_set_text(label_diag_prin, buf_diag_prin);
    gtk_label_set_text(label_diag_sec, buf_diag_sec);
}

void update_single_cell(GtkGrid *grid, int order, int **magicSquare, int row, int col) {
    GtkWidget *label = gtk_grid_get_child_at(grid, col + 1, row + 1);
    if (label != NULL) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", magicSquare[row][col]);
        gtk_label_set_text(GTK_LABEL(label), buf);
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
        initStepState(order, stepRow, stepCol, &step_state);
        
        // Crear grid vacío
        fill_grid(grid, builder, order, step_state.magic_square, NULL, NULL, 0, 0);
    }
    
    // Avanzar un paso
    int result = getNextStep(&step_state, stepRow, stepCol);
    
    if (result) {
        // Actualizar solo la celda actual
        update_single_cell(grid, order, step_state.magic_square, step_state.current_row, step_state.current_col);
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
    
    fill_grid(grid, builder, order, magicSquarePtr, sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria);
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

    // Conectar señales de los botones
    GtkButton *finalize_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDFinalize"));
    g_signal_connect(finalize_btn, "clicked", G_CALLBACK(on_finalize_clicked), builder);

    GtkButton *step_btn = GTK_BUTTON(gtk_builder_get_object(builder, "IDStep"));
    g_signal_connect(step_btn, "clicked", G_CALLBACK(on_step_clicked), builder);

    gtk_widget_show_all(window);
    gtk_main();

    // Liberar memoria al salir
    if (step_state.initialized) {
        freeStepState(&step_state);
    }

    g_object_unref(builder);
    return 0;
}