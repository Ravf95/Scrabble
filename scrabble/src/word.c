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
extern char bonus[N][N];

/* ==============================================================================================
 	 TIPOS DE DATOS DEFINIDOS POR EL USUARIO
   ============================================================================================= */

typedef struct
{
    char letra;
    int cantidadLetra;
    int valorletra;
} Letras;

/* ==============================================================================================
 	 VARIABLES GLOBALES DEL ARCHIVO
   ============================================================================================= */

static Letras Pozo[29] =
{
    { 'A', 12, 1 	},
    { 'E', 12, 1  	},
    { 'I', 6, 1 	},
    { 'O', 9, 1 	},
    { 'D', 5, 2 	},
    { 'N', 5, 1 	},
    { 'R', 5, 1 	},
    { 'S', 6, 1 	},
    { 'T', 4, 1 	},
    { 'C', 4, 3 	},
    { 'L', 4, 1 	},
    { 'U', 5, 1 	},
    { 'M', 2, 3 	},
    { 'P', 2, 3 	},
    { 'G', 2, 2 	},
    { 'B', 2, 3 	},
    { 'H', 2, 4 	},
    { 'V', 1, 4 	},
    { 'F', 1, 4 	},
    { 'Q', 1, 5 	},
    { 'J', 1, 8 	},
    { 'X', 1, 8 	},
    { 'Y', 1, 4 	},
    { 'Z', 1, 10	},
    { ']', 1, 5 	},        // CH= ]
    { '^', 1, 8 	},        // LL= ^
    { '_', 1, 8 	},        // Ñ=  _
    { '`', 1, 8 	},        // RR= `
    { ';', 2, 0 	},		  // Comodin
};

static int nro_letras_pozo	= NRO_LETRAS_POZO;

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int obtener_indice_ficha_pozo(char letra);

/* ==============================================================================================
 	 FUNCIONES
   ============================================================================================= */

/*
 * parámetros:
 * h_v = orientación de las letras colocadas en el tablero
 * tx  = posición x de cualquier letra colocada en el turno actual
 * ty  = posición y de cualquier letra colocada en el turno actual
 * *ptrStart = puntero que almacena la posición de la primera letra
 * *ptrEnd   = puntero que almacena la posición de la última letra
 */

char *obtener_palabra_formada(int h_v, int tx, int ty, int *ptrStart, int *ptrEnd)
{
	static char word[N];
	int count, i, start, end;

	count = 0;
	start = 0;
	end   = N - 1;

	switch (h_v)
	{
		// horizontal
		case 0:
			// se obtiene la primera letra
			// de la palabra formada
			for (i = ty; i >= 0; i--)
			{
				if (tabla[tx][i] == CHR_NULL)
				{
					start = i + 1;
					break;
				}
			}

			// se obtiene la ultima letra
			// de la palabra formada
			for (i = ty; i < N; i++)
			{
				if (tabla[tx][i] == CHR_NULL)
				{
					end = i - 1;
					break;
				}
			}

			// se almacena cada letra de la palabra formada
			for (i = start, count = 0; i <= end; i++, count++)
				word[count] = tabla[tx][i];

			break;

		// vertical
		case 1:
			// se obtiene la primera letra
			// de la palabra formada
			for (i = tx; i >= 0; i--)
			{
				if (tabla[i][ty] == CHR_NULL)
				{
					start = i + 1;
					break;
				}
			}

			// se obtiene la ultima letra
			// de la palabra formada
			for (i = tx; i < N; i++)
			{
				if (tabla[i][ty] == CHR_NULL)
				{
					end = i - 1;
					break;
				}
			}

			// se almacena cada letra de la palabra formada
			for (i = start, count = 0; i <= end; i++, count++)
				word[count] = tabla[i][ty];

			break;
	}

	word[count] = '\0';

	if (ptrStart != NULL)
		*ptrStart = start;

	if (ptrEnd != NULL)
		*ptrEnd = end;

	return word;
}

int obtener_valor_palabra(int h_v, int cjugadas, int ax, int ay, int start, char *word, int posplayed[][4])
{
	int i, j;
	int total = 0;
	int valor_letra;
	int a, b;

	int bonus_palabra = 1;
	int x;
	int tx, ty;

	for (i = 0, j = start; word[i] != '\0'; i++, j++)
	{
		valor_letra = obtener_valor_letra(word[i]);

		if (h_v == 0) // h
		{
			a = ax;
			b = j;
		}
		else
		{
			a = j;
			b = ay;
		}

		for (x = 0; x <= cjugadas; x++)
		{
			tx = posplayed[x][0];
			ty = posplayed[x][1];

			if (a == tx && ty == b)
			{
				switch (bonus[a][b])
				{
					case S_DOBLE_LETRA:
						valor_letra *= 2;
						break;

					case S_TRIPLE_LETRA:
						valor_letra *= 3;
						break;

					case S_DOBLE_PALABRA:
						bonus_palabra *= 2;
						break;

					case S_TRIPLE_PALABRA:
						bonus_palabra *= 3;
						break;
				}
			}
		}

		total += valor_letra;
	}

	total *= bonus_palabra;

	return total;
}

int obtener_valor_letra(char c)
{
	int valor_letra, x;
	valor_letra = 0;

	for (x = 0; x < LETRAS_N; x++)
	{
		if (Pozo[x].letra == c)
		{
			valor_letra = Pozo[x].valorletra;
			break;
		}
	}

	return valor_letra;
}

int obtener_valor_letras_atril(char *atril)
{
	int i, total;
	total = 0;

	for (i = 0; atril[i] != '\0'; i++)
	{
		if (atril[i] == CHR_NULL)
			continue;

		total += obtener_valor_letra(atril[i]);
	}

	return total;
}

/*
 * Obtiene la orientación de la palabra
 * con una sola letra colocada en el
 * tablero
 */

int obtener_orientacion_letra(int tx, int ty)
{
	if (ty != N - 1)
		if (tabla[tx][ty + 1] != CHR_NULL)
			return 0;

	if (ty != 0)
		if (tabla[tx][ty - 1] != CHR_NULL)
			return 0;

	return 1;
}

int obtener_numero_letras_pozo(void)
{
	return nro_letras_pozo;
}

char obtener_letra_pozo_index(int index)
{
	return Pozo[index].letra;
}

int obtener_cantidad_letra_pozo(char letra)
{
	int x;
	x = obtener_indice_ficha_pozo(letra);
	return Pozo[x].cantidadLetra;
}

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int obtener_indice_ficha_pozo(char letra)
{
	int i, index;
	index = 0;

	for (i = 0; i < LETRAS_N; i++)
	{
		if (Pozo[index].letra == letra)
		{
			index = i;
			break;
		}
	}

	return index;
}

/* ==============================================================================================
 	 PROCEDIMIENTOS
   ============================================================================================= */

void repartir_letras_atril(char atril[])
{
    int x, i;

    for (i = 0; i < ATRIL_CHR - 1; i++)
    {
    	if (atril[i] != CHR_NULL)
    		continue;

		while (1)
		{
			if (nro_letras_pozo <= 0)
				return;

			x = rand() % 2;

			switch (x)
			{
				case 0:
					x = rand() % 8;
					break;

				case 1:
					x = rand() % 29;

					if (x < 8)
						x = x + 8;

					break;
			}

			if (Pozo[x].cantidadLetra != 0)
				break;
		}

		atril[i] = Pozo[x].letra;
		Pozo[x].cantidadLetra--;
		nro_letras_pozo--;
    }

    atril[i] = '\0';

    // gtk
    ui_change_text_pozo_nletras(nro_letras_pozo);
}

void cambiar_letras_atril(char *cf_atril, char *atril)
{
	int i, x;

    for (i = 0; i < ATRIL_CHR - 1; i++)
    {
    	if (cf_atril[i] == CHR_NULL)
    	{
    		x = obtener_indice_ficha_pozo(atril[i]);

			Pozo[x].cantidadLetra++;
			nro_letras_pozo++;

    		atril[i] = CHR_NULL;
    	}

    	cf_atril[i] = '-';
    }

    cf_atril[i] = '\0';
    repartir_letras_atril(atril);
}

void asignar_cantidad_letra_pozo(char letra, int cantidad)
{
	int x;
	x = obtener_indice_ficha_pozo(letra);
	Pozo[x].cantidadLetra += cantidad;
	nro_letras_pozo += cantidad;
}
