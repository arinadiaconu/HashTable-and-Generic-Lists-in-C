/* DIACONU Arina-Ioana */

/*-- Liste2Generice.h -- Lista generica dublu inalntuita, circulara
              si info = pointer la element --*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef _LISTA_DUBLU_INLANTUITA_
#define _LISTA_DUBLU_INLANTUITA_

typedef struct celula2
{
  void *info;                 /* adresa element extern */
  struct celula2 *pre, *urm;  /* legaturi spre celulele vecine */
} TCelula2, *TLDI;

//inserarea primului element in lista
int InserareListaVida(TLDI *a, void* x);

//inserarea inainte de inceputul listei
int InserareListaInaintePrim(TLDI *a, void* x);

//inserare inainte de un element diferit de inceputul listei
int InserareListaInainte(TLDI *a, void* x);

//distrugere lista
void DistrugeLista(TLDI *s);

//numarul de elemente din lista
size_t LungimeLG(TLDI* a);  

#endif
