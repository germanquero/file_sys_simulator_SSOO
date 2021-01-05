#include<string.h>


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
    printf("%s is not a command\n", orden);
    return 1;
}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){

}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){

}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){

}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){

}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    int i;
    printf("Bytemaps de bloques:\n");
    for ( i = 0; i < MAX_BLOQUES_PARTICION; i++)
    {
        printf("%d",ext_bytemaps->bmap_bloques[i]);
    }
    printf("\nBytemaps de inodos:\n");
    for ( i = 0; i < MAX_INODOS; i++)
    {
        printf("%d",ext_bytemaps->bmap_inodos[i]);
    }
    printf("\n");
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
    printf("Numero de inodos de la particion:%d\n",psup->s_inodes_count);
    printf("Numero de bloques de la particion:%d\n",psup->s_blocks_count);
    printf("Numero de inodos libres:%d\n",psup->s_free_inodes_count);
    printf("Numero de bloques libres:%d\n",psup->s_free_blocks_count);
    printf("Primer bloque de datos:%d\n",psup->s_first_data_block);
    printf("Tamaño de los bloques:%d\n",psup->s_block_size);
}

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){ //devuelve 0 si encuentra un nombre igual y si no devuelve 1
    int i;
    for ( i = 0; i < MAX_FICHEROS; i++)
    {
        if (strcmp(directorio[i].dir_nfich,nombre)==0)
        {
            return 0;
        }
        
    }
    return 1;                           
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
    int i,j;
    for ( i = 1; i < MAX_FICHEROS; i++)
    {
        if (directorio[i].dir_inodo==0xFFFF)
        {
            break;
        }
        
        printf("%s\t",directorio[i].dir_nfich);
        printf("tamaño:%d\t",inodos->blq_inodos[directorio[i].dir_inodo].size_fichero);
        printf("inodos:%d\t",directorio[i].dir_inodo);
        printf("bloques: ");
        for (j = 0; i < MAX_NUMS_BLOQUE_INODO; j++)
        {
            if (inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]==0xFFFF)
            {
                break;
            }
            
            printf("%d ",inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
        }
        
        printf("\n");

    }
    

}