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

#ifndef _GTKGUI_H
#define _GTKGUI_H

// archivos externos
#define GLADE_FILE 							"ui.glade"
#define CSSFILE								"ui.css"
#define HTML_HELP_FILE						"/scrabble/html/scrabble_doc.html"
#define ARCHIVO_REGISTRO_NOMBRE				"/scrabble/html/ultima_partida_registro_fichas.html"

// Widgets gestionados por el builder
#define GTK_BUILD_N_OBJECTS 				36

// Funciones
int start_game_gtkgui(void);
void *get_game_widget_with_id(unsigned int id);

/*
 *  Widgets gestionados por el builder
 */

// ventana: window_init
#define MAIN_WINDOW							get_game_widget_with_id(0)
#define TABLA_GRID							get_game_widget_with_id(1)
#define OPCIONES_GRID						get_game_widget_with_id(2)
#define TP_JUGADA_BOTON  					get_game_widget_with_id(3)
#define CD_FICHAS_BOTON 					get_game_widget_with_id(4)

// ventana: nuevo juego
#define NUEVO_JUEGO_MEMU_ITEM				get_game_widget_with_id(5)
#define NUEVO_JUEGO_WINDOW					get_game_widget_with_id(6)
#define BOTON_CANCELAR_WINDOW_NUEVO_J		get_game_widget_with_id(7)
#define BOTON_ACEPTAR_WINDOW_NUEVO_J		get_game_widget_with_id(8)
#define ENTRY_PLAYER_NAME_1					get_game_widget_with_id(9)
#define ENTRY_PLAYER_NAME_2					get_game_widget_with_id(10)
#define RADIO_BUTTON_ORDEN_J1				get_game_widget_with_id(31)
#define RADIO_BUTTON_ORDEN_J2				get_game_widget_with_id(32)
#define RADIO_BUTTON_ORDEN_ALEATORIO		get_game_widget_with_id(33)
#define RADIO_BUTTON_MODO_JVSJ				get_game_widget_with_id(34)
#define RADIO_BUTTON_MODO_JVSC				get_game_widget_with_id(35)

// barra de estado
#define LABEL_N_LETRAS_POZO					get_game_widget_with_id(11)
#define LABEL_SCORE_JUGADOR_1				get_game_widget_with_id(12)
#define LABEL_SCORE_JUGADOR_2				get_game_widget_with_id(13)
#define	STATUSBAR							get_game_widget_with_id(14)
#define LABEL_LETRAL_POZO					get_game_widget_with_id(21)
#define LABEL_TURNO_ACTUAL					get_game_widget_with_id(22)

// separadores : window_init
#define SEPARADOR_1							get_game_widget_with_id(15)
#define SEPARADOR_2							get_game_widget_with_id(16)

// otros
#define BOTON_COMO_JUGAR					get_game_widget_with_id(17)
#define BOTON_ACERCA_DE						get_game_widget_with_id(18)
#define ACERCA_DE_WINDOW					get_game_widget_with_id(19)
#define BOTON_SALIR							get_game_widget_with_id(20)

// vetana: comodin
#define COMODIN_WINDOW						get_game_widget_with_id(23)
#define COMBO_BOX_LETRAS_COMODIN			get_game_widget_with_id(24)
#define BUTTON_COMODIN_ACEPTAR				get_game_widget_with_id(25)
#define BUTTON_COMODIN_CANCELAR				get_game_widget_with_id(26)

// ventana: cambiar fichas
#define CAMBIAR_FICHAS_WINDOW				get_game_widget_with_id(27)
#define BOX_ATRIL_CAMBIAR_FICHAS			get_game_widget_with_id(28)
#define BUTTON_ACEPTAR_CAMBIAR_FICHAS		get_game_widget_with_id(29)
#define BUTTON_CANCELAR_CAMBIAR_FICHAS		get_game_widget_with_id(30)

#endif
