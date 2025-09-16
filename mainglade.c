#include <gtk/gtk.h>
#include <stdio.h>
#include "magic_square.h"

void fill_grid(GtkGrid *grid, int order, int magicSquare[order][order], int sumasFilas[order], int sumasColumnas[order], int sumaDiagonalPrincipal, int sumaDiagonalSecundaria) {
    // Limpiar el grid
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Sumas de columnas (primera fila, excepto la primera celda)
    for (int j = 0; j < order; j++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasColumnas[j]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, j + 1, 0, 1, 1);
    }
    // Suma diagonal secundaria (última celda de la primera fila)
    char buf_diag_sec[16];
    snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", sumaDiagonalSecundaria);
    GtkWidget *label_diag_sec = gtk_label_new(buf_diag_sec);
    gtk_grid_attach(grid, label_diag_sec, order, 0, 1, 1);

    // Sumas de filas (primera columna, excepto la primera celda)
    for (int i = 0; i < order; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasFilas[i]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, 0, i + 1, 1, 1);
    }
    // Suma diagonal principal (última celda de la primera columna)
    char buf_diag_prin[16];
    snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", sumaDiagonalPrincipal);
    GtkWidget *label_diag_prin = gtk_label_new(buf_diag_prin);
    gtk_grid_attach(grid, label_diag_prin, 0, order, 1, 1);

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

void on_finalize_clicked(GtkButton *button, gpointer user_data) {
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

    int magicSquare[order][order];
    int sumasFilas[order];
    int sumasColumnas[order];
    int sumaDiagonalPrincipal = 0;
    int sumaDiagonalSecundaria = 0;

    getMagicSquareStatistics(order, stepRow, stepCol, &magicSquare, sumasFilas, sumasColumnas, &sumaDiagonalPrincipal, &sumaDiagonalSecundaria);

    fill_grid(grid, order, magicSquare, sumasFilas, sumasColumnas, sumaDiagonalPrincipal, sumaDiagonalSecundaria);
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

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    return 0;
}