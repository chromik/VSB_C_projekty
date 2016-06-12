#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const unsigned int C1 = 100; // minimalni cena jedne polozky
const unsigned int C2 = 1000; // maximalni cena jedne polozky
const unsigned int N = 3; // pocet nakladnich aut
const unsigned int P = 10; // maximalni pocet polozek (max. velikost nakladu)

const unsigned int MAX_CENA_NAKLADU = P * C2; // maximalni mozna cena nakladu jednoho nakladaku

class NakladniAuto{
	public:
		NakladniAuto();
		unsigned int Cislo();
		unsigned int PocetPolozek();
		unsigned int CenaNakladu();
		void VyskladnitPolozku();

		static unsigned int pocetNakladaku; // staticka promenna, ktera obsahuje pocet nakladaku (=pocet instanci tridy NakladniAuto)


	private:
		unsigned int cislo; // cislo nakladaku
		unsigned int pocetPolozek; // pocet polozek v nakladaku
		unsigned int cenaPolozky; // cena jedne polozky v nakladaku

};

unsigned int NakladniAuto::pocetNakladaku = 0; //inicializace staticke promenne na 0

NakladniAuto::NakladniAuto() // konstruktor
{
	++NakladniAuto::pocetNakladaku; /* zvetsi se staticka promenna, ktera obsahuje pocet
	 nakladaku (=pocet instanci tridy NakladniAuto) o jedničku */

	this->cislo = NakladniAuto::pocetNakladaku; // priradi nakladaku cislo z parametru funkce
	this->pocetPolozek = rand() % P + 1; // vygeneruje pocet polozek od 1 do P
	this->cenaPolozky = rand() % (C2 - C1 +1) + C1; // vygeneruje cenu polozky, tak aby platilo: C1 <= cenaPolozky <= C2

	cout << "G" << this->cislo << "[" << this->pocetPolozek << "][" << this->cenaPolozky << "]" << endl; // vypise info o vygenerovani
}



unsigned int NakladniAuto::Cislo() // funkce vrati cislo nakladaku
{
	return this->cislo;
}

unsigned int NakladniAuto::PocetPolozek() // cislo vrati kolik ma nakladak polozek (=kolik je v nem nalozeno nakladu)
{
	return this->pocetPolozek;
}

unsigned int NakladniAuto::CenaNakladu() // funkce vrati celkovou cenu nakladu
{
	return this->cenaPolozky * this->pocetPolozek; // cena nakladu je celkovy pocet polozek * cena jedne polozky
}

void NakladniAuto::VyskladnitPolozku() // funkce vyskladni jednu polozku z nakladaku
{
	--this->pocetPolozek; // pocet polozek se se o jednicku dekrementuje
}


class Fronta{
    public:
        Fronta();
        void Vlozit(NakladniAuto *nakladak);
        NakladniAuto *VratNakladniAuto();
        void OdeberNakladniAuto();
        bool JePrazdna();
    private:
        struct FrontaItem{ // struktru pro polozku fronty
            NakladniAuto *nakladak; // ukazatel na nakladak
            FrontaItem *dalsi; // ukazatel na dalsi polozku fronty
        };
        FrontaItem *zacatek;
};


bool Fronta::JePrazdna() // zjisti zda je fronta prazdna
{
	if (zacatek == NULL) // pokud zacatek je NULL
		return true; // fronta je prazdna -> vratim TRUE
	else // jinak
		return false; // fronta neni prazdna -> vratim FALSE
}

void Fronta::Vlozit(NakladniAuto *nakladak) // vlozi nakladak do fronty
{

	FrontaItem *item = new FrontaItem(); // alokuje se nova polozka fronty
	item->nakladak = nakladak; // do teto polozky se priradi nakladak
	item->dalsi = NULL; // ukazatel na dalsi se nastavi na NULL, jelikoz nove vlozeny prvek bude ve fronte posledni, takze nasledovnika nema

	if (this->zacatek == NULL) // pokud je zacatek fronty NULL (=fronta je prazdna -> vkladam do prazdne fronty)
	{
		this->zacatek = item; // nastavim zacatek fronty na vkladany prvek
	}
	else // Jinak (pokud fronta jiz neco obsahuje)
	{
		FrontaItem *pointer = this->zacatek;  // nastavim si pointer na zacatek fronty
		while (pointer->dalsi != NULL) // a dokud ma prvek, na ktery pointer ukazuje nejakeho nasledovnika
			pointer = pointer->dalsi; // nastavim pointer na tohoto nasledovnika
		// tim vlastne docilim toho, ze pointer po skonceni while cyklu bude ukazova na posledni prvek ve fronte

		pointer->dalsi = item; /*pointer ukazuje tedy na posledni prvek ve fronte a jako nasledovnika tohoto prvku nastavim 
		vkladany prvek, cimz docilim toho, ze jsem vlozil prvek na konec fronty */
	}
}


