#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "SortedMap.h"
#include "Map.h"

#define MENU printf("1)Mostrar Canciones\n2)Cambiar Tonalidad Manualmente\n3)Mostrar digitacion\n4)Agregar Cancion\n5)Salir\n")
#define MENU1 printf("1)Mostrar Ordenadas\n2)Buscar Por:\n3)Volver\n")
#define MENU1_1 printf("Mostrar Ordenadas Por:\n1)Nombre\n2)Fecha\n3)Volver\n")
#define MENU1_2 printf("Buscar Por:\n1)Autor\n2)Fecha\n3)Favoritas\n4)Volver\n")
#define MENU2 printf("1)Cambiar por semitonos\n2)Cambiar a tonalidad nueva\n3)Salir\n")

// FUNCION PARA COMPARAR CADENAS.
int cmpString(const void * key1, const void * key2) {
    const char * Key1 = key1;
    const char * Key2 = key2;
    return strcmp(Key1,Key2);
}
// FUNCION HASH PARA CADENAS.
long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}
// FUNCION PARA COMPARAR CADENAS.
int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

// ESTRUCTURA DE TIPO ACORDE.
typedef struct{
    const char* nombre;
    const char* tipo;
    const char* digitacion;
}acorde;

// ESTRUCTURA DE TIPO CANCION.
typedef struct{
    const char* id;
    const char* anio;
    int cantAcordes;
    const char* nombre;
    const char* artista;
    const char* letra;
    acorde** acordes;
}cancion;

char * _strdup (const char *);
const char *get_csv_field (char *, int );
char* get_txt(const char* );
int llenarMapa(SortedMap *, SortedMap *,Map *,Map *,Map *,Map *);
Map* llenarAcordes();
void mostrarTodas(SortedMap*);
cancion * mostrarCancion(Map *,const char *,Map*,char**);
char** crearEscala( );
char* validarStrings(char*);
void llenarFavoritos(SortedMap *);
void guardarFav(cancion *, SortedMap *);
void mostrarDigitacion(Map *,char **, char *, char*);
void cambiarAcordeSemitono(int, int,cancion *,Map *, char**,int,int);
void cambiarTonalidadNueva(int,char **, Map *,cancion *);
void agregarCancion(Map *, int,char **,Map *);
void subMenu(Map*,SortedMap*,Map *, char **, Map*);
void menuTonalidad(int, cancion*,Map *,char ** );
int buscarPos(char *);
int cambiarPos(int, int, int);
int calcularDiferenciaPos(char*,char*);

