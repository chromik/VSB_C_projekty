#include <iostream>
#include <cstdlib> // kvuli funkci rand()

using namespace std;

#include "student.h" // includuji i studenta, protoze v seznamu se s nim pracuje
#include "seznam.h"


Seznam::Seznam() // konstruktor seznamu
{
	this->hlava = NULL; // hlava se nastavi na NULL (=prazdny seznam)
	this->iterator = NULL; // iterator se nastavi taky na NULL (nemuze na nic ukazovat, kdyz je seznam prazdny)
}

Seznam::~Seznam() /* destruktor, nejprve dealokuje vsechny studenty z polozek seznamu 
	a nasledne dealokuje i samotne polozky (ve kterych tito studenti byli ulozeni) */
{
	for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // pomoci iteratoru projdu cely seznam
	{
		delete this->iterator->student; // a dealokuju u kazde polozky seznamu studenta;
	}

	while (hlava != NULL) // cyklus probiha dokud hlava neni NULL (=tedy dokud hlava ukazuje na nejakou polozku seznamu)
	{
		iterator = hlava; // iterator se nastavi na hlavu seznamu (na prvni polozku v seznamu)
		this->hlava = this->hlava->dalsi; // hlava se nastavi na dalsi polozku v seznamu (ale iterator porad ukazuje na prvni)
		delete iterator; // polozka na ktery iterator ukazuje se smaze
	}
}

void Seznam::VlozeniZnamek() // Vlozi kazdemu zakovi v seznamu nahodnou znamku
{
	for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // pomoci iteratoru projdu cely seznam
	{
		int nahodna_znamka = (rand() % 3) + 1; // vygeneruje nahodnou znamku od 1 do 3
		this->iterator->student->VlozitZnamku(nahodna_znamka); /* zavola se metoda studenta, ktera mu vlozi znamku*/
		cout << "Z" << this->iterator->student->Cislo() << "[" << nahodna_znamka << "]" << endl; // vypis info o vlozeni znamky
	}
}

void Seznam::VypsatPrumernyProspech() // vypise prumerny prospech kazdeho zaka v seznamu
{
	for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // pomoci iteratoruy prijdu cely seznam
	{
		cout.setf(ios::fixed);
		cout.precision(3);
		cout << "P" << iterator->student->Cislo() << "[" << iterator->student->PrumernyProspech() << "]" << endl; // a vypisu prospech kazdeho studenta
	}
}

void Seznam::Pridat(Student *student) // prida do seznamu studenta
{
	SeznamPolozka *polozka = new SeznamPolozka; // vytvori dynamicky novou polozku seznamu
	polozka->student = student; // do teto polozky priradi studenta z parametru
	polozka->dalsi = this->hlava; /* ukazatel dalsi teto polozky se nastavi na hlavu seznamu,
		tedy na polozku, na kterou ukazuje hlava seznamu.
		Pokud hlava seznamu neukazuje na zadnou polozku (hlava seznamu je NULL), nastavi se tento
		ukazatel na NULL;
	*/
	this->hlava = polozka; // hlava seznamu se nastavi na nove vlozenou polozku
}


// DEFINICE PRIVATE METOD:

void Seznam::IteratorReset()  // nastavi ukazatel iterator na zacatek seznamu
{
	this->iterator = this->hlava; // hlava je zacatek seznamu, takze se iterator nastavi na hlavu
}

void Seznam::IteratorDalsi() /* nastavi ukazatel iterator na dalsi polozku v seznamu
	PRIKLAD: pokud iterator ukazuje na 2. polozku, bude po zavolani teto metody ukazovat na 3.,
	pokud ukazuje na 3. bude na 4... (prostě se přesune iterator dále v seznamu)
  */
{ 
	this->iterator = this->iterator->dalsi; // iterator se nastavi na dalsi polozku (v pripade, ze dalsi uz neni, tak na NULL)
}
		
bool Seznam::IteratorJeKonec() // zjisti zda iterator jiz neni mimo seznam 
{ 
	if (this->iterator == NULL) /* pokud iterator ukazuje na NULL, tedy je mimo seznam 
			*/
		return true; // vrat TRUE, ze je mimo seznam
	else // jinak
		return false; // vrat FALSE (=jeste neni mimo seznam - ukazuje na nejakou polozku seznamu)
}



/* POZNAMKA:

	for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi())

se dá přepsat na:

	this->IteratorReset(); 				//nastavim promennou "iterator" na zacatek seznamu (na 1. prvek)
	while (!this->IteratorJeKonec()) 	// dokud neni "iterator" rovno NULL (nacházím se stále v seznamu == "iterator" ukazuje na nejaky prvek seznamu)
	{
		NECO_DELEJ.....

		this->IteratorDalsi(); 			// prejdi na dalsi prvek v seznamu (= iterator=iterator->dalsi)
	}

*/