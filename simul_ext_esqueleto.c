#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"
#include "func.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo ,FILE *fich);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()
{
	 char comando[LONGITUD_COMANDO];
	 char orden[LONGITUD_COMANDO];
	 char argumento1[LONGITUD_COMANDO];
	 char argumento2[LONGITUD_COMANDO];
	 
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
     //...
     
     fent = fopen("particion.bin","r+b");
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    

     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BYTE_MAPS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     char com[10],string[100],string2[100];
      
     // Buce de tratamiento de comandos
     for (;;){
		 do {
		 printf (">> ");
		 fflush(stdin);
		 fgets(comando, LONGITUD_COMANDO, stdin);
		 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
	      if (strcmp(orden,"dir")==0) {
            Directorio(directorio,&ext_blq_inodos);
            continue;
            }
         if (strcmp(orden,"h")==0) {
            printf("\n1. info\n2. dir\n3. rename \"originalname\" \"newname\"\n4. imprimir \"filename\"\n5. bytemaps\n6. remove \"filename\"\n7. copy \"originalname\" \"newfilename\"\n8. salir\n\n");
            continue;
            }
         if (strcmp(orden,"bytemaps")==0) {
            Printbytemaps(&ext_bytemaps);
            continue;
            }
         if (strcmp(orden,"rename")==0) {
            Renombrar(directorio,&ext_blq_inodos,argumento1,argumento2, fent);
            continue;
            }
         if (strcmp(orden,"imprimir")==0) {
            Imprimir(directorio,&ext_blq_inodos,memdatos,argumento1);
            continue;
            }
         if (strcmp(orden,"remove")==0) {
            Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,argumento1,fent);
            continue;
            }
         if (strcmp(orden,"copy")==0) {
            Copiar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,memdatos,argumento1,argumento2,fent);
            continue;
            }
         if (strcmp(orden,"info")==0) {
            LeeSuperBloque(&ext_superblock);
            continue;
            }
         //Si el comando es salir se habrán escrito todos los metadatos
         if (strcmp(orden,"salir")==0){
            Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
            GrabarByteMaps(&ext_bytemaps,fent);
            GrabarSuperBloque(&ext_superblock,fent);
            GrabarDatos(memdatos,fent);
            fclose(fent);
            return 0;
         }
     }
}
