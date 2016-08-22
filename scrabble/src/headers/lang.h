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

#ifndef _LANG_H
#define _LANG_H

// mensajes de error [tablero]
#define D_INV_LETRA_TABLERO_HV 					"Solo puedes colocar las fichas\nde manera horizonal o vertical, no ambas\n\nSe te han devuelto las fichas"
#define D_INV_LETRA_CONTIGUA					"Las fichas tienen que ser contiguas\n\nSe te han devuelto las fichas"
#define D_LETRAS_ATRIL							"No quedan letras que mover en el atril"
#define D_LETRAS_DEVUELTAS						"Se te han devuelto las fichas colocadas al atril"
#define D_LETRAS_POS_CENTRO_PRIMER_TURNO		"Tienes que colocar una\nletra en la casilla central\n\nSe te han devuelto las fichas"
#define D_PALABRA_DICC							"La palabra no existe\nen el diccionario\n\nSe te han devuelto las fichas"
#define D_PALABRA_CONTIGUA						"La fichas tienen que ser contiguas\na una palabra del tablero\n\nSe te han devuelto las fichas"
#define D_PRIMER_TURNO							"Es el primer turno, no puedes\nterminar la jugada sin haber\ncolocado una letra en el tablero"
#define D_SELECCIONAR_CAMBIAR_FICHAS			"Debes seleccionar alguna ficha\npara cambiar"

// fin del juego
#define E_JUGADOR_GANADOR						"Ha ganado el juego!"
#define E_EMPATE								"Empate!"

// símbolos Unicode
#define U_BLACK_STAR 							"\u2605"
#define U_SUBSCRIPT_ZERO						"\u2080"
#define U_SUBSCRIPT_ONE 						"\u2081"
#define U_SUBSCRIPT_TWO 						"\u2082"
#define U_SUBSCRIPT_THREE						"\u2083"
#define U_SUBSCRIPT_FOUR 						"\u2084"
#define U_SUBSCRIPT_FIVE 						"\u2085"
#define U_SUBSCRIPT_SIX 						"\u2086"
#define U_SUBSCRIPT_SEVEN 						"\u2087"
#define U_SUBSCRIPT_EIGHT 						"\u2088"
#define U_SUBSCRIPT_NINE 						"\u2089"
#define U_LATIN_CAPITAL_LETTER_N_TILDE			"\u00D1"

// casillas con bonus del tablero
#define DOBLE_PALABRA 							"DOBLE \nPALABRA"
#define DOBLE_LETRA	  							"DOBLE \nLETRA  "
#define TRIPLE_LETRA  							"TRIPLE\nLETRA  "
#define TRIPLE_PALABRA 							"TRIPLE\nPALABRA"

// Textos de ayuda
#define TOOLTIP_CELDA_VACIA_TABLERO				"presiona una\nletra del atril\ny presiona\naqui!"
#define TOOLTIP_CELDA_VACIA_ATRIL				"presiona una\nletra y luego\npresiona una\ncasilla del\ntablero!"

// barra de estado de puntuación
#define SCORE_JUGADOR							"Puntuacion de"

// verificación de datos nuevo_juego : ventana : window_nuevo_juego
#define CAMPO_NOMBRE_JUGADOR_1_VACIO			"El nombre del jugador 1\nno puede estar esta vacio"
#define CAMPO_NOMBRE_JUGADOR_2_VACIO			"El nombre del jugador 2\nno puede estar esta vacio"
#define CAMPO_NOMBRES_IGUALES					"El nombre del jugador 1 y del jugador 2\nno pueden ser iguales"

// otros
#define P_SHOW_HTML_HELP_PAGE					"No se puede mostrar\nlas reglas del juego"

#endif
