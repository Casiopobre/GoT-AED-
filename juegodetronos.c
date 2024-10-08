#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"

void anhadirPersonaje(TABB *tree){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxName;
    crearLista(&character.parents);
    crearLista(&character.siblings);
    crearLista(&character.killed);

    printf("\nIntroduza o nome do personaxe a engadir: ");
    scanf(" %[^\n\r]", character.name);

    printf("\nIntroduza o nome da casa (? se e descoñecido): ");
    scanf(" %[^\n\r]", character.house);

    printf("\nIntroduza se o seu personaxe e ou non da realeza (0 = no; 1 = si): ");
    scanf("%d", &character.royal);

    printf("\nIntroduza a lista dos nomes dos proxenitores do seu personaxe ('fin' para terminar): ");
    do{
        scanf(" %[^\n\r]", auxName);
        insertarElementoLista(&character.parents, finLista(character.parents), auxName);
    } while (strcmp(auxName.nameP, "fin") != 0);

    printf("\nIntroduza a lista de irmans e irmas do seu personaxe ('fin' para terminar): ");
    do{
        scanf(" %[^\n\r]", auxName);
        insertarElementoLista(&character.siblings, finLista(character.siblings), auxName);
    } while (strcmp(auxName.nameP, "fin") != 0);

    printf("\nIntroduza a lista de persoas asasinadas polo seu personaxe ('fin' para terminar):");
    do{
        scanf(" %[^\n\r]", auxName);
        insertarElementoLista(&character.killed, finLista(character.killed), auxName);
    } while (strcmp(auxName.nameP, "fin") != 0);

    insertarElementoAbb(tree, character);
}

void _imprimirLista(TLISTA list){
    TIPOELEMENTOLISTA element;
    TPOSICION position = primeroLista(list);

    // Comprobacion de que a lista non estea vacia
    if(esListaVacia(list)){
        perror("A lista está vacía :(\n");
        return;
    }

    // Recorremos a lista imprimindo os seus elementos
    while(position != finLista(list)){
        recuperarElementoLista(list, position, &element);
        printf("%s, ", element);
        position = siguienteLista(list, position);
    }
}

void _imprimirPersonaje(TIPOELEMENTOABB character){
    printf("%s\n", character.name);
    printf("\tCasa: %s\n", character.house);
    if(character.royal)
        printf("\tRealeza: si\n");
    else
        printf("\tRealeza: no\n");
    printf("\tProxenitores: ");
    _imprimirLista(character.parents);
    printf("\n\tIrmans/irmás: ");
    _imprimirLista(character.siblings);
    printf("\n\tPersoas asasinadas: ");
    _imprimirLista(character.killed);
}

void listadoPersonajes(TABB tree){
    TIPOELEMENTOABB character;
    if(!esAbbVacio(tree)){
        listadoPersonajes(izqAbb(tree));
        leerElementoAbb(tree, &character);
        _imprimirPersonaje(character);
        listadoPersonajes(derAbb(tree));
    }
}

void eliminarPersonaje(TABB *tree){
    char *name;
    printf("\nInserte o nome do personaxe a eliminar: ");
    scanf(" %[^\n\r]", name);
}
