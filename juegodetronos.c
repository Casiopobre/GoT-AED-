#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"
#define MAX 30 // Tamaño maximo do nome dun personaxe
#define BUF 512 // Tamaño do buffer para ler cada liña do arquivo
#define AUXMAX 256 // Tamaño maximo da informacion de cada lista

void anhadirPersonaje(TABB *tree){
    // Declaramos as variables e creamos as listas para o personaxe
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxName;
    crearLista(&character.parents);
    crearLista(&character.siblings);
    crearLista(&character.killed);

    // Solicitamos os datos ao usuario e os imos gardando no personaxe
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

    // Insertamos o personaxe na arbore
    insertarElementoAbb(tree, character);
}

/**
 * Funcion auxiliar que imprime os datos dunha lista
 * @param list Lista da que se queren imprimir os datos
 */
void _imprimirLista(TLISTA list){
    // Declaramos as variables e igualamos a posicion a primeira da lista para poder iterala
    TIPOELEMENTOLISTA element;
    TPOSICION position = primeroLista(list);

    // Recorremos a lista imprimindo os seus elementos
    while(position != finLista(list)){
        recuperarElementoLista(list, position, &element);
        printf("%s, ", element.nameP);
        position = siguienteLista(list, position);
    }
}

/**
 * Funcion auxiliar que imprime os datos dun personaxe
 * @param character Personaxedo que queremos imprimir os datos
 */
void _imprimirPersonaje(TIPOELEMENTOABB character){
    // Para un personaxe, imprimimos todos os seus datos
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
    // Recorrido recursivo inorden para imprimir os personaxes
    if(!esAbbVacio(tree)){
        listadoPersonajes(izqAbb(tree));
        leerElementoAbb(tree, &character);
        _imprimirPersonaje(character);
        listadoPersonajes(derAbb(tree));
    }
}

void eliminarPersonaje(TABB *tree){ 
    // Declaramos a variable onde imos gardar o nome do personaxe a buscar
    TIPOCLAVE name = malloc(sizeof(char) * MAX);
    if(name == NULL){
        perror("Erro reservando memoria para o nome\n");
        return;
    }
    TIPOELEMENTOABB character;

    // Pedimoslle ao usuario o nome do personaxe a asasinar (TIPOCLAVE)
    printf("\nInserte o nome do personaxe a eliminar: ");
    scanf(" %[^\n\r]", name);

    // Mediante a clave (o nome) buscamos o personaxe correspondente e o eliminamos (se esta na arbore)
    buscarNodoAbb(*tree, name, &character);
    if(esMiembroAbb(*tree, character))
        suprimirElementoAbb(tree, character);
    else
        printf("Personaxe non atopado\n");
    
    free(name);
}
/**
 * Función auxiliar que toma unha cadea separada por comas e garda os seus elementos nunha lista
 * @param list Punteiro á lista onde imos gardar os elementos
 * @param string Cadea de caracteres que imos separar por comas e gardar na lista 
 */
