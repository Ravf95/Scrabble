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
 	 VARIABLES GLOBALES DEL PROGRAMA
   ============================================================================================= */

// matriz utilizada para almacenar las coordenadas
// de las fichas del tablero del juego
char tabla[N][N];

// matriz utilizada para representar las coordenadas
// de las casillas con puntos de bonificación del
// tablero del juego

char bonus[N][N] =
{
	{'@', ' ', ' ', '&', ' ', ' ', ' ', '@', ' ', ' ', ' ', '&', ' ', ' ', '@'},
	{' ', '#', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '#', ' '},
	{' ', ' ', '#', ' ', ' ', ' ', '&', ' ', '&', ' ', ' ', ' ', '#', ' ', ' '},
	{'&', ' ', ' ', '#', ' ', ' ', ' ', '&', ' ', ' ', ' ', '#', ' ', ' ', '&'},
	{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' '},
	{' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' '},
	{' ', ' ', '&', ' ', ' ', ' ', '&', ' ', '&', ' ', ' ', ' ', '&', ' ', ' '},
	{'@', ' ', ' ', '&', ' ', ' ', ' ', '#', ' ', ' ', ' ', '&', ' ', ' ', '@'},
	{' ', ' ', '&', ' ', ' ', ' ', '&', ' ', '&', ' ', ' ', ' ', '&', ' ', ' '},
	{' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' '},
	{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' '},
	{'&', ' ', ' ', '#', ' ', ' ', ' ', '&', ' ', ' ', ' ', '#', ' ', ' ', '&'},
	{' ', ' ', '#', ' ', ' ', ' ', '&', ' ', '&', ' ', ' ', ' ', '#', ' ', ' '},
	{' ', '#', ' ', ' ', ' ', '+', ' ', ' ', ' ', '+', ' ', ' ', ' ', '#', ' '},
	{'@', ' ', ' ', '&', ' ', ' ', ' ', '@', ' ', ' ', ' ', '&', ' ', ' ', '@'}
};

// diccionario
char **g_diccionario;

const char *g_user_home_dir;

// alamacenan los nombres
// de los jugadores
const char *player_name_1;
const char *player_name_2;

// puntuación de los jugadores
int player_score[2] = { 0, 0 };

// widgets de uso general
GtkWidget *cel_tabla_label[N][N];
GtkWidget *index_tabla_label[4][N + 2];
GtkWidget *atril_label[ATRIL_CHR - 1];
GtkWidget *cf_atril_label[ATRIL_CHR - 1];
