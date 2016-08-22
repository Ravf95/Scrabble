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
extern GtkWidget *cel_tabla_label[N][N];
extern GtkWidget *index_tabla_label[4][N + 2];
extern GtkWidget *atril_label[ATRIL_CHR - 1];
extern GtkWidget *cf_atril_label[ATRIL_CHR - 1];

/* ==============================================================================================
 	 VARIABLES GLOBALES DEL ARCHIVO
   ============================================================================================= */

static GtkBuilder 		*builder;
static GtkCssProvider 	*css;
static GdkDisplay		*display;
static GdkScreen 		*screen;
static GtkWidget 		*event_box_tabla[N][N];
static GtkWidget		*event_box_atril[ATRIL_CHR - 1];
static GtkWidget		*event_box_cf_atril[ATRIL_CHR - 1];

// identificador de los objetos
// a manipular del builder
static const char WidgetID[GTK_BUILD_N_OBJECTS][32] =
{
		{ "window_init" 								},
		{ "grid_game_table" 							},
		{ "grid_opciones_tabla" 						},
		{ "button_pasar_terminar_jugada" 				},
		{ "button_cambiar_devolver_fichas" 				},
		{ "menu_item_nuevo_juego" 						},
		{ "window_nuevo_juego" 							},
		{ "button_cancelar_nj" 							},
		{ "button_aceptar_nj" 							},
		{ "entry_player_name_1"							},
		{ "entry_player_name_2"							},
		{ "label_letras_pozo" 							},
		{ "label_score_jugador_1" 						},
		{ "label_score_jugador_2"						},
		{ "statusbar"									},
		{ "separator1"									},
		{ "separator2"									},
		{ "ayuda_item_como_jugar" 						},
		{ "ayuda_item_acerca_de"						},
		{ "aboutdialog"									},
		{ "menu_item_salir"								},
		{ "label_letras_pozo"							},
		{ "label_turno_jugador" 						},
		{ "window_comodin" 								},
		{ "comboboxtext_letras"							},
		{ "button_comodin_aceptar" 						},
		{ "button_comodin_cancelar" 					},
		{ "window_cambiar_fichas"						},
		{ "box_atril_cambiar_fichas"					},
		{ "button_aceptar_cf"							},
		{ "button_cancelar_cf"							},
		{ "radiobutton_orden_j_1"						},
		{ "radiobutton_orden_j_2"						},
		{ "radiobutton_orden_j_aleatorio"				},
		{ "radiobutton_modo_juego_jvsj"					},
		{ "radiobutton_modo_juego_jvsc"					}
};

static void *widgetList[GTK_BUILD_N_OBJECTS];

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static void administrar_widgets_builder(void);
static void ui_juego_nuevos_widgets(void);
static void ui_juego_widgets_propiedades(void);
static void ui_juego_signals(void);
static void ui_css_load(void);

/* ==============================================================================================
 	 FUNCIONES GLOBALES
   ============================================================================================= */

int start_game_gtkgui(void)
{
	gtk_init(NULL, NULL); // inicializar gtk

	/*
	 * [WIDGETS]
	 */

	administrar_widgets_builder();
	ui_juego_nuevos_widgets();

	/*
	 * [PROPIEDADES]
	*/

	ui_juego_widgets_propiedades();

	/*
	 * [TABLERO DEL JUEGO]
	 */

	ui_tablero_nuevo();

	/*
	 * [SIGNALS]
	 */

	ui_juego_signals();

	/*
	 * [CSS]
	 */

	ui_css_load();

	// mostrar todos los widgets
	// contenidos en la ventana
	gtk_widget_show_all(GTK_WIDGET(MAIN_WINDOW));

	ui_main_widgets_status(0, 1);

	// iterar hasta el gtk_quit
	gtk_main();

	gtk_widget_destroy(GTK_WIDGET(CAMBIAR_FICHAS_WINDOW));
	gtk_widget_destroy(GTK_WIDGET(NUEVO_JUEGO_WINDOW));
	gtk_widget_destroy(GTK_WIDGET(ACERCA_DE_WINDOW));
	gtk_widget_destroy(GTK_WIDGET(COMODIN_WINDOW));

	return 0;
}

