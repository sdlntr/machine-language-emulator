


/*
Déclare toutes les instructions utilisées dans les fichiers à traduire dans un tableau, ainsi que les code instructions correspondants.
Chaque code possède le même indice dans son tableau que l'instruction qui lui correspond dans l'autre tableau.
*/
char *instructions[15] = {
	"push", "ipush", "push#", "pop", "ipop", "dup", "op", "jmp", "jpz", "call", "ret", "rnd", "write", "read", "halt"
};
char *codes_instructions[15] = {
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "63"
};

//Renvoie le nombre de lignes contenues dans le fichier à traduire.
int Nombre_Lignes(FILE *src)
{
	char caractere;
	int nbr = 0;
	if (src == NULL)
	{
		return -1;
	}
	while (caractere != EOF)
	{
		caractere = getc(src);
		if (caractere == '\n')
		{
			nbr += 1;
		}
	}

	return nbr;
}

//Alloue dynamiquement chaque ligne du tableau d'instructions et y place les instructions du programme.
void Source_Vers_Lignes(FILE *src, char ** lignes, int n)
{
	for (int i = 0; i < n; ++i)
	{
		lignes[i] = calloc(tailleMAX + 1, sizeof(char));
		if (lignes[i] == NULL)
		{
			printf("ERREUR D'ALLOCATION DE LIGNES[%d].\n", i);
			exit(1);
		}
		fgets(lignes[i], tailleMAX, src);
	}
}

/*
	-Vérifie que chaque etiquette est unique. 
	Dans le cas où deux etiquettes pour deux lignes différentes sont similaires, la fonction affiche un message d'erreur et arrête le programme.
*/
void Verification_Etiquettes(char ** etiquettes, int n)
{

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (etiquettes[i][0] != '?' && etiquettes[j][0] != '?' && strcmp(etiquettes[i], etiquettes[j]) == 0)
			{
				printf("Erreur, il y a la meme etiquette <%s> pour les lignes %d et %d.\n", etiquettes[i], j + 1, i + 1);
				exit(1);
			}
		}
	}
}

/*
    -Détecter la présence d'étiquettes pour chaque ligne du fichier.
    -Stocker éventuellement les étiquettes dans un tableau, sinon mettre "?" pour désigner une absence d'étiquette à la dite ligne.
*/

void Remplir_Etiquettes(char ** etiquettes, char ** lignes, int n)
{
	int booleen_etiquette = 0;
	int i = 0;
	while (i < n) //PARCOURS DU TABLEAU DE LIGNES AFIN DE DETECTER LES ETIQUETTES
	{
		booleen_etiquette = 0;
		int j = 0;
		int k;
		for (k = 0; lignes[i][k] != '\0'; k++)
		{
			if (lignes[i][k] == ':')
			{
				j = k;
				booleen_etiquette = 1;
			}
		}
		if (booleen_etiquette == 1) //SI UNE ETIQUETTE EST PRESENTE, ON LA RECUPERE
		{
			int debut = 0;
			while (lignes[i][debut] == ' ')
			{
				debut = debut + 1;
			}
			for (k = 0; k < j - debut; k++)
			{
				etiquettes[i][k] = lignes[i][debut + k];
			}
			etiquettes[i][k] = '\0';
		}
		else
		{
			etiquettes[i] = "?";
		}
		i = i + 1;
	}
}

