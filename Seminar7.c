
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
};
typedef struct Nod Nod;

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* cap) {
	if (cap) {
		while (cap) {
			afisareMasina(cap->info);
			cap = cap->next;
		}
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if ((*lista)) {
		Nod* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = temp;
	}
	else
	{
		(*lista) = temp;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(ht.dim * sizeof(Nod*));
	for (int i = 0; i < ht.dim; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 3) % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int hashCode = calculeazaHash(masina.id, hash.dim);
	if (!hash.vector[hashCode]) {
		// nu avem coliziune
		adaugaMasinaInLista(&hash.vector[hashCode], masina);
	}
	else {
		// avem coliziune
		adaugaMasinaInLista(&hash.vector[hashCode], masina);
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	if (f) {
		while (!feof(f)) {
			inserareMasinaInTabela(ht, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Clusterul %d:\n", i + 1);
		afisareListaMasini(ht.vector[i]);
		printf("\n------------------------\n");
	}
}

//void dezalocareTabelaDeMasini(HashTable *ht) {
//	//sunt dezalocate toate masinile din tabela de dispersie
//}

//float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
//	//calculeaza pretul mediu al masinilor din fiecare cluster.
//	//trebuie sa returnam un vector cu valorile medii per cluster.
//	//lungimea vectorului este data de numarul de clustere care contin masini
//	return NULL;
//}

Masina getMasinaDupaId(HashTable ht, int id) {
	Masina m;
	int hashCode = calculeazaHash(id, ht.dim);
	Nod* cautare = ht.vector[hashCode];
	while (cautare) {
		if (cautare->info.id == id) {
			m = cautare->info;
			m.numeSofer = (char*)malloc(sizeof(char) * (strlen(cautare->info.numeSofer) + 1));
			strcpy(m.numeSofer, cautare->info.numeSofer);
			m.model = (char*)malloc(sizeof(char) * (strlen(cautare->info.model) + 1));
			strcpy(m.model, cautare->info.model);
			return m;
		}
		cautare = cautare->next;
	}
	return m;
}

int main() {
	HashTable hashTable = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(hashTable);
	Masina m = getMasinaDupaId(hashTable, 8);
	afisareMasina(m);
	return 0;
}