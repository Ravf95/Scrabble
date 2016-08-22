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
extern char tabla[N][N];

/* ==============================================================================================
 	 FUNCIONES
   ============================================================================================= */

int verificar_letra_atril(char atril[], unsigned int i)
{
	// ¿la posición de la letra ingresada está en el rango?
	if (i < 0 || i >= (ATRIL_CHR - 1))
		return 1;

	// ¿la posición ingresada contiene alguna letra?
	if (atril[i] == CHR_NULL)
		return 3;

	return 0;
}

int verificar_posicion_atril_tablero(int x, int y)
{
	// ¿la fila/columna seleccionada está en el rango con respecto a la tabla?
	if (x  < 0 || x > (N - 1))
		return 1;

	// ¿la fila/columna seleccionada está en el rango con respecto a la tabla?
	if (y  < 0 || y > (N - 1))
		return 2;

	// ¿la fila/columna contiene alguna letra en la tabla?
	if (tabla[x][y] != CHR_NULL)
		return 3;

	return 0;
}

int mover_pieza_atril(char atril[], unsigned int i)
{
	if (i < 0 || i >= (ATRIL_CHR - 1))
		return 1;

	// gtk
	ui_atril_letra_vacia(i, atril[i]);

	atril[i] = CHR_NULL;

	return 0;
}

int mover_pieza_tablero(int posplayed[][4], int cjugadas)
{
	if (cjugadas < 0 || cjugadas >= (ATRIL_CHR - 1))
		return 1;

	unsigned int px = posplayed[cjugadas][0];
	unsigned int py = posplayed[cjugadas][1];

	tabla[px][py] = posplayed[cjugadas][3];

	// gtk
	ui_tablero_nueva_letra(px, py);

	return 0;
}

// obtiene la orientación de las letras colocadas en el tablero
// vertical o horizontal

int verificar_posicion_fichas_tablero(int posplayed[][4], int cjugadas)
{
	if (cjugadas < 0 || cjugadas >= (ATRIL_CHR - 1))
		return -1;

	int i, orientacion;
	orientacion = 0; // horizontal

	// ¿coloco una sola ficha?
	if (cjugadas == 0)
	{
		orientacion = obtener_orientacion_letra(posplayed[0][0], posplayed[0][1]);
		return orientacion;
	}

	// ¿las fichas están colocadas de manera horizontal?
	// misma posición x para todas las fichas
	for (i = 0; i <= cjugadas; i++)
	{
		if (posplayed[0][0] != posplayed[i][0])
		{
			orientacion = 2;
			break;
		}
	}

	if (orientacion == 0)
		return orientacion;

	orientacion = 1; // vertical

	// ¿las fichas están colocadas de manera vertical?
	// misma posición y para todas las fichas

	for (i = 0; i <= cjugadas; i++)
	{
		if (posplayed[0][1] != posplayed[i][1])
		{
			orientacion = 2;
			break;
		}
	}

	return orientacion;
}

// verifica si las letras colocadas en el tablero
// son contiguas entre si
int verificar_posicion_fichas_tablero_contiguas(int posplayed[][4], int cjugadas)
{
	if (cjugadas < 0 || cjugadas >= (ATRIL_CHR - 1))
		return -1;

	int i, contiguas, tx, ty;
	contiguas = 0; // son contiguas

	// ¿solo coloco una ficha?
	if (cjugadas == 0)
		return contiguas;

	for (i = 0; i <= cjugadas; i++)
	{
		tx = posplayed[i][0];
		ty = posplayed[i][1];

		if (ty != N - 1)
			if (tabla[tx][ty + 1] != CHR_NULL)
				continue;

		if (ty != 0)
			if (tabla[tx][ty - 1] != CHR_NULL)
				continue;

		if (tx != N - 1)
			if (tabla[tx + 1][ty] != CHR_NULL)
				continue;

		if (tx != 0)
			if (tabla[tx - 1][ty] != CHR_NULL)
				continue;

		contiguas = 1;
		break;
	}

	return contiguas;
}

int obtener_adversario(int turno)
{
	if (turno == 1)
		return 0;

	return 1;
}

/* ==============================================================================================
 	 PROCEDIMIENTOS
   ============================================================================================= */

// retornas todas las fichas usadas en el tablero
// del turno correspondiente
void devolver_piezas_atril(char atril[], int posplayed[][4], int cjugadas)
{
	if (cjugadas < 0 || cjugadas >= (ATRIL_CHR - 1))
		return;

	int i;

	for (i = 0; i <= cjugadas; i++)
	{
		atril[ posplayed[i][2] ] = (char) posplayed[i][3];
	}

	// gtk
	ui_atril_nuevas_letras(atril);
}

// remueve todas las fichas colocadas en el turno
// correspondiente
void sacar_piezas_tablero(int posplayed[][4], int cjugadas)
{
	if (cjugadas < 0 || cjugadas >= (ATRIL_CHR - 1))
		return;

	int i;
	unsigned int px;
	unsigned int py;

	for (i = 0; i <= cjugadas; i++)
	{
		px = posplayed[i][0];
		py = posplayed[i][1];

		tabla[px][py] = CHR_NULL;

		// gtk
		ui_tablero_letra_vacia(px, py);
	}
}
