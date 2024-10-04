#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "lista.h"

void anhadirPersonaje(){
    TIPOELEMENTOABB *character = malloc(sizeof(TIPOELEMENTOABB));
    printf("\nIntroduza o nome do personaxe a engadir: ");
    scanf("%s", character->name);
    while(strcmp(character->name, "fin") != 0){
        printf("\nIntroduza o nome da casa (? se e descoñecido): ");
        scanf("%s", character->house);
        printf("\nIntroduza se o seu personaxe e ou non da realeza (0 = si; 1 = no): ;");
        scanf("%s", &character->royal);
    }
}
