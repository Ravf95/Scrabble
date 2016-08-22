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
 	 TIPOS DE DATOS DEFINIDOS POR EL USUARIO
   ============================================================================================= */

typedef enum
{
	CASILLA_ATRIL_TABLERO_PRESIONADA,
	BOTON_TERMINAR_PASAR_JUGADA,
	BOTON_TRAER_CAMBIAR_FICHAS,
	ACEPTAR_NUEVO_JUEGO
} aciones_del_juego;

/* ==============================================================================================
 	 VARIABLES LOCALES DEL ARCHIVO
   ============================================================================================= */

static char atril[2][ATRIL_CHR] =
{
	{ ATRIL_CHR_DEFAULT },
	{ ATRIL_CHR_DEFAULT }
};

static int tipo_jugador[2];
static int cturno;
static int cjugadas; // contador de jugada
static int status = j_solicitar_posc_letra_atril;
static int turno;
static int posplayed[ATRIL_CHR - 1][4];	// x, y, (int)posatril, (int)atril letra

static char cf_atril[ATRIL_CHR];
static gboolean cf_atril_changed;

/* ==============================================================================================
 	 VARIABLES EXTERNAS
   ============================================================================================= */

// globals.c
extern const char *player_name_1;
extern const char *player_name_2;
extern const char *g_user_home_dir;

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static void accion_casilla_atril_tablero_presionada(GObject *object);
static void accion_aceptar_nuevo_juego(void);
static void obtener_orden_jugada_nuevo_juego(void);
static void obtener_modo_juego(void);

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static gboolean verificar_valores_window_nuevo_juego(void);

/* ==============================================================================================
 	 PROCEDIMIENTOS GLOBALES
   ============================================================================================= */

/*
 * Ventana: main window
 */

void tablero_press_callback(GObject *object, GdkEvent *event, gpointer data)
{
	if (status == j_game_end)
		return;

	if (tipo_jugador[turno] == 1)
		return;

	accion_casilla_atril_tablero_presionada(object);
}

void atril_press_callback(GObject *object, GdkEvent *event, gpointer data)
{
	if (status == j_game_end)
		return;

	if (tipo_jugador[turno] == 1)
		return;

	accion_casilla_atril_tablero_presionada(object);
}

void boton_terminar_pasar_jugada_press_callback(GtkButton *button, gpointer data)
{
	// mensaje de error
	static char *ptrMsg;

	if (status == j_game_end)
		return;

	if (tipo_jugador[turno] == 1)
		return;

	// finalizar jugada del jugador
	status = j_tabla_terminar_jugada;

	// iterar hasta
	// que se le solicite al
	// jugador volver a seleccionar
	// casillas del atril/tablero
	while
	(
			status != j_solicitar_posc_letra_atril &&
			status != j_game_end
	)
	{
		desplazar_piezas(&status, atril[turno], posplayed, &cjugadas, cturno);
		verificar_piezas_desplazadas(&status, atril, &cturno, &cjugadas, posplayed, &turno);

		// ¿algun error?
		if (get_err_status() != -1)
		{
			ptrMsg = get_err_msg_code();
			quick_message(ptrMsg, MAIN_WINDOW);
			set_err_msg_code(-1, 0);
		}
	}
}

void boton_traer_cambiar_fichas_press_callback(GtkButton *button, gpointer data)
{
	// mensaje de error
	static char *ptrMsg;

	if (status == j_game_end)
		return;

	if (tipo_jugador[turno] == 1)
		return;

	// cambiar fichas
	if (cjugadas == 0)
	{
		if (cturno == 0)
		{
			quick_message(D_PRIMER_TURNO, MAIN_WINDOW);
			return;
		}

		cf_atril_changed = FALSE;
		ui_cf_atril_copiar(atril[turno]);
		gtk_widget_show_all(GTK_WIDGET(CAMBIAR_FICHAS_WINDOW));
	}
	// traer fichas
	else
	{
		// si el jugador desplazo una ficha
		// su contador está en
		// ficha desplazada + 1
		// se resta -1 para obtener la
		// cantidad real, si no desplazo
		// ninguna ficha, tiene -1 en el contador esto
		// no ocasiona ningún error por la condición
		// lógica de las funciones que se llaman
		// en desplazar piezas
		cjugadas -= 1;

		status = j_tabla_devolver_piezas_atril;
		desplazar_piezas(&status, atril[turno], posplayed, &cjugadas, cturno);

		// mensaje
		quick_message(D_LETRAS_DEVUELTAS, MAIN_WINDOW);
	}

	// ¿algún error?
	if (get_err_status() != -1)
	{
		ptrMsg = get_err_msg_code();
		quick_message(ptrMsg, MAIN_WINDOW);
		set_err_msg_code(-1, 0);
	}
}