void _procesarCadena(TLISTA *list, char string[AUXMAX]){
    TIPOELEMENTOLISTA aux;
    // Separamos a cadea por comas en tokens e gardamos cada token (nome) na lista
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
/**
 * Funcion auxiliar que imprime unha lista nun arquivo separando os seus elementos por comas
 * @param list Lista coa informacion a imprimir
 * @param file Punteiro ao arquivo no que imprimri os datos
 */
void _imprimirListaArquivo(TLISTA list, FILE* file){
    // Iteramos sobre a lista imprimirndo cada elemento no arquivo
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

/**
 * Funcion auxiliar que imprime a informacion dun persnaxe nun arquivo
 * @param tree Arbore co personaxe
 * @param file Punteiro ao arquivo no que imos a imprimir a información
 */
void _imprimirPersonaxeArquivo(TABB tree, FILE* file){
    TIPOELEMENTOABB character;
    // FAcemos o recorrido postorden para que ao ler de novo o arquivo, a arbore non estea desbalanceada
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
    // Se lle pasamos un arquivo ao programa, actualiza o arquivo inicial
    if(argc>=3){
        printf("Actualizando o arquivo inicial...\n");
        file = fopen(argv[2], "w");
    // Se non lle pasamos un arquivo, crea un novo cun nome proporcionado polo usuario
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

/**
 * Funcion auxiliar que atopa o asasino ou asasinos dun personaxe
 * @param tree Punteiro a arbore cos datos dos personaxes
 * @param murderedChar String co nome do personaxe asasinado
 * @param murdererArray Array de strings onde imos gardar os asasinos do personaxe
 * @param arrayPos Punteiro a un int inicializado a 0 noutra función para que non se modifique nesta funcion (recursiva)
 */
void _auxFindMurderer(TABB tree, char *murderedChar, char (*murdererArray)[MAX], int *arrayPos){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxChar;
    // Recorremos a arbore co recorrido inorden recursivo 
    if(!esAbbVacio(tree)){
        _auxFindMurderer(izqAbb(tree), murderedChar, murdererArray, arrayPos);
        leerElementoAbb(tree, &character);
        TPOSICION pos = primeroLista(character.killed);
        //Se o personaxe da iteracion ten no seu campo killed ao noso personaxe, gardase na array
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

/**
 * Funcion auxiliar que atopa os fillos dun personaxe
 * @param tree Punteiro a arbore cos datos dos personaxes
 * @param parent String co nome do pai do que se queren buscra os fillos
 * @param childArray Array de strings cos nomes dos fillos
 * @param arrayPos Punteiro a un int inicializado a 0 noutra función para que non se modifique nesta funcion (recursiva)
 */
void _auxFindChildren(TABB tree, char *parent, char (*childArray)[MAX], int *arrayPos){
    TIPOELEMENTOABB character;
    TIPOELEMENTOLISTA auxChar;
    // Recorremos a arbore
    if(!esAbbVacio(tree)){
        _auxFindChildren(izqAbb(tree), parent, childArray, arrayPos);
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
        _auxFindChildren(derAbb(tree), parent, childArray, arrayPos);
    }
}

void buscarHijos(TABB tree){
    char parent[MAX];
    char childArray[MAX][MAX];
    int arrayPos = 0;

    printf("Introduza o nome do personaxe do que quere atopar o(s) fillo/a(s): \n");
    scanf(" %[^\n\r]", parent);

    _auxFindChildren(tree, parent, childArray, &arrayPos);

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

/**
 * Funcion que conta o numero de elementos dunha lista
 * @param list Lista da que se quere saber o numero de elementos
 * @return A lonxitude da lista
 */
int _countMurdered(TLISTA list){
    if(!esListaVacia(list)){
        int n = longitudLista(list);
        return n;
    }
    return 0;
}

/**
 * Funcion uxiliar que atopa o nunmero maximo de asasinatos realizados por un personaxe
 * @param tree Arbore cos datos dos personaxes
 * @param maxMurdered Punteiro a un enteiro co valor do maximo numero de persoas asasinadas poor un personaxe inicializado a 0
 * @return O nunmero maximo de asasinatos realizados por un personaxe
 */
int _auxMaxKills(TABB tree, int *maxMurdered){
    TIPOELEMENTOABB character;
    int auxMaxMurdered;
    // Recorremos a arbore contando o numero de asasinatos de cada personaxe e quedandonos co mais alto
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

/**
 * Funcion auxiliar que atopa os personaxes con mais asasinatos
 * @param tree Arbore cos datos dos personaxes
 * @param killerArray Array onde se van a gardar os nomes dos personaxes con mais asasinatos
 * @param maxKills Valor co maximo numero de asasinatos cometidos por un personaxe
 * @param counter Punteiro a un int inicializado a 0 noutra función para que non se modifique nesta funcion (recursiva)
 */
void _auxKillers(TABB tree, char (*killerArray)[MAX], int maxKills, int *counter){
    TIPOELEMENTOABB character;
    // Recorremos a abore e gardamos todos os personaxes que teñan o maior numero de asasinatos
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


/*
void _auxCopyList(TLISTA list1, TLISTA list2){
    TIPOELEMENTOLISTA element;
    TPOSICION pos1 = primeroLista(list1);
    TPOSICION pos2 = primeroLista(list2);
    while(pos1 != finLista(list1)){
        recuperarElementoLista(list1, pos1, &element);
        insertarElementoLista(&list2, pos2, element);
        pos1 = siguienteLista(list1, pos1);
        pos2 = siguienteLista(list2, pos2);
    }
}
*/

void modificarPersonaje(TABB *tree){
    TIPOCLAVE modifiedChar = malloc(sizeof(char) * MAX);
    if(modifiedChar == NULL){
        perror("Erro reservando memoria para o nome\n");
        return;
    }
    TIPOELEMENTOABB character, auxChar;
    char option;
    TIPOELEMENTOLISTA auxName;

    printf("\nInserte o nome do personaxe a modificar: ");
    scanf(" %[^\n\r]", modifiedChar);

    // Buscamos o personaxe que queremos modificar pola clave (o nome)
    buscarNodoAbb(*tree, modifiedChar, &character);

    if(esMiembroAbb(*tree, character)){
        do{
            printf("\033[1;35m");
            printf("\n--------------------------------------------------------");
            printf("\n~*~*~*~*~*~ Seleccione un campo para modificar: ~*~*~*~*~*~\n");
            printf("\nn) Nome\n");
            printf("\nc) Casa\n");
            printf("\nr) Realeza\n");
            printf("\np) Proxenitores\n");
            printf("\ni) Irmans/irmas\n");
            printf("\na) Persoas asasinadas\n");
            printf("\ns) Saír\n");
            printf("--------------------------------------------------------\n");
            printf("\033[1;0m");
            printf("\nOpcion:");
            scanf(" %c", &option);
            switch (option){
            case 'n': case 'N':
                printf("\nIntroduce o novo nome do personaxe: ");
                scanf(" %[^\n\r]", character.name);
                // "Copiamos" o personaxe atopado a outro auxiliar
                buscarNodoAbb(*tree, modifiedChar, &auxChar);
                // Creamos as listas do personaxe auxiliar para que ao eliminalo non se eliminen da memoria (cambiamoslles as direccións)
                crearLista(&auxChar.parents);
                crearLista(&auxChar.siblings);
                crearLista(&auxChar.killed);
                // Modificamos o personaxe atopado
                modificarElementoAbb(*tree, auxChar);
                suprimirElementoAbb(tree, auxChar);
                // Insertmos o personaxe co nome modificado
                insertarElementoAbb(tree, character);
                break;
            case 'c': case 'C':
                printf("\nIntroduce a nova casa do personaxe: ");
                scanf(" %[^\n\r]", character.house);
                break;
            case 'r': case 'R':
                printf("\nIntroduce o novo status de realeza do personaxe (0 = non; 1 = si): ");
                scanf(" %d", &character.royal);
                break;
            case 'p': case 'P':
                destruirLista(&character.parents);
                crearLista(&character.parents);
                printf("\nIntroduza a lista dos nomes dos novos proxenitores do seu personaxe ('fin' para terminar): ");
                do {
                    scanf(" %[^\n\r]", auxName.nameP);
                    if((strcmp(auxName.nameP, "fin") != 0))
                        insertarElementoLista(&character.parents, finLista(character.parents), auxName);
                } while (strcmp(auxName.nameP, "fin") != 0);
                break;
            case 'i': case 'I':
                destruirLista(&character.siblings);
                crearLista(&character.siblings);
                printf("\nIntroduza a lista dos novos irmans e irmas do seu personaxe ('fin' para terminar): ");
                do{
                    scanf(" %[^\n\r]", auxName.nameP);
                    if(strcmp(auxName.nameP, "fin") != 0)
                        insertarElementoLista(&character.siblings, finLista(character.siblings), auxName);
                } while (strcmp(auxName.nameP, "fin") != 0);
                break;
            case 'a': case 'A':
                destruirLista(&character.killed);
                crearLista(&character.killed);
                printf("\nIntroduza a nova lista de persoas asasinadas polo seu personaxe ('fin' para terminar):");
                do{
                    scanf(" %[^\n\r]", auxName.nameP);
                    if(strcmp(auxName.nameP, "fin") != 0)
                        insertarElementoLista(&character.killed, finLista(character.killed), auxName);
                } while (strcmp(auxName.nameP, "fin") != 0);
                break;
            case 's': case 'S':
                return;
                break;
            default:
                printf("Opción non valida\n");
                break;
            }
        } while(option != 's');
    } else
        printf("Personaxe non atopado\n");
    
    free(modifiedChar);
}
