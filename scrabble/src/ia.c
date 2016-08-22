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

#pragma GCC diagnostic ignored 		"-Wunused-variable"
#pragma GCC diagnostic ignored 		"-Wunused-function"

/* ==============================================================================================
 	 VARIABLES EXTERNAS
   ============================================================================================= */

extern char tabla[N][N];
const char *player_name_2;
extern char **g_diccionario;

/* ==============================================================================================
 	 VARIABLES LOCALES DEL ARCHIVO
   ============================================================================================= */

static char tabla_palabra_formada[N - 1];
static char tabla_palabras_encontradas[N][N];
static int tabla_orientacion_palabra_formada;
static int i_index_palabra_formada;
static int j_index_palabra_formada;
static int word_start;
static int word_end;

/* ==============================================================================================
 	 MACROS
   ============================================================================================= */

#define CHR_LETRA_USADA				'*'
#define POS_ULTIMA_LETRA 			(word_end - word_start)

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

static void obtener_nueva_palabra_formada(void);
static void marcar_palabra_formada(int h_v, int start, int end);
static void restablecer_datos_busqueda(void);
static void almacenar_ficha_utilizada(char letra, int pos, int *cjugadas, int posplayed[][4]);
static void posicionar_coordenada_letra(int *tx, int *ty, int pxy);
static void desplazar_fichas_tablero(int *cjugadas, int posplayed[][4]);

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static gboolean verificar_palabra_formada_plural(void);
static gboolean es_una_vocal(char letra);
static gboolean verificar_agregar_letra_final_palabra_formada(void);
static int palabra_formada_obtener_pos_x(void);
static int palabra_formada_obtener_pos_y(void);
static gboolean verificar_pieza_desplazada_diccionario_ia(int *status, int *cjugadas, int *turno, int posplayed[][4]);
static gboolean palabra_contenida_primeras_letras(char *str1, char *str2, int *n_letras);
static gboolean palabra_contenida_ultimas_letras(char *str1, char *str2, int *n_letras);
static gboolean palabra_contenida_mitad_letras(char *str1, char *str2, int *n_letras_derecha, int *n_letras_izquierda, int indice);
static int contador_de_espacios_nulos(int direccion, int *contador_dir1, int *contador_dir2);

/* ==============================================================================================
 	 FUNCIONES
   ============================================================================================= */

