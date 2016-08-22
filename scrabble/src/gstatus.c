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
extern int player_score[2];
extern const char *g_user_home_dir;

/* ==============================================================================================
 	 VARIABLES LOCALES DEL ARCHIVO
   ============================================================================================= */

static int i_orientacion;
static int errCode;
static int errStatus = -1;
static gboolean jugada_pasada;

/* ==============================================================================================
 	 PROCEDIMIENTOS
   ============================================================================================= */

// obtener los datos ingresados por el jugador
// - posición de la letra a mover del atril
// - fila del tablero seleccionada para mover la letra
// - columna del tablero seleccionada para mover la letra
void solicitar_datos(int *status, char atril[], int posplayed[][4], int cjugadas, GObject *object)
{
	int aux;

	switch (*status)
	{
		case j_solicitar_posc_letra_atril:
			aux = GPOINTER_TO_INT(g_object_get_data(object, "x_atril")) - 1;
			posplayed[cjugadas][2] = aux;

			// ¿la casilla seleccionada es comodín?
			if (atril[aux] == COMODIN)
			{
				gtk_widget_show_all(GTK_WIDGET(COMODIN_WINDOW));
				break;
			}

			posplayed[cjugadas][3] = atril[aux];
			*status = j_verificar_posc_letra_atril;

			break;

		case j_solicitar_posc_tabla_x_y:
			posplayed[cjugadas][0] = GPOINTER_TO_INT(g_object_get_data(object, "x_tablero")) - 1;
			posplayed[cjugadas][1] = GPOINTER_TO_INT(g_object_get_data(object, "y_tablero")) - 1;
			*status = j_verificar_posc_tabla_x_y;

			break;
	}
}

// controlar los datos ingresados por el jugador
// - posición de la letra a mover del atril
// - fila del tablero seleccionada para mover la letra
// - columna del tablero seleccionada para mover la letra
void verficar_datos_ingresados(int *status, char atril[], int posplayed[][4], int cjugadas)
{
	int aux;

	switch (*status)
	{
		case j_verificar_posc_letra_atril:
			aux = verificar_letra_atril(atril, posplayed[cjugadas][2]);
			*status = j_solicitar_posc_tabla_x_y;

			if (aux)
				*status = j_solicitar_posc_letra_atril;

			break;

		case j_verificar_posc_tabla_x_y:
			aux = verificar_posicion_atril_tablero(posplayed[cjugadas][0], posplayed[cjugadas][1]);
			*status = j_tabla_desplazar_ficha_atril;

			if (aux != 0)
				*status = j_solicitar_posc_tabla_x_y;

			break;
	}
}

// realizar los correspondientes movimientos
// en el tablero del juego con los datos
// ingresados por el jugador
void desplazar_piezas(int *status, char atril[], int posplayed[][4], int *cjugadas, int cturno)
{
	int aux;

	switch (*status)
	{
		case j_tabla_desplazar_ficha_atril:
			*status = j_solicitar_posc_letra_atril;

			aux = *cjugadas;
			mover_pieza_tablero(posplayed, aux);
			mover_pieza_atril(atril, posplayed[aux][2]);
			*cjugadas += 1;

			// gtk
			ui_cambiar_texto_label(*cjugadas);

			// ¿el siguiente número de jugada
			// excede el número de posiciones del atril?
			if (*cjugadas >= ATRIL_CHR - 1)
				*status = j_tabla_terminar_jugada;

			break;

		case j_tabla_terminar_jugada:
			*status = j_tabla_piezas_horizontal_vertical;

			// ¿desplazo todas las fichas en el tablero?
			if (*cjugadas >= ATRIL_CHR - 1)
			{
				*cjugadas = ATRIL_CHR - 2;
			}
			else
			{
				*status = j_game_terminar_turno;

				// ¿realizo alguna jugada en el turno actual?
				if (*cjugadas > 0)
				{
					*cjugadas -= 1; // el siguiente movimiento esta incompleto
					*status = j_tabla_piezas_horizontal_vertical;
					break;
				}

				// ¿es el primer turno y no desplazo ninguna ficha?
				if (cturno == 0)
				{
					*status = j_solicitar_posc_letra_atril;

					errCode = 1;
					errStatus = *status;

					break;
				}

				jugada_pasada = TRUE;

				// contar los turnos pasados
				contar_turno_pasado(cturno);
			}

			break;

		case j_tabla_piezas_horizontal_vertical:
			aux = verificar_posicion_fichas_tablero(posplayed, *cjugadas);
			*status = j_tabla_piezas_contiguas;

			// ¿las fichas no están colocadas de manera vertical/horizontal?
			if (aux == 2)
			{
				*status = j_tabla_devolver_piezas_atril;

				// ...
				errCode = 2;
				errStatus = *status;
				// ...
				break;
			}

			i_orientacion = aux;

			break;

		case j_tabla_piezas_contiguas:
			aux = verificar_posicion_fichas_tablero(posplayed, *cjugadas);
			aux = verificar_posicion_fichas_tablero_contiguas(posplayed, *cjugadas);
			*status = j_verificar_pieza_desplazada_casilla_central;

			// ¿no son contiguas?
			if (aux == 1)
			{
				*status = j_tabla_devolver_piezas_atril;

				// ...
				errCode = 1;
				errStatus = *status;
				// ...
				break;
			}

			break;

		case j_tabla_devolver_piezas_atril:
			devolver_piezas_atril(atril, posplayed, *cjugadas);
			sacar_piezas_tablero(posplayed, *cjugadas);

			*cjugadas = 0;
			*status = j_solicitar_posc_letra_atril;

			// gtk
			ui_cambiar_texto_label(*cjugadas);

			break;
	}
}

