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

#ifndef _GUICALLBACK_H
#define _GUICALLBACK_H

// callbacks
void tablero_press_callback(GObject *object, GdkEvent *event, gpointer data);
void atril_press_callback(GObject *object, GdkEvent *event, gpointer data);
void boton_terminar_pasar_jugada_press_callback(GtkButton *button, gpointer data);
void boton_traer_cambiar_fichas_press_callback(GtkButton *button, gpointer data);
void menuitem_como_jugar_press_callback(GtkMenuItem *item, gpointer data);
void menuitem_acerca_de_press_callback(GtkMenuItem *item, gpointer data);

// ventana: nuevo juego
void menuitem_nuevo_juego_press_callback(GtkMenuItem *item, gpointer data);
void window_nuevo_juego_salir(GtkWidget *widget, GdkEvent *event, gpointer data);
void boton_cancelar_window_nuevo_juego(GtkButton *button, gpointer data);
void boton_aceptar_window_nuevo_juego(GtkButton *button, gpointer data);

// ventana: comodin
void window_comodin_salir(GtkWidget *widget, GdkEvent *event, gpointer data);
void boton_aceptar_comodin_press_callback(GtkButton *button, gpointer data);
void boton_cancelar_comodin_press_callback(GtkButton *button, gpointer data);

// ventana: cambiar fichas
void window_cambiar_fichas_salir(GtkWidget *widget, GdkEvent *event, gpointer data);
void boton_aceptar_cambiar_fichas_press_callback(GtkButton *button, gpointer data);
void boton_cancelar_cambiar_fichas_press_callback(GtkWidget *widget, GdkEvent *event, gpointer data);
void cambiar_letras_atril_press_callback(GObject *object, GdkEvent *event, gpointer data);


// funciones
int obtener_tipo_jugador_turno_actual(void);

#endif
