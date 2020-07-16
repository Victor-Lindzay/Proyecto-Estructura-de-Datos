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

int cmpString(const void * key1, const void * key2) {
    const char * Key1 = key1;
    const char * Key2 = key2;
    return strcmp(Key1,Key2);
}

long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

typedef struct{
    const char* nombre;
    const char* tipo;
    const char* digitacion;
}acorde;

typedef struct{
    const char* id;
    const char* anio;
    int cantAcordes;
    const char* nombre;
    const char* artista;
    const char* letra;
    acorde** acordes;
}cancion;

typedef struct{
    const char * artista;
    SortedMap * listaCanciones;
}Artista;

typedef struct{
    const char* fecha;
    SortedMap * listaCanciones;
}Fecha;

char * _strdup (const char *);
const char *get_csv_field (char *, int );
char* get_txt(const char* );
int llenarMapa(SortedMap *, SortedMap *,Map *,Map *,Map *,Map *);
Map* llenarAcordes();
void mostrarTodas(SortedMap*);
cancion * mostrarCancion(Map *, const char *);
char** crearEscala( );
char* cambiarMinusculas(char*);
char* validarStrings(char*);
void llenarFavoritos(SortedMap *);
void guardarFav(cancion *, SortedMap *);
void MostrarArtista(Map*,char*);
void MostrarFecha(Map*,char*);
void mostrarDigitacion(Map *,char **, char *, char*);
void cambiarAcordeSemitono(int, int,cancion *,Map *, char**);
void agregarCancion(Map *, int,char **,Map * );
void subMenu(Map* ,SortedMap*,Map*,char**);
void menuTonalidad(int, cancion*,Map *,char ** );
int buscarPos(char *);
int cambiarPos(int, int, int);
int calcularDiferenciaPos(char*,char*);

int main(){
    //Se crea cada mapa respectivo y se llena.
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
            case 1:
                do{
                    MENU1;
                    printf("Ingrese su opcion: ");
                    scanf("%s",opcion);
                    system("cls");
                    switch(atoi(opcion)){
                        case 1:
                            do{
                                system("cls");
                                MENU1_1;
                                printf("Ingrese su opcion: ");
                                scanf("%s",opcion);
                                system("cls");
                                switch(atoi(opcion)){
                                case 1:
                                    mostrarTodas(alfa);
                                    subMenu(xID,favs,chord,escala);
                               break;
                                case 2:
                                    mostrarTodas(num);
                                    subMenu(xID,favs,chord,escala);
                                    break;
                                }
                            }while(atoi(opcion)!=3);
                            break;
                        case 2:
                            do{
                                system("cls");
                                MENU1_2;
                                printf("Ingrese su opcion: ");
                                scanf("%s",opcion);
                                system("cls");
                                switch(atoi(opcion)) {
                                    case 1:
                                        printf("Ingrese el grupo o artista:\n");
                                        getchar();
                                        char* cadena = (char*) malloc (sizeof(char));
                                        scanf("%[^\n]s",cadena);
                                        cadena=validarStrings(cadena);
                                        MostrarArtista(artistas,cadena);
                                        subMenu(xID,favs,chord,escala);
                                        fflush(stdin);
                                        break;
                                    case 2:
                                        printf("Ingrese el an~o a buscar\n");
                                        getchar();
                                        char fecha[10];
                                        scanf("%s",fecha);
                                        MostrarFecha(fechas,fecha);
                                        subMenu(xID,favs,chord,escala);
                                        break;
                                    case 3:
                                        mostrarTodas(favs);
                                        subMenu(xID,favs,chord,escala);
                                        break;
                                }
                        }while(atoi(opcion) != 4);
                        break;
                    }
                }while(atoi(opcion) != 3);
                break;
            case 2:
                menuTonalidad(1,NULL,chord,escala);
                break;
            case 3:
                getchar();
                char * nota = (char *)malloc(sizeof(char));
                char * tipo = (char *)malloc(sizeof(char));
                printf("Ingresar el acorde y su tipo separado por un espacio: Ejemplos:");
                printf("\nSol mayor; Do# 7; Reb menor\n(todos los bemoles seran cambiados a sus sostenidos por el programa 'Mib -- > Re#'\n");
                scanf("%s%s",nota,tipo);
                mostrarDigitacion(chord,escala,nota,tipo);
                break;
            case 4:
                agregarCancion(xNombre,para_agregar+1,escala,chord);
                break;
        }
    }while(atoi(opcion)!=5);
    return 0;
}

