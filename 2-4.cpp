#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int M = 1000; // max. zustatek limit
const int N = 20; // pocet uctu k vygenerovani
const int P = 10; // max. procento uroku limit
const int Y = 12; // pocet let k simulaci

class Ucet{
	public:
		Ucet(int cislo);
		int Cislo();
		void Vlozit();
		void Vybrat();
		int AktualniStav();
		void PripsatUrok();
	private:
		int cislo;
		int procento;
		int zustatek; // aktualni zustatek na uctu

};

Ucet::Ucet(int cislo) // konstruktor uctu, parametr je cislo, ktere se priradi uctu
{
	this->cislo = cislo; // priradi se cislo z parametru
	this->procento = rand() % (P - 1) + 1; // nastavi se nahodne procento od 1 do P-1
	this->zustatek = rand() % (M - 1) + 1; // nastavi se nahodne pocatecni vklad od 1 do M-1

    cout << "G" << this->cislo << "[" << this->zustatek << "][" <<  this->procento << "]" << endl; // vypise se info o vygenerovani
}

void Ucet::Vlozit() // VLozi nahodne vygenerovanou castku na uctu
{
	int zustatekana_castka = (rand() % (M-1)) + 1; // vygeneruje se nahodne castka k vlozeni od 1 do M-1
	cout << "V" << this->cislo << "[" << this->zustatek << "][" << zustatekana_castka << "]" << endl; // vypise se info o uctu pred vlozenim castky

	this->zustatek += zustatekana_castka; // na ucet se vlozi castka
}

void Ucet::Vybrat() // Vybere z uctu nahodnout castku
{
	int odebirana_castka  = (rand() % (M-1)) + 1; // nahodne se vygeneruje castka k odberu od 1 do M-1
	cout << "X" << this->cislo << "[" << this->zustatek << "][" << odebirana_castka << "]" << endl; // vypise se info o uctu pred odebranim
	this->zustatek -= odebirana_castka; // odebere se castka z uctu
}

int Ucet::AktualniStav() // Vrati aktualni stav uctu
{
	return this->zustatek;
}

void Ucet::PripsatUrok()
{
	cout << "U" << this->cislo << "[" << this->zustatek << "]"; // vypise info o starem zustatku

	this->zustatek += (this->procento * zustatek) / 100; // zvetsi zustatek o dane procento uroku

	cout << "[" << this->zustatek << "]" << endl; // vypise info o novem zustatku
}


class Seznam{
    public:
        Seznam(); 
        ~Seznam();
        void Vlozit(Ucet *ucet);

        void IteratorReset();  // nastavi iterator na 1. prvek v seznamu
        void IteratorDalsi(); // iterator presune na dalsi prvek v seznamu
        bool IteratorJeKonec(); // vrati true, jestlize je iterator na konci seznamu
        Ucet *VratUcet();
    private:
        struct SeznamItem{ // struktru pro polozku seznamu
            Ucet *ucet; // ukazatel na ucet
            SeznamItem *dalsi; // ukazatel na dalsi polozku seznamu
        };
        SeznamItem *hlava; // ukazatel na hlavu seznamu (=zacatek seznamu)
        SeznamItem *iterator; // iterat - coz je ukazatel na polozku (muze byt libovolna) v seznamu
};

Seznam::~Seznam()
{
    for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // projdi cely seznam
    {
        delete iterator->ucet; // a dealokuje vsechny Ucety v seznamu
    }

    while (hlava != NULL) // dokud na zacatku seznamu je nejaka polozka
    {
        iterator = hlava; // nastav iterator na tuto polozku
        hlava = hlava->dalsi; // zacatek seznamu nastav na dalsi polozku (nebo v pripade ze uz zadna neni na NULL)
        delete iterator; // smaz tuhle polozku
    }

}

void Seznam::IteratorReset() // nastavi ukazatel iterator na 1. polozku v seznamu
{
    iterator = hlava; // na 1. polozku v seznamu ukazuje vzdy "hlava", takze se iterator nastavuje na hlavu
}

void Seznam::IteratorDalsi() // nastavi ukazatel na dalsi polozku v seznamu (pokud jsem byl na 1. polozce, prejde se na 2... pokud jsem byl na 2., tak na treti, atd....)
{
    iterator = iterator->dalsi;
}

bool Seznam::IteratorJeKonec() //zjisti, zda iterator uz je za koncem seznamu
{
    if (iterator == NULL) // pokud je iterator NULL, znamena to, ze uz jsem prosel cely seznam a nyni jsem za nim
        return true; // pokud jsem tedy prosel cely seznam, vrati se TRUE
    else
        return false; // jinak se vrati FALSE a to znamena, ze iterator jeste ukazuje na nejaky prvek v seznamu
}

Ucet* Seznam::VratUcet() // vrati ucet, na ktery ukazuje zrovna iterator
{
    return iterator->ucet; // vrati se ucet z polozky seznamu, na kterou ukazuje iterator
}

Seznam::Seznam()  // konstruktor Seznamu
{
    hlava = NULL; // seznam je ze zacatku prazdny, tak se jeho hlava nastavi na NULL
}

void Seznam::Vlozit(Ucet *ucet) // vlozi ucet do seznamu
{
    SeznamItem *item = new SeznamItem; // vytvori novou polozku seznamu

    item->dalsi = this->hlava; /* ukazatel dasli nastavi teto polozce na hlavu seznamu
         (to znamena, ze pokud v seznamu neco je, tak ukazatel dalsi bude ukazovat na 1. polozku v seznamu) 
            pokud vsak v seznamu nic neni, bude ukazatel dalsi ukazovat na NULL
         */
    item->ucet = ucet; // do nove vytvorene polozky seznamu se priradi ucet z parametru funkce
    this->hlava = item; // hlava seznamu se nastavi na nove vlozenou polozku (polozka tedy byla vlozena na zacatek seznamu)
}

int main()
{
	srand (time(NULL)); // inicializace generatoru nahodnych cisel
    Seznam seznam; // vytvoreni seznamu

    for (int i = 1; i <= N; ++i) // cyklus jde od 1 do N - probiha teda N-krát (vlozi se behem nej tedy do seznamu N ucetu)
    {
        Ucet *ucet = new Ucet(i); //vytvorí se novy ucet (priradi se mu cislo i - pro prvni ucet 1, pro druhý 2, ...)
        seznam.Vlozit(ucet); // ucet se vlozi do seznamu
    }

    for (int i = 1; i <= Y*12; ++i) // for cyklus jde Y*12, kde je Y je pocet roku a 12 pocet mesicu v roce - simulace jde tedy pro Y roku po mesicich
    {
    	for (seznam.IteratorReset(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi()) // projdi celý seznam
        {
            Ucet *ucet = seznam.VratUcet(); // do promenne ucet uloz ucet, na ktery v teto iteraci cyklu ukazuje iterator
            int operace = rand() % 3; // do promenne operace prirad nahodne cislo od 0 do 2

            if (operace == 0)  // pokud byla nahodne vygenerovana 0
                ucet->Vlozit();
            else if (operace == 1) // pokud byla nahodne vygenerova 1
                ucet->Vybrat();

            if ( i%12 == 0) // pokud je krok delitelny 12ti, pripise (v pripade zaporneho stavu odepise) se k (od) uctu urok
            	ucet->PripsatUrok(); 
        }

    }
    
	return 0;
}