void menuitem_nuevo_juego_press_callback(GtkMenuItem *item, gpointer data)
{
	if (!gtk_window_is_active(GTK_WINDOW(MAIN_WINDOW)))
		return;

	if (player_name_1 == NULL && player_name_2 == NULL)
	{
		gtk_widget_show_all(GTK_WIDGET(NUEVO_JUEGO_WINDOW));
	}
	else
	{
		char file[PATH_MAX];
		snprintf(file, sizeof(file), "%s/scrabble/scrabble", g_user_home_dir);

		execl(file, "", 0);
		gtk_main_quit();
	}
}

void menuitem_como_jugar_press_callback(GtkMenuItem *item, gpointer data)
{
	if (!gtk_window_is_active(GTK_WINDOW(MAIN_WINDOW)))
		return;

	gboolean view;
	GdkWindow *gdk_window;

	char file[PATH_MAX];
	snprintf(file, sizeof(file), "file://%s%s", g_user_home_dir, HTML_HELP_FILE);

	view = gtk_show_uri
	(
			gtk_widget_get_screen(GTK_WIDGET(item)),
			file,
			gtk_get_current_event_time(),
			NULL
	);

	if (!view)
	{
		quick_message(P_SHOW_HTML_HELP_PAGE, MAIN_WINDOW);
		return;
	}

	gdk_window = gtk_widget_get_window(GTK_WIDGET(MAIN_WINDOW));
	gdk_window_set_cursor(gdk_window, NULL);
}

void menuitem_acerca_de_press_callback(GtkMenuItem *item, gpointer data)
{
	if (!gtk_window_is_active(GTK_WINDOW(MAIN_WINDOW)))
		return;

	gtk_dialog_run(GTK_DIALOG(ACERCA_DE_WINDOW));
	gtk_widget_hide(GTK_WIDGET(ACERCA_DE_WINDOW));
}

/*
 * Ventana: nuevo juego
 */

void window_nuevo_juego_salir(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_widget_hide_on_delete(GTK_WIDGET(NUEVO_JUEGO_WINDOW));
}

void boton_cancelar_window_nuevo_juego(GtkButton *button, gpointer data)
{
	gtk_widget_hide(GTK_WIDGET(NUEVO_JUEGO_WINDOW));
}

void boton_aceptar_window_nuevo_juego(GtkButton *button, gpointer data)
{
	if (verificar_valores_window_nuevo_juego())
	{
		gtk_widget_hide(GTK_WIDGET(NUEVO_JUEGO_WINDOW));
		accion_aceptar_nuevo_juego();
		ui_main_widgets_status(1, 1);
	}
}

/*
 * Ventana: comodin
 */

void window_comodin_salir(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_widget_hide_on_delete(GTK_WIDGET(COMODIN_WINDOW));
}

void boton_aceptar_comodin_press_callback(GtkButton *button, gpointer data)
{
	int index, atril_index;
	index = gtk_combo_box_get_active(GTK_COMBO_BOX(COMBO_BOX_LETRAS_COMODIN));

	if (index == -1)
		return;

	atril_index = posplayed[cjugadas][2];
	atril[turno][atril_index] = obtener_letra_pozo_index(index);

	ui_atril_nuevas_letras(atril[turno]);
	gtk_widget_hide(GTK_WIDGET(COMODIN_WINDOW));
}

void boton_cancelar_comodin_press_callback(GtkButton *button, gpointer data)
{
	gtk_widget_hide(GTK_WIDGET(COMODIN_WINDOW));
}

/*
 * Ventana: cambiar fichas
 */

void window_cambiar_fichas_salir(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_widget_hide_on_delete(GTK_WIDGET(CAMBIAR_FICHAS_WINDOW));
}

void boton_aceptar_cambiar_fichas_press_callback(GtkButton *button, gpointer data)
{
	if (!cf_atril_changed)
	{
		quick_message(D_SELECCIONAR_CAMBIAR_FICHAS, CAMBIAR_FICHAS_WINDOW);
		return;
	}

	gtk_widget_hide(GTK_WIDGET(CAMBIAR_FICHAS_WINDOW));
	cambiar_letras_atril(cf_atril, atril[turno]);

	status = j_game_terminar_turno;
	contar_jugada_sin_puntaje(cturno);
	verificar_piezas_desplazadas(&status, atril, &cturno, &cjugadas, posplayed, &turno);
}

