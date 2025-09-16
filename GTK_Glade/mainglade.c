#include <gtk/gtk.h>
#include "source/magic_square.h"

void fill_grid(GtkGrid *grid, int order, int magicSquare[order][order], int sumasFilas[order], int sumasColumnas[order], int sumaDiagonalPrincipal, int sumaDiagonalSecundaria) {
    // Limpiar el grid
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Primera fila: sumas de columnas y suma diagonal secundaria
    for (int j = 0; j < order; j++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasColumnas[j]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, j + 1, 0, 1, 1);
    }
    // Última celda de la primera fila: suma diagonal secundaria
    char buf_diag_sec[16];
    snprintf(buf_diag_sec, sizeof(buf_diag_sec), "%d", sumaDiagonalSecundaria);
    GtkWidget *label_diag_sec = gtk_label_new(buf_diag_sec);
    gtk_grid_attach(grid, label_diag_sec, order, 0, 1, 1);

    // Primera columna: sumas de filas y suma diagonal principal
    for (int i = 0; i < order; i++) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", sumasFilas[i]);
        GtkWidget *label = gtk_label_new(buf);
        gtk_grid_attach(grid, label, 0, i + 1, 1, 1);
    }
    // Última celda de la primera columna: suma diagonal principal
    char buf_diag_prin[16];
    snprintf(buf_diag_prin, sizeof(buf_diag_prin), "%d", sumaDiagonalPrincipal);
    GtkWidget *label_diag_prin = gtk_label_new(buf_diag_prin);
    gtk_grid_attach(grid, label_diag_prin, 0, order, 1, 1);

    // Celdas de la matriz
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

// En tu callback de Finalize:
void on_finalize_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = (GtkBuilder *)user_data;

    GtkSpinButton *spin_order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "IDOrder"));
    GtkComboBoxText *combo_form = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "IDForm"));
    GtkGrid *grid = GTK_GRID(gtk_builder_get_object(builder, "IDMatrix"));

    gtk_widget_set_size_request(GTK_WIDGET(grid), 300, 300);

    // Prueba: agrega un label "TEST" en la celda (0,0)
    GtkWidget *test_label = gtk_label_new("TEST");
    gtk_grid_attach(grid, test_label, 0, 0, 1, 1);
    gtk_widget_show_all(GTK_WIDGET(grid));

    int order = gtk_spin_button_get_value_as_int(spin_order);

    int stepRow = -1, stepCol = 1;
    const gchar *form = gtk_combo_box_text_get_active_text(combo_form);
    if (form) {
        if (g_strcmp0(form, "arriba-derecha") == 0) { stepRow = -1; stepCol = 1; }
        else if (g_strcmp0(form, "arriba-izquierda") == 0) { stepRow = -1; stepCol = -1; }
        else if (g_strcmp0(form, "abajo-derecha") == 0) { stepRow = 1; stepCol = 1; }
        else if (g_strcmp0(form, "abajo-izquierda") == 0) { stepRow = 1; stepCol = -1; }
        else if (g_strcmp0(form, "caballo") == 0) { stepRow = 2; stepCol = 1; }
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

    builder = gtk_builder_new_from_file("GTK_Glade/GUI_MagicSquare.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "IDWindow"));

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(builder);
    return 0;
}