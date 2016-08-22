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

#ifndef _WORD_H
#define _WORD_H

// Funciones
char *obtener_palabra_formada(int h_v, int tx, int ty, int *ptrStart, int *ptrEnd);
int obtener_valor_letra(char c);
int obtener_orientacion_letra(int tx, int ty);
int obtener_valor_palabra(int h_v, int cjugadas, int ax, int ay, int start, char *word, int posplayed[][4]);
int obtener_numero_letras_pozo(void);
int obtener_valor_letras_atril(char *atril);
char obtener_letra_pozo_index(int index);
int obtener_cantidad_letra_pozo(char letra);

// Procedimientos
void repartir_letras_atril(char atril[]);
void cambiar_letras_atril(char *cf_atril, char *atril);
void asignar_cantidad_letra_pozo(char letra, int cantidad);

#endif