void boton_cancelar_cambiar_fichas_press_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_widget_hide(GTK_WIDGET(CAMBIAR_FICHAS_WINDOW));
}

void cambiar_letras_atril_press_callback(GObject *object, GdkEvent *event, gpointer data)
{
	unsigned int x;
	x = GPOINTER_TO_INT(g_object_get_data(object, "x_cf_atril"));

	if (atril[turno][x] == CHR_NULL)
		return;

	cf_atril[x] = CHR_NULL;
	cf_atril_changed = TRUE;
	ui_cf_letra_atril_seleccionada(x);
}

int obtener_tipo_jugador_turno_actual(void)
{
	return tipo_jugador[turno];
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static void accion_casilla_atril_tablero_presionada(GObject *object)
{
	// obtener las posiciones del atril
	// o del tablero mediante las casillas
	// del juego presionadas
	solicitar_datos(&status, atril[turno], posplayed, cjugadas, object);

	// iterar hasta el
	// desplazamiento de fichas,
	// salir del ciclo cada
	// vez que se solicite al jugador
	// posiciones x/y del tablero o atril
	while
	(
		   status != j_tabla_terminar_jugada
		&& status != j_solicitar_posc_letra_atril
		&& status != j_solicitar_posc_tabla_x_y
	)
	{
		verficar_datos_ingresados(&status, atril[turno], posplayed, cjugadas);
		desplazar_piezas(&status, atril[turno], posplayed, &cjugadas, -1);
	}
}

static void accion_aceptar_nuevo_juego(void)
{
	crear_nuevo_archivo_registro();

	repartir_letras_atril(atril[0]);
	repartir_letras_atril(atril[1]);

	// gtk

	// si la ia comienza el juego
	// mostrar el atril del jugador
	if (tipo_jugador[turno] == 1)
		ui_atril_nuevas_letras(atril[0]);
	else
		ui_atril_nuevas_letras(atril[turno]);

	ui_change_text_statusbar_name_score();
	ui_change_text_turno_actual(turno);

	if (tipo_jugador[turno] == 0)
		ui_msg_turno(turno, 1);

	// ¿la ia comienza el juego?
	if (tipo_jugador[turno] == 1)
		ia_jugar(atril, &cturno, &turno, &cjugadas, posplayed, &status);
}

static void obtener_modo_juego(void)
{
	// 0 = Jugador
	// 1 = Computadora

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RADIO_BUTTON_MODO_JVSJ)))
	{
		tipo_jugador[0] = 0;
		tipo_jugador[1] = 0;
		return;
	}

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RADIO_BUTTON_MODO_JVSC)))
	{
		tipo_jugador[0] = 0;
		tipo_jugador[1] = 1;
		return;
	}
}

static void obtener_orden_jugada_nuevo_juego(void)
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RADIO_BUTTON_ORDEN_J1)))
	{
		turno = 0;
		return;
	}

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RADIO_BUTTON_ORDEN_J2)))
	{
		turno = 1;
		return;
	}

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RADIO_BUTTON_ORDEN_ALEATORIO)))
	{
		turno = rand() % 2;
		return;
	}
}

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static gboolean verificar_valores_window_nuevo_juego(void)
{
	const char *entry1 = gtk_entry_get_text(GTK_ENTRY(ENTRY_PLAYER_NAME_1));
	const char *entry2 = gtk_entry_get_text(GTK_ENTRY(ENTRY_PLAYER_NAME_2));

	if (entry1[0] == '\0')
	{
		quick_message(CAMPO_NOMBRE_JUGADOR_1_VACIO, NUEVO_JUEGO_WINDOW);
		return FALSE;
	}

	if (entry2[0] == '\0')
	{
		quick_message(CAMPO_NOMBRE_JUGADOR_2_VACIO, NUEVO_JUEGO_WINDOW);
		return FALSE;
	}

	if (strcmp(entry1, entry2) == 0)
	{
		quick_message(CAMPO_NOMBRES_IGUALES, NUEVO_JUEGO_WINDOW);
		return FALSE;
	}

	player_name_1 = entry1;
	player_name_2 = entry2;

	obtener_modo_juego();
	obtener_orden_jugada_nuevo_juego();

	return TRUE;
}
