


/*
	-Verifie si l'argument d'une commande PUSH ou équivalent est bel et bien une case memoire deja utilisée.
	En effet, dans le cas d'un PUSH qui n'a été précédé d'aucun READ, la valeur empilée sera soit 0, c'est-à-dire
	la valeur contenue dans toute case mémoire à l'initialisation, soit une autre donnée. Ainsi, afin d'éviter des
	manipulations hasardeuses de la mémoire, la fonction parcoure un tableau d'adresses déjà utilisées par le programme.
	Si l'adresse mémoire en question n'a toujours pas été utiisée, la fonction renvoie 1. Sinon, elle renvoie 0.	
*/
/********************************************************************/
int Dans_Memoire(int argument, int *memoire_occupee, int taille_mo)
{
	for (int i = 0; i < taille_mo; i++)
	{
		if (memoire_occupee[i] == argument)
		{
			return 1;
		}
	}
	return 0;
}

/*
	-Crée les maillons de la liste chainée d'instructions à partir du nombre d'instructions du programme.
*/

/********************************************************************/

void creer_Tasks(int Nbr_Lignes, P_de_T *Instr)
{

	Tasks *Ptr_Tasks = malloc(sizeof(Tasks));
	if (Ptr_Tasks == NULL)
	{
		exit(1);
	}

	Ptr_Tasks = Instr->First_Task;
	int i;
	for (i = 0; i < Nbr_Lignes; i++)
	{
		Ptr_Tasks->next = malloc(sizeof(Tasks));
		if (i != (Nbr_Lignes - 1))
		{
			Ptr_Tasks = Ptr_Tasks->next;
		}
		else
		{
			Ptr_Tasks->next = NULL;
		}
	}
}
/*
	-Remplit la liste chainée l'instructions à partir d'un tableau.
	D'abord, le fichier hexa.txt est lu ligne par ligne et un tableau de lignes est remplie par celles-ci.
	Une fois la lecture terminée, la fonction se charge de séparer le code instruction et l'argument
	en détectant les espaces les séparant.
*/
/*************************************************************************************/

void Liste_des_instructions(char *machine, int Nbr_Lignes, P_de_T *Instr)
{
	FILE *src = fopen(machine, "r");
	Tasks *Crea = malloc(sizeof(Tasks));
	Crea = Instr->First_Task;

	int i;
	char ** lignes = malloc((Nbr_Lignes + 1) *sizeof(char *));

	for (i = 0; i < Nbr_Lignes; i++)
	{

		lignes[i] = malloc(tailleMAX *sizeof(char) + 1);

		fgets(lignes[i], tailleMAX, src);

		int j = 0;

		while (lignes[i][j] != ' ' && lignes[i][j] != '\0')
		{
			j++;
		}
		Crea->Ins = malloc(j *sizeof(char));
		int k = 0;
		for (k = 0; k < j; k++)
		{
			Crea->Ins[k] = lignes[i][k];
			if (k == j - 1)
			{
				Crea->Ins[j] = '\0';
			}
		}

		int d = j + 1;

		while (lignes[i][d] != '\n' && lignes[i][j] != '\0')
		{
			d++;
		}
		Crea->Adr = malloc((d - j - 1) *sizeof(char));

		k = 0;
		for (k = 0; k < (d - j - 1); k++)
		{

			Crea->Adr[k] = lignes[i][j + k + 1];
			if (k == (d - j - 2))
			{
				Crea->Adr[d - j - 1] = '\0';
			}
		}
		Crea->rang = i;
		Crea = Crea->next;
	}

	fclose(src);

}
/*
	-Execute a proprement parler le programme.
	Les pointeurs entiers PC et SP Sont initialisés à 0 au début, et la mémoire de travail est créée.
	
*/
/****************************************************************************************************************************************/