int main(){
    //SE CREA CADA MAPA RESPECTIVO Y SE LLENA.
    Map * xNombre = createMap(stringHash, stringEqual);
    Map * xID = createMap(stringHash, stringEqual);
    Map * artistas = createMap(stringHash, stringEqual);
    Map * fechas = createMap(stringHash, stringEqual);
    SortedMap * alfa = createSortedMap(cmpString);
    SortedMap * num = createSortedMap(cmpString);
    SortedMap * favs = createSortedMap(cmpString);
    int para_agregar = llenarMapa(alfa,num,xID,artistas,fechas,xNombre);
    Map * chord = llenarAcordes();
    llenarFavoritos(favs);

    char** escala = crearEscala(); // SE CREA LA ESCALA MAYOR DE DO PARA EL CAMBIO DE TONALIDAD.

    char opcion[10];
    do{
        system("cls");
        MENU;
        printf("Ingrese su opcion: ");
        scanf("%s",opcion);
        system("cls");
        switch(atoi(opcion)){
            case 1:  //MOSTRAR CANCIONES.
                do{
                    MENU1;
                    printf("Ingrese su opcion: ");
                    scanf("%s",opcion);
                    system("cls");
                    switch(atoi(opcion)){
                        case 1:  //MOSTRAR ORDENADAS POR.
                            do{
                                system("cls");
                                MENU1_1;
                                printf("Ingrese su opcion: ");
                                scanf("%s",opcion);
                                system("cls");
                                switch(atoi(opcion)){
                                case 1:  //NOMBRE.
                                    mostrarTodas(alfa);
                                    subMenu(xID,favs,chord,escala,chord);
                               break;
                                case 2:  //AÑO.
                                    mostrarTodas(num);
                                    subMenu(xID,favs,chord,escala,chord);
                                    break;
                                }
                            }while(atoi(opcion)!=3);
                            break;
                        case 2:  //BUSCAR POR.
                            do{
                                system("cls");
                                MENU1_2;
                                printf("Ingrese su opcion: ");
                                scanf("%s",opcion);
                                system("cls");
                                switch(atoi(opcion)) {
                                    case 1:  //AUTOR.
                                        printf("Ingrese el grupo o artista:\n");
                                        getchar();
                                        char* cadena = (char*) malloc (sizeof(char));
                                        scanf("%[^\n]s",cadena);
                                        validarStrings(cadena);
                                        SortedMap * aux = searchMap(artistas,cadena);
                                        mostrarTodas(aux);
                                        subMenu(xID,favs,chord,escala,chord);
                                        fflush(stdin);
                                        break;
                                    case 2:  //AÑO.
                                        printf("Ingrese el an~o a buscar\n");
                                        getchar();
                                        char fecha[10];
                                        scanf("%s",fecha);
                                        SortedMap * nee = searchMap(fechas,fecha);
                                        mostrarTodas(nee);
                                        subMenu(xID,favs,chord,escala,chord);
                                        break;
                                    case 3:  //FAVORITAS.
                                        mostrarTodas(favs);
                                        subMenu(xID,favs,chord,escala,chord);
                                        break;
                                }
                        }while(atoi(opcion) != 4);
                        break;
                    }
                }while(atoi(opcion) != 3);
                break;
            case 2:  //CAMBIAR TONALIDAD MANUAKMENTE.
                menuTonalidad(1,NULL,chord,escala);
                system("pause");
                break;
            case 3:  //MOSTRAR DIGITACION.
                getchar();
                char * nota = (char *)malloc(sizeof(char));
                char * tipo = (char *)malloc(sizeof(char));
                printf("Ingresar el acorde y su tipo separado por un espacio: Ejemplos:");
                printf("\nSol mayor; Do# 7; Reb menor\n(todos los bemoles seran cambiados a sus sostenidos por el programa 'Mib -- > Re#')\n");
                scanf("%s%s",nota,tipo);
                mostrarDigitacion(chord,escala,nota,tipo);
                system("pause");
                break;
            case 4:  //AGREGAR CANCION.
                printf("El programa se cerrara luego de ingresar una nueva cancion\n");
                agregarCancion(xNombre,para_agregar+1,escala,chord);
                break;
        }
    }while(atoi(opcion)!=5);
    return 0;
}

/* Menu para seleccionar una cancion */
void subMenu(Map* xID,SortedMap* favs,Map * digitacion, char ** escala, Map* chord) {
    cancion* aux=(cancion *)malloc(sizeof(cancion));
    char opcion[10];
    do {
        printf("\n1)Seleccionar por ID\n2)Volver\nIngrese su opcion: ");
        do {
            scanf("%s",opcion);
        }while(atoi(opcion) != 1 && atoi(opcion) != 2);
        switch(atoi(opcion)){
        case 1:
            do{
                printf("Ingrese el ID: ");
                char id[10];
                scanf("%s",id);
                system("cls");
                aux=mostrarCancion(xID, id,chord,escala);
            }while(aux == NULL);
            do{
                printf("\n1)Guardar como favorita\n2)Transponer acordes\n3)Volver\nIngrese su opcion: ");
                scanf("%s",opcion);
                switch(atoi(opcion)){
                case 1:
                    guardarFav(aux,favs);
                    break;
                case 2:
                    menuTonalidad(0,aux,digitacion,escala);
                    strcpy(opcion,"3");
                    break;
                }
            }while(atoi(opcion) != 3);
            strcpy(opcion,"2");
            printf("\nVolvera al menu para mostrar ordenadas\n");
            system("pause");
        }
    }while(atoi(opcion) != 2);
}