void *get_game_widget_with_id(unsigned int id)
{
	if (id < 0 && id >= GTK_BUILD_N_OBJECTS)
	{
		g_printf("ERROR: gtkgui.c > f:get_game_widget_with_id : indice fuera de rango (%d).\n", id);
		exit(-1);
	}

	return widgetList[id];
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

/*
 * Crear/Modificar
 * widgets antes de la
 * llamada del gtk_main()
 */

// Widgets:
// - grid de tablero de juego
// - grid de opciones
// - botones: terminar_pasar_jugada / cambiar_traer_fichas
// - labels del tablero del juego
// - labels del atril
// - eventos: grid del tablero de juego, grid de opciones
static void ui_juego_nuevos_widgets(void)
{
	int i, j;

	// [Celdas del tablero del juego]
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			// nueva etiqueta por cada celda
			cel_tabla_label[i][j] = gtk_label_new(NULL);
			assert(cel_tabla_label[i][j]);

			// nuevo evento por cada celda
			event_box_tabla[i][j] = gtk_event_box_new();
			assert(event_box_tabla[i][j]);

			gtk_container_add(GTK_CONTAINER(event_box_tabla[i][j]), cel_tabla_label[i][j]);

			// agregar el label contenido
			// en el event box
			// al grid de la tabla
			gtk_grid_attach(GTK_GRID(TABLA_GRID), event_box_tabla[i][j], j + 1, i + 1, 1, 1);
		}
	}

	// [Celdas de tipo indice del tablero el juego]
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < N + 2; j++)
		{
			index_tabla_label[i][j] = gtk_label_new(NULL);
			assert(index_tabla_label[i][j]);
		}
	}

	// celdas de tipo índice

	// agregar el label
	// al grid de la tabla
	// en una determinada posicion x e y

	// primera fila [1]
	for (j = 0; j < N + 2; j++)
		gtk_grid_attach(GTK_GRID(TABLA_GRID), index_tabla_label[0][j], j, 0, 1, 1);

	// ultima fila [16]
	for (j = 0; j < N + 2; j++)
		gtk_grid_attach(GTK_GRID(TABLA_GRID), index_tabla_label[1][j], j, 16, 1, 1);

	// primera columna [1]
	for (j = 0; j < N + 2; j++)
		gtk_grid_attach(GTK_GRID(TABLA_GRID), index_tabla_label[2][j], 0, j, 1, 1);

	// ultima columna [16]
	for (j = 0; j < N + 2; j++)
		gtk_grid_attach(GTK_GRID(TABLA_GRID), index_tabla_label[3][j], 16, j, 1, 1);

	// -----------------------

	// [Letras del atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		atril_label[i] = gtk_label_new(NULL);
		assert(atril_label[i]);

		event_box_atril[i] = gtk_event_box_new();
		assert(event_box_atril[i]);

		/* Cada label esta contenido
		 * dentro de un event box
		 */

		gtk_container_add(GTK_CONTAINER(event_box_atril[i]), atril_label[i]);

		// agregar el label contenido
		// en el event box
		// al grid
		gtk_grid_attach(GTK_GRID(OPCIONES_GRID), event_box_atril[i], i, 1, 1, 1);
	}

	// [Cambiar fichas atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		cf_atril_label[i] = gtk_label_new(NULL);
		assert(cf_atril_label[i]);

		event_box_cf_atril[i] = gtk_event_box_new();
		assert(event_box_cf_atril[i]);

		gtk_container_add(GTK_CONTAINER(event_box_cf_atril[i]), cf_atril_label[i]);
		gtk_box_pack_start(GTK_BOX(BOX_ATRIL_CAMBIAR_FICHAS), event_box_cf_atril[i], TRUE, TRUE, 2);
	}
}

/*
 * Asignar/Modificar las propiedades
 * de los widgets antes de la llamada
 * al gtk_main()
 */

