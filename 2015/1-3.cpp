#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int M = 50; //pocet vagonu
const int N = 4; //kapacita jednoho kolejiste



/* ZASOBNIK */
struct Zasobnik
{
	int poradi; // poradi zasobniku (od vytvoreni)
	int *vagon_id; // pole s cisly vagonu
	unsigned int vrchol; // ukazatel na vrchol zasobniku
	unsigned int max_velikost; // velikost zasobniku
};

void zasobnik_inicializace(struct Zasobnik **zasobnik, const int velikost)
{

	*zasobnik = new struct Zasobnik;
	static int poradi = 0; // staticka promenna nastavena na 0

	(*zasobnik)->poradi = poradi++; // zapise poradi zasobniku a zvysi statickou o jedna
	(*zasobnik)->max_velikost = velikost; // nastavi max. velikost zasobniku z parametru
	(*zasobnik)->vagon_id = new int[velikost]; // alokuje pole na tuto velikost
	(*zasobnik)->vrchol = 0; // nastavi vrchol zasobniku na 0 (= prazdny zasobnik)
}

void zasobnik_vlozit(struct Zasobnik *zasobnik, int prvek)
{
	zasobnik->vagon_id[zasobnik->vrchol++] = prvek;
}

int zasobnik_odebrat(struct Zasobnik *zasobnik)
{
	return zasobnik->vagon_id[--zasobnik->vrchol];
}

bool zasobnik_jePlny(struct Zasobnik *zasobnik)
{
	return zasobnik->vrchol == zasobnik->max_velikost;
}

bool zasobnik_jePrazdny(struct Zasobnik *zasobnik)
{
	return !zasobnik->vrchol;
}


/* SEZNAM */
struct Seznam
{
	struct pSeznam *hlava; // ukazuje na prvni polozku v seznamu
};

struct pSeznam // polozka v seznamu
{
	struct Zasobnik *zasobnik; // ukazatel na zasobnik
	struct pSeznam *dalsi; // ukazatel na dalsi polozku v seznamu
};

/* Vlozi zasobnik na zacatek seznamu (bude na nej ukazovat hlava) */
void seznam_vlozit(struct Seznam *seznam, struct Zasobnik *zasobnik)
{
	struct pSeznam *polozka = new struct pSeznam; // vytvorim novou polozku seznamu
	polozka->zasobnik = zasobnik; // do teto polozky vlozim zasobnik z parametru funkce
	polozka->dalsi = seznam->hlava; // u teto polozky nastavim nasledovníkem hlavu seznamu
	seznam->hlava = polozka; // a hlavu seznamu nastavim na tuto polozku 

}

Zasobnik * seznam_getZasobnik(struct Seznam *seznam)
{
	if (seznam->hlava == NULL) // kdyz je hlava NULL
		return NULL; // je seznam prazdny a vratim NULL
	else // jinak 
		return seznam->hlava->zasobnik; // vratim zasobnik, na ktery ukazuje hlava
}


void seznam_remove(struct Seznam *seznam)
{
	struct pSeznam *tmp = seznam->hlava->dalsi; // ulozim si do tmp dalsi polozku
	delete seznam->hlava; // aktualni polozku smazu
	seznam->hlava = tmp; // nastavim hlavu na polozku, kterou jsem si ulozil
}

bool seznam_isEmpty(struct Seznam *seznam)
{
	return seznam->hlava == NULL; // kdyz je ukazuje hlava na NULL, je seznam prazdny a vrati se TRUE
}


int main(void)
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel

	Zasobnik *vlak;	//zasobnik reprezentujici vlak
	zasobnik_inicializace(&vlak, M); //inicializace tohoto zasobniku na velikost M

	Seznam seznam; // vytvoreni seznamu
	seznam.hlava = NULL; // nastaveni hlavy na NULL (=seznam je prazdny)

	for (int i = 1; i <= M; ++i) {
		zasobnik_vlozit(vlak, i); // naplni vlak vagony od 1 do M
		cout << "G" << i << endl;
	}


	while(!zasobnik_jePrazdny(vlak)) { // dokud se vlak nevyprazdni
		
		int vagon = zasobnik_odebrat(vlak); // odeber jeden vagon

		struct Zasobnik *zasobnik = seznam_getZasobnik(&seznam); // vem zasobnik ze seznamu

		if (zasobnik == NULL || zasobnik_jePlny(zasobnik)) { // pokud v seznamu zadny zasobnik neni nebo je zasobnik plny
			zasobnik_inicializace(&zasobnik, N); // inicializuj novy zasobnik o kapacite N
			seznam_vlozit(&seznam, zasobnik); // a vloz jej do seznamu
		}

		zasobnik_vlozit(zasobnik, vagon); // vlozi vagon do zasobniku
		cout << "O" << vagon << "[K" << zasobnik->poradi << "]" << endl; // vypise info

	}

	while(!seznam_isEmpty(&seznam)) { // dokud jsou na kolejistich odlozene vagony (= dokud neni seznam prazdny)
		struct Zasobnik *kolejiste = seznam_getZasobnik(&seznam); // vemu kolejiste (zasobnik) ze seznamu
		int poradi = kolejiste->poradi; // ulozim si cislo toho kolejiste (zasobniku)
		while(!zasobnik_jePrazdny(kolejiste)) { // dokud na kolejisti neco je (dokud Zasobnik neni prazdny)
			int vagon = zasobnik_odebrat(kolejiste); // odeber jeden vagon

			zasobnik_vlozit(vlak, vagon); // vloz jej zpatky na vlak

			cout << "P" << vagon << "[K" << poradi << "]" << endl; // vypis info o pripojeni
		}
		delete [] kolejiste->vagon_id; // dealokace pole uvnitr zasobniku Kolejiste
		delete kolejiste; // dealokace zasobniku Kolejiste
		seznam_remove(&seznam); // odstranim toho kolejiste ze seznamu

	}

	delete [] vlak->vagon_id; // dealokace pole uvnitr zasobniku Vlaku
	delete vlak; // dealokace zasobniku Vlak
	return 0;
}