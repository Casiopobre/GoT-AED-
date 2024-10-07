#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"

void anhadirPersonaje(TABB *arbol){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxName;
    crearLista(&character.parents);
    crearLista(&character.siblings);
    crearLista(&character.killed);

    printf("\nIntroduza o nome do personaxe a engadir: ");
    scanf("%s", character.name);

    printf("\nIntroduza o nome da casa (? se e descoñecido): ");
    scanf("%s", character.house);

    printf("\nIntroduza se o seu personaxe e ou non da realeza (0 = no; 1 = si): ");
    scanf("%d", &character.royal);

    printf("\nIntroduza a lista dos nomes dos pais do seu personaxe ('fin' para terminar): ");
    do{
        scanf("%[^\n\r]", auxName);
        insertarElementoLista(&character.parents, finLista(character.parents), auxName);
    } while (strcmp(&auxName, "fin") != 0);

    printf("\nIntroduza a lista de irmans e irmas do seu personaxe ('fin' para terminar): ");
    do{
        scanf("%[^\n\r]", auxName);
        insertarElementoLista(&character.siblings, finLista(character.siblings), auxName);
    } while (strcmp(&auxName, "fin") != 0);

    printf("\nIntroduza a lista de persoas asasinadas polo seu personaxe ('fin' para terminar):");
    do{
        scanf("%[^\n\r]", auxName);
        insertarElementoLista(&character.killed, finLista(character.killed), auxName);
    } while (strcmp(&auxName, "fin") != 0);

    
        
}