/*
  Traduit les instructions contenues dans le tableau Instructions du langage assembleur au langage machine,
  puis mettre chaque ligne traduite dans un tableau contenant les instructions traduites.
*/
void Traduction(char ** lignes, char ** etiquettes, char ** lignes_bis, int n)
{
	int k;
	int l;
	int argcode;
	int booleen_halt=0;
	for (int i = 0; i < n; i++)
	{
		int j = 0;

		for (k = 0; lignes[i][k] != '\0'; k++) //PARCOURE CHAQUE LIGNE CARACTERE PAR CARACTERE JUSQU'A L'INSTRUCTION
		{
			if (lignes[i][k] == ':') //TESTE LA PRESENCE DES DEUX POINTS REPRESENANT LA FIN D'UNE ETIQUETTE
			{
				j = k + 1;
				//booleen_etiquette = 1;
				break;
			}
		}
		char caractere = lignes[i][j];
		while (caractere == ' ' || caractere == '\t')
		{
			j = j + 1;
			caractere = lignes[i][j];
		}
		k = 0;
		while (lignes[i][j + k] != ' ' && lignes[i][j + k] != '\0')
		{
			k = k + 1;
		}
		char *instruction = calloc((k + 1), sizeof(char));
		for (l = 0; l < k && lignes[i][(j + l)] != '\n'; l++)
		{
			instruction[l] = lignes[i][(j + l)];
		}

		j = j + k + 1;
		instruction[k] = '\0';

		l = 0;
		while (l < 15 && strcmp(instruction, instructions[l]) != 0) //RECONAISSANCE DES INSTRUCTIONS
		{
			l = l + 1;
		}

		if (l >= 15) //L'INSTRUCTION EST INCONNUE, ON RENVOIE UN MESSAGE D'ERREUR
		{
			printf("Erreur: L'instruction a la ligne %d du fichier est incorrecte.\n", i + 1);
			exit(1);
		}
		else
		{
			lignes_bis[i][0] = codes_instructions[l][0];
			lignes_bis[i][1] = codes_instructions[l][1];
			lignes_bis[i][2] = ' ';
		}
		if (l == 1 || l == 4 || l == 5 || l == 10 || l == 14) //PAS D'ARGUMENTS
		{
			if (lignes[i][j - 1] != '\0')
			{
				printf("Erreur a la ligne %d du fichier, il y a trop d'arguments.\n", i + 1);
				exit(1);
			}
			lignes_bis[i][2] = ' ';
			for (k = 3; k < 11; k++)
			{
				lignes_bis[i][k] = '0';
			}
			lignes_bis[i][k] = '\0';
		}
		else //AVEC ARGUMENTS
		{
			k = j;
			while (lignes[i][k] != '\0')
			{
				k = k + 1;
			}
			k = k - 1;
			char *argument = calloc((k - j + 1), sizeof(char));
			int longueur = k - j + 1;
			for (k = 0; k < longueur - 1; k++)
			{
				argument[k] = lignes[i][j + k];
			}
			if(argument[0]=='\0') //TEST SI L'ARGUMENT EST VIDE ALORS QU'IL DEVRAIT Y EN AVOIR
				{
					printf("Erreur a la ligne %d du fichier, manque d'arguments.\n",i+1);
					exit(1);
				}
			if (l == 7 || l == 8 || l == 9) //L'ARGUMENT DEVRAIT ETRE UNE ADRESSE / ETIQUETTE
			{
				l = 0;
				
				while (l < n && strcmp(argument, etiquettes[l]) != 0)
				{
					l = l + 1;
				}

				if (l >= n)
				{ // L'ADRESSE N'EST PAS UNE ETIQUETTE QUI EXISTE, L'ARGUMENT EST SOIT UN NOMBRE REPRESENTANT UN SAUT POSSIBLE A EFFECTUER, SOIT IL S'AGIT D'UNE ERREUR
					char *endptr;
					argcode = strtol(argument, &endptr, 10); //LES CARACTERES NON NUMERIQUES DE L'ARGUMENT VONT DANS ENDPTR
					if (endptr[0] != '\0') //IL Y A DES CARACTERES PRESENTS ALORS QUE CE N'EST PAS UNE ETIQUETTE RECONNUE, DONC L'ETIQUETTE EST INEXISTANTE
					{

						printf("L'etiquette <%s> n'existe pas. Veuillez verifier que vous n'avez pas entre de caracteres ou d'arguments en trop.\n", argument);
						exit(1);
					}
					else //C'EST UN NOMBRE, DONC LA DIFFERENCE ENTRE LE RANG DE LA LIGNE DESIGNE PAR LE SAUT ET PC(LIGNE ACTUELLE+1)
					{

						if (argcode < (-i - 1) || argcode >= (n - i - 1)) //SI LE SAUT EN QUESTION N'EST PAS POSSIBLE, IL S'AGIT D'UNE ERREUR
						{
							printf("Le saut ne pointe vers aucune instruction existante.\n");
							exit(1);
						}
					}
				}
				else
				{

					argcode = l - i;
					argcode = argcode - 1; //VU QUE L'ON DOIT AJOUTER LA DIFFERENCE A PC, IL FAUT ELARGIR LA DIFFERENCE DE 1

				}

			}
			else //L'ARGUMENT DEVRAIT ETRE UNE ADRESSE MEMOIRE EXISTANTE, UN NOMBRE ENTIER A EMPILER OU UN CODE OPERATION
			{
				int u;
				for (u = 0; argument[u] != '\0'; u++) //ON VERIFIE SI LA PARTIE ARGUMENT DE LA LIGNE NE CONTIENT PAS D'ESPACE, CE QUI VOUDRAIT DIRE QU'IL Y A PLUSIEURS ARGUMENTS
				{
					if (argument[u] == ' ')//UN ESPACE EST DETECTE, ON RENVOIE UN MESSAGE D'ERREUR
					{
						printf("L'argument <%s> a la ligne %d du fichier est invalide. Veuillez verifier si vous n'avez entre de caracteres de d'arguments en trop.\n", argument, i + 1);
						exit(1);
					}
				}
				char *endptr;
				argcode = strtol(argument, &endptr, 10); //ON VERIFIE SI DES CARACTERES NON NUMERIQUES SONT PRESENTS DANS L'ARGUMENT
				if (endptr[0] != '\0')
				{
					printf("L'argument <%s> a la ligne %d du fichier est invalide. L'argument est cense indiquer un nombre.\n", argument, i + 1);
					exit(1);
				}
				if (l == 6) //L'ARGUMENT DEVRAIT ETRE UN CODE OPERATION
				{
					if (argcode > 15 || argcode < 0)
					{
						printf("Erreur a la ligne %d du fichier, le code operation est erronee.\n", i + 1);
						exit(1);
					}
				}
				if (l != 2 && (argcode < 0 || argcode > 4999)) //L'ARGUMENT DEVRAIT ETRE UNE ADRESSE MEMOIRE ENTRE 0 ET 4999, A PART S'IL S'AGIT D'UN PUSH#
				{
					printf("L'argument a la ligne %d du fichier est invalide. Il n'existe pas d'emplacement memoire d'adresse %d.\n", i + 1, argcode);
					exit(1);
				}

			}
			char *argcodestr = calloc(9, sizeof(char));
			sprintf(argcodestr, "%08x", argcode);
			strcat(lignes_bis[i], argcodestr);
			free(argcodestr);
			free(argument);
		}
		if (strcmp(instruction, "halt") == 0) //SI ON VIENT DE TRAITER UN HALT, ON LE SIGNALE EN BACSULANT LE BOOLEEN A 1
		{
			booleen_halt = 1;
		}
		free(instruction);
	}
	if (booleen_halt == 0) //SI APRES AVOIR PARCOURU TOUTES LES INSTRUCTIONS AUCUNE D'ENTRE ELLES N'ETAIT UN HALT, ALORS ON RENVOIE UN MESSAGE D'ERREUR
	{
		printf("Erreur, le progamme ne contient pas d'instruction halt.\n");
		exit(1);
	}
	printf("Succes.\n\n");
}
/*
	-Ecris les lignes traduites en langage machine a partir d'un tableau dans un nouveau fichier destination. 
	Comme chaque ligne est deja au bon format suite a la traduction, tout ce qui reste à faire est de les placer une par une
	dans le fichier avec un retour a la ligne a la fin de chaque ligne du programme.
*/
void Lignes_A_Destination(char ** lignes_bis, FILE *dst, int n)
{
	for (int i = 0; i < n; i++) //ECRIT LES LIGNES TRADUITES DU PROGRAMME EN LANGAGE MACHINE DANS LE NOUVEAU FICHIER
	{
		fprintf(dst, "%s\n", lignes_bis[i]);

	}
}



