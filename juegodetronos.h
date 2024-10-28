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
* Funcion que crea a base de datos dende un arquivo. Devolve 1 se se lle introduce un arquivo, e 0 en caso contrario
* @param *tree Punteiro a arbore na que imos a gardar os personaxes
* @param argc Número de argumentos recibidos polo main
* @param **argv Vector de strings cos argumentos recibidos polo main
*/
void cargarArchivo(TABB *tree, int argc, char **argv);

/*
* Funcion que garda os datos nun ficheiro
* @param *tree Punteiro a arbore na que imos a gardar os personaxes
* @param argc Número de argumentos recibidos polo main
* @param **argv Vector de strings cos argumentos recibidos polo main
*/
void guardarDatos(TABB tree, int argc, char **argv);

/*
* Funcion que busca o asasino un personaxe na base de datos
* @param tree Arbore na que imos a buscar o asasino
*/
void buscarAsesino(TABB tree);

/*
* Funcion que busca os fillos dun personaxe na base de datos
* @param tree Arbore na que imos a buscar os fillos
*/
void buscarHijos(TABB tree);

/*
* Funcion que busca o personaxe con mais asasinatos
* @param tree Arbore na que temos a base de datos
*/
void buscarKiller(TABB tree);

#endif //JUEGODETRONOS_H