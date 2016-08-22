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
 	 VARIALBES GLOBALES DEL PROGRAMA
   ============================================================================================= */

extern const char *player_name_1;
extern const char *player_name_2;
extern const char *g_user_home_dir;

/* ==============================================================================================
 	MACROS
   ============================================================================================= */

#define HTML_INICIO								"<html>"\
												"<head>"\
													"<title>Registro de fichas utilizadas</title>"\
												"</head>"\
												"<body>"\
												"Registro de fichas utilizadas en esta partida<br><br>"

#define HTML_NUEVA_JUGADA						"Jugada Nro. %d<br>"\
												"Jugador: %s<br>"\
												"Fichas usadas: %s<br><br>"

#define HTML_FIN								"</body></html>"

#define HTML_SIZE_BUFFER						512

/* ==============================================================================================
 	 VARIALBES GLOBALES DEL DOCUMENTO
   ============================================================================================= */

static char html_buffer[HTML_SIZE_BUFFER];
static FILE *archivo_registro;

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES
   ============================================================================================= */

static void agregar_nueva_linea(char *str);

/* ==============================================================================================
 	 PROCEDIMIENTOS
   ============================================================================================= */

void crear_nuevo_archivo_registro(void)
{
	char file[PATH_MAX];
	snprintf(file, sizeof(file), "%s%s", g_user_home_dir, ARCHIVO_REGISTRO_NOMBRE);

	archivo_registro = fopen(file, "w");
	assert(archivo_registro);

	agregar_nueva_linea(HTML_INICIO);
}

void cerrar_archivo_registro(void)
{
	if (archivo_registro != NULL)
	{
		agregar_nueva_linea(HTML_FIN);
		fclose(archivo_registro);
		archivo_registro = NULL;
	}
}

void registro_juego_actual_nueva_jugada(int cturno, int turno, int posplayed[][4], int cjugadas)
{
	int i, j;
	char fichas_usadas[32];

	if (verificar_jugada_pasada())
	{
		snprintf(fichas_usadas, 32, "paso su jugada");
	}
	else
	{
		for (i = 0, j = 0; i <= cjugadas; i++)
		{
			fichas_usadas[j] = (char) posplayed[i][3];

			if (i + 1 <= cjugadas)
			{
				fichas_usadas[++j] = ',';
				fichas_usadas[++j] = ' ';
			}

			j++;
		}

		fichas_usadas[j] = '\0';
	}

	if (turno == 0)
		snprintf(html_buffer, HTML_SIZE_BUFFER, HTML_NUEVA_JUGADA, cturno + 1, player_name_1, fichas_usadas);
	else
		snprintf(html_buffer, HTML_SIZE_BUFFER, HTML_NUEVA_JUGADA, cturno + 1, player_name_2, fichas_usadas);

	agregar_nueva_linea(html_buffer);
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES
   ============================================================================================= */

static void agregar_nueva_linea(char *str)
{
	int status;
	status = fputs(str, archivo_registro);

	if (status < 0 || status == EOF)
	{
		g_printf("error al manipular el archivo de registro de partida");
		exit(-1);
	}
}
