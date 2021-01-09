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
              char *nombreantiguo, char *nombrenuevo);
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
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     /*
     Printbytemaps(&ext_bytemaps);
     LeeSuperBloque(&ext_superblock);
     char prueba[10]="HOLA.txt";
     int h = BuscaFich(directorio,&ext_blq_inodos,prueba);
     printf("%d\n",h);
     Directorio(directorio,&ext_blq_inodos);
     i=Renombrar(directorio,&ext_blq_inodos,"HOLA.txt","OLAKEASE.txt");
     Directorio(directorio,&ext_blq_inodos);
     i=Imprimir(directorio,&ext_blq_inodos,memdatos,"BelloGal.txt");
     Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,"OLAKEASE.txt",fent);
     Printbytemaps(&ext_bytemaps);
     Directorio(directorio,&ext_blq_inodos);
     printf("////////////////%s",memdatos[2].dato);
     Copiar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,memdatos,"BelloGal.txt","Hola.txt",fent);
     printf("\n");
     Directorio(directorio,&ext_blq_inodos);
     Printbytemaps(&ext_bytemaps);
     Imprimir(directorio,&ext_blq_inodos,memdatos,"Hola.txt");
     printf("///////////////%s",memdatos[2].dato)*/
     
      char com[10],string[100],string2[100];
      /*for(;;){
         printf ("\n>> ");
		   fflush(stdin);
		   fgets(com, LONGITUD_COMANDO, stdin);
         for ( i = 0;com[i]!='\0'; i++)
         {
         }
         com[i-1]='\0';
         if (strcmp(com,"info")==0)
            LeeSuperBloque(&ext_superblock);
         else if(strcmp(com,"h")==0)
            printf("info-dir-rename-imprimir-bytemaps-remove-copy");
         else if(strcmp(com,"bytemaps")==0)
            Printbytemaps(&ext_bytemaps);
         else if(strcmp(com,"dir")==0)
            Directorio(directorio,&ext_blq_inodos);
         else if(strcmp(com,"rename")==0){
            printf("\nNombre archivo:");
		      gets(string);
            printf("\nNombre nuevo:");
		      gets(string2);
            Renombrar(directorio,&ext_blq_inodos,string,string2);////
         }
         else if(strcmp(com,"imprimir")==0){
            printf("\nNombre archivo:");
		      gets(string);
            Imprimir(directorio,&ext_blq_inodos,memdatos,string);
         }
         else if(strcmp(com,"remove")==0){
            printf("\nNombre archivo:");
		      gets(string);
            Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,string,fent);
         }
         else if(strcmp(com,"copy")==0){
            printf("\nNombre archivo origen:");
		      gets(string);
            printf("\nNombre archivo nuevo:");
		      gets(string2);
            Copiar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,memdatos,string,string2,fent);
         }
         else
         {
            printf("\nerror");
         }
         
      }*/
     // Buce de tratamiento de comandos
     for (;;){
		 do {
		 printf (">> ");
		 fflush(stdin);
		 fgets(comando, LONGITUD_COMANDO, stdin);
		 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
	      if (strcmp(orden,"dir")==0) {
            Directorio(&directorio,&ext_blq_inodos);
            continue;
            }
         if (strcmp(orden,"h")==0) {
            printf("\n1. info\n2. dir\n3. rename \"originalname\" \"newname\"\n4. imprimir \"filename\"\n5. bytemaps\n6. remove \"filename\"\n7.copy \"originalname\" \"newfilename\"\n\n");
            continue;
            }
         if (strcmp(orden,"bytemaps")==0) {
            Printbytemaps(&ext_bytemaps);
            continue;
            }
         if (strcmp(orden,"rename")==0) {
            Renombrar(directorio,&ext_blq_inodos,argumento1,argumento2);
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
         // Escritura de metadatos en comandos rename, remove, copy     
         //Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
         //GrabarByteMaps(&ext_bytemaps,fent);
         //GrabarSuperBloque(&ext_superblock,fent);
         //if (grabardatos)
         //  GrabarDatos(&memdatos,fent);
         //grabardatos = 0;
         //Si el comando es salir se habrán escrito todos los metadatos
         //faltan los datos y cerrar
         if (strcmp(orden,"salir")==0){
         //   GrabarDatos(&memdatos,fent);
            fclose(fent);
            return 0;
         }
     }
}
