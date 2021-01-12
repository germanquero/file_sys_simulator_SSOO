#include<string.h>
#include<stdio.h>

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
    char *lista_ordenes[9] = {"info", "bytemaps", "dir", "rename", "imprimir", "remove", "copy", "salir", "h"};
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
    for(i = 0; i < 9; ++i){
        if(strcmp(orden, lista_ordenes[i]) == 0)
            return 0;
    }
    printf("%s is not a command\n", orden);
    return 1;
}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){
   fseek(fich,SIZE_BLOQUE*2,SEEK_SET);
   fwrite(inodos, SIZE_BLOQUE, 1, fich);
   fseek(fich,SIZE_BLOQUE*3,SEEK_SET);
   fwrite(directorio, SIZE_BLOQUE, 1, fich);
}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){
   fseek(fich,SIZE_BLOQUE,SEEK_SET);
   fwrite(ext_bytemaps, SIZE_BLOQUE, 1, fich);
}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){
    fseek(fich,0,SEEK_SET);
    fwrite(ext_superblock, SIZE_BLOQUE, 1, fich);
}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){
    int i;
    fseek(fich,SIZE_BLOQUE*4,SEEK_SET);
    fwrite(memdatos->dato,SIZE_BLOQUE*MAX_BLOQUES_DATOS,1,fich);
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

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){
    int i;
    for ( i = 0; i < MAX_FICHEROS; i++)
    {
        if (strcmp(directorio[i].dir_nfich,nombre)==0)
            return 1;
    }
    return 0;                           
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
    int i,j;
    for ( i = 1; i < MAX_FICHEROS; i++)
    {
        if (directorio[i].dir_inodo==0xFFFF)
        {
            continue;
        }
        printf("%s\t",directorio[i].dir_nfich);
        printf("tamaño:%d\t",inodos->blq_inodos[directorio[i].dir_inodo].size_fichero);
        printf("inodos:%d\t",directorio[i].dir_inodo);
        printf("bloques: ");
        for (j = 0; ; j++)
        {
            if (inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]==0xFFFF)
                break;
            printf("%d ",inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
        }
        printf("\n");
    }   
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, FILE *fich){
    int i,flag=0;
    for ( i = 0; i < MAX_FICHEROS; i++)
    {
        if (strcmp(directorio[i].dir_nfich,nombrenuevo)==0)
        {
            printf("ERROR: El fichero %s ya existe\n",directorio[i].dir_nfich);
            return 0;
        }
        if (strcmp(directorio[i].dir_nfich,nombreantiguo)==0)
        {
            flag=1;
            strcpy(directorio[i].dir_nfich,nombrenuevo);
        }
    }
    if (flag==0)
        printf("ERROR: Fichero %s no encontrado\n",nombreantiguo);
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
    int i,j,inodo,flag=0;
    int bloques[MAX_NUMS_BLOQUE_INODO];
    char texto[4][SIZE_BLOQUE];
    for ( i = 0; i < 4; i++)
    {
        strcpy(texto[i],"");
    }
    for (i = 0; i < MAX_FICHEROS; i++)
    {
        if (strcmp(directorio[i].dir_nfich,nombre)==0){
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        printf("ERROR: Fichero %s no encontrado\n",nombre);
        return 0;
    }
    inodo=directorio[i].dir_inodo;
    for ( i = 0; i < MAX_NUMS_BLOQUE_INODO; i++)
    {
        if (inodos->blq_inodos[inodo].i_nbloque[i]==0xFFFF)
                break;
        bloques[i]=inodos->blq_inodos[inodo].i_nbloque[i];
        memcpy(&texto[i],memdatos[bloques[i]-4].dato, SIZE_BLOQUE);
    }
    texto[i][0]='\0';
    puts(texto[0]);
    return 1;
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich){
    int i,j,inodo,bloques[MAX_NUMS_BLOQUE_INODO],flag=0;
    for ( i = 0; i < MAX_FICHEROS; i++)
    {
        if (strcmp(directorio[i].dir_nfich,nombre)==0){
             flag=1;
            break;
        }
    }
    if (flag==0)
    {
        printf("ERROR: Fichero %s no encontrado\n",nombre);
        return 0;
    }
    strcpy(directorio[i].dir_nfich,"");                                            //Recursos de directorio
    inodo=directorio[i].dir_inodo;
    directorio[i].dir_inodo=0xFFFF;
    inodos->blq_inodos[inodo].size_fichero=0;                                      //Recursos inodos y bytemaps
    ext_bytemaps->bmap_inodos[inodo]=0;     
    for ( j = 0; j < MAX_NUMS_BLOQUE_INODO; j++)
    {
        if (inodos->blq_inodos[inodo].i_nbloque[j]==0xFFFF)
            break;

        ext_bytemaps->bmap_bloques[inodos->blq_inodos[inodo].i_nbloque[j]]=0;
        inodos->blq_inodos[inodo].i_nbloque[j]=0xFFFF;
    }
    ext_superblock->s_free_blocks_count=ext_superblock->s_free_blocks_count+j;     //Recursos superbloque
    ++ext_superblock->s_free_inodes_count;
    return 0;
}

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich){
    int i,j,x,y,num_bloques,flag=0;
    for ( i = 0; i < MAX_FICHEROS; i++)              //comprobacion ficheros
    {
        if (strcmp(directorio[i].dir_nfich,nombredestino)==0)
        {
            printf("ERROR: El fichero %s ya existe\n",nombredestino);
            return 0;
        }
        if (strcmp(directorio[i].dir_nfich,nombreorigen)==0)
        {
            flag=1;
        }
    }
    if (flag==0)
    {
        printf("ERROR: Fichero %s no encontrado\n",nombreorigen);
        return 0;
    }
    for (i= 0; i < MAX_FICHEROS ; i++)          //asignacion directorio e inodo
    {
        if (directorio[i].dir_inodo==0xFFFF)
        {
            strcpy(directorio[i].dir_nfich,nombredestino);
            for ( j = 0; j < MAX_INODOS; j++)
            {
                if (ext_bytemaps->bmap_inodos[j]==0)
                {
                    ext_bytemaps->bmap_inodos[j]=1;
                    directorio[i].dir_inodo=j;
                    break;
                }
            }  
            break;
        }
    }
    int inodoD=directorio[i].dir_inodo;
    for ( j = 0; j < MAX_FICHEROS; j++)
    {
        if (strcmp(directorio[j].dir_nfich,nombreorigen)==0)
        {
            int inodoO=directorio[j].dir_inodo;
            inodos->blq_inodos[inodoD].size_fichero=inodos->blq_inodos[inodoO].size_fichero;//asigna tamaño fichero
            for ( num_bloques= 0; num_bloques < MAX_BLOQUES_PARTICION; num_bloques++)       //bucle para numero de bloques del fichero origen
            {
                if (inodos->blq_inodos[inodoO].i_nbloque[num_bloques]==0xFFFF)
                    break;
            }
            for (x = 0,y=0; y < num_bloques; x++)                                           //bucle para asignar bloques
            {
                if(ext_bytemaps->bmap_bloques[x]==0){
                    ext_bytemaps->bmap_bloques[x]=1;
                    inodos->blq_inodos[inodoD].i_nbloque[y]=x;
                    ++y;
                }
            }
            break;
        }
    }
    int inodoO=directorio[j].dir_inodo;
    int bloquesO[num_bloques],bloquesD[num_bloques];
    ext_superblock->s_free_blocks_count=ext_superblock->s_free_blocks_count-num_bloques;     //actualizacion superbloque
    --ext_superblock->s_free_inodes_count;
    for ( i = 0; i < num_bloques; i++)                                                       //asignacion de memoria por cada bloque
    {
        int bloqueD=inodos->blq_inodos[inodoD].i_nbloque[i]-4;
        int bloqueO=inodos->blq_inodos[inodoO].i_nbloque[i]-4;
        memcpy(memdatos[bloqueD].dato,memdatos[bloqueO].dato,SIZE_BLOQUE);
    }
}