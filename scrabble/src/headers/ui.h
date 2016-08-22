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

#ifndef _UI_H
#define _UI_H

// procedimientos
void ui_atril_letra_vacia(int i, char letra);
void ui_atril_nuevas_letras(char *atril);
void ui_cf_atril_copiar(char *atril);
void ui_cf_letra_atril_seleccionada(int i);
void ui_tablero_nuevo(void);
void ui_tablero_nueva_letra(int i, int j);
void ui_tablero_letra_vacia(int i, int j);
void ui_cambiar_texto_label(int cjugadas);
void quick_message(gchar *message, GtkWindow *parent);

void ui_change_text_statusbar_name_score(void);
void ui_change_text_pozo_nletras(int n);
void ui_msg_turno(int turno, int first);
void ui_main_widgets_status(int status, int statusbar);
void ui_change_text_turno_actual(int turno);

#endif