// Widgets afectados
// - ventana principal
// - grid del tablero de juego
// - grid de opciones
static void ui_juego_widgets_propiedades(void)
{
	int i, j;

	// [Celdas del tablero del juego]
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			// A cada objeto dentro del contenedor
			// event_box se le asigna
			// los valores x e y de las posiciones
			// de la celda del tablero
			g_object_set_data(G_OBJECT(event_box_tabla[i][j]), "x_tablero", GINT_TO_POINTER(i + 1));
			g_object_set_data(G_OBJECT(event_box_tabla[i][j]), "y_tablero", GINT_TO_POINTER(j + 1));

			// texto de ayuda
			gtk_widget_set_tooltip_text(cel_tabla_label[i][j], TOOLTIP_CELDA_VACIA_TABLERO);

			// evento press button
			gtk_widget_set_events(GTK_WIDGET(event_box_tabla[i][j]), GDK_BUTTON_PRESS_MASK);
		}
	}

	// [Celdas de tipo indice del tablero el juego]
	for (i = 0; i < 4; i++)
		for (j = 0; j < N + 2; j++)
			gtk_widget_set_name(index_tabla_label[i][j], "casilla_indice");

	// [Letras del atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		// cada event box tiene
		// de las casillas del atril
		// tiene asignado una posición
		g_object_set_data(G_OBJECT(event_box_atril[i]), "x_atril", GINT_TO_POINTER(i + 1));

		// texto de ayuda
		gtk_widget_set_tooltip_text(atril_label[i], TOOLTIP_CELDA_VACIA_ATRIL);

		// evento press button
		gtk_widget_set_events(GTK_WIDGET(event_box_atril[i]), GDK_BUTTON_PRESS_MASK);
	}

	// [Cambiar fichas atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		g_object_set_data(G_OBJECT(event_box_cf_atril[i]), "x_cf_atril", GINT_TO_POINTER(i));
		gtk_widget_set_events(GTK_WIDGET(event_box_cf_atril[i]), GDK_BUTTON_PRESS_MASK);
	}

	// [Tablero del juego]
	gtk_grid_insert_row(GTK_GRID(TABLA_GRID), N + 2); // + 2 para las filas de tipo indice
	gtk_grid_insert_column(GTK_GRID(TABLA_GRID), N + 2); // + 2 para las columnas de tipo indice

	// [Tablero del opciones]
	gtk_grid_set_row_spacing(GTK_GRID(OPCIONES_GRID), 0);
	gtk_grid_set_column_spacing(GTK_GRID(OPCIONES_GRID), 3);

	// [Ventanas heredadas]
	gtk_window_set_transient_for(GTK_WINDOW(CAMBIAR_FICHAS_WINDOW), GTK_WINDOW(MAIN_WINDOW));
	gtk_window_set_transient_for(GTK_WINDOW(NUEVO_JUEGO_WINDOW), GTK_WINDOW(MAIN_WINDOW));
	gtk_window_set_transient_for(GTK_WINDOW(ACERCA_DE_WINDOW), GTK_WINDOW(MAIN_WINDOW));
}

