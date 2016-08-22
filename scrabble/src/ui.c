/*
	MIT License

	Copyright (c) 2016 Ravf, Luis Prieto

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "headers/scrabble.h"

/* ==============================================================================================
 	 VARIABLES EXTERNAS
   ============================================================================================= */

// globals.c
extern char bonus[N][N];
extern char tabla[N][N];
extern const char *player_name_1;
extern const char *player_name_2;
extern GtkWidget *atril_label[ATRIL_CHR - 1];
extern GtkWidget *index_tabla_label[4][N + 2];
extern GtkWidget *cel_tabla_label[N][N];
extern GtkWidget *cf_atril_label[ATRIL_CHR - 1];
extern int player_score[2];

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static void ui_tooltip_status(GObject *object, gboolean band);
static char *reemplazar_caracteres_especiales(char c);

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static char *unicode_letra_score(char letra);
static char set_space(char c);

/* ==============================================================================================
 	 PROCEDIMIENTOS GLOBALES
   ============================================================================================= */
/*
 * Letra removida
 * del atril
 */

void ui_atril_letra_vacia(int i, char letra)
{
	GtkWidget *label;
	label = atril_label[i];

	gtk_widget_set_name(label, "letras_del_atril_vacia");

	// ocultar tooltip
	ui_tooltip_status(G_OBJECT(label), FALSE);
}

/*
 * Nuevas letras
 * en el atril
 */

void ui_atril_nuevas_letras(char *atril)
{
	int i;
	char text[64];
	char *ptrTexto;
	size_t length;
	GtkWidget *label;

	ptrTexto = NULL;
	length   = sizeof(text);

	// insertar nuevas letras
	// en los labels
	// si la posición del atril
	// no contiene alguna letra
	// esta debe contener el
	// carácter CHR_NULL

	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		label = atril_label[i];

		if (atril[i] == CHR_NULL)
		{
			ui_atril_letra_vacia(i, atril[i]);
			continue;
		}

		ptrTexto = reemplazar_caracteres_especiales(atril[i]);
		snprintf(text, length, "%s%s%c", ptrTexto, unicode_letra_score(atril[i]), set_space(atril[i]));

		gtk_label_set_text(GTK_LABEL(label), text);
		gtk_widget_set_name(label, "letras_del_atril");

		// mostrar tooltip
		ui_tooltip_status(G_OBJECT(label), TRUE);
	}
}

/*
 * Cambiar letras
 * del atril
 */

void ui_cf_atril_copiar(char *atril)
{
	int i;
	char text[64];
	char *ptrTexto;
	size_t length;
	GtkWidget *label;

	ptrTexto = NULL;
	length   = sizeof(text);

	// insertar nuevas letras
	// en los labels
	// si la posición del atril
	// no contiene alguna letra
	// esta debe contener el
	// carácter CHR_NULL
	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		label = cf_atril_label[i];

		if (atril[i] == CHR_NULL)
		{
			gtk_widget_set_name(label, "letras_del_atril_vacia");
			continue;
		}

		ptrTexto = reemplazar_caracteres_especiales(atril[i]);
		snprintf(text, length, "%s%s%c", ptrTexto, unicode_letra_score(atril[i]), set_space(atril[i]));

		gtk_label_set_text(GTK_LABEL(label), text);
		gtk_widget_set_name(label, "letras_del_atril");
	}
}

void ui_cf_letra_atril_seleccionada(int i)
{
	GtkWidget *label;
	label = cf_atril_label[i];

	gtk_widget_set_name(label, "cf_letras_del_atril_seleccionada");
}

/*
 * Nuevo marco del
 * tablero
 */

void ui_tablero_marco(void)
{
	int j;
	char text[64];
	GtkWidget *widget;


	for (j = 0; j < N + 2; j++)
	{
		if (j > 0 && j <= N)
		{
			/*
			 * indice de columna [1 y 16]
			 */

			text[0] = (char) 65 + j - 1; // A++
			text[1] = '\0';

			// primera fila [1]
			widget = index_tabla_label[0][j];
			gtk_label_set_text(GTK_LABEL(widget), text);

			// segunda fila [16]
			widget = index_tabla_label[1][j];
			gtk_label_set_text(GTK_LABEL(widget), text);

			/*
			 * indices de fila [1 y 16]
			 */

			snprintf(text, sizeof(text), "%2d", j); // 1++

			// primera columna [1]
			widget = index_tabla_label[2][j];
			gtk_label_set_text(GTK_LABEL(widget), text);

			// ultima columna [16]
			widget = index_tabla_label[3][j];
			gtk_label_set_text(GTK_LABEL(widget), text);
		}
	}
}

/*
 *  Nueva representacion incial
 *  del tablero
 */

