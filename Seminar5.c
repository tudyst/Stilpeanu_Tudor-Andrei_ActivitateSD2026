#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* aux = lista.first;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* aux = lista.last;
	while (aux) {
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = listaDubla->last;
	nou->next = NULL;
	if (listaDubla->last) {
		listaDubla->last->next = nou;
	}
	else {
		listaDubla->first = nou;
	}
	listaDubla->last = nou;
}

void adaugaLaInceputInLista(ListaDubla* listaDubla, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = listaDubla->first;
	if (listaDubla->first) {
		listaDubla->first->prev = nou;
	}
	else {
		listaDubla->last = nou;
	}
	listaDubla->first = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla listaMasini;
	Masina masina;
	listaMasini.first = NULL;
	listaMasini.last = NULL;
	while (!feof(f)) {
		masina = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&listaMasini, masina);
	}
	fclose(f);
	return listaMasini;
}

void dezalocareLDMasini(ListaDubla* listaMasini) {
	Nod* aux = listaMasini->first;
	if (listaMasini->first) {
		if (listaMasini->first->next) {
			while (aux) {
				free((aux->prev->info).model);
				free((aux->prev->info).numeSofer);
				free(aux->prev);
				aux = aux->next;
			}
		}
		free(listaMasini->last->info.model);
		free(listaMasini->last->info.numeSofer);
		free(listaMasini->last);
		listaMasini->first = NULL;
		listaMasini->last = NULL;
	}
}

float calculeazaPretMediu(ListaDubla listaMasini) {
	int nr = 0;
	float suma = 0;
	Nod* aux = listaMasini.first;
	while (aux) {
		nr++;
		suma += aux->info.pret;
		aux = aux->next;
	}
	if (nr != 0)
		return suma / nr;
	return 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	afisareListaMasiniDeLaSfarsit(lista);
	printf("Medie: %2f", calculeazaPretMediu(lista));
	dezalocareLDMasini(&lista);
	afisareListaMasiniDeLaInceput(lista);
	return 0;
}