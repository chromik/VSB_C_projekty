#include <iostream>
#include <cstdlib> // kvuli funkci rand()
#include <ctime> // kvuli inicializaci generatoru nahodnych cisel

using namespace std;


const int N = 5; // pocet zaku
const int Z = 2; // pocet znamek u jednoho zaka

class Student{
	public:
		Student(int cislo, int pocet_znamek); // konstruktor, parametry jsou cislo studenta a pocet znamek
		~Student(); // destruktor
		int Cislo(); // vraci cislo studenta
		void VlozitZnamku(int znamka); // vlozi studentovi znamku z parametru
		float PrumernyProspech(); // vrati prumerny prospech studenta
	private:
		int cislo; // cislo studenta
		int aktualni_pocet_znamek; // pocet znamek, ktere jsou u studenta ulozene v poli
		int *znamky; // promenna typu ukazatel na int, pozdeji se do ni priradi dynamicky alokovane pole pro ulozeni znamek
};

Student::Student(int cislo, int pocet_znamek) // kostruktor studenta s parametry
{
	this->cislo = cislo; // studentovi se priradi cislo z parametru
	this->znamky = new int[pocet_znamek]; // vytvori se pole pro ulozeni znamek o velikosti, ktera je dana parametrem pocet_znamek
	this->aktualni_pocet_znamek = 0; /// aktualni pocet znamek se nastavi na 0, protoze objekt student pri vytvoreni zadne znamky nema
}

Student::~Student() // destruktor studenta
{
	delete [] this->znamky; // smaze se dynamicky alokovane pole znamek
}

int Student::Cislo() // vrati cislo studenta
{
	return this->cislo;
}

void Student::VlozitZnamku(int znamka) // vlozi studentovi znamku z parametru
{	
	this->znamky[this->aktualni_pocet_znamek++] = znamka; /* znamka se ulozi do pole,
		vlozeni prvni znamky se ulozi na index 0, druhe na index 1, treti na index 3, protoze pole indexuji
		pomoci promenne "aktualni_pocet_znamek" ktery se ihned po vlozeni nove znamky inkrementuje
		*/
}

float Student::PrumernyProspech() //spocita prumerny prospech studenta
{
	float suma = 0; // vytvori se promenna, ktera se nastavi na 0
	for (int i = 0; i < Z; ++i) // ve for cyklu od 0 do Z-1 (probiha Z krát)
	{
		suma += this->znamky[i]; // se pricte k sume "i"-tá známka
	} // takze po skonceni cyklu je v "suma" ulozen soucet vsech znamek
	return suma / Z; // tento soucet se vydeli Z (počtem známek), tak se získá průměrná známka a ta se vrátí
}

class Seznam{
	public:
		Seznam();
		~Seznam();
		void Pridat(Student *student);

		void VlozeniZnamek(); // vlozi kazdemu studentovi v seznamu znaku
		void VypsatPrumernyProspech();
	private:
		struct SeznamPolozka{ //definice struktury prestavujici jednu polozku v seznamu
			Student *student; // struktura obsahuje ukazatel na studenta
			SeznamPolozka *dalsi; //a ukazatel na dalsi strukturu stejneho typu (dalsi polozka v seznamu)
		};

		SeznamPolozka *hlava; // ukazatel na zacatek seznamu (ukazatel na prvni polozku seznamu)
		SeznamPolozka *iterator; /* ukazatel na libovolnou polozku v seznamu, na kterou polozku bude
		 ukazovat lze nastavit pomocí metod  IteratorReset a IteratorDalsi;
		*/




		/* nasledujici tri Metody nize lze taky zapsat samozrejme mimo tridu stylem: 
		
		void Seznam::IteratorReset() 
		void Seznam::IteratorDalsi()
		bool Seznam::IteratorJeKonec()

		a zde ponechat pouze deklaraci 
		
		Mohly by klidne byt taky PUBLIC, ale vzhledem k tomu, ze je stejne pouziva jen Seznam a z venku se k nim nikde 
		nepristupuje, jsou PRIVATE (nicmene pokud by se daly do PUBLIC, nic to na funkcnosti programu nemeni)
		*/

		void IteratorReset() {// nastavi ukazatel iterator na zacatek seznamu
			this->iterator = this->hlava; // hlava je zacatek seznamu, takze se iterator nastavi na hlavu
		}

		void IteratorDalsi() {/* nastavi ukazatel iterator na dalsi polozku v seznamu
			EXAMPLE: pokud iterator ukazuje na 2. polozku, bude po zavolani teto metody ukazovat na 3.,
			 pokud ukazuje na 3. bude na 4... (prostě se přesune iterator dále v seznamu)
		 */
			this->iterator = this->iterator->dalsi;
		}
		
		bool IteratorJeKonec() { /* zjisti zda iterator jiz neni mimo seznam */
			if (this->iterator == NULL) /* pokud iterator ukazuje na NULL, tedy je mimo seznam 
			*/
				return true; // vrat TRUE, ze je mimo seznam
			else // jinak
				return false; // vrat FALSE (=jeste neni mimo seznam - ukazuje na nejakou polozku seznamu)
		}
};

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
		cout << "Z" << this->iterator->student->Cislo() << "[" << nahodna_znamka << "]" << endl;
	}
}

void Seznam::VypsatPrumernyProspech() // vypise prumerny prospech kazdeho zaka v seznamu
{
	for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // pomoci iteratoruy prijdu cely seznam
	{
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

int main(void)
{
	srand((unsigned)time(NULL)); // inicializace generatoru nahodnych cisel (pouziva se, aby rand() negenerovalo stale stejna cisla)
	Seznam seznam; // vytvoreni objektu seznam

	for (int i = 1; i <= N; ++i) // for cyklus od 1 do N (=generuje se N studentu)
	{
		Student *student = new Student(i, Z); // studentovi se priradi cislo (i) a pocet znamek (Z)
		cout << "G" << student->Cislo() << "[" << Z << "]" << endl; // vypise se Info o vygenerovani, cislo studenta a pocet znamek
		seznam.Pridat(student); // student se prida do seznamu
	}


	for (int i = 0; i < Z; ++i) // for cyklus jde Z krát (přidává se Z známek každému studentovi)
	{
		seznam.VlozeniZnamek(); // nad seznamem se zavola metoda VlozeniZnamek(), ktera kazdemu studentovi v seznamu vlozi jednu znamku

	}

	seznam.VypsatPrumernyProspech(); /* nad seznamem se zavola metoda VypsatPrumernyProspech(), která vypíše prumerny prospech
		kazdeho zaka ze seznamu*/

	return 0;
}