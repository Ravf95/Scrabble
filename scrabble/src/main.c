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
extern const char *g_user_home_dir;

/* ============================================================================================== */

int main(void)
{
	int i, j;

	g_user_home_dir = getenv("HOME");

	if (g_user_home_dir == NULL)
	{
		printf("ERROR: main.c > main : No se puedo obtener el directorio home del usuario\n");
		exit(-1);
	}

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			tabla[i][j] = CHR_NULL;

	// Inicializar generador de números pseudo-aleatorios con la semilla tiempo
	srand(time(NULL));

	reservar_memoria_dicc();
    start_game_gtkgui();
    destruir_memoria_reservada_dicc();
    cerrar_archivo_registro();

    return 0;
}