// Widgets afectados:
// - ventana principal
// - grid del tablero del juego
// - grid de opciones
// - labels del atril
// - botones: terminar_pasar_jugada / cambiar_traer_fichas
// - items del barmenu
static void ui_juego_signals(void)
{
	int i, j;

	// callbacks

	/* Contenedor principal
	 * Ventana: main_window
	 */

	// ventana
	g_signal_connect
	(
			G_OBJECT(MAIN_WINDOW),
			"destroy",
			G_CALLBACK(gtk_main_quit),
			NULL
	);

	// [Celdas del tablero del juego]
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			g_signal_connect
			(
					G_OBJECT(event_box_tabla[i][j]),
					"button_press_event",
					G_CALLBACK(tablero_press_callback),
					NULL
			);

	// [Letras del atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
		g_signal_connect
		(
				G_OBJECT(event_box_atril[i]),
				"button_press_event",
				G_CALLBACK(atril_press_callback),
				NULL
		);

	// [Boton terminar/pasar jugada]
	g_signal_connect
	(
			G_OBJECT(TP_JUGADA_BOTON),
			"clicked",
			G_CALLBACK(boton_terminar_pasar_jugada_press_callback),
			NULL
	);

	// [Boton traer/cambiar fichas]
	g_signal_connect
	(
			G_OBJECT(CD_FICHAS_BOTON),
			"clicked",
			G_CALLBACK(boton_traer_cambiar_fichas_press_callback),
			NULL
	);

	// [BARMENU]

	// [Menu] item : nuevo juego
	g_signal_connect
	(
			G_OBJECT(NUEVO_JUEGO_MEMU_ITEM),
			"activate",
			G_CALLBACK(menuitem_nuevo_juego_press_callback),
			NULL
	);

	// [Menu] item : como jugar
	g_signal_connect
	(
			G_OBJECT(BOTON_COMO_JUGAR),
			"activate",
			G_CALLBACK(menuitem_como_jugar_press_callback),
			NULL
	);

	// [Menu] item : salir
	g_signal_connect
	(
			G_OBJECT(BOTON_SALIR),
			"activate",
			G_CALLBACK(gtk_main_quit),
			NULL
	);

	// [Menu] item : como jugar
	g_signal_connect
	(
			G_OBJECT(BOTON_ACERCA_DE),
			"activate",
			G_CALLBACK(menuitem_acerca_de_press_callback),
			NULL
	);

	/* Contenedor principal
	 * Ventana: window_2
	 */

	// ventana : window_2
	g_signal_connect
	(
			G_OBJECT(NUEVO_JUEGO_WINDOW),
			"delete-event",
			G_CALLBACK(window_nuevo_juego_salir),
			NULL
	);

	// boton cancelar
	g_signal_connect
	(
			G_OBJECT(BOTON_CANCELAR_WINDOW_NUEVO_J),
			"clicked",
			G_CALLBACK(boton_cancelar_window_nuevo_juego),
			NULL
	);

	// boton aceptar
	g_signal_connect
	(
			G_OBJECT(BOTON_ACEPTAR_WINDOW_NUEVO_J),
			"clicked",
			G_CALLBACK(boton_aceptar_window_nuevo_juego),
			NULL
	);

	/* Contenedor principal
	 * Ventana: comodin
	 */

	g_signal_connect
	(
			G_OBJECT(COMODIN_WINDOW),
			"delete-event",
			G_CALLBACK(window_comodin_salir),
			NULL
	);

	g_signal_connect
	(
			G_OBJECT(BUTTON_COMODIN_ACEPTAR),
			"clicked",
			G_CALLBACK(boton_aceptar_comodin_press_callback),
			NULL
	);

	g_signal_connect
	(
			G_OBJECT(BUTTON_COMODIN_CANCELAR),
			"clicked",
			G_CALLBACK(boton_cancelar_comodin_press_callback),
			NULL
	);

	/* Contenedor principal
	 * Ventana: cambiar fichas
	 */

	g_signal_connect
	(
			G_OBJECT(CAMBIAR_FICHAS_WINDOW),
			"delete-event",
			G_CALLBACK(window_cambiar_fichas_salir),
			NULL
	);

	g_signal_connect
	(
			G_OBJECT(BUTTON_ACEPTAR_CAMBIAR_FICHAS),
			"clicked",
			G_CALLBACK(boton_aceptar_cambiar_fichas_press_callback),
			NULL
	);

	g_signal_connect
	(
			G_OBJECT(BUTTON_CANCELAR_CAMBIAR_FICHAS),
			"clicked",
			G_CALLBACK(boton_cancelar_cambiar_fichas_press_callback),
			NULL
	);

	// [Cambiar letras del atril]
	for (i = 0; i < ATRIL_CHR - 1; i++)
		g_signal_connect
		(
				G_OBJECT(event_box_cf_atril[i]),
				"button_press_event",
				G_CALLBACK(cambiar_letras_atril_press_callback),
				NULL
		);
}

/*
 * Crea una lista con los objetos que contiene el
 * builder, almacena la dirección de cada objeto
 * cuyo identificador es igual a WidgetID, en el
 * vector de punteros widgetList
 */

static void administrar_widgets_builder(void)
{
	int i;

	if (builder == NULL)
	{
		builder 	 	 	= gtk_builder_new_from_file(GLADE_FILE);
		assert(builder);

		for (i = 0; i < GTK_BUILD_N_OBJECTS; i++)
		{
			widgetList[i] 	= gtk_builder_get_object(builder, WidgetID[i]);
			assert(widgetList[i]);
		}

		assert(i == GTK_BUILD_N_OBJECTS);
	}
}

static void ui_css_load(void)
{
	css = gtk_css_provider_new();

	if (!gtk_css_provider_load_from_path(css, CSSFILE, NULL))
		exit(-1);

	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