void ui_tablero_nuevo(void)
{
	int i, j;

	// agregar el marco
	// al tablero
	ui_tablero_marco();

	// imprimir la tabla del juego
	// la misma esta contenida dentro
	// de los indices

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			ui_tablero_letra_vacia(i, j);
}

/*
 *  Nueva letra
 *  en una determinada casilla
 *  del tablero
 */

void ui_tablero_nueva_letra(int i, int j)
{
	char text[64];
	char *ptrTexto;
	GtkWidget *widget;

	ptrTexto = NULL;
	widget   = cel_tabla_label[i][j];

	ptrTexto = reemplazar_caracteres_especiales(tabla[i][j]);
	snprintf(text, sizeof(text), "%s%s%c", ptrTexto, unicode_letra_score(tabla[i][j]), set_space(tabla[i][j]));

	gtk_label_set_text(GTK_LABEL(widget), text);
	gtk_widget_set_name(widget, "casilla_ocupada");

	// ocultar tooltip
	ui_tooltip_status(G_OBJECT(widget), FALSE);
}

/*
 *  Letra vacia
 *  en una determinada
 *  casilla del tablero
*/

void ui_tablero_letra_vacia(int i, int j)
{
	GtkWidget *widget;
	widget = cel_tabla_label[i][j];

	// verificar la posicion de la casilla

	if (i == 7 && j == 7) // casilla del centro
	{
		gtk_label_set_text(GTK_LABEL(widget), U_BLACK_STAR);
		gtk_widget_set_name(widget, "casilla_del_centro");
	}
	else
	{
		// Representación de las
		// castillas de tipo bonus

		switch(bonus[i][j])
		{
			case S_TRIPLE_PALABRA:
				gtk_label_set_text(GTK_LABEL(widget), TRIPLE_PALABRA);
				gtk_widget_set_name(widget, "casilla_triple_palabra");
				break;

			case S_DOBLE_PALABRA:
				gtk_label_set_text(GTK_LABEL(widget), DOBLE_PALABRA);
				gtk_widget_set_name(widget, "casilla_doble_palabra");
				break;

			case S_TRIPLE_LETRA:
				gtk_label_set_text(GTK_LABEL(widget), TRIPLE_LETRA);
				gtk_widget_set_name(widget, "casilla_triple_letra");
				break;

			case S_DOBLE_LETRA:
				gtk_label_set_text(GTK_LABEL(widget), DOBLE_LETRA);
				gtk_widget_set_name(widget, "casilla_doble_letra");
				break;

			default: // casilla normal
				gtk_label_set_text(GTK_LABEL(widget), NULL);
				gtk_widget_set_name(widget, "casilla_normal");
				break;
		}
	}

	// mostrar tooltip
	ui_tooltip_status(G_OBJECT(widget), TRUE);
}

/*
 * Manejo del texto de los botones
 * de acuerdo a la cantidad de jugadas
 */

void ui_cambiar_texto_label(int cjugadas)
{
	if (cjugadas > 0)
	{
		gtk_button_set_label(GTK_BUTTON(TP_JUGADA_BOTON), "Terminar jugada");
		gtk_button_set_label(GTK_BUTTON(CD_FICHAS_BOTON), "Traer fichas   ");
	}
	else
	{
		gtk_button_set_label(GTK_BUTTON(TP_JUGADA_BOTON), "Pasar jugada   ");
		gtk_button_set_label(GTK_BUTTON(CD_FICHAS_BOTON), "Cambiar fichas ");
	}
}

/* Manejar el texto de la
 * barra de estado:
 * cantidad de letras en el pozo,
 * nombre del jugador 1,
 * nombre del jugador 2
 */

void ui_change_text_statusbar_name_score(void)
{
	char text[256];

	snprintf(text, sizeof(text), "%s %s: %d", SCORE_JUGADOR, player_name_1, player_score[0]);
	gtk_label_set_text(GTK_LABEL(LABEL_SCORE_JUGADOR_1), text);

	snprintf(text, sizeof(text), "%s %s: %d", SCORE_JUGADOR, player_name_2, player_score[1]);
	gtk_label_set_text(GTK_LABEL(LABEL_SCORE_JUGADOR_2), text);
}

void ui_change_text_pozo_nletras(int n)
{
	char text[256];

	snprintf(text, sizeof(text), "Letras disponibles en el pozo: %d", n);
	gtk_label_set_text(GTK_LABEL(LABEL_LETRAL_POZO), text);
}

void ui_change_text_turno_actual(int turno)
{
	char text[256];

	strncpy(text, "Turno actual: ", sizeof(text));

	if (turno == 0)
		strncat(text, player_name_1, sizeof(text));
	else if (turno == 1)
		strncat(text, player_name_2, sizeof(text));
	else
		strncat(text, "Ninguno", sizeof(text));

	gtk_label_set_text(GTK_LABEL(LABEL_TURNO_ACTUAL), text);
};

