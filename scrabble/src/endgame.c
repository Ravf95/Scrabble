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
extern int player_score[2];
extern const char *player_name_1;
extern const char *player_name_2;

/* ==============================================================================================
 	 VARIABLES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int c_turno_consecutivo_pasado;
static int c_jugadas_consecutivas_sin_score;

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static gboolean check_condicion_1(int cjugadas, char atril[][ATRIL_CHR]);
static gboolean check_condicion_2(void);
static gboolean check_condicion_3(void);

/* ==============================================================================================
 	 PROCEDIMIENTOS
   ============================================================================================= */

gboolean check_end_game(int cjugadas, char atril[][ATRIL_CHR])
{
	if (check_condicion_1(cjugadas, atril))
		return TRUE;

	if (check_condicion_2())
		return TRUE;

	if (check_condicion_3())
		return TRUE;

	return FALSE;
}

void contar_turno_pasado(int cturno)
{
	static int nturno_anterior;

	if (nturno_anterior == 0 || (nturno_anterior + 1 == cturno))
		c_turno_consecutivo_pasado++;
	else
		c_turno_consecutivo_pasado = 0;

	nturno_anterior = cturno;
}

void contar_jugada_sin_puntaje(int cturno)
{
	static int turno_anterior;

	if	(turno_anterior == 0 || (turno_anterior + 1 == cturno))
		c_jugadas_consecutivas_sin_score++;
	else
		c_jugadas_consecutivas_sin_score = 0;

	turno_anterior = cturno;
}

/*
 * Cada jugador resta la suma de sus fichas no jugadas de su total de
 * puntos, si un jugador uso todas sus fichas, agrega a su total de puntos
 * la suma de las fichas no jugadas del adversario.
 */

void obtener_puntacion_ultima_jugada(char atril[][ATRIL_CHR])
{
	int score_atril_j1, score_atril_j2;

	score_atril_j1 = obtener_valor_letras_atril(atril[0]);
	score_atril_j2 = obtener_valor_letras_atril(atril[1]);

	player_score[0] -= score_atril_j1;
	player_score[1] -= score_atril_j2;

	if (player_score[0] < 0)
		player_score[0] = 0;

	if (player_score[1] < 0)
		player_score[1] = 0;

	if (score_atril_j1 == 0)
		player_score[0] += score_atril_j2;

	if (score_atril_j2 == 0)
		player_score[1] += score_atril_j1;
}

void obtener_ganador()
{
	int result;
	char msg[256];
	result = abs(player_score[0]) - abs(player_score[1]);

	if (result > 0)
	{
		snprintf(msg, sizeof(msg), "%s\n%s", player_name_1, E_JUGADOR_GANADOR);
	}
	else if (result < 0)
	{
		snprintf(msg, sizeof(msg), "%s\n%s", player_name_2, E_JUGADOR_GANADOR);
	}
	else
	{
		snprintf(msg, sizeof(msg), "%s", E_EMPATE);
	}

	// gtk
	quick_message(msg, MAIN_WINDOW);
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

/*
 * Todas las fichas han sido robadas
 * y uno de los jugadores ha usado
 * todas las fichas de su atril
 */

static gboolean check_condicion_1(int cjugadas, char atril[][ATRIL_CHR])
{
	if (obtener_numero_letras_pozo() != 0)
		return FALSE;

	int i, jugador_1_ficha_contador, jugador_2_ficha_contador;

	jugador_1_ficha_contador = 0;
	jugador_2_ficha_contador = 0;

	for (i = 0; i < ATRIL_CHR - 1; i++)
	{
		if (atril[0][i] != CHR_NULL)
			jugador_1_ficha_contador++;

		if (atril[1][i] != CHR_NULL)
			jugador_2_ficha_contador++;
	}

	if (jugador_1_ficha_contador == 0)
		return TRUE;

	if (jugador_2_ficha_contador == 0)
		return TRUE;

	return TRUE;
};

/*
 * Los jugadores han pasado dos veces
 * en turnos consecutivos
 */

static gboolean check_condicion_2(void)
{
	if (c_turno_consecutivo_pasado != 4)
		return FALSE;

	return TRUE;
}

/*
 * Todos los jugadores han hecho
 * 12 jugadas consecutivas sin puntajes
 */

static gboolean check_condicion_3(void)
{
	if (c_jugadas_consecutivas_sin_score != 24)
		return FALSE;

	return TRUE;
}
