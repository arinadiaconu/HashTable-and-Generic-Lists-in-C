/* DIACONU Arina-Ioana */

/*              --- Liste2Generice.c ---
    operatii lista generica dublu inlantuita, circulara */

#include "Liste2Generice.h"

//inserarea primului element
int InserareListaVida(TLDI *a, void* x)  
{
	//se aloca o celula de lista
    TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) return 0;      //testarea alocarii
	
    //atribuirea pointerului la continutul listei
    aux->info = x;

    //se fac legaturile
    aux->pre = aux;
    aux->urm = aux;

    //se retine primul element din lista
    *a = aux;
    
    return 1;
}

//inserare inainte de primul element din lista
int InserareListaInaintePrim(TLDI *a, void* x) 
{
    //se aloca o celula de lista
    TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) return 0;      //testarea alocarii

    //atribuirea pointerului la continutul listei
    aux->info = x;
    
    //se refac legaturile
    aux->urm = *a;
    aux->pre = (*a)->pre;
    (*a)->pre->urm = aux;
    (*a)->pre = aux;
    
    //se retine adresa noului prim element din lista
    *a = aux;
    
    return 1;
}

//inserare inainte de un element diferit de primul
int InserareListaInainte(TLDI *a, void* x)
{
    //se aloca o celula de lista
    TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) return 0;      //testarea alocarii

    //atribuirea pointerului la continutul listei
    aux->info = x;

    //se refac legaturile
    aux->urm = *a;
    aux->pre = (*a)->pre;
    aux->pre->urm = aux;
    aux->urm->pre = aux;

    return 1;
}

// transforma lista in lista vida
void DistrugeLista(TLDI *s)
{
	TLDI p = (*s)->urm, aux;
	while(p != *s)   //parcurgerea listei
	{
		aux = p;    
		p = p->urm;
		free(aux->info);    //eliberarea informatiei
		free(aux);  //eliberarea celulei
	}
    //eliberarea ultimei celule
	aux = p;
	free(aux->info);
	free(aux);
    *s = NULL;
}

// numarul de elemente din lista
size_t LungimeLG(TLDI* a)   
{
	size_t lg = 0;
	TLDI p = *a;
    if(*a == NULL)  //lista vida
        return lg;

    // parcurge lista, numarand celulele
	for (lg = 1, p = p->urm; p != *a; p = p->urm)
        lg++;

	return lg;
}