// verifica los correspondientes movimientos
// en el tablero del juego con los datos
// ingresados por el jugador
void verificar_piezas_desplazadas(int *status, char atril[][ATRIL_CHR], int *cturno, int *cjugadas, int posplayed[][4], int *turno)
{
	int aux;

	switch (*status)
	{
		case j_verificar_pieza_desplazada_casilla_central:
			*status = j_verificar_pieza_desplazada_palabra_contigua;

			// ¿el primer turno?
			if (*cturno == 0)
			{
				// ¿el jugador coloco una ficha en el centro del tablero?
				for (aux = 0; aux <= *cjugadas; aux++)
				{
					if (posplayed[aux][0] == 7 && posplayed[aux][1] == 7)
					{
						aux = -1;
						break;
					}
				}

				// ¿no coloco una ficha en el centro?
				if (aux != -1)
				{
					*status = j_tabla_devolver_piezas_atril;

					// ...
					errCode = 3;
					errStatus = *status;
					// ...
				}
			}

			break;

		case j_verificar_pieza_desplazada_palabra_contigua:
		{
			*status = j_verificar_pieza_desplazada_diccionario;

			// ¿el primer turno?
			if (*cturno == 0)
				break;

			int i, h_v, contador;
			char *word = NULL;

			word = obtener_palabra_formada(i_orientacion, posplayed[0][0], posplayed[0][1], NULL, NULL);
			aux = strlen(word);

			// ¿la longitud de la palabra que coloco
			// es igual a la palabra formada?
			if (aux - 1 == *cjugadas)
			{
				h_v = (i_orientacion == 0) ? 1 : 0;
				contador = -1;

				// ¿forma alguna palabra en la dirección opuesta
				// en la que fue colocada?
				for (i = 0; i <= *cjugadas; i++)
				{
					word = obtener_palabra_formada(h_v, posplayed[i][0], posplayed[i][1], NULL, NULL);

					if (word[1] == '\0')
						contador++;
				}

				if (contador == *cjugadas)
					aux = -1;
			}

			if (aux == -1)
			{
				*status = j_tabla_devolver_piezas_atril;

				// ...
				errCode = 4;
				errStatus = *status;
				// ...

				break;
			}

			break;
		}

		case j_verificar_pieza_desplazada_diccionario:
		{
			int i, puntaje, start, h_v;
			char *word = NULL;

			puntaje = 0;
			*status   = j_game_terminar_turno;

			word = obtener_palabra_formada(i_orientacion, posplayed[0][0], posplayed[0][1], &start, NULL);
			aux  = verficar_palabra_dicccionario(word);

			// ¿la palabra existe en el diccionario?
			if (aux)
			{
				puntaje = obtener_valor_palabra(i_orientacion, *cjugadas, posplayed[0][0], posplayed[0][1], start, word, posplayed);

				// cambiar la orientación de la palabra
				h_v = (i_orientacion == 0) ? 1 : 0;

				// buscar otras palabras en la orientación contraria
				for (i = 0; i <= *cjugadas; i++)
				{
					word = obtener_palabra_formada(h_v, posplayed[i][0], posplayed[i][1], &start, NULL);

					// si word solo contiene la letra de
					// partida omitir el mismo

					if (word[0] == '\0')
						continue;

					if (word[1] == '\0')
						continue;

					aux = verficar_palabra_dicccionario(word);

					if (!aux)
						break;

					puntaje += obtener_valor_palabra(h_v, *cjugadas, posplayed[i][0], posplayed[i][1], start, word, posplayed);
				}
			}

			// ¿utilizo todas las letras de su atril?
			if (*cjugadas == 6)
				puntaje += 50;

			if (!aux)
			{
				*status = j_tabla_devolver_piezas_atril;

				// ...
				errCode   = 5;
				errStatus = *status;
				// ...

				break;
			}

			player_score[*turno] += puntaje;

			break;
		}

		case j_game_terminar_turno:
		{
			// verificar fin del juego
			if (check_end_game(*cjugadas, atril))
			{
				obtener_puntacion_ultima_jugada(atril);
				ui_change_text_statusbar_name_score();
				ui_change_text_turno_actual(-1);
				ui_main_widgets_status(0, 0);
				obtener_ganador();

				*status = j_game_end;

				cerrar_archivo_registro();

				// mostar historial de fichas usadas
				char file[PATH_MAX];
				gboolean show;
				snprintf(file, sizeof(file), "file://%s%s", g_user_home_dir, ARCHIVO_REGISTRO_NOMBRE);

				show = gtk_show_uri
				(
						gdk_screen_get_default(),
						file,
						gtk_get_current_event_time(),
						NULL
				);

				if (show)
					gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(MAIN_WINDOW)), NULL);

				break;
			}

			// registar jugada
			registro_juego_actual_nueva_jugada(*cturno, *turno, posplayed, *cjugadas);

			// completar las letras del
			// actual jugador
			if (obtener_tipo_jugador_turno_actual() == 0)
				repartir_letras_atril(atril[*turno]);

			*status 		 = j_solicitar_posc_letra_atril;
			*cjugadas		 = 0;
			*cturno   		+= 1;
			*turno 		 	 = obtener_adversario(*turno);

			errStatus = -1;

			if (obtener_tipo_jugador_turno_actual() == 0)
				ui_msg_turno(*turno, 0);

			ui_cambiar_texto_label(*cjugadas);
			ui_change_text_turno_actual(*turno);
			ui_change_text_statusbar_name_score();

			if (obtener_tipo_jugador_turno_actual() == 0)
				ui_atril_nuevas_letras(atril[*turno]);

			// ia
			if (obtener_tipo_jugador_turno_actual() == 1)
				ia_jugar(atril, cturno, turno, cjugadas, posplayed, status);

			jugada_pasada = FALSE;

			break;
		}
	}
}

