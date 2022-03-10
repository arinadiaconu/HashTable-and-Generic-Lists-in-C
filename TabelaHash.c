/* DIACONU Arina-Ioana */

/*  TabelaHash.c - functii tabela hash */


#include "TabelaHash.h"

TH* InitializareTabelaHash(size_t M, TFHash fh)
{
    //alocare tabela hash
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h)     //testarea alocarii
        return NULL;

    //alocare vectori de pointeri la lista dublu inlantuita generica
    h->v = (TLDI *) calloc(M, sizeof(TLDI));
    if(!h->v)   //testarea alocarii
    {
        free(h);
        return NULL;
    }

    h->M = M;   //lungimea vectorului de pointeri
    h->fh = fh; //functia hash atribuita tabelei

    return h;
}

void DistrugeTabelaHash(TH **ah, TF feliberare_element)
{
    TLDI *p, el, aux;
    int i;

    //parcurgerea tabelei cu pointeri
    for (i = 0, p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++, i++) 
    {
        //daca exista elemente corespunzatoare acestui hash
        if(*p != NULL)
        {
            //se parcurge lista pana la penultimul element
            for(el = *p; el->urm != *p; ) 
            {
                //se elibereaza continutul celulei, iar apoi celula
                aux = el;
                el = el->urm;
                feliberare_element(aux->info);
                free(aux);
            }
            //se elibereaza ultima celula din lista
            aux = el;
            feliberare_element(aux->info);
            free(aux);
            (*ah)->v[i] = NULL;
        }
    }
    free((*ah)->v); //eliberarea vectorului de pointeri la liste
    free(*ah);  //eliberarea tabelei hash
    *ah = NULL;
}

void AfisareTabelaHash(TH *ah, TFA afisareElem, FILE *f)
{
    TLDI p, el;
    int i;

    //se parcurge tabela hash  
    for(i = 0; i < ah->M; i++) 
    {
        p = ah->v[i];
        if(p)   //daca lista nu este vida
        {  
            fprintf(f, "%d: ",i);
            //se parcurge lista
            for(el = p; el->urm != p; el = el->urm)
            {
                afisareElem(el->info, f);
                fprintf(f, " ");
            }
            //afisarea ultimului element
            afisareElem(el->info, f);
            fprintf(f, " \n");
        }
    }
}

void AfisareBucket(TH *ah, int n, FILE *f, TFA afisareElem)
{
    //functia este apelata doar daca lista nu este vida

    TLDI p = ah->v[n];
	
    //se parcurge lista, afisandu-se fiecare element
    for(; p->urm != ah->v[n]; p = p->urm)
	{
        afisareElem(p->info, f);
        fprintf(f, " ");
    }
    //afisarea ultimului element
    afisareElem(p->info, f);
    fprintf(f, " \n");
}

//daca elementul exista deja in tabela, functia intoarce 0
//altfel se incearca inserarea elementului si se intoarce rezultatul inserarii
int InserareTabelaHash(TH *a, void *ae, TFCmp fcompara, int nr_bucketuri)
{
    int cod = a->fh(ae, nr_bucketuri), rez;
    TLDI el = a->v[cod];
    
    if(el != NULL)  //daca lista nu este vida
    {
        //mai intai se parcurge lista si se cauta un element identic

        for(el = a->v[cod]; el->urm != a->v[cod]; el = el->urm) 
        {
            //verificare daca elementul a fost deja inserat in tabela
            if (fcompara(el->info, ae) == 0)
                return 0;
        }
        if (fcompara(el->info, ae) == 0)   //ultimul element din lista
            return 0;

        //daca a ajuns aici inseamna ca trebuie inserat elementul
        //se parcurge lista pentru a-l insera ordonat
        for(el = a->v[cod]; el->urm != a->v[cod];) 
        {
            /*primul element mai mare, din punct de vedere lexicografic,
            decat cel care trebuie inserat */
            if (fcompara(el->info, ae) > 0) 
            {
                //inserare inainte de primul element
                if(el == a->v[cod])
                {
                    rez = InserareListaInaintePrim(a->v+cod, ae);
                    return rez;
                }
                else    //inserarea inainte de un element diferit de primul
                {
                    rez = InserareListaInainte(&el, ae);
                    return rez;
                }
            }
            el = el->urm;   //avansul in lista
        }

        /* testul pentru ultimul element 
        sau pentru o lista cu un singur element */
        if (fcompara(el->info, ae) > 0)
        {
            if(el == a->v[cod])
            {
                rez = InserareListaInaintePrim(a->v+cod, ae);
                return rez;
            }
            else
            {
                rez = InserareListaInainte(&el, ae);
                return rez;
            }
        }
        else    //cazul in care nu se gaseste niciun element 'mai mare'
        {
            //se insereaza la sfarsitul listei
            rez = InserareListaInainte(a->v+cod, ae);
            return rez;
        }
    }
    else    //lista este vida
    {
        //se insereaza un singur element
        rez = InserareListaVida(a->v+cod, ae);
        return rez;
    }
}