void ia_jugar(char atril[][ATRIL_CHR], int *cturno, int *turno, int *cjugadas, int posplayed[][4], int *status)
{
	int i, x, j, espacios_nulos, espacio_nulo_dir2, n_letras, n_letras_dir2;
	size_t longitud;
	char new_word[N], new_word_dir2[N];
	gboolean palabra_definida;

	palabra_definida = FALSE;

	// ¿ primer turno ?
	// desplazar alguna ficha
	if (*cturno == 0)
	{
		*cjugadas = 0;
		posplayed[0][0] = 7;
		posplayed[0][1] = 7;

		i = rand() % (LETRAS_N - 1);
		posplayed[0][3] = obtener_letra_pozo_index(i);

		palabra_definida = TRUE;
		tabla[7][7] = posplayed[0][3];
		desplazar_fichas_tablero(cjugadas, posplayed);
	}

	while (1)
	{
		if (i_index_palabra_formada == N && j_index_palabra_formada == N)
			break;

		if (palabra_definida)
			break;

		obtener_nueva_palabra_formada();

		if (tabla_palabra_formada[0] == '\0')
			break;

		// buscar en el diccionario
		// palabras que contengan la palabra actual
		for (i = 0; g_diccionario[i] != '\0'; i++)
		{
			longitud = strlen(g_diccionario[i]) - 1;

			if (longitud > 14)
				continue;

			if (strstr(g_diccionario[i], tabla_palabra_formada) == NULL)
				continue;

			// ¿la palabra está contenida entre otras letras?
			// entonces se completa entre en medio

			if (palabra_contenida_mitad_letras(g_diccionario[i], tabla_palabra_formada, &n_letras, &n_letras_dir2, i))
			{
				contador_de_espacios_nulos(2, &espacios_nulos, &espacio_nulo_dir2);

				// ¿la cantidad de letras que están en los extremos es mayor que 7?
				if (n_letras + n_letras_dir2 > 7)
					continue;

				if (espacios_nulos < n_letras)
					continue;

				if (espacio_nulo_dir2 < n_letras_dir2)
					continue;

				// verificar disponibilidad de letras
				// en los extremos

				// extremo 1

				for (x = 0; x < n_letras; x++)
				{
					if (obtener_cantidad_letra_pozo(g_diccionario[i][x]) <= 0)
						break;

					new_word[x] = g_diccionario[i][x];
				}

				new_word[x] = '\0';

				if (strlen(new_word) != n_letras)
					continue;

				// extremo 2

				for (x = POS_ULTIMA_LETRA + n_letras_dir2, j = 0; x < strlen(g_diccionario[i]); x++, j++)
				{
					if (obtener_cantidad_letra_pozo(g_diccionario[i][x]) <= 0)
						break;

					new_word_dir2[j] = g_diccionario[i][x];
				}

				new_word_dir2[j] = '\0';

				// ¿algunas letras no estan disponibles?
				if (strlen(new_word_dir2) != n_letras_dir2)
					continue;

				// almacenar fichas

				// extremo 1

				for (j = n_letras - 1; j >= 0; j--)
					almacenar_ficha_utilizada(new_word[j],  0 -j -1, cjugadas, posplayed);

				// extremo 2

				for (j = 0; j < n_letras_dir2; j++)
					almacenar_ficha_utilizada(new_word_dir2[j], POS_ULTIMA_LETRA + j + 1, cjugadas, posplayed);

				*cjugadas -= 1; // la última jugada está incompleta
				gstatus_asignar_orientacion_palabra(tabla_orientacion_palabra_formada);

				if (verificar_pieza_desplazada_diccionario_ia(status, cjugadas, turno, posplayed))
				{
					palabra_definida = TRUE;
					desplazar_fichas_tablero(cjugadas, posplayed);
					break;
				}
			}

			// ¿la palabra está contenida al inicio?
			// entonces se completa el final
			if (palabra_contenida_primeras_letras(g_diccionario[i], tabla_palabra_formada, &n_letras))
			{
				espacios_nulos = contador_de_espacios_nulos(1, NULL, NULL);

				if (espacios_nulos == 0)
					break;

				if (espacios_nulos < n_letras)
					continue;

				// almacenar las letras a utilizar
				// y verificar que estén disponibles en el pozo

				for (x = strlen(tabla_palabra_formada), j = 0; x < strlen(g_diccionario[i]); x++, j++)
				{
					if (obtener_cantidad_letra_pozo(g_diccionario[i][x]) <= 0)
						break;

					new_word[j] = g_diccionario[i][x];
				}

				new_word[j] = '\0';

				// ¿algunas letras no estan disponibles?
				if (strlen(new_word) != n_letras)
					continue;

				for (j = 0; j < n_letras; j++)
					almacenar_ficha_utilizada(new_word[j], POS_ULTIMA_LETRA + j + 1, cjugadas, posplayed);

				*cjugadas -= 1; // la ultima jugada esta incompleta
				gstatus_asignar_orientacion_palabra(tabla_orientacion_palabra_formada);

				if (verificar_pieza_desplazada_diccionario_ia(status, cjugadas, turno, posplayed))
				{
					palabra_definida = TRUE;
					desplazar_fichas_tablero(cjugadas, posplayed);
					break;
				}
			}

			// ¿la palabra esta contenida al final?
			// entonces se completa el inicio
			if (palabra_contenida_ultimas_letras(g_diccionario[i], tabla_palabra_formada, &n_letras))
			{
				espacios_nulos = contador_de_espacios_nulos(0, NULL, NULL);

				if (espacios_nulos == 0)
					break;

				if (espacios_nulos < n_letras)
					continue;

				for (x = 0; x < n_letras; x++)
				{
					if (obtener_cantidad_letra_pozo(g_diccionario[i][x]) <= 0)
						break;

					new_word[x] = g_diccionario[i][x];
				}

				new_word[x] = '\0';

				if (strlen(new_word) != n_letras)
					continue;

				for (j = n_letras - 1; j >= 0; j--)
					almacenar_ficha_utilizada(new_word[j],  0 -j -1, cjugadas, posplayed);

				*cjugadas -= 1; // la ultima jugada esta incompleta
				gstatus_asignar_orientacion_palabra(tabla_orientacion_palabra_formada);

				if (verificar_pieza_desplazada_diccionario_ia(status, cjugadas, turno, posplayed))
				{
					palabra_definida = TRUE;
					desplazar_fichas_tablero(cjugadas, posplayed);

					break;
				}
			}
		}

		if (palabra_definida)
			break;

		// plural
		if (verificar_palabra_formada_plural())
		{
			almacenar_ficha_utilizada('S', POS_ULTIMA_LETRA + 1, cjugadas, posplayed);

			*cjugadas = 0;
			gstatus_asignar_orientacion_palabra(tabla_orientacion_palabra_formada);

			if (verificar_pieza_desplazada_diccionario_ia(status, cjugadas, turno, posplayed))
			{
				palabra_definida = TRUE;
				desplazar_fichas_tablero(cjugadas, posplayed);
				break;
			}
		}
	}

	// pasar turno
	if (!palabra_definida)
	{
		asignar_jugada_pasada(TRUE);
		contar_turno_pasado(*cturno);
	}

	restablecer_datos_busqueda();

	// terminar el turno de la ia
	*status = j_game_terminar_turno;
	verificar_piezas_desplazadas(status, atril, cturno, cjugadas, posplayed, turno);
}

