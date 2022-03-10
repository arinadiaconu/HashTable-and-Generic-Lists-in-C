/* DIACONU Arina-Ioana */

/*-- main.c  */
#include <stdio.h>
#include <stdlib.h>
#include "Liste2Generice.h"
#include "TabelaHash.h"
#include <string.h>

#define N 100

//structura ce retine elemente de tip {key, ip}
typedef struct pereche
{
	char *nume, *ip;
} TDNS;

int codHash(void * element, int nr_bucket)
{
    TDNS *pereche_dns = (TDNS *) element;
    char *nume = pereche_dns->nume;	//se extrage numele din structura
    int i = 0, s = 0;

	//se parcurge numele litera cu litera
    while(nume[i] != '\0')
    {
        s = s + (int)nume[i];	//se insumeaza codul ascii al literelor
		i++;
    }

	/* se intoarce restul impartirii sumei 
	la numarul de liste din tabela hash */
    return (s % nr_bucket);
}

//afisarea unui element, de fapt afisarea adresei ip
void afisareIP(void * element, FILE *f)
{
	TDNS *pereche_dns = (TDNS *) element;
	fprintf(f, "%s", pereche_dns->ip);
}

//eliberarea memoriei pentru o strucutura de tip {key, ip}
void eliberare_pereche(void *a)
{
	TDNS *pereche = (TDNS *) a;

	free(pereche->nume);	//eliberarea campului nume
	if(pereche->ip != NULL)
		free(pereche->ip);	//eliberarea campului ip
	free(pereche);	//eliberarea structurii
}

//daca doua perechi au acelasi nume de site, returneaza 1
//daca una din componenete este diferita, returneaza 0
int cmpDNS(void * e1, void * e2)
{
	TDNS *pereche_dns1 = (TDNS *) e1;
	TDNS *pereche_dns2 = (TDNS *) e2;

	if (strcmp(pereche_dns1->nume, pereche_dns2->nume) != 0)
		return 0;
	return 1;
}

/* functia utilizata la inserarea ordonata, care intoarce rezultatul
functiei strcmp pentru 2 siruri de caractere */
int ordoneaza(void *e1, void *e2)
{
	TDNS *pereche_dns1 = (TDNS *) e1;
	TDNS *pereche_dns2 = (TDNS *) e2;

	return (strcmp(pereche_dns1->nume, pereche_dns2->nume));
}

TH * GenerareHash(size_t M)
{
    TH *h = NULL;

	//initializare tabela hash
	h = (TH *) InitializareTabelaHash(M, codHash);
	if(h == NULL)	//testarea initializarii
		return NULL;

	return h;
}

int main(int argc, char ** argv)
{    
    int nr_bucket = (atoi)(argv[1]);	//numarul de liste din tabela

    TH *tabelah = NULL;
	tabelah = GenerareHash(nr_bucket);	//se genereaza tabela
	if (tabelah == NULL) 	//testul generarii tabelei hash
        return 1;

	FILE *file_in, *file_out;
	char *line = NULL, *numeFisier_in = argv[2], *numeFisier_out = argv[3];
	size_t len = 0;

	//se deschide fisierul ce contine datele de intrare
	file_in = fopen(numeFisier_in, "rt");	
	if (file_in == NULL)	//testul deschiderii
		return 1;

	//se deschide fisierul ce contine datele de iesire
	file_out = fopen(numeFisier_out, "w");
	if (file_out == NULL)	//testul deschiderii
	{
		fclose(file_in);
		return 1;
	}

	//se citeste linie cu linie din fisierul cu datele de intrare
	while ( getline(&line, &len, file_in) != -1)
	{
		/* se retine primul cuvant, ce da si comanda pentru prelucrarea
		tabelei hash, iar apoi se testeaza pentru fiecare functie */
		char *comanda = strtok(line, " ");
		if (comanda[strlen(comanda) - 1] == '\n')
				comanda[strlen(comanda) - 1] = '\0';
		
		if(strcmp(comanda, "put") == 0)	//se insereaza un element
		{
			//se retin celelalte 2 cuvinte din linie
			char *site = strtok(NULL, " ");
			char *ip = strtok(NULL, " ");
			if (ip[strlen(ip) - 1] == '\n')
				ip[strlen(ip) - 1] = '\0';
			
			//se aloca si se construieste structura cu sirurile citite
			TDNS *el = (TDNS *)malloc(sizeof(TDNS));
			if(el == NULL)	return 1;	//testul alocarii

			el->nume = (char *)malloc(N);
			if(!el->nume)	//testul alocarii
			{
				free(el);
				return 1;
			}
			el->ip = (char *)malloc(N);
			if(!el->ip)	//testul alocarii
			{
				eliberare_pereche(el);
				return 1;
			}
			
			strcpy(el->nume, site);
			strcpy(el->ip, ip);

			//se realizeaza inserarea elementului construit
			int rez;
			rez = InserareTabelaHash(tabelah,(void*)el,ordoneaza,nr_bucket);
			
			if(!rez)	//daca inserarea a esuat, se elibereaza memoria
				eliberare_pereche((void *)el);
		}
		else if(strcmp(comanda, "get") == 0)	//se cauta un element
		{
			//se retine si celalalt sir de caractere din linie
			char *site = strtok(NULL, " ");
			if (site[strlen(site) - 1] == '\n')
				site[strlen(site) - 1] = '\0';

			//se aloca si se construieste structura cu sirul citit
			TDNS *e = (TDNS *)malloc(sizeof(TDNS));
			if(e == NULL)	return 1;

			e->nume = (char *)malloc(N);
			if(!e->nume)	//testul alocarii
			{
				free(e);
				return 1;
			}
			strcpy(e->nume, site);
			e->ip = NULL;

			TDNS *rez;
			rez =(TDNS*)GasesteTabelaHash(tabelah,(void *)e,cmpDNS,nr_bucket);

			if(rez)	//daca a fost gasit, se afiseaza elementul
				fprintf(file_out, "%s\n", rez->ip);
			else
				fprintf(file_out, "NULL\n");	

			eliberare_pereche((void *) e);	//eliberarea memoriei
				
		}
		else if(strcmp(comanda, "remove") == 0)	//sterge un element
		{
			//se retine si celalalt sir de caractere din linie
			char *site = strtok(NULL, " ");
			if (site[strlen(site) - 1] == '\n')
				site[strlen(site) - 1] = '\0';

			//se aloca si se construieste structura cu sirul citit
			TDNS *el = (TDNS *)malloc(sizeof(TDNS));
			if(el == NULL)	return 1;	//testul alocarii

			el->nume = (char *)malloc(N);
			if(!el->nume)	//testul alocarii
			{
				free(el);
				return 1;
			}
			strcpy(el->nume, site);
			el->ip = NULL;

			StergTH(tabelah, (void*)el, cmpDNS, eliberare_pereche, nr_bucket);

			eliberare_pereche((void *) el);	//eliberarea memoriei
			
		}
		else if(strcmp(comanda, "find") == 0)
		//se verifica existenta unui element	
		{
			//se retine si celalalt sir de caractere din linie
			char *site = strtok(NULL, " ");
			if (site[strlen(site) - 1] == '\n')
				site[strlen(site) - 1] = '\0';

			//se aloca si se construieste structura cu sirul citit
			TDNS *el = (TDNS *)malloc(sizeof(TDNS));
			if(el == NULL)	return 1;	//testul alocarii

			el->nume = (char *)malloc(N);
			if(!el->nume)	//testul alocarii
			{
				free(el);
				return 1;
			}
			strcpy(el->nume, site);
			el->ip = NULL;

			char *rez;
			rez = ExistaTabelaHash(tabelah, el, cmpDNS, nr_bucket);
			fprintf(file_out, "%s\n", rez);	//se afiseaza rezultatul cautarii

			eliberare_pereche((void *) el);	//eliberarea memoriei

		}
		else if(strcmp(comanda, "print") == 0)	//afisarea tabelei hash
		{
			AfisareTabelaHash(tabelah, afisareIP, file_out);
		}
		else if(strcmp(comanda, "print_bucket") == 0)
		//afisarea unei singure liste din tabela hash
		{
			//se retine si numarul listei sub forma de sir de caractere
			char *nr = strtok(NULL, " ");
			if (nr[strlen(nr) - 1] == '\n')
				nr[strlen(nr) - 1] = '\0';

			int nr_b = atoi(nr);	//convertirea la numar intreg

			//daca lista care trebuie afisata exista in tabela hash
			if(nr_b < nr_bucket)	
			{
				if(tabelah->v[nr_b] != NULL)	//daca lista nu este vida
					AfisareBucket(tabelah, nr_b, file_out, afisareIP);
				else	//daca lista este vida
					fprintf(file_out, "VIDA\n");
			}
		}

		if(!comanda)	//testul pentru eliberarea memoriei
		{
			free(comanda);
		}
	}

	free(line);	//eliberarea memoriei
	if(!numeFisier_in)
		free(numeFisier_in);
	if(!numeFisier_out)
		free(numeFisier_out);

	//eliberarea memoriei pentru tabela hash
	DistrugeTabelaHash(&tabelah, eliberare_pereche);	
	
	//inchiderea fisierelor
	fclose(file_in);
	fclose(file_out);
	
    return 0;
}