void Execution_Langage_Machine(char *machine, int Nbr_Lignes)
{
	// ON INITIALISE PC ET SP à 0//
	int PC = 0;
	int *Tableau_Memoire;
	Tableau_Memoire = calloc(5000, sizeof(int));
	if (Tableau_Memoire == NULL)
	{
		printf("La mamoire n'a pas ete allouee.\n");
		exit(1);
	}
	int SP = 0;

	// CREE LA LISTE CHAINEE D'INSTRUCTION
	P_de_T *Instructions = malloc(sizeof(P_de_T));
	Instructions->First_Task = malloc(sizeof(Tasks));
	creer_Tasks(Nbr_Lignes, Instructions);
	Instructions->taille = Nbr_Lignes;

	//FAIT POINTER SP VERS LE PREMIER ELEMENT VIDE DE LA PILE
	Liste_des_instructions(machine, Nbr_Lignes, Instructions);

	Tasks *tmp = malloc(sizeof(Tasks));
	tmp = Instructions->First_Task;
	while (tmp != NULL)
	{
		printf("%s %s\n", tmp->Ins, tmp->Adr);
		tmp = tmp->next;
	}
	printf("\n");

	//ICI ON PREND L'INSTRUCTION ET L'ARGUMENT, CONVERTIT LEURS VALEURS EN DECIMAL ET ON INCREMENTE PC
	int instruction;
	int argument;
	int booleen_halt = 0;
	int taille_mo = 0;
	int *memoire_occupee = calloc(taille_mo, sizeof(int));
	Tasks *Ptr_Tasks;
	Ptr_Tasks = malloc(sizeof(Tasks));

	//CONTINUE D'EXECUTER LE PROGRAMME TANT QUE HALT N'A PAS ETE EXECUTEE
	while (booleen_halt == 0)
	{
		
		Ptr_Tasks = Instructions->First_Task;
		while (Ptr_Tasks != NULL && Ptr_Tasks->rang != PC)
		{
			Ptr_Tasks = Ptr_Tasks->next;
		}
		instruction = strtol(Ptr_Tasks->Ins, NULL, 16);
		argument = strtol(Ptr_Tasks->Adr, NULL, 16);

		//printf("%s %s\n",Ptr_Tasks->Ins,Ptr_Tasks->Adr);
		PC = PC + 1;
		//printf("PC : %d\n",PC);
		switch (instruction)
		{
			case (0): //push
				if (!Dans_Memoire(argument, memoire_occupee, taille_mo))
				{
					printf("Erreur, aucune valeur a l'adresse %d\n", argument);
					exit(1);
				}
				Tableau_Memoire[SP] = Tableau_Memoire[argument];
				SP = SP + 1;
				break;
			case (1): //ipush
			if(Tableau_Memoire[SP - 1]<0 || Tableau_Memoire[SP - 1]>4999)
			{
				printf("Erreur: l'adresse memoire %d n'existe pas. Fin du programme.\n",Tableau_Memoire[SP - 1]);
				exit(1);
			}
				Tableau_Memoire[SP - 1] = Tableau_Memoire[Tableau_Memoire[SP - 1]];
				break;
			case (2): //push#
				Tableau_Memoire[SP] = argument;
				SP = SP + 1;
				break;
			case (3): //pop
				if(SP==0)
				{
					printf("Erreur: la pile ne peut etre depilee car elle est vide. Fin du programme.\n");
					exit(1);
				}
				SP = SP - 1;
				Tableau_Memoire[argument] = Tableau_Memoire[SP];
memoire_occupee = realloc(memoire_occupee, (taille_mo + 1) *sizeof(int));
	memoire_occupee[taille_mo] = argument;
	taille_mo = taille_mo + 1;
				break;
			case (4): //ipop
			if(Tableau_Memoire[SP - 1]<0 || Tableau_Memoire[SP - 1]>4999)
			{
				printf("Erreur: l'adresse memoire %d n'existe pas. Fin du programme.\n",Tableau_Memoire[SP - 1]);
				exit(1);
			}
				if(SP<=1)
				{
					printf("Erreur: la pile ne contient qu'un seul element. Fin du programme.\n");
					exit(1);
				
				}
memoire_occupee = realloc(memoire_occupee, (taille_mo + 1) *sizeof(int));
	memoire_occupee[taille_mo] = argument;
	taille_mo = taille_mo + 1;
				Tableau_Memoire[Tableau_Memoire[SP - 1]] = Tableau_Memoire[SP - 2];
				SP = SP - 2;

				break;
			case (5): //dup
				if(SP==0)
				{
					printf("Erreur: la pile ne contient aucun element a dupliquer. Fin du programme.\n");
					exit(1);
				}
				Tableau_Memoire[SP] = Tableau_Memoire[SP - 1];
				SP = SP + 1;
				break;
			case (6): //op
				if(SP==0)
				{
					printf("Operation impossible: la pile ne contient aucun element. Fin du programme.\n");
					exit(1);
				}
				switch (argument)
				{
					case (0): //ET LOGIQUE BIT A BIT
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] & Tableau_Memoire[SP];
						break;
					case (1): //OU LOGIQUE BIT A BIT
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] | Tableau_Memoire[SP];
						break;
					case (2): //OU-EXCLUSIF LOGIQUE BIT A BIT
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] ^ Tableau_Memoire[SP];
						break;
					case (3): //NON LOGIQUE BIT A BIT
						Tableau_Memoire[SP - 1] = ~Tableau_Memoire[SP - 1];
						break;
					case (4): //INVERSE LA VALEUR AU SOMMET DE LA PILE
						Tableau_Memoire[SP - 1] = -Tableau_Memoire[SP - 1];
						break;
					case (5): //ADDITION
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] + Tableau_Memoire[SP];
						break;
					case (6): //SOUSTRACTION
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] - Tableau_Memoire[SP];
						break;
					case (7): //MULTIPLICATION
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] * Tableau_Memoire[SP];
						break;
					case (8): //DIVISION
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] / Tableau_Memoire[SP];

						break;
					case (9): //MODULO
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						Tableau_Memoire[SP - 1] = Tableau_Memoire[SP - 1] % Tableau_Memoire[SP];
						break;
					case (10): //TEST D'EGALITE
						if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] == Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
						}
						break;
					case (11): //TEST D'INEGALITE
					if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] != Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
						}
						break;
					case (12): //TEST >
					if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] > Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
						}
						break;
					case (13): //TEST >=
					if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] >= Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
						}
						break;
					case (14): //TEST <
					if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] < Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
						}
						break;
					case (15): //TEST <=
					if(SP<=1)
						{
							printf("Operation impossible: la pile ne contient pas assez d'elements. Fin du programme.\n");
							exit(1);
						}
						SP = SP - 1;
						if (Tableau_Memoire[SP - 1] <= Tableau_Memoire[SP])
						{
							Tableau_Memoire[SP - 1] = 0;
						}
						else
						{
							Tableau_Memoire[SP - 1] = 1;
							break;
						}
						break;
					default:
						printf("Erreur.\n");
						exit(1);
				}
				break;
			case (7): //jmp

				PC = PC + argument;
				break;
			case (8): //jpz
				SP = SP - 1;
				if (Tableau_Memoire[SP] == 0)
				{
					PC = PC + argument;
				}
				break;
			case (9): //call
				Tableau_Memoire[SP] = PC;
				SP = SP+1;
				PC = PC + argument;
				break;
			case (10): //ret
				PC = Tableau_Memoire[SP - 1];
				SP = SP - 1;
				break;
			case (11): //rnd
				Tableau_Memoire[SP] = rand() % argument;
				SP = SP + 1;
				break;
			case (12): //write
				printf("%d\n", Tableau_Memoire[argument]);
				break;
			case (13): //read

				printf("ENTREZ UN NOMBRE: ");
				int retour = scanf("%d", &Tableau_Memoire[argument]);
				
				if (retour == 0)
				{
					printf("Erreur. Fin d'execution.\n");
					exit(1);
				}

	memoire_occupee = realloc(memoire_occupee, (taille_mo + 1) * sizeof(int));
	memoire_occupee[taille_mo] = argument;
	taille_mo = taille_mo + 1;
	
				break;
			case (99): //halt
				booleen_halt = 1;
				break;
			default:
				printf("Erreur.\n");
				exit(1);
		}
	}
	free(memoire_occupee);
	free(Tableau_Memoire);
}
/****************************************************************************************************************************************/