void subMenu(Map* xID,SortedMap* favs,Map * digitacion, char ** escala) {
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
                aux=mostrarCancion(xID, id);
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
                    break;
                }
            }while(atoi(opcion) != 3);
            //system("cls");
        }
    }while(atoi(opcion) != 2);
}

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
                    cambiarAcordeSemitono(1,manual,musica,digitacion,escala);
                    break;
                case 2:
                    cambiarAcordeSemitono(-1,manual,musica,digitacion,escala);
                    break;
                }
            }while(atoi(opcion)!=3);
            strcpy(opcion,"0");
            break;
        case 2:
            printf("Acorde cambiado manualmente\n");
            do{
                printf("\n1)Ingresar nueva tonalidad\n2)Salir\n");
                scanf("%s",opcion);
                switch(atoi(opcion)) {
                case 1:
                    printf("Tonalidad cambiada\n");
                    break;
                }
            }while(atoi(opcion)!=2);
            strcpy(opcion,"0");
            break;
        }
    }while(atoi(opcion)!=3);
}

int llenarMapa(SortedMap * alfa, SortedMap * num, Map * mapa, Map* artistas, Map* fechas,Map* xNombre){
    char linea[1024];
    FILE *fp;
    fp=fopen("Canciones.csv","r");
    int aux;
    while (fgets(linea,sizeof(linea),fp)!= NULL){
        cancion * nueva= (cancion *)malloc(sizeof(cancion));
        nueva->acordes = (acorde**) malloc (sizeof(acorde*));
        nueva->id = get_csv_field(linea,2);
        nueva->nombre = get_csv_field(linea,3);
        nueva->artista = get_csv_field(linea,4);
        nueva->anio = get_csv_field(linea,5);
        nueva->letra = get_txt(get_csv_field(linea,6));
        nueva->cantAcordes = atoi(get_csv_field(linea,7));
        aux = atoi(nueva->id);
        int j=0;
        for(int i = 8;j<nueva->cantAcordes; i = i+2, j++) {
            nueva->acordes[j] = (acorde*) malloc (sizeof(acorde));
            nueva->acordes[j]->nombre = get_csv_field(linea,i);
            if(get_csv_field(linea,i+1) != NULL) {
                nueva->acordes[j]->tipo = get_csv_field(linea,i+1);
            }
        }
        if(searchMap(artistas,nueva->artista) == NULL) {
            Artista* aux = (Artista*) malloc (sizeof(Artista));
            aux->artista = nueva->artista;
            aux->listaCanciones = createSortedMap(cmpString);
            insertSortedMap(aux->listaCanciones,nueva->nombre,nueva);
            insertMap(artistas,aux->artista,aux);
        }else{
            Artista* aux = searchMap(artistas,nueva->artista);
            insertSortedMap(aux->listaCanciones,nueva->nombre,nueva);
        }
        if(searchMap(fechas,nueva->anio) == NULL) {
            Fecha* f = (Fecha*) malloc (sizeof(Artista));
            f->fecha = nueva->anio;
            f->listaCanciones = createSortedMap(cmpString);
            insertSortedMap(f->listaCanciones,nueva->nombre,nueva);
            insertMap(fechas,f->fecha,f);

        }else{
            Fecha* f = searchMap(fechas,nueva->anio);
            insertSortedMap(f->listaCanciones,nueva->nombre,nueva);
        }
        insertSortedMap(alfa,nueva->nombre,nueva);
        insertSortedMap(num,nueva->anio,nueva);
        insertMap(mapa,nueva->id,nueva);
        insertMap(xNombre,nueva->nombre,nueva);
        free(nueva);
    }
    fclose(fp);
    return aux;

}

