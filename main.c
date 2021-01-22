#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#define tailleMAX 100
#define tailleMAXetiquette 100
#include "traduction.c"
#include "execution.c"


int main(int argc, char *argv[])
{
	if (argc < 2)//VERIFIE S'IL N'Y A PAS DE FICHIER EN ARGUMENT
	{
		printf("Pas assez d'arguments.\n");
		exit(1);
	}
	if (argc > 2)//VERIFIE SI LE NOMBRE D'ARGUMENTS EST TROP ELEVE
	{
		printf("Pas assez d'arguments.\n");
		exit(1);
	}
	FILE *src = fopen(argv[1], "r");
	if (src == NULL)
	{
		printf("Le fichier %s ne s'ouvre pas.\n", argv[1]);
		exit(1);
	}
	FILE *dst = fopen("hexa.txt", "w");
	if (dst == NULL)
	{
		printf("Le fichier hexa.txt n'arrive pas a se generer.\n");
		exit(1);
	}
	int n = Nombre_Lignes(src);
	char ** lignes = malloc((n + 1) *sizeof(char *));
	char ** lignes_bis = malloc((n + 1) * 12);
	char ** etiquettes = malloc((n + 1) *sizeof(char *));

	rewind(src); //REMET LE CURSEUR AU DEBUT DU FICHIER

	/*int i, j, k, l, argcode;
	int booleen_halt = 0;
	int booleen_etiquette = 0;*/
	

	for (int j = 0; j < n; j++)
	{
		lignes_bis[j] = malloc(tailleMAX *sizeof(char) + 1);
		if (lignes_bis[j] == NULL)
		{
			printf("ERREUR D'ALLOCATION DE LIGNES_BIS[J].\n");
			exit(1);
		}
		etiquettes[j] = malloc(tailleMAXetiquette *sizeof(char) + 1);
		if (etiquettes[j] == NULL)
		{
			printf("ERREUR D'ALLOCATION DE ETIQUETTES[J].\n");
			exit(1);
		}
	}

	rewind(src); //REMET LE CURSEUR AU DEBUT DU FICHIER
	
	/*****************************
	     
	     PARTIE TRADUCTION
	     
	*****************************/
	
	Source_Vers_Lignes(src, lignes, n);
	Remplir_Etiquettes(etiquettes, lignes, n);
	Verification_Etiquettes(etiquettes, n);
	Traduction(lignes, etiquettes, lignes_bis, n);
	Lignes_A_Destination(lignes_bis, dst, n);
	
	free(lignes);
	free(lignes_bis);
	free(etiquettes);
	fclose(src);
	fclose(dst);
	
	/*****************************
	     
	     PARTIE EXECUTION
	     
	*****************************/
	
	srand(time(NULL));

	P_de_T *Instructions = malloc(sizeof(P_de_T));
	Instructions->First_Task = malloc(sizeof(Tasks));
	char fichier[9] = "hexa.txt\0";
	Execution_Langage_Machine(fichier, n);
	printf("ok");
	printf("Execution terminee.\n");
	
	return 0;
}
