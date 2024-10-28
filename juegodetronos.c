#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"
#define MAX 30 // Tamaño maximo do nome dun personaxe
#define BUF 512 // Tamaño do buffer para ler cada liña do arquivo
#define AUXMAX 256 // Tamaño maximo da informacion de casa lista

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
        suprimirElementoAbb(tree, character);
    else
        printf("Personaxe non atopado\n");
    
    free(name);
}

// Función auxiliar que toma unha cadea separada por comas e garda os seus elementos nunha lista
void _procesarCadena(TLISTA *list, char string[AUXMAX]){
    TIPOELEMENTOLISTA aux;
    char *token = strtok(string, ",");
    while(token != NULL){
        if(strcmp(token, "-") != 0){
            strcpy(aux.nameP, token);
            insertarElementoLista(list, finLista(*list), aux);
        }
        token = strtok(NULL, ",");
    }
}

void cargarArchivo(TABB *tree, int argc, char **argv){
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
        // Engadimos o personaxe á árbore
        insertarElementoAbb(tree, character);
    }
    fclose(file);  
}

void _imprimirListaArquivo(TLISTA list, FILE* file){
    TPOSICION pos = primeroLista(list);
    TIPOELEMENTOLISTA name;
    while(pos != finLista(list)){
        if(pos!=primeroLista(list)){
            fprintf(file, ",");
        }
        recuperarElementoLista(list, pos, &name);
        fprintf(file, "%s", name.nameP);
        pos = siguienteLista(list, pos);
    }
}

void _imprimirPersonaxeArquivo(TABB tree, FILE* file){
    TIPOELEMENTOABB character;
    if(!esAbbVacio(tree)){
        
        _imprimirPersonaxeArquivo(izqAbb(tree), file);
        _imprimirPersonaxeArquivo(derAbb(tree), file);

        leerElementoAbb(tree, &character);
        
        fprintf(file, "%s|%s|%d|", character.name, character.house, character.royal);
        if(esListaVacia(character.parents))
            fprintf(file, "-");
        else
            _imprimirListaArquivo(character.parents, file);
        fprintf(file, "|");
        if(esListaVacia(character.siblings))
            fprintf(file, "-");
        else
            _imprimirListaArquivo(character.siblings, file);
        fprintf(file, "|");
        if(esListaVacia(character.killed))
            fprintf(file, "-");
        else
            _imprimirListaArquivo(character.killed, file);
        fprintf(file, "|\n");
    }
}

void guardarDatos(TABB tree, int argc, char **argv){
    FILE *file;
    char fileName[MAX];

    if(argc>=3){
        printf("Actualizando o arquivo inicial...\n");
        file = fopen(argv[2], "w");
    } else{
        printf("Introduza o nome co que quere gardar o arquivo de personaxes: \n");
        scanf(" %[^\n\r]", fileName);
        file = fopen(fileName, "w");
    }
    if(file == NULL){
        perror("Erro na apertura do arquivo. \n");
        exit(EXIT_FAILURE);
    }

    _imprimirPersonaxeArquivo(tree, file);
    fclose(file);
    printf("Arquivo creado/actualizado con éxito :)\n");
}

void _auxFindMurderer(TABB tree, char *murderedChar, char (*murdererArray)[MAX], int *arrayPos){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxChar;
    // Recorremos a arbore
    if(!esAbbVacio(tree)){
        _auxFindMurderer(izqAbb(tree), murderedChar, murdererArray, arrayPos);
        leerElementoAbb(tree, &character);
        TPOSICION pos = primeroLista(character.killed);
        while(pos != finLista(character.killed)){
            recuperarElementoLista(character.killed, pos, &auxChar);
            if(strcmp(auxChar.nameP, murderedChar) == 0){
                strcpy(murdererArray[*arrayPos], character.name);
                (*arrayPos)++;
            }
            pos = siguienteLista(character.killed, pos);
        }
        _auxFindMurderer(derAbb(tree), murderedChar, murdererArray, arrayPos);
    }
}

