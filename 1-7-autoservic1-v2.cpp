#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 5; // pocet dnu, ve ktere jsou provadeny opravy
const int Z = 7; // maximalni pocet zakazniku na den

typedef struct
{
	int identifikace; // identifikace zakaznik
	int den_opravy;	// den, ve ktery chce zakaznik opravit automobil
} Zakaznik;

typedef struct
{
	int aktualni_velikost; // aktualni velikost fronta - kolik je v ni zakazniku
	int zacatek;	// na kterem indexu ve fronte je prvni prvek
	Zakaznik zakaznik[Z];	// pole ukazatelu na zakazniky
} Fronta;

// inicializuje pocatecni promenne fronty a alokuje pamet pro pole
void inicializaceFronty(Fronta *fronta)
{
	fronta->aktualni_velikost = 0;
	fronta->zacatek = 0;
}

// zjisti, zda je fronta plna -> pokud ano - vrati TRUE X jinak FALSE
bool jePlna(Fronta *fronta)
{
	if (fronta->aktualni_velikost == Z)
		return true;
	else
		return false;
}

// zjisti, zda je fronta prazna -> pokud ano - vrati TRUE X jinak FALSE
bool jePrazdna(Fronta *fronta)
{
	return !fronta->aktualni_velikost;
}

// nacte pole front a pokud jsou vsechny plne, tak vrati TRUE X jina FALSE
bool frontyPlne(Fronta *fronta)
{
	for (int i = 0; i < N; ++i)
		if (!jePlna(&fronta[i]))
			return false;
	return true;
}

// prida zakaznika do fronty
void pridat(Fronta *fronta,Zakaznik zakaznik)
{
	fronta->zakaznik[(fronta->zacatek + fronta->aktualni_velikost++) % Z]=zakaznik;
}

// odebere zakaznika z fronty
Zakaznik odebrat(Fronta *fronta)
{
	fronta->zacatek = fronta->zacatek % Z;
	--fronta->aktualni_velikost;
	return fronta->zakaznik[fronta->zacatek++];
}

int main(void)
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	
	Fronta fronta[N];	//pole front (N front pro N dní)
	for (int i = 0; i < N; ++i) {
		inicializaceFronty(&fronta[i]); //pocatecni inicializace techto front
	}


	Zakaznik zakaznik; //ukazatel na strukturu typu Zakaznik
	int identifikace = 1;	// pocatecni nastaveno id -> id prvniho bude 1
	while (!frontyPlne(fronta))	{//delej, dokud vsechny fronty nejsou plne
		zakaznik.identifikace = identifikace;
		zakaznik.den_opravy = rand() % N;
		cout << "G" << zakaznik.identifikace << "[" << zakaznik.den_opravy << "]" << endl; // vypise ID a DEN OPRAVY vygenerovaneho zakaznika

		if (!jePlna(&fronta[zakaznik.den_opravy])) {// pokud fronta pro daný den není plná
			pridat(&fronta[zakaznik.den_opravy],zakaznik);	//vlož zákazníka do fronty
			cout << "V" << zakaznik.identifikace << "[F" << zakaznik.den_opravy << "]" << endl; // vypis, ze byl zakaznik pridan do fronty
		}
		else {	// JINAK (fronta pro daný den je plna)
			cout << "X" << zakaznik.identifikace << "[F" << zakaznik.den_opravy << "]" << endl; //vypis, ze zakaznik byl odmitnut
		}
		++identifikace; //inkrementuje id zakaznika == nastavi id pro dalsiho zakaznika
	}

	for (int i = 0; i < N; ++i) { //pro vsechny dny
		while (!jePrazdna(&fronta[i])) { // kontroluj, zda je fronta daneho dne neprazdna (neco obsahuje)
			zakaznik = odebrat(&fronta[i]);	// odeber prvek z fronty a nastav na ukazatel
			cout << "O" << zakaznik.identifikace << "[" << zakaznik.den_opravy << "]" << endl; // vypis, ze oprava byla realizovana
		}
	}
	return 0;
}