/* ==============================================================================================
 	 PROCEDIMIENTOS LOCALES DEL ARCHIVO
   ============================================================================================= */

// obtiene una palabra del tablero
// a partir de 2 letras contiguas

static void obtener_nueva_palabra_formada(void)
{
	int h_v;
	char *word;

	word = NULL;
	word_end = 0;
	word_start = 0;
	tabla_palabra_formada[0] = '\0';

	if (i_index_palabra_formada == N)
		i_index_palabra_formada = 0;

	for (; i_index_palabra_formada < N; i_index_palabra_formada++)
	{
		if (j_index_palabra_formada == N)
			j_index_palabra_formada = 0;

		for (; j_index_palabra_formada < N; j_index_palabra_formada++)
		{
			if (tabla[i_index_palabra_formada][j_index_palabra_formada] == CHR_NULL)
				continue;

			if (tabla_palabras_encontradas[i_index_palabra_formada][j_index_palabra_formada] == CHR_LETRA_USADA)
				continue;

			for (h_v = 0; h_v < 2; h_v++)
			{
				word = obtener_palabra_formada(h_v, i_index_palabra_formada, j_index_palabra_formada, &word_start, &word_end);

				if (word[0] == '\0')
					continue;

				if (word[1] == '\0')
					continue;

				strncpy(tabla_palabra_formada, word, N);
				tabla_orientacion_palabra_formada = h_v;
				marcar_palabra_formada(h_v, word_start, word_end);
				return;
			}
		}
	}
}

static void marcar_palabra_formada(int h_v, int start, int end)
{
	int i;

	switch (h_v)
	{
		case 0:
			for (i = start; i <= end; i++)
				tabla_palabras_encontradas[i_index_palabra_formada][i] = CHR_LETRA_USADA;

			break;
		case 1:
			for (i = start; i <= end; i++)
				tabla_palabras_encontradas[i][j_index_palabra_formada] = CHR_LETRA_USADA;

			break;
	}
}