void buscarAsesino(TABB tree){
    char murderedChar[MAX];
    char murdererArray[MAX][MAX];
    int arrayPos = 0;

    printf("Introduza o nome do personaxe do que quere atopar o(s) asesino(s): \n");
    scanf(" %[^\n\r]", murderedChar);

    _auxFindMurderer(tree, murderedChar, murdererArray, &arrayPos);

    // Imprimimos a array de asasinos
    if(arrayPos == 0){
        printf("Non se atoparon asasinos para %s\n", murderedChar);
        return;
    }
    printf("Asasino(s) de %s: ", murderedChar);
    for(int i = 0; i < arrayPos; i++){
        printf("%s, ", murdererArray[i]);
    }
}

void _auxFindParent(TABB tree, char *parent, char (*childArray)[MAX], int *arrayPos){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxChar;
    // Recorremos a arbore
    if(!esAbbVacio(tree)){
        _auxFindParent(izqAbb(tree), parent, childArray, arrayPos);
        leerElementoAbb(tree, &character);
        // Recorremos a lista de pais do personaxe
        TPOSICION pos = primeroLista(character.parents); 
        while(pos != finLista(character.parents)){
            recuperarElementoLista(character.parents, pos, &auxChar);
            if(strcmp(auxChar.nameP, parent) == 0){
                strcpy(childArray[*arrayPos], character.name);
                (*arrayPos)++;
            }
            pos = siguienteLista(character.parents, pos);
        }
        _auxFindParent(derAbb(tree), parent, childArray, arrayPos);
    }
}

void buscarHijos(TABB tree){
    char parent[MAX];
    char childArray[MAX][MAX];
    int arrayPos = 0;

    printf("Introduza o nome do personaxe do que quere atopar o(s) fillo/a(s): \n");
    scanf(" %[^\n\r]", parent);

    _auxFindParent(tree, parent, childArray, &arrayPos);

    // Imprimimos a array de asasinos
    if(arrayPos == 0){
        printf("Non se atoparon fillos/as para %s\n", parent);
        return;
    }
    printf("Fillo/a(s) de %s: ", parent);
    for(int i = 0; i < arrayPos; i++){
        printf("%s, ", childArray[i]);
    }
}

int _countMurdered(TLISTA list){
    if(!esListaVacia(list)){
        int n = longitudLista(list);
        return n;
    }
    return 0;
}

int _auxMaxKills(TABB tree, int *maxMurdered){
    TIPOELEMENTOABB character;
    int auxMaxMurdered;
    if(!esAbbVacio(tree)){
        _auxMaxKills(izqAbb(tree), maxMurdered);
        leerElementoAbb(tree, &character);
        auxMaxMurdered = _countMurdered(character.killed);
        if(auxMaxMurdered > *maxMurdered){
            *maxMurdered = auxMaxMurdered;
        }
        _auxMaxKills(derAbb(tree), maxMurdered);
    }
    return *maxMurdered;
}

void _auxKillers(TABB tree, char (*killerArray)[MAX], int maxKills, int *counter){
    TIPOELEMENTOABB character;
    if(!esAbbVacio(tree)){
        _auxKillers(izqAbb(tree), killerArray, maxKills, counter);
        leerElementoAbb(tree, &character);
        if(maxKills == _countMurdered(character.killed)){
            strcpy(killerArray[*counter], character.name);
            (*counter)++;
        }
        _auxKillers(derAbb(tree), killerArray, maxKills, counter);

    }
}

void buscarKiller(TABB tree){
    int maxMurdered = 0;
    char killerArray[MAX][MAX];
    int counter = 0;

    int maxKills = _auxMaxKills(tree, &maxMurdered);
    _auxKillers(tree, killerArray, maxKills, &counter);

    printf("O(s) personaxe(s) con mais asasinatos (%d baixas confirmadas) e/son: ", maxKills);
    for(int i = 0; i < counter; i++){
        printf("%s, ", killerArray[i]);
    }
}
