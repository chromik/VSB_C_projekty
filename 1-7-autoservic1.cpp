#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N=5; // pocet dnu, ve ktere jsou provadeny opravy
const int Z=7; // maximalni pocet zakazniku na den

typedef struct
{
	int identifikace; // identifikace zakaznik
	int den_opravy;	// den, ve ktery chce zakaznik opravit automobil
} Zakaznik;

typedef struct
{
	int aktualni_velikost; // aktualni velikost fronta - kolik je v ni zakazniku
	int celkova_velikost;	// maximalni velikost fronty - kolik zakazniku v ni muze byt
	int zacatek;	// na kterem indexu ve fronte je prvni prvek
	Zakaznik **zakaznik;	// pole ukazatelu na zakazniky
} Fronta;

// vygeneruje strukturu zakaznika s danou identifikaci a nahodnym dnem opravy
Zakaznik * generovaniZakaznika(int identifikace)
{
	Zakaznik *zakaznik=new Zakaznik;
	zakaznik->identifikace=identifikace;
	zakaznik->den_opravy=rand()%N;
	return zakaznik;
}

// inicializuje pocatecni promenne fronty a alokuje pamet pro pole
void inicializaceFronty(Fronta *fronta)
{
	fronta->zakaznik=new Zakaznik*[Z];
	fronta->aktualni_velikost=0;
	fronta->celkova_velikost=Z;
	fronta->zacatek=0;
}

// uvolni dynamicky alokovane pole fronty
void dealokaceFronty(Fronta *fronta)
{
	delete [] fronta->zakaznik;
}

// uvolni dynamicky alokovane pole fronty 
void dealokaceFront(Fronta *fronta,int const pocet)
{
	for (int i=0;i<pocet;++i)
		dealokaceFronty(&fronta[i]);
}

// inicialuzuje pocatecni promenne u vsech front v poli front
void inicializaceFront(Fronta *fronta,int const pocet)
{
	for (int i=0;i<pocet;++i)
		inicializaceFronty(&fronta[i]);
}

// zjisti, zda je fronta plna -> pokud ano - vrati TRUE X jinak FALSE
bool jePlna(Fronta *fronta)
{
	if (fronta->aktualni_velikost == fronta->celkova_velikost)
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
bool frontyPlne(Fronta *fronta,int const pocet)
{
	int plnych=0;
	for (int i=0;i<pocet;++i)
	{
		if (jePlna(&fronta[i]))
			++plnych;
	}
	if (plnych==pocet) 
		return true;
	else
		return false;
}

// prida zakaznika do fronty
void pridat(Fronta *fronta,Zakaznik *zakaznik)
{
	fronta->zakaznik[(fronta->zacatek + fronta->aktualni_velikost++) % fronta->celkova_velikost ]=zakaznik;
}

// odebere zakaznika z fronty
Zakaznik * odebrat(Fronta *fronta)
{
	fronta->zacatek = fronta->zacatek % fronta->celkova_velikost;
	--fronta->aktualni_velikost;
	return fronta->zakaznik[fronta->zacatek++];
}

int main(void)
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	
	Fronta fronta[N];	//pole front (N front pro N dní)
	inicializaceFront(fronta,N); //pocatecni inicializace techto front


	Zakaznik *zakaznik; //ukazatel na strukturu typu Zakaznik
	int identifikace=1;	// pocatecni nastaveno id -> id prvniho bude 1
	while (!frontyPlne(fronta,N))	//delej, dokud vsechny fronty nejsou plne
	{
		zakaznik=generovaniZakaznika(identifikace); //vygeneruje zakaznika a nastavi na nej ukazatel
		cout << "G" << zakaznik->identifikace << "[" << zakaznik->den_opravy << "]" << endl; // vypise ID a DEN OPRAVY vygenerovaneho zakaznika

		if (!jePlna(&fronta[zakaznik->den_opravy])) // pokud fronta pro daný den není plná
		{
			pridat(&fronta[zakaznik->den_opravy],zakaznik);	//vlož zákazníka do fronty
			cout << "V" << zakaznik->identifikace << "[F" << zakaznik->den_opravy << "]" << endl; // vypis, ze byl zakaznik pridan do fronty
		}
		else	// JINAK (fronta pro daný den je plna)
		{
			cout << "X" << zakaznik->identifikace << "[F" << zakaznik->den_opravy << "]" << endl; //vypis, ze zakaznik byl odmitnut
			delete zakaznik; // dealokuj zakaznika
		}
		++identifikace; //inkrementuje id zakaznika == nastavi id pro dalsiho zakaznika
	}

	for (int i=0;i<N;++i) //pro vsechny dny
	{
		while (!jePrazdna(&fronta[i])) // kontroluj, zda je fronta daneho dne neprazdna (neco obsahuje)
		{
			zakaznik=odebrat(&fronta[i]);	// odeber prvek z fronty a nastav na ukazatel
			cout << "O" << zakaznik->identifikace << "[" << zakaznik->den_opravy << "]" << endl; // vypis, ze oprava byla realizovana
			delete zakaznik;	// dealokuj zakaznika
		}
	}

	dealokaceFront(fronta,N);
	return 0;
}