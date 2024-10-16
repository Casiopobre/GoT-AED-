#ifndef JUEGODETRONOS_H
#define JUEGODETRONOS_H
#include "abb.h"


/*
* Funcion que engade un personaxe a base de datos
* @param *tree Punteiro a arbore na que imos insertar o personaxe
*/
void anhadirPersonaje(TABB *tree);

/*
* Funcion que lista os personaxes por orde alfabetica
* @param tree Arbore na que está guardada a información dos personaxes
*/
void listadoPersonajes(TABB tree);

/*
* Funcion que elimina un personaje
* @param *tree Punteiro a arbore da que imos a eliminar o personaxe
*/
void eliminarPersonaje(TABB *tree);

/*
* Funcion que crea la base de datos deste un archivo
* @param *tree Punteiro a arbore na que imos a gardar os personaxes
* @param argc Número de argumentos recibidos polo main
* @param **argv Vector de strings cos argumentos recibidos polo main
*/
void cargar_archivo(TABB *tree, int argc, char **argv);

#endif //JUEGODETRONOS_H