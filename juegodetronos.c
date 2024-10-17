#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"
#define MAX 30
#define BUF 512
#define AUXMAX 256

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
    do {
        scanf(" %[^\n\r]", auxName.nameP);
        if((strcmp(auxName.nameP, "fin") != 0))
            insertarElementoLista(&character.parents, finLista(character.parents), auxName);

    } while (strcmp(auxName.nameP, "fin") != 0);


    printf("\nIntroduza a lista de irmans e irmas do seu personaxe ('fin' para terminar): ");
    do{
        scanf(" %[^\n\r]", auxName.nameP);
        if(strcmp(auxName.nameP, "fin") != 0)
            insertarElementoLista(&character.siblings, finLista(character.siblings), auxName);
    } while (strcmp(auxName.nameP, "fin") != 0);

    printf("\nIntroduza a lista de persoas asasinadas polo seu personaxe ('fin' para terminar):");
    do{
        scanf(" %[^\n\r]", auxName.nameP);
        if(strcmp(auxName.nameP, "fin") != 0)
            insertarElementoLista(&character.killed, finLista(character.killed), auxName);
    } while (strcmp(auxName.nameP, "fin") != 0);

    insertarElementoAbb(tree, character);
}

void _imprimirLista(TLISTA list){
    TIPOELEMENTOLISTA element;
    TPOSICION position = primeroLista(list);

    // Recorremos a lista imprimindo os seus elementos
    while(position != finLista(list)){
        recuperarElementoLista(list, position, &element);
        printf("%s, ", element.nameP);
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
    printf("\n");
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
    TIPOCLAVE name = malloc(sizeof(char) * MAX);
    if(name == NULL){
        perror("Erro reservando memoria para o nome\n");
        return;
    }
    TIPOELEMENTOABB character;

    printf("\nInserte o nome do personaxe a eliminar: ");
    scanf(" %[^\n\r]", name);

    buscarNodoAbb(*tree, name, &character);
    if(esMiembroAbb(*tree, character))
        suprimirElementoAbb(tree, character); // DA DOUBLE FREE AYUDA
    else
        printf("Personaxe non atopado\n");
    
    free(name);
}

// Función auxiliar que toma unha cadea separada por comas e garda os seus elementos nunha lista
void _procesarCadena(TLISTA *list, char string[AUXMAX]){
    TIPOELEMENTOLISTA aux;
    char *token = strtok(string, ",");
    while(token != NULL){
        //printf("%s ", token);
        strcpy(aux.nameP, token);
        insertarElementoLista(list, finLista(*list), aux);
        token = strtok(NULL, ",");
    }
}

void cargar_archivo(TABB *tree, int argc, char **argv){
    // Comprobación de que haxa 3 argumentos
    if(argc < 3){
        printf("Numero de argumentos menor que 3. Traballando sen arquivo...\n");
        return;
    }
    // Comprobción de que o modo introducido é correcto
    if(strcmp("-f", argv[1]) != 0){
        printf("Modo introducido incorrecto. Por favor, introduza -f se quere traballar cun arquivo.\n");
        exit(EXIT_FAILURE);
    }
    // Comprobación de que o arquivo se abre correctamente
    FILE* file = fopen(argv[2], "r");
    if(file == NULL){
        perror("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }
    // Lemos o arquivo e gardamos os datos
    char buffer[BUF];
    TIPOELEMENTOABB character;
    char auxParents[AUXMAX], auxSiblings[AUXMAX], auxKilled[AUXMAX];
    fseek(file, 0, SEEK_SET);
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        sscanf(buffer, "%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|\n", character.name, character.house, &character.royal, auxParents, auxSiblings, auxKilled);
        // Gardamos os elementos das listas
        crearLista(&character.parents);
        crearLista(&character.siblings);
        crearLista(&character.killed);
        _procesarCadena(&character.parents, auxParents);
        _procesarCadena(&character.siblings, auxSiblings);
        _procesarCadena(&character.killed, auxKilled);
        // Añadimos o personaxe á árbore
        insertarElementoAbb(tree, character);
    }
    fclose(file);    
}
