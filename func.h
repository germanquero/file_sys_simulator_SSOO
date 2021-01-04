int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
    char *lista_ordenes[8] = {"info", "bytemaps", "dir", "rename", "imprimir", "remove", "copy", "salir"};
    char comando[3][100];
    int i = 0,j = 0, k = 0;
    for(j = 0; j < 100; ++j, ++k){
        if(strcomando[j] == '\0' || strcomando[j] == '\n'){
            comando[i][k] = '\0';
            break;
        }
        else if(strcomando[j] == ' '){
            comando[i][k] = '\0';
            ++j;
            ++i;
            k = 0;
        }
        comando[i][k] = strcomando[j];
    }
    strcpy(orden, comando[0]);
    strcpy(argumento1, comando[1]);
    strcpy(argumento2, comando[2]);
    for(i = 0; i < 8; ++i){
        if(strcmp(orden, lista_ordenes[i]) == 0)
            return 0;
    }
    return 1;
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){

}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){

}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){

}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){

}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){

}