/*Menu para cambiar tonalidad */
void menuTonalidad(int manual,cancion * musica,Map * digitacion,char **escala) {
    char opcion[10];
    do {
        MENU2;
        printf("Ingrese su opcion: ");
        scanf("%s",opcion);
        switch(atoi(opcion)) {
        case 1:
            do {
                printf("\n1)Subir semitono\n2)Bajar semitono\n3)Salir\n");
                scanf("%s",opcion);
                switch(atoi(opcion)) {
                case 1:
                    cambiarAcordeSemitono(1,manual,musica,digitacion,escala,1,0);
                    strcpy(opcion,"3");
                    break;
                case 2:
                    cambiarAcordeSemitono(-1,manual,musica,digitacion,escala,1,0);
                    strcpy(opcion,"3");
                    break;
                }
            }while(atoi(opcion)!=3);
            strcpy(opcion,"3");
            break;
        case 2:
            cambiarTonalidadNueva(manual,escala,digitacion,musica);
            strcpy(opcion,"3");
            break;
        }
    }while(atoi(opcion)!=3);
}

/*Se llenan los mapas generales y por artistas*/
int llenarMapa(SortedMap * alfa, SortedMap * num, Map * mapa, Map* artistas, Map* fechas,Map* xNombre){
    char linea[1024];
    FILE *fp;
    fp=fopen("Canciones.csv","r");
    int aux;
    /*Se crea una cancion para cada linea del archivo*/
    while (fgets(linea,sizeof(linea),fp)!= NULL){
        cancion * nueva= (cancion *)malloc(sizeof(cancion));
        nueva->id = get_csv_field(linea,2);
        nueva->nombre = get_csv_field(linea,3);
        nueva->artista = get_csv_field(linea,4);
        nueva->anio = get_csv_field(linea,5);
        nueva->letra = get_txt(get_csv_field(linea,6));
        nueva->cantAcordes = atoi(get_csv_field(linea,7));
        aux = atoi(nueva->id);
        nueva->acordes = (acorde**) malloc (((nueva->cantAcordes)*2)*sizeof(acorde*));
        /*Ciclo para guardar los acordes en su respectiva estructura */
        for(int i = 8, j = 0;j<nueva->cantAcordes; i = i+2, j++) {
            nueva->acordes[j] = (acorde*) malloc (sizeof(acorde));
            nueva->acordes[j]->nombre = get_csv_field(linea,i);
            if(get_csv_field(linea,i+1) != NULL) {
                nueva->acordes[j]->tipo = get_csv_field(linea,i+1);
            }
        }
        /*  Crea el mapa ordenado para artista si no existe e inserta la cancion,
            si existe inserta la canción en el mapa ordenado,
            se pone la lista en el mapa hash    */
        if(searchMap(artistas,nueva->artista) == NULL) {
            SortedMap* art = createSortedMap(cmpString);
            insertSortedMap(art,nueva->nombre,nueva);
            insertMap(artistas,nueva->artista,art);

        }else{
            SortedMap* art = searchMap(artistas,nueva->artista);
            insertSortedMap(art,nueva->nombre,nueva);
        }
        /*  Crea el mapa ordenado para fecha si no existe e inserta la cancion,
            si existe inserta la canción en el mapa ordenado,
            se pone la lista en el mapa hash    */
        if(searchMap(fechas,nueva->anio) == NULL) {
            SortedMap* fech = createSortedMap(cmpString);
            insertSortedMap(fech,nueva->nombre,nueva);
            insertMap(fechas,nueva->anio,fech);

        }else{
            SortedMap* fech = searchMap(fechas,nueva->anio);
            insertSortedMap(fech,nueva->nombre,nueva);

        }
        /*  se inserta la canción en cada mapa respectivo */
        insertSortedMap(alfa,nueva->nombre,nueva);
        insertSortedMap(num,nueva->anio,nueva);
        insertMap(mapa,nueva->id,nueva);
        insertMap(xNombre,nueva->nombre,nueva);
    }
    fclose(fp);
    return aux;

}

/*Funcion que llena la lista de favoritos*/
void llenarFavoritos(SortedMap * lista){
    FILE* fp;
    fp=fopen("Favoritos.csv","r");
    char linea[1024];
    /*  se crea una cancion para cada linea del archivo
        y se guarda en un mapa ordenado */
    while (fgets(linea,sizeof(linea),fp)!= NULL){
        cancion * nueva= (cancion *)malloc(sizeof(cancion));
        nueva->id = get_csv_field(linea,1);
        nueva->nombre = get_csv_field(linea,2);
        nueva->artista = get_csv_field(linea,3);
        nueva->anio = get_csv_field(linea,4);
        insertSortedMap(lista,nueva->nombre,nueva);
    }
    fclose(fp);
}