// Function to open a dialog box with a message
void quick_message(gchar *message, GtkWindow *parent)
{
	 GtkWidget *dialog, *label, *content_area;
	 GtkDialogFlags flags;

	 // Create the widgets
	 flags = GTK_DIALOG_MODAL;
	 dialog = gtk_dialog_new_with_buttons ("Scrabble",
										   GTK_WINDOW(MAIN_WINDOW),
										   flags,
										   "_OK",
										   GTK_RESPONSE_NONE,
										   NULL);

	 content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
	 label = gtk_label_new(message);

	 gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	 gtk_window_set_skip_taskbar_hint(GTK_WINDOW(dialog), TRUE);
	 gtk_window_set_skip_pager_hint(GTK_WINDOW(dialog), TRUE);
	 gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);

	 // Ensure that the dialog box is destroyed when the user responds
	 g_signal_connect_swapped (dialog,
							   "response",
							   G_CALLBACK (gtk_widget_destroy),
							   dialog);

	 // Add the label, and show everything we’ve added
	 gtk_container_add(GTK_CONTAINER(content_area), label);
	 gtk_widget_show_all(dialog);
}

void ui_msg_turno(int turno, int first)
{
	char text[128];

	if (turno == 0)
		strncpy(text, player_name_1, sizeof(text));
	else
		strncpy(text, player_name_2, sizeof(text));

	if (first)
		strncat(text, " comienza el juego !", sizeof(text));
	else
	{
		if (verificar_jugada_pasada())
			snprintf(text,  sizeof(text), "%s paso su jugada !", (turno == 0) ? player_name_2 : player_name_1);
		else
			strncat(text, " es tu turno !", sizeof(text));
	}
	quick_message(text, MAIN_WINDOW);
}

void ui_main_widgets_status(int status, int statusbar)
{
	void (*ui_gtk_show_hide)(GtkWidget *widget);
	ui_gtk_show_hide = gtk_widget_hide;

	if (status == 1)
		ui_gtk_show_hide = gtk_widget_show;

	ui_gtk_show_hide(GTK_WIDGET(SEPARADOR_1));
	ui_gtk_show_hide(GTK_WIDGET(OPCIONES_GRID));

	if (statusbar)
	{
		ui_gtk_show_hide(GTK_WIDGET(SEPARADOR_2));
		ui_gtk_show_hide(GTK_WIDGET(STATUSBAR));
	}
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

/*
 * Cambia el estado
 * de los tooltips
 * de los widgets
 */

static void ui_tooltip_status(GObject *object, gboolean band)
{
	g_object_set(object, "has-tooltip", band, NULL);
}

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

/*
 * Representación de
 * los valores de
 * las letras del juego
 */


static char *unicode_letra_score(char letra)
{
	int c;
	static char text[15];

	text[0] = '\0';
	c = obtener_valor_letra(letra);

	switch (c)
	{
		case 0:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_ZERO);
			break;

		case 1:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_ONE);
			break;

		case 2:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_TWO);
			break;

		case 3:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_THREE);
			break;

		case 4:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_FOUR);
			break;

		case 5:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_FIVE);
			break;

		case 8:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ZERO, U_SUBSCRIPT_EIGHT);
			break;

		case 10:
			snprintf(text, sizeof(text), "%s%s", U_SUBSCRIPT_ONE, U_SUBSCRIPT_ZERO);
			break;
	}

	return text;
}

/*
 * Reemplaza el símbolo utilizado
 * para representar el carácter especial
 * por el carácter especial
 */

static char *reemplazar_caracteres_especiales(char c)
{
	static char formato[64];

	switch (c)
	{
		case CHR_RR:
			snprintf(formato, sizeof(formato), "RR");
			break;

		case CHR_LL:
			snprintf(formato, sizeof(formato), "LL");
			break;

		case CHR_CH:
			snprintf(formato, sizeof(formato), "CH");
			break;

		case CHR_NT:
			snprintf(formato, sizeof(formato), "%s", U_LATIN_CAPITAL_LETTER_N_TILDE);
			break;

		case COMODIN:
			snprintf(formato, sizeof(formato), "   ");
			break;

		default:
			formato[0] = c;
			formato[1] = '\0';
			break;
	}

	return formato;
}

// asignado de espacios
// que omite caracteres dobles

static char set_space(char c)
{
	char space;
	space = 0;

	if
	(
			c != CHR_RR
		 && c != CHR_LL
		 && c != CHR_CH
		 && c != COMODIN
		 && c != CHR_NULL
	)
	space = ' ';

	return space;
}