/* functia cauta un element in tabela si intoarce 'True'
    sau 'False' in functie de rezultatul cautarii */
char* ExistaTabelaHash(TH *h, void *ae, TFCmp fcompara, int nr_bucketuri)
{
    TLDI el;
    int cod = h->fh(ae, nr_bucketuri); //se calculeaza codul pentru element

    el = h->v[cod];
    if(el != NULL)  //lista nu e vida
    {
        //se parcurge lista corespunzatoare hashului calculat
        for(el = h->v[cod]; el->urm != h->v[cod]; el = el->urm)
        {
            if(fcompara(ae, el->info) == 1)
                return "True";  //a fost gasit elementul
        }
        //verificarea ultimului element
        if(fcompara(ae, el->info) == 1)
            return "True";
        else
            return "False"; /* a fost parcursa toata lista si
                            nu a fost gasit elementul */
    }
    else    //lista este vida
        return "False"; 
}

//stergerea unui element din tabela hash
int StergTH(TH *h, void *ae, TFCmp fcompara, TF f_elib, int nr_bucketuri)
{
    TLDI el, aux;
    //se calculeaza codul hash pentru acel element
    int cod = h->fh(ae, nr_bucketuri);

    //parcurg lista
    for(el = h->v[cod]; (el != NULL) && (el->urm != h->v[cod]); el = el->urm)
    {
        if(fcompara(ae, el->info) == 1) //am gasit elementul
        {
            aux = el;   //pointer catre element
            if(el == h->v[cod]) //se elimina primul element
            {
                //se refac legaturile
                h->v[cod] = el->urm;
                aux->pre->urm = aux->urm;
                aux->urm->pre = aux->pre;
            }
            else    //se elimina un element diferit de primul
            {
                //se refac legaturile
                aux->pre->urm = aux->urm;
                aux->urm->pre = aux->pre;
            }

            f_elib(el->info); //eliberarea memoriei pentru informatie
            free(el);   //eliberarea celulei
            return 1;
        }
    }
    //testul pentru ultimul element
    if( (el != NULL) && (fcompara(ae, el->info) == 1) ) 
    {
        aux = el;   //pointer catre element
        if(el == h->v[cod]) //mai este doar un element in lista
        {
            h->v[cod] = NULL;
        }
        else    //se elimina un element de pe ultima pozitie in lista
            {
                //se refac legaturile
                aux->pre->urm = aux->urm;
                aux->urm->pre = aux->pre;
            }

            f_elib(el->info); //eliberarea memoriei pentru informatie
            free(el);   //eliberarea celulei
            return 1;
    }

    //daca nu se gaseste element
    return 0;
}

//cautarea in tabela hash si intoarcerea elementului, daca este gasit
void *GasesteTabelaHash(TH *a, void *ae, TFCmp fcompara, int nr_bucketuri)
{
    int cod = a->fh(ae, nr_bucketuri);
    //se calculeaza codul hash pentru acel element
    TLDI el = a->v[cod];

    //se parcurge lista si se cauta elementul dat
    if(el != NULL)  //daca lista nu este vida
    {
        for(el = a->v[cod]; el->urm != a->v[cod]; el = el->urm) 
        {
            if (fcompara(el->info, ae) == 1)    //daca este gasit
                return (void *)el->info;
        }
        //ultimul element din lista
        if (fcompara(el->info, ae) == 1)
            return (void *)el->info;
    }

    //daca nu a fost gasit elementul cautat
    return NULL;
}