/*Funcion que llena el mapa Hash de los acorde para mostrar digitacion */
Map* llenarAcordes() {
    Map * acordes = createMap(stringHash, stringEqual);
    FILE * fp;
    fp=fopen("digitacion.csv","r");
    char linea[1024];

    /*  Se lee linea a linea el archivo y se guardan los datos
        en la estructura de acorde  */
    while (fgets(linea,sizeof(linea),fp)!= NULL){
        acorde* nuevo = (acorde*) malloc (sizeof(acorde)); // SE CREA UN NUEVO ACORDE
        char* nombre = (char *)get_csv_field(linea,2); // SE OBTIENE EL NOMBRE
        char* digitacion = (char *)get_csv_field(linea,3); // SE OBTIENE LA DIGITACION
        /*  Guarda el dibujo de la digitacion   */
        for(int i = 4; i <= 9; i++) {
            if(get_csv_field(linea,i) != NULL) {
                strcat(digitacion, "\n");
                strcat(digitacion,get_csv_field(linea,i));
            }
        }
        nuevo->nombre = nombre; // SE LE ASIGNA EL NOMBRE AL ACORDE
        nuevo->digitacion = digitacion; // SE LE ASIGNA LA DIGITACION AL ACORDE
        insertMap(acordes,nuevo->nombre,nuevo);
    }
    fclose(fp);
    return acordes;

}

/*Funcion para guardar favoritos en el archivo e insertarlo en su mapa*/
void guardarFav(cancion* aux, SortedMap * mapa){
    /*  Valida que la canción no este guardada  */
    if (searchSortedMap(mapa,aux->nombre)!=NULL){
        printf("\nLa cancion ya se encuentra guardada.\n");
        return;
    }
    FILE *fp;
    fp = fopen("Favoritos.csv","a");
    fprintf(fp,"%05d,%s,%s,%s\n",atoi(aux->id),aux->nombre,aux->artista,aux->anio);
    fclose(fp);
    insertSortedMap(mapa,aux->nombre,aux);
    printf("\nLa cancion se ha guardado con exito");
}

/*Muestra todas las canciones dentro de un mapa ordenado*/
void mostrarTodas(SortedMap * mapa){
    if (mapa == NULL){
        printf("\nNo encontrado");
    }
    cancion* aux = firstSortedMap(mapa);
    printf("|%-6s|%-30s|%-30s|%-5s|\n","ID","Titulo","Artista","Fecha");
    while (aux){
        printf("|%-6s|%-30s|%-30s|%-5s|\n",aux->id,aux->nombre,aux->artista,aux->anio);
        aux=nextSortedMap(mapa);
    }
}

/*Muestra una cancion especifica dando letra y acordes */
cancion * mostrarCancion(Map * mapa,const char * id,Map* chord,char** escala){
    cancion * aux = searchMap(mapa,id);
    if (aux == NULL){
        printf("El ID no es valido\n");
        return NULL;
    }
    system("cls");
    printf("|%-30s|%-30s|%-5s|\n",aux->nombre,aux->artista,aux->anio);
    printf("Letra:\n %s\n",aux->letra);
    printf("Acordes:\n");
    /*  escribe acordes guardados dentro del arreglo de la estructura de la cancion*/
    for(int i = 0; i<aux->cantAcordes; i++) {
        char nombre[10];
        strcpy(nombre,aux->acordes[i]->nombre);
        char tipo[10];
        strcpy(tipo,aux->acordes[i]->tipo);
        mostrarDigitacion(chord,escala,nombre,tipo);
    }
    return aux;
}

/*Funcion para mostrar la digitacion en guitarra de los acordes */
void mostrarDigitacion(Map * chord,char ** escala,char * nota, char * tipo){
    validarStrings(nota);
    validarStrings(tipo);
    int pos = buscarPos(nota);
    if (pos == -1){
        printf("Acorde no valido\n");
        system("pause");
        return;
    }
    strcpy(nota,escala[pos]);
    strcat(nota,tipo);
    acorde * letra= (acorde *)malloc(sizeof(acorde));
    letra = searchMap(chord,nota);
    if(letra == NULL){
        printf("El acorde no existe");
        return;
    }
    else{
        printf("%s:\n%s\n",nota,letra->digitacion);
    }
}