static void almacenar_ficha_utilizada(char letra, int pos, int *cjugadas, int posplayed[][4])
{
	int tx, ty;

	tx = palabra_formada_obtener_pos_x();
	ty = palabra_formada_obtener_pos_y();

	posicionar_coordenada_letra(&tx, &ty, pos);

	tabla[tx][ty] = letra;
	posplayed[*cjugadas][0] = tx;
	posplayed[*cjugadas][1] = ty;
	posplayed[*cjugadas][3] = letra;
	*cjugadas += 1;
}

static void restablecer_datos_busqueda(void)
{
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			tabla_palabras_encontradas[i][j] = CHR_NULL;

	i_index_palabra_formada = 0;
	j_index_palabra_formada = 0;
}

static void posicionar_coordenada_letra(int *tx, int *ty, int pxy)
{
	if (tabla_orientacion_palabra_formada == 1)
	{
		*tx += pxy;
	}
	else
	{
		*ty += pxy;
	}
}

static void desplazar_fichas_tablero(int *cjugadas, int posplayed[][4])
{
	int i, tx, ty;
	char letra;

	for (i = 0; i <= *cjugadas; i++)
	{
		tx = posplayed[i][0];
		ty = posplayed[i][1];
		letra = posplayed[i][3];

		asignar_cantidad_letra_pozo(letra, -1);

		// gtk
		ui_tablero_nueva_letra(tx, ty);
	}
}

/* ==============================================================================================
 	 FUNCIONES LOCALES DEL ARCHIVO
   ============================================================================================= */

static gboolean verificar_pieza_desplazada_diccionario_ia(int *status, int *cjugadas, int *turno, int posplayed[][4])
{
	int i;

	*status = j_verificar_pieza_desplazada_diccionario;
	verificar_piezas_desplazadas(status, NULL, NULL, cjugadas, posplayed, turno);

	if (*status == j_tabla_devolver_piezas_atril)
	{
		for (i = 0; i <= *cjugadas; i++)
			tabla[posplayed[i][0]][posplayed[i][1]] = CHR_NULL;

		*cjugadas = 0;
		set_err_msg_code(-1, -1);

		return FALSE;
	}

	return TRUE;
}

static int palabra_formada_obtener_pos_x(void)
{
	int x;

	switch (tabla_orientacion_palabra_formada)
	{
		case 0:
			x = i_index_palabra_formada;
			break;

		case 1:
			x = word_start;
			break;
	}

	return x;
}

static int palabra_formada_obtener_pos_y(void)
{
	int y;

	switch (tabla_orientacion_palabra_formada)
	{
		case 0:
			y = word_start;
			break;

		case 1:
			y = j_index_palabra_formada;
			break;
	}

	return y;
}

static gboolean verificar_palabra_formada_plural(void)
{
	char letra;
	letra = tabla_palabra_formada[POS_ULTIMA_LETRA];

	if (obtener_cantidad_letra_pozo('S') <= 0)
		return FALSE;

	if (letra == 'S')
		return FALSE;

	if (!verificar_agregar_letra_final_palabra_formada())
		return FALSE;

	if (!es_una_vocal(letra))
		return FALSE;

	return TRUE;
}

static gboolean verificar_agregar_letra_final_palabra_formada(void)
{
	if (word_end + 1 > N -1)
		return FALSE;

	return TRUE;
}

static gboolean es_una_vocal(char letra)
{
	int i;
	static char vocales[6] = { 'A', 'E', 'I', 'O', 'U' };

	for (i = 0; vocales[i] != '\0'; i++)
	{
		if (letra == vocales[i])
			return TRUE;
	}

	return FALSE;
}

// palabra_contenida_primeras_leras("casabe", "casa")
static gboolean palabra_contenida_primeras_letras(char *str1, char *str2, int *n_letras)
{
	int i, j, resultado;
	size_t longitud_str1, longitud_str2;

	longitud_str1 = strlen(str1);
	longitud_str2 = strlen(str2);

	resultado = longitud_str1 - longitud_str2;
	*n_letras = resultado;

	if (resultado < 0 || resultado == 0)
		return FALSE;

	for (i = 0; i < longitud_str2; i++)
	{
		if (str1[i] != str2[i])
			return FALSE;
	}

	return TRUE;
}

