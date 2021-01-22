#ifndef STRUCTURES_H
#define STRUCTURES_H


//STRUCTURE D'ELEMENT

/********************************************************************/
typedef struct Tasks
{
	int rang;
	char *Ins;
	char *Adr;
	struct Tasks *next;
}
Tasks;


//STRUCTURE DE LISTE CHAINEE

/********************************************************************/

typedef struct Pile_de_Tasks
{
	int taille;
	Tasks *First_Task;
}
P_de_T;



#endif
