#ifndef JUEGODETRONOS_H
#define JUEGODETRONOS_H
#include "abb.h"


/*
* Funcion que engade un personaxe a base de datos
* @param *tree Punteiro a arbore na que imor insertar o personaxe
*/
void anhadirPersonaje(TABB *tree);

/*
* Funcion que lista os personaxes por orde alfabetica
* @param tree Punteiro a arbore na que imor insertar o personaxe
*/
void listadoPersonajes(TABB tree);

#endif //JUEGODETRONOS_H