char * _strdup (const char *s) {
    size_t len = strlen (s) + 1;
    void *new = malloc (len);

    if (new == NULL)
    	return NULL;

    return (char *) memcpy (new, s, len);
}

const char *get_csv_field (char * tmp, int i) {
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}

char* get_txt(const char* direccion) {
    FILE *fp;
    fp = fopen(direccion,"r");

    if(fp == NULL) {
        printf("Error al abrir el archivo");
        return NULL;
    }

    fseek(fp,0,SEEK_END);
    int elementos = ftell(fp);
    rewind(fp);

    char* cadena = (char*) calloc (sizeof(char),elementos);
    if(cadena == NULL) {
        printf("Error al abrir el archivo");
        return NULL;
    }
    fread(cadena,sizeof(char),elementos,fp);
    return cadena;
}

/*Funcion para crear secuencia de notas */
char** crearEscala() {
    char** escala = (char**) malloc (12 * sizeof(char*));
    escala[0] = "Do ";
    escala[1] = "Do# ";
    escala[2] = "Re ";
    escala[3] = "Re# ";
    escala[4] = "Mi ";
    escala[5] = "Fa ";
    escala[6] = "Fa# ";
    escala[7] = "Sol ";
    escala[8] = "Sol# ";
    escala[9] = "La ";
    escala[10] = "La# ";
    escala[11] = "Si ";
    return escala;
}

/*Funcion que cambia la primera letra del String a mayuscula y als demas a minusculas*/
char* validarStrings(char* cadena){
    cadena[0] = toupper(cadena[0]);
    for(int i = 1; i < strlen(cadena); i++) {
        cadena[i] = tolower(cadena[i]);
    }
    return cadena;
}

/*Funcion que busca la posicion de una nota dentro de su arreglo*/
int buscarPos(char * nota){
    int pos;
    if(strcmp(nota,"Do")==0)pos = 0;
    else if(strcmp(nota,"Do#")==0 || strcmp(nota,"Reb")==0)pos = 1;
    else if(strcmp(nota,"Re")==0)pos = 2;
    else if(strcmp(nota,"Re#")==0 || strcmp(nota,"Mib")==0)pos = 3;
    else if(strcmp(nota,"Mi")==0)pos = 4;
    else if(strcmp(nota,"Fa")==0)pos = 5;
    else if(strcmp(nota,"Fa#")==0 || strcmp(nota,"Solb")==0)pos = 6;
    else if(strcmp(nota,"Sol")==0)pos = 7;
    else if(strcmp(nota,"Sol#")==0 || strcmp(nota,"Lab")==0)pos = 8;
    else if(strcmp(nota,"La")==0)pos = 9;
    else if(strcmp(nota,"La#")==0 || strcmp(nota,"Sib")==0)pos = 10;
    else if(strcmp(nota,"Si")==0)pos = 11;
    else{
        //char * chord=(char *)malloc(sizeof(char));
        //printf("Ingrese una nota valida: ");
        return -1;
    }
    return pos;

}

/*  Funcion que cambia la posicion para hacer la transformación de notas
    con la cantidad de semitonos    */
int cambiarPos(int pos,int cant,int factor){
    cant%=12;
    pos+=(factor*cant);
    if (pos>11) pos-=12;
    else if(pos<0) pos+=12;
    return pos;
}

/*  Funcion que calcula la diferencia entre una posicion y otra
    para cambiar la nota a una nota nueva   */
int calcularDiferenciaPos(char * posAntigua, char* posNueva) {
     int pos1, pos2;
     pos1=buscarPos(posAntigua);
     pos2=buscarPos(posNueva);
     return (pos2-pos1);
}