void llenarFavoritos(SortedMap * lista){
    FILE* fp;
    fp=fopen("Favoritos.csv","r");
    char linea[1024];
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

Map* llenarAcordes() {
    Map * acordes = createMap(stringHash, stringEqual);
    // SE LEE EL ARCHIVO CON LOS ACORDES
    FILE * fp;
    fp=fopen("digitacion.csv","r");
    // CADENA QUE GUARDA LA LINE COMPLETA DEL ARCHIVO CSV
    char linea[1024];

    //SE OBTIENEN LOS DATOS DEL ARCHIVO CSV
    while (fgets(linea,sizeof(linea),fp)!= NULL){
        acorde* nuevo = (acorde*) malloc (sizeof(acorde)); // SE CREA UN NUEVO ACORDE
        char* nombre = (char *)get_csv_field(linea,2); // SE OBTIENE EL NOMBRE
        char* digitacion = (char *)get_csv_field(linea,3); // SE OBTIENE LA DIGITACION
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

void guardarFav(cancion* aux, SortedMap * mapa){
    if (searchSortedMap(mapa,aux->nombre)!=NULL){
        printf("\nLa cancion ya se encuentra guardada.\n");
        return;
    }
    FILE *fp;
    fp = fopen("Favoritos.csv","a");
    fprintf(fp,"%05d,%s,%s,%s\n",atoi(aux->id),aux->nombre,aux->artista,aux->anio);
    fclose(fp);
    insertSortedMap(mapa,aux->nombre,aux);
}

void mostrarTodas(SortedMap * mapa){
    cancion* aux = firstSortedMap(mapa);
    printf("|%-6s|%-30s|%-30s|%-5s|\n","ID","Titulo","Artista","Fecha");
    while (aux){
        printf("|%-6s|%-30s|%-30s|%-5s|\n",aux->id,aux->nombre,aux->artista,aux->anio);
        aux=nextSortedMap(mapa);
    }
}

void MostrarArtista(Map* mapa, char* cadena) {
    Artista* aux = searchMap(mapa,cadena);
    if(aux != NULL) {
        mostrarTodas(aux->listaCanciones);
    }else{
        printf("Artista no encontrado\n");
    }
}

void MostrarFecha(Map* mapa,char * numero) {
    Fecha* aux = searchMap(mapa, numero);
    if(aux != NULL) {
        mostrarTodas(aux->listaCanciones);
    }else{
        printf("Artista no encontrado\n");
    }
}

cancion * mostrarCancion(Map * mapa,const char * id){
    cancion * aux = searchMap(mapa,id);
    if (aux == NULL){
        printf("El ID no es valido\n");
        return NULL;
    }
    system("cls");
    printf("|%-30s|%-30s|%-5s|\n",aux->nombre,aux->artista,aux->anio);
    printf("Letra:\n %s\n",aux->letra);
    for(int i = 0; i<aux->cantAcordes; i++) {
        printf("%s%s\n",aux->acordes[i]->nombre,aux->acordes[i]->tipo);
    }
    return aux;
}

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
        printf("%s:\n%s",nota,letra->digitacion);
        system("pause");
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

char* cambiarMinusculas(char* cadena) {
    for(int i = 0; i < strlen(cadena); i++) {
        cadena[i] = tolower(cadena[i]);
    }
    return cadena;
}

char* validarStrings(char* cadena){
    cadena[0] = toupper(cadena[0]);
    for(int i = 1; i < strlen(cadena); i++) {
        cadena[i] = tolower(cadena[i]);
    }
    return cadena;
}

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

int cambiarPos(int pos,int cant,int factor){
    cant%=12;
    pos+=(factor*cant);
    if (pos>11) pos-=12;
    else if(pos<0) pos+=12;
    return pos;
}

int calcularDiferenciaPos(char * posAntigua, char* posNueva) {
     int pos1, pos2;
     pos1=buscarPos(posAntigua);
     pos2=buscarPos(posNueva);
     return (pos2-pos1);
}

void cambiarAcordeSemitono(int factor, int manual,cancion * aux,Map * digitacion, char** escala){
    char * nota = (char *)malloc(sizeof(char));
    char * tipo = (char *)malloc(sizeof(char));
    char * completo = (char *)malloc(sizeof(char));
    acorde * dibujo = (acorde *)malloc(sizeof(acorde));
    int pos;
    char cantTonos[10], cant[10];
    do{
        printf("Ingrese la cantidad de semitonos(valor entre 1 y 11: ");
        scanf("%s",&cantTonos);
    }while(atoi(cantTonos)<1 || atoi(cantTonos)>11);
    if(manual == 1){
        printf("\nIngrese la cantidad de acordes: ");
        scanf("%s",&cant);
        for (int i=0;i<atoi(cant);i++){
            printf("\nIngresar el acorde y su tipo separado por un espacio: Ejemplos:");
            printf("\nSol mayor\nDo# 7,Reb menor(todos los bemoles 'Mib' seran cambiados a sus sostenidos por el programa 'Re#'\n");
            do{
                printf("Ingrese un acorde valido: ");
                scanf("%s%s",nota,tipo);
                validarStrings(nota);
                validarStrings(tipo);
                strcpy(completo,nota);
                strcat(completo," ");
                strcat(completo,tipo);
            }while(searchMap(digitacion,completo)!=NULL);
            pos = buscarPos(nota);
            pos = cambiarPos(pos,atoi(cantTonos),factor);
            strcpy(nota,escala[pos]);
            strcat(nota,tipo);
            printf("\n%s --> %s",completo,nota);
            dibujo = searchMap(digitacion,nota);
            printf("\n%s",dibujo->digitacion);
            return;
        }
    }
    for(int i = 0;i<aux->cantAcordes;i++){
        nota=(char *)aux->acordes[i]->nombre;
        pos = buscarPos((char *)aux->acordes[i]->nombre);
        pos = cambiarPos(pos,atoi(cantTonos),factor);
        strcpy(completo,escala[pos]);
        strcat(completo,aux->acordes[i]->tipo);
        strcat(nota," ");
        strcat(nota,aux->acordes[i]->tipo);
        printf("\n%s --> %s",nota,completo);
        dibujo = searchMap(digitacion,completo);
        printf("\n%s",dibujo->digitacion);
    }

}

void agregarCancion(Map * mapa,int id, char ** escala,Map* digitacion){
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
    FILE *cancion;
    FILE *letra;
    cancion = fopen("Canciones.csv","a");
    char ruta[100]="Letras\\";
    strcat(ruta,nombre);
    strcat(ruta,".txt");
    letra = fopen(ruta,"w");
    fprintf(cancion,"%05d,%s,%s,%d,Letras\\\\%s.txt,%d",id,nombre,artista,fecha,nombre,cont);
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
            else fprintf(cancion,",%s,%s",nota,tipo);
        }while(searchMap(digitacion,acorde)==NULL || pos == -1);
    }
    fprintf(cancion,"\n");
    printf("Ingresa la letra, escriba fin para terminar:\n");
    do{
        scanf("%[^\n]s",linea);
        validarStrings(linea);
        getchar();
        if (strcmp(linea,"Fin")!=0) fprintf(letra,"%s\n",linea);
    }while(strcmp(linea,"Fin")!=0);
    printf("Guardada\n");
    system("pause");

    free(nombre);
    //fclose(cancion);
    //fclose(letra);
}