void set_err_msg_code(int errstatus, int errcode)
{
	errStatus = errstatus;
	errCode	= errcode;
}

void gstatus_asignar_orientacion_palabra(int h_v)
{
	i_orientacion = h_v;
}

void asignar_jugada_pasada(gboolean band)
{
	jugada_pasada = TRUE;
}
/* ==============================================================================================
 	 FUNCIONES
   ============================================================================================= */

gboolean verificar_jugada_pasada(void)
{
	return jugada_pasada;
}

int get_err_status(void)
{
	return errStatus;
}


char *get_err_msg_code(void)
{
	static char err[128];
	size_t t = 0;

	switch (errStatus)
	{
		case j_tabla_devolver_piezas_atril:
			switch (errCode)
			{
				case 1: // las piezas no son contiguas
					t = sizeof(D_INV_LETRA_CONTIGUA);
					strncpy(err, D_INV_LETRA_CONTIGUA, t);
					break;

				case 2: // las piezas no estan colocadas de manera h/v
					t = sizeof(D_INV_LETRA_TABLERO_HV);
					strncpy(err, D_INV_LETRA_TABLERO_HV, t);
					break;

				case 3:	// primer turno: el jugador no coloco una de sus fichas en el centro
					t = sizeof(D_LETRAS_POS_CENTRO_PRIMER_TURNO);
					strncpy(err, D_LETRAS_POS_CENTRO_PRIMER_TURNO, t);
					break;

				case 4: // las letras colocadas no son contiguas a otra palabra en el tablero
					t = sizeof(D_PALABRA_CONTIGUA);
					strncpy(err, D_PALABRA_CONTIGUA, t);
					break;

				case 5: // la palabra no existe en el diccionario
					t = sizeof(D_PALABRA_DICC);
					strncpy(err, D_PALABRA_DICC, t);
					break;
			}

			break;

		case j_solicitar_posc_letra_atril:
			if (errCode == 1)
			{
				t = sizeof(D_PRIMER_TURNO);
				strncpy(err, D_PRIMER_TURNO, t);
				break;
			}

			break;
	}

	err[t] = '\0';

	return err;
}