// palabra_contenida_ultimas_leras("becasa", "casa")
static gboolean palabra_contenida_ultimas_letras(char *str1, char *str2, int *n_letras)
{
	int i, j, resultado;
	size_t longitud_str1, longitud_str2;

	longitud_str1 = strlen(str1);
	longitud_str2 = strlen(str2);

	resultado = longitud_str1 - longitud_str2;
	*n_letras = resultado;

	if (resultado < 0 || resultado == 0)
		return FALSE;

	for (i = resultado, j = 0; j < longitud_str2; i++, j++)
	{
		if (str1[i] != str2[j])
			return FALSE;
	}

	if (i < longitud_str1)
		if (str1[i] != '\0')
			return FALSE;

	return TRUE;
}

// palabra contenida en la midad de las letras ("becasabee","casa")
static gboolean palabra_contenida_mitad_letras(char *str1, char *str2, int *n_letras_dir1, int *n_letras_dir2, int indice)
{
	int contador;
	contador = 0;
	char *ptr;

	size_t longitud_str1, longitud_str2,longitud_dir1,longitud_dir2;
	ptr = strstr(g_diccionario[indice], tabla_palabra_formada);

	longitud_str1 = strlen(str1);
	longitud_str2 = strlen(str2);

	while (*ptr != '\0')
	{
		contador++;
		ptr++;
	}

	longitud_dir1 = (longitud_str1) - contador;
	longitud_dir2 = contador - (longitud_str2);

	if (longitud_dir1 == 0 || longitud_dir2 == 0)
		return FALSE;

	*n_letras_dir1 = longitud_dir1;
	*n_letras_dir2 = longitud_dir2;

	return TRUE;

}

static int contador_de_espacios_nulos(int direccion, int *contador_dir1, int *contador_dir2)
{
	int i, contador, contador_Dir1, contador_Dir2;
	contador = 0;

	contador_Dir1 = 0;
	contador_Dir2 = 0;

	switch (tabla_orientacion_palabra_formada)
	{
		case 0: // horizontal
			switch (direccion)
			{
				case 0: // al principio de la palabra
					for (i = word_start - 1; i >= 0; i--)
					{
						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						contador++;
					}

					break;

				case 1: // al final de la palabra
					for (i = word_end + 1; i <= word_end + 7; i++)
					{
						if (i > N - 1)
							break;

						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						contador++;
					}

					break;

				case 2: //en el medio de la palabra
					for (i = word_start - 1; i >= 0; i--)
					{
						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						contador_Dir1++;
					}

					for (i = word_end + 1; i <= word_end + 7; i++)
					{
						if (i > N - 1)
							break;

						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						   contador_Dir2++;
					}

					break;
			}

			break;

		case 1: // vertical
			switch (direccion)
			{
				case 0: // al principio de la palabra
					for (i = word_start - 1; i >= 0; i--)
					{
						if (tabla[i][j_index_palabra_formada] != CHR_NULL)
							break;

						contador++;
					}

					break;

				case 1: // al final de la palabra
					for (i = word_end + 1; i <= word_end + 7; i++)
					{
						if (i > N - 1)
							break;

						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						contador++;
					}

					break;

				case 2: //en el medio de la palabra
					for (i = word_start - 1; i >= 0; i--)
					{
						if (tabla[i][j_index_palabra_formada] != CHR_NULL)
							break;

							contador_Dir1++;
					}

					for (i = word_end + 1; i <= word_end + 7; i++)
					{
						if (i > N - 1)
							break;

						if (tabla[i_index_palabra_formada][i] != CHR_NULL)
							break;

						contador_Dir2++;
					}

					break;
			}

			break;
	}

	if (contador_dir1 != NULL)
		*contador_dir1 = contador_Dir1;

	if (contador_dir2 != NULL)
		*contador_dir2 = contador_Dir2;

	return contador;
}
