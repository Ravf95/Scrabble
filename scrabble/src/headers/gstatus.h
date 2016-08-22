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

#ifndef _GSTATUS_H
#define _GSTATUS_H

enum j_solicitar
{
	j_solicitar_posc_letra_atril = 1,
	j_solicitar_posc_tabla_x_y,
	j_solicitar_items
};

enum j_verificar
{
	j_verificar_posc_letra_atril = j_solicitar_items,
	j_verificar_posc_tabla_x_y,
	j_verificar_items
};

enum j_tabla
{
	j_tabla_desplazar_ficha_atril = j_verificar_items,
	j_tabla_piezas_horizontal_vertical,
	j_tabla_piezas_contiguas,
	j_tabla_devolver_piezas_atril,
	j_tabla_terminar_jugada,
	j_tabla_items
};

enum j_verificar_pieza_desplazada
{
	j_verificar_pieza_desplazada_casilla_central = j_tabla_items,
	j_verificar_pieza_desplazada_palabra_contigua,
	j_verificar_pieza_desplazada_diccionario,
	j_verificar_pieza_desplazada_items
};

enum j_game
{
	j_game_terminar_turno = j_verificar_pieza_desplazada_items,
	j_game_end
};

// Procedimientos
void solicitar_datos(int *status, char atril[], int posplayed[][4], int cjugadas, GObject *object);
void verficar_datos_ingresados(int *status, char atril[], int posplayed[][4], int cjugadas);
void desplazar_piezas(int *status, char atril[], int posplayed[][4], int *cjugadas, int cturno);
void verificar_piezas_desplazadas(int *status, char atril[][ATRIL_CHR], int *cturno, int *cjugadas, int posplayed[][4], int *turno);
void set_err_msg_code(int errstatus, int errcode);
void gstatus_asignar_orientacion_palabra(int h_v);
void asignar_jugada_pasada(gboolean band);

// Funciones
int get_err_status(void);
char *get_err_msg_code(void);
gboolean verificar_jugada_pasada(void);

#endif