/* Funcion que permite cambiar a una tonalidad nueva    */
void cambiarTonalidadNueva(int manual,char ** escala, Map * digitacion,cancion * aux){
    char * pos1 = (char*)malloc(sizeof(char));
    char * pos2 = (char*)malloc(sizeof(char));
    char * tipoTon = (char*)malloc(sizeof(char));
    char * completo1 = (char *)malloc(sizeof(char));
    char * completo2 = (char *)malloc(sizeof(char));
    acorde * dibujo =(acorde *)malloc(sizeof(acorde));
    int val;
    /*  Verifica que hacer en caso de que sea una transformacion
        de acordes manual o se realice a traves de una cancion  */
    if (manual == 1){
        /*Se leen y se validan los datos necesarios para cambiar a la nueva tonalidad*/
        do{
            printf("\nIngrese tonalidad valida para cambiar(ingresar nota y tipo separado por espacio'ej.do menor': ");
            scanf("%s%s",pos1,tipoTon);
            validarStrings(pos1);
            validarStrings(tipoTon);
            val = buscarPos(pos1);
            strcpy(completo1,escala[val]);
            strcat(completo1,tipoTon);
        }while(val<0 && searchMap(digitacion,completo1) != NULL);
        do{
            printf("\nIngrese tonalidad nueva valida(solo nota, ya que el tipo se mantiene): ");
            scanf("%s",pos2);
            validarStrings(pos2);
            val = buscarPos(pos2);
            strcpy(completo2,escala[val]);
            strcat(completo2,tipoTon);
        }while(val<0 && searchMap(digitacion,completo2) != NULL);
        val = calcularDiferenciaPos(pos1,pos2);
        dibujo=searchMap(digitacion,completo2);
        printf("%s --> %s\n%s",completo1,completo2,dibujo->digitacion);
        cambiarAcordeSemitono(1,manual,aux,digitacion,escala,0,val);
        return;
    }
    /*  Cuando se cambian desde una cancion no es necesaria la validacion de datos */
    do{
        printf("\nIngrese tonalidad nueva valida(solo nota ya que el tipo se mantiene): ");
        scanf("%s",pos2);
        validarStrings(pos2);
        val = buscarPos(pos2);
        strcpy(completo2,escala[val]);
        strcat(completo2,aux->acordes[0]->tipo);
    }while(val<0 && searchMap(digitacion,completo2) != NULL);
    val = calcularDiferenciaPos((char *)aux->acordes[0]->nombre,pos2);
    cambiarAcordeSemitono(1,manual,aux,digitacion,escala,0,val);
}

/*Funcion que permite cambiar la nota en base a semitonos */
void cambiarAcordeSemitono(int factor, int manual,cancion * aux,Map * digitacion, char** escala,int j,int dif){
    char * nota = (char *)malloc(sizeof(char));
    char * tipo = (char *)malloc(sizeof(char));
    char * completo = (char *)malloc(sizeof(char));
    acorde * dibujo = (acorde *)malloc(sizeof(acorde));
    int cant,pos,cantTonos;
    if (j == 1){
        do{
            printf("Ingrese la cantidad de semitonos(valor entre 1 y 11): ");
            scanf("%d",&cantTonos);
        }while(cantTonos<1 || cantTonos>11);
    }
    else cantTonos=dif;
    /*Se leen y validan datos necesarios para cambiar a la nueva tonalidad en caso de ser manual*/
    if(manual == 1){
        printf("\nIngrese la cantidad de acordes: ");
        scanf("%d",&cant);
        printf("\nIngresar el acorde y su tipo separado por un espacio: Ejemplos:");
        printf("\nSol mayor\nDo# 7,Reb menor(todos los bemoles 'Mib' seran cambiados a sus sostenidos por el programa 'Re#'\n");
        for (int i=0;i<cant;i++){
            do{
                printf("Ingrese un acorde valido para el acorde[%d]: ",i+1);
                scanf("%s%s",nota,tipo);
                getchar();
                validarStrings(nota);
                validarStrings(tipo);
                pos = buscarPos(nota);
                strcpy(completo,escala[pos]);
                strcat(completo,tipo);
                printf("%s",completo);
            }while(searchMap(digitacion,completo)==NULL);
            printf("\nacorde bien ingresado\n");
            pos = buscarPos(nota);
            pos = cambiarPos(pos,cantTonos,factor);
            strcpy(nota,escala[pos]);
            strcat(nota,tipo);
            printf("\n%s --> %s",completo,nota);
            dibujo = searchMap(digitacion,nota);
            printf("\n%s\n",dibujo->digitacion);
        }
        return;
    }
    /*No necesita validacion de datos ya que provienen de la cancion*/
    for(int i = 0;i<aux->cantAcordes;i++){
        nota=(char *)aux->acordes[i]->nombre;
        pos = buscarPos((char *)aux->acordes[i]->nombre);
        pos = cambiarPos(pos,cantTonos,factor);
        completo = (char *)malloc(sizeof(char));
        strcpy(completo,escala[pos]);
        //printf("nota %s completo1 %s\n",completo);
        strcat(completo,aux->acordes[i]->tipo);
        //printf("nota %s completo 2 %s\n",nota,completo);
        strcat(nota," ");
        strcat(nota,aux->acordes[i]->tipo);
        printf("\n%s --> %s",nota,completo);
        dibujo = searchMap(digitacion,completo);
        if (dibujo) printf("\n%s",dibujo->digitacion);
    }
    return;
}

/*Funcion que permite guardar una cancion nueva en el archivo csv*/
void agregarCancion(Map * mapa,int id, char ** escala,Map* digitacion){

    /*  Esta funcion principalmente lee los datos,
        verifica que la cancion no exista,
        y transforma las cadenas a un formato que no cause errores en el programa */
    char * nombre=(char *)malloc(sizeof(char));
    char * artista =(char *)malloc(sizeof(char));
    getchar();
    printf("Ingresar el nombre de la cancion: ");
    scanf("%[^\n]s",nombre);
    getchar();
    validarStrings(nombre);
    printf("\nIngresar el artista o grupo: ");
    scanf("%[^\n]s",artista);
    validarStrings(artista);
    getchar();
    cancion * aux = searchMap(mapa,nombre);
    if (aux!=NULL && strcmp(aux->artista,artista)==0){
        printf("\nLa cancion ya existe\n");
        system("pause");
        return;
    }

    int fecha,cont,pos;
    char nota[10];
    char acorde[10];
    char tipo[10];
    char * linea=(char*)malloc(sizeof(char));
    printf("\nIngrese an~o de lanzamiento: ");
    scanf("%d",&fecha);
    getchar();
    printf("\nIngrese cantidad de acordes: ");
    scanf("%d",&cont);
    getchar();
    FILE *sonata;
    FILE *letra;
    sonata = fopen("Canciones.csv","a");
    char ruta[100]="Letras\\";
    strcat(ruta,nombre);
    strcat(ruta,".txt");
    letra = fopen(ruta,"w");
    fprintf(sonata,"n,%05d,%s,%s,%d,Letras\\\\%s.txt,%d",id,nombre,artista,fecha,nombre,cont);
    printf("Ingresar el acorde y su tipo separado por un espacio: Ejemplos:");
    printf("\nSol mayor; Do# 7; Reb menor\n(todos los bemoles seran cambiados a sus sostenidos por el programa 'Mib --> Re#'");
    for (int i = 1; i<=cont;i++){
        do{
            printf("\nIngrese una nota y tipo(mayor,menor,7) validos: ");
            scanf("%s%s",acorde,tipo);
            getchar();
            validarStrings(acorde);
            validarStrings(tipo);
            strcpy(nota,acorde);
            pos = buscarPos(acorde);
            if(pos == -1) continue;
            strcpy(acorde,escala[pos]);
            strcat(acorde,tipo);
            printf("%s",acorde);
            if(searchMap(digitacion,acorde)==NULL) printf("El acorde no existe");
            else fprintf(sonata,",%s,%s",nota,tipo);
        }while(searchMap(digitacion,acorde)==NULL || pos == -1);
    }
    fprintf(sonata,"\n");
    printf("Ingresa la letra, escriba fin para terminar:\n");
    do{
        scanf("%[^\n]s",linea);
        validarStrings(linea);
        getchar();
        if (strcmp(linea,"Fin")!=0) fprintf(letra,"%s\n",linea);
    }while(strcmp(linea,"Fin")!=0);
    printf("Guardada\n");
    system("pause");
    exit(2);
    //fclose(cancion);
    //fclose(letra);
}
