#include <stdio.h>
#include <stdlib.h>
#include "juegodetronos.h"
#include "abb.h"

int main(int argc, char **argv){
    char option;
    TABB tree;
    // Creamos a arbore
    crearAbb(&tree);
    cargarArchivo(&tree, argc, argv);
    
    do{
        printf("\033[1;35m");
        printf("\n--------------------------------------------------------");
        printf("\n~*~*~*~*~*~*~*~ Seleccione unha opción ~*~*~*~*~*~*~*~\n");
        printf("\na) Engadir personaxe\n");
        printf("\nl) Listado alfabetico de personaxes\n");
        printf("\ne) Eliminar personaxe\n");
        printf("\nb) Buscar asasino/a(s)\n");
        printf("\nh) Buscar fillo/a(s)\n");
        printf("\nk) Buscar killer\n");
        printf("\nm) Modificar personaxe\n");
        printf("\ns) Saír\n");
        printf("--------------------------------------------------------\n");
        printf("\033[1;0m");
        printf("\nOpcion:");
        scanf(" %c", &option);
        switch (option){
        case 'a': case 'A':
            anhadirPersonaje(&tree);
            break;
        case 'l': case 'L':
            listadoPersonajes(tree);
            break;
        case 'e': case 'E':
            eliminarPersonaje(&tree);
            break;
        case 'b': case 'B':
            buscarAsesino(tree);
            break;
        case 'h': case 'H':
            buscarHijos(tree);
            break;
        case 'k': case 'K':
            buscarKiller(tree);
            break;
        case 'm': case 'M':
            modificarPersonaje(&tree);
            break;
        case 's': case 'S':
            // Gardamos os datos nun arquivo
            guardarDatos(tree, argc, argv);
            // Liberamos a memoria
            destruirAbb(&tree);
            break;
        default:
            printf("Opcion non valida :(");
            break;
        }
    } while(option != 's' && option != 'S');
    
    return EXIT_SUCCESS;
}