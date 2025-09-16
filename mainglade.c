#include <gtk/gtk.h>
#include <stdio.h>
#include "magic_square.h"

// Callback para el botón Finalize
void on_finalize_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = (GtkBuilder *)user_data;

    GtkSpinButton *spin_order = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "IDOrder"));
    GtkComboBoxText *combo_form = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "IDForm"));

    int order = gtk_spin_button_get_value_as_int(spin_order);

    // Obtener la forma seleccionada
    int stepRow = -1, stepCol = 1; // valores por defecto
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

    printf("Matriz generada:\n");
    printMatrix(order, &magicSquare);
    printf("Sumas de filas:\n");
    printVector(order, sumasFilas);
    printf("Sumas de columnas:\n");
    printVector(order, sumasColumnas);
    printf("Suma diagonal principal: %d\n", sumaDiagonalPrincipal);
    printf("Suma diagonal secundaria: %d\n", sumaDiagonalSecundaria);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

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