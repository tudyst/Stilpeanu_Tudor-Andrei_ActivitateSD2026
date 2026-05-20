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
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id)
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		else if ((*radacina)->info.id < masinaNoua.id)
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
	}
	else {
		Nod* temp = (Nod*)malloc(sizeof(Nod));
		temp->info = masinaNoua;
		temp->stanga = NULL;
		temp->dreapta = NULL;
		(*radacina) = temp;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&arbore, citireMasinaDinFisier(f));
	}
	fclose(f);
	return arbore;
}

// 3 moduri de parcurgere - preordine, postordine, inordine
// preordine: radacina - stanga - dreapta
// postordine: stanga - dreapta - radacina
// inordine: stanga - radacina - dreapta

void afisareMasiniDinArborePreordine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePreordine(radacina->stanga);
		afisareMasiniDinArborePreordine(radacina->dreapta);
	}
}

void afisareMasiniDinArborePostordine(Nod* radacina) {
	if (radacina) {
		afisareMasiniDinArborePostordine(radacina->stanga);
		afisareMasiniDinArborePostordine(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

// parcurgere in ordine crescatoare, descrescator (d-r-s)
void afisareMasiniDinArboreInordine(Nod* radacina) {
	if (radacina) {
		afisareMasiniDinArboreInordine(radacina->stanga);
		afisareMasina(radacina->info);
		afisareMasiniDinArboreInordine(radacina->dreapta);
	}
}

// folosim postordine la dezalocare ca sa stergem radacina ultima
void dezalocareArboreDeMasini(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);
		if ((*radacina)->info.model) {
			free((*radacina)->info.model);
		}
		if ((*radacina)->info.numeSofer) {
			free((*radacina)->info.numeSofer);
		}
		free(*radacina);
		*radacina = NULL;
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			Masina m = radacina->info;
			m.model = (char*)malloc(strlen(radacina->info.model) + 1);
			strcpy(m.model, radacina->info.model);
			m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
			strcpy(m.numeSofer, radacina->info.numeSofer);
			return m;
		}
		if (radacina->info.id < id) {
			return getMasinaByID(radacina->dreapta, id);
		}
		if (radacina->info.id > id) {
			return getMasinaByID(radacina->stanga, id);
		}
		else {
			Masina m;
			m.id = -1;
			return m;
		}
	}
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		int stanga = determinaNumarNoduri(radacina->stanga);
		int dreapta = determinaNumarNoduri(radacina->dreapta);
		return stanga + dreapta + 1;
	}
	return 0;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);
		return max(inaltimeStanga, inaltimeDreapta) + 1;
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		int pretTotalStanga = calculeazaPretTotal(radacina->stanga);
		int pretTotalDreapta = calculeazaPretTotal(radacina->dreapta);
		return pretTotalStanga + pretTotalDreapta + radacina->info.pret;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		float suma = calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0)
			return suma + radacina->info.pret;
		else {
			return suma;
		}
	}
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareMasiniDinArboreInordine(radacina);
	afisareMasina(getMasinaByID(radacina, 5));

	printf("Numar total noduri in arbore: %d\n", determinaNumarNoduri(radacina));

	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));

	printf("Pret total masini: %2.2f\n", calculeazaPretTotal(radacina));

	printf("Pretul masinilor soferului %s: %2.2f\n", "Ionescu", calculeazaPretulMasinilorUnuiSofer(radacina, "Ionescu"));

	dezalocareArboreDeMasini(&radacina);
	afisareMasiniDinArboreInordine(radacina);

	return 0;
}