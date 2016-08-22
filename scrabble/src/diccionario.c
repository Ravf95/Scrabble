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
 	 MACROS
   ============================================================================================= */

#define DICCIONARIO								"lemario.txt"
#define LONGITUD_PALABRA_DICC					64

/* ==============================================================================================
 	 VARIABLES EXTERNAS
   ============================================================================================= */

// globals.c
extern char **g_diccionario;

/* ==============================================================================================
 	 VARIABLES GLOBALES DEL ARCHIVO
   ============================================================================================= */

static int g_lineas;

/* ==============================================================================================
 	PROTOTIPOS DE FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int cargar_archivo_diccionario(int load);
static char *str_to_upper(char *word);

/* ==============================================================================================
 	PROTOTIPOS DE PROCEDIMIENTOS
   ============================================================================================= */

void reservar_memoria_dicc();
void destruir_memoria_reservada_dicc();

/* ==============================================================================================
 	PROTOTIPOS DE FUNCIONES
   ============================================================================================= */

int verficar_palabra_dicccionario(char *word);

/* ==============================================================================================
 	PROCEDIMIENTOS
   ============================================================================================= */

void reservar_memoria_dicc()
{
	int i;

	g_lineas = cargar_archivo_diccionario(0);
	assert(g_lineas);

	g_diccionario = (char **) malloc(sizeof(char *) * g_lineas);
	assert(g_diccionario);

	for (i = 0; i < g_lineas; i++)
	{
		g_diccionario[i] = (char *) malloc(sizeof(char) * LONGITUD_PALABRA_DICC);
		assert(g_diccionario[i]);
	}

	cargar_archivo_diccionario(1);
}

void destruir_memoria_reservada_dicc()
{
	int i;

	for (i = 0; i < g_lineas; i++)
	{
		free(g_diccionario[i]);
		g_diccionario[i] = NULL;
	}

	if (g_diccionario != NULL)
		free(g_diccionario);

	g_diccionario = NULL;
}

/* ==============================================================================================
 	FUNCIONES
   ============================================================================================= */

int verficar_palabra_dicccionario(char *word)
{
	int i;

	for (i = 0; i < g_lineas; i++)
		if (strcmp(g_diccionario[i], word) == 0)
			return 1;

	return 0;
}

int obtener_palabra_indice_letra(char letra)
{
	int i;

	for (i = 0; i < g_lineas; i++)
	{
		if (g_diccionario[i][0] == letra)
			return i;
	}

	return i;
}

/* ==============================================================================================
 	FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int cargar_archivo_diccionario(int load)
{
	FILE *file;

	char text[LONGITUD_PALABRA_DICC];
	char word[LONGITUD_PALABRA_DICC];
	int count = 0;
	int c;
	int i;

	file = fopen(DICCIONARIO, "r");
	assert(file);

	while (!feof(file))
	{
		c = fgetc(file);

		// omitir lineas
		// con comentarios
		if (c == ';')
		{
			while
			(
				fgetc(file) != '\n'
			);

			continue;
		}

		// obtener la palabra de cada linea con el salto de linea
		if (fgets(word, sizeof(word), file) != NULL)
		{
			if (load)
			{
				// copiar la palabra completa
				// en mayusculas incluyendo
				// la primera letra y omitir
				// el salto de linea

				text[0] = c;

				for (i = 0, c = 1; word[i] != '\0'; i++, c++)
					text[c] = word[i];

				text[i] = '\0';

				strncpy(g_diccionario[count], str_to_upper(text), LONGITUD_PALABRA_DICC);
			}

			count++;
		}
	}

	fclose(file);
	return count;
}

static char *str_to_upper(char *word)
{
	int i;
	static char new_word[LONGITUD_PALABRA_DICC];

	for (i = 0; word[i] != '\0'; i++)
	{
		switch (word[i])
		{
			case CHR_RR:
			case CHR_LL:
			case CHR_NT:
			case CHR_CH:
			case COMODIN:
			case CHR_NULL:
				new_word[i] = word[i];
				break;

			default:
				new_word[i] = (char) toupper(word[i]);
				break;
		}
	}

	new_word[i] = '\0';

	return new_word;
}
