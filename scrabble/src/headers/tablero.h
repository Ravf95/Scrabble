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

#ifndef _TABLERO_H
#define _TABLERO_H

// símbolos de los puntos extra
#define S_TRIPLE_PALABRA    		'@'
#define S_DOBLE_PALABRA     		'#'
#define S_TRIPLE_LETRA      		'+'
#define S_DOBLE_LETRA       		'&'

// n - filas y columnas de la tabla del juego
#define N 							15

// numero de letras disponibles en el juego
#define LETRAS_N					29

// cantidad de letras en el pozo (depende de la estructura pozo)
#define NRO_LETRAS_POZO				100

// cantidad de caracteres en el atril
#define ATRIL_CHR 					8 // 0 .. (ATRIL_CHR - 1)

// letras por defecto en el atril
#define ATRIL_CHR_DEFAULT 			"        "

// carácter sin valor en el juego
#define CHR_NULL 					' '

// letras compuestas
#define CHR_RR						'`'
#define CHR_NT						'_'
#define CHR_LL						'^'
#define CHR_CH						']'

// comodin
#define COMODIN						';'

// Funciones
int verificar_letra_atril(char atril[], unsigned int i);
int verificar_posicion_atril_tablero(int x, int y);
int mover_pieza_atril(char atril[], unsigned int i);
int mover_pieza_tablero(int posplayed[][4], int cjugadas);
int verificar_posicion_fichas_tablero(int posplayed[][4], int cjugadas);
int verificar_posicion_fichas_tablero_contiguas(int posplayed[][4], int cjugadas);
int obtener_adversario(int turno);

// Procedimientos
void devolver_piezas_atril(char atril[], int posplayed[][4], int cjugadas);
void sacar_piezas_tablero(int posplayed[][4], int cjugadas);

#endif