NakladniAuto* Fronta::VratNakladniAuto() // vrati ukazatel na nakladak na zacatku fronty, ale neodebere jej
{
	return this->zacatek->nakladak;
}

void Fronta::OdeberNakladniAuto() // odebere nakladak z fronty a vrati na nej ukazatel
{
	if (this->zacatek == NULL) // pokud ve Fronte nic neni
		return; // ukonci se funkce

	else
	{
		FrontaItem *pointer = this->zacatek; // nastav pointer na zacatek fronty (=na odebirany prvek)
		this->zacatek = this->zacatek->dalsi; /* presun zacatek fronty na dalsi prvek (=odebirany prvek neni jiz na zacatku fronty,
		ted je na zacatku jeho nasledovnik (pokud zadny nasledovnik neni, tak NULL */

		delete pointer->nakladak;
		delete pointer; // dealokuje se prvekFronty, ve kterem byl tento nakladak ulozen
	}
	
}

Fronta::Fronta()  // konstruktor Frontau
{
    this->zacatek = NULL; // fronta je na zacatku prazdna, tak se jeji zacatek nastavi na NULL
}





int main(void)
{
	srand((unsigned)time(NULL));
	Fronta fronta[3]; // vytvor tri fronty

	do
	{
		if (NakladniAuto::pocetNakladaku < N) // pokud jeste nebylo vygenerovano N nakladaku
		{
			NakladniAuto *nakladak = new NakladniAuto(); // vygeneruj novy nakladak
			
			if (nakladak->CenaNakladu() <= MAX_CENA_NAKLADU/3) // pokud je cena mensi nebo rovna nez tretina maximalni ceny nakladu
			{
				fronta[0].Vlozit(nakladak); // vloz nakladak do 1. fronty (s indexem 0)
				cout << "F" << nakladak->Cislo() << "[1]" << endl;

			}
			else if (nakladak->CenaNakladu() <= 2*MAX_CENA_NAKLADU/3)
			{
				fronta[1].Vlozit(nakladak); // vloz nakladak do 2. fronty (s indexem 1)
				cout << "F" << nakladak->Cislo() << "[2]" << endl;
			}
			else
			{
				fronta[2].Vlozit(nakladak); // vloz nakladak do 3. fronty (s indexem 2)
				cout << "F" << nakladak->Cislo() << "[3]" << endl;
			}
		}

		for (int i = 0; i < 3; ++i) // cyklus od 0 do 2 (pro 3 fronty s indexem 0,1,2)
		{
			if (!fronta[i].JePrazdna()) // pokud fronta neni prazdna
			{
				NakladniAuto *nakladak=fronta[i].VratNakladniAuto(); // vezme z fronty nakladak a ulozi jej do promenne nakladak
				if (nakladak->CenaNakladu() > 0) // pokud je cena nakladu vyssi nez 0 (= nakladak obsahuje nejaky naklad)
				{
					nakladak->VyskladnitPolozku(); // vyskladni jednu polozku (=odebere jednu polozku z nakladaku)
					cout << "V" << nakladak->Cislo() << "[" << nakladak->PocetPolozek() << "]" << endl; // vypise info o vyskladneni
				}

				if (nakladak->CenaNakladu() == 0) // pokud je cena nakladu 0 (=nakladak je prazdny)
				{
					cout << "X" << nakladak->Cislo() << "[" << i+1 << "]" << endl; // vypis info o odstraneni auta z fronty
					fronta[i].OdeberNakladniAuto(); // odeber z fronty tento nakladak
				}
			}
		}


	}
	while (!fronta[0].JePrazdna() || !fronta[1].JePrazdna() || !fronta[2].JePrazdna() || NakladniAuto::pocetNakladaku < N); 
	// cyklus bezi, dokud nejsou vsechny fronty prazdne a dokud nebylo vygenerovano zatim N nakladaku

	return 0;
}