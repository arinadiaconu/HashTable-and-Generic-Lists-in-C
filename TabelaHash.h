/* DIACONU Arina-Ioana */

/*           ---TabelaHash.h ---
    functii de prelucrare ale tabelei hash */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "Liste2Generice.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFElem)(void *);        // functie prelucrare element
typedef int (*TFCmp)(void *, void *); // functie de comparare doua elemente
typedef void (*TF)(void *);           // functie eliberare un element
typedef void (*TFA)(void *, FILE *);  // functie afisare element
typedef int (*TFHash)(void *, int);   // functia hash

//structura pentru tabela hash
typedef struct
{
    size_t M;   //numarul de liste din tabela
    TFHash fh;  //functia hash
    TLDI *v;    //vectorul de liste
} TH;

/* functii tabela hash */
TH* InitializareTabelaHash(size_t M, TFHash fh);
void DistrugeTabelaHash(TH **ah, TF feliberare_element);
void AfisareTabelaHash(TH *ah, TFA afisareElem, FILE *f);

//inserare ordonata in tabela hash
int InserareTabelaHash(TH *a, void *ae, TFCmp fcompara, int nr_bucketuri);

//stergerea unui element din tabela hash
int StergTH(TH *h, void *ae, TFCmp fcompara, TF f, int nr_bucketuri);

/* functia cauta un element in tabela si intoarce 'True'
    sau 'False' in functie de rezultatul cautarii */
char* ExistaTabelaHash(TH *h, void *ae, TFCmp fcompara, int nr_bucketuri);

void* GasesteTabelaHash(TH *a, void *ae, TFCmp fcompara, int nr_bucketuri);
void AfisareBucket(TH *ah, int n, FILE *f, TFA afisareElem);
#endif