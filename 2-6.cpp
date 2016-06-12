#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const unsigned int C1 = 100; // minimalni cena jedne polozky
const unsigned int C2 = 1000; // maximalni cena jedne polozky
const unsigned int N = 10; // pocet vagonu k vygenerovani
const unsigned int P = 10; // maximalni pocet polozek v jednom vagonu

const unsigned int MAXCENA = P*C2; // maximalni cena zbozi v jednom vagonu

class Vagon{
	public:
		//Vagon(void); // konstruktor bez parametru
		Vagon(unsigned int cislo); // konstruktor s parametrem
		unsigned int Cislo(); // vrati cislo vagonu
		unsigned int PocetPolozek(void); // vrati pocet polozek ve vagonu
		unsigned int CenaNakladu(void); // vrati cenu nakladu ve vagonu
		void VylozitPolozku(void); // vylozi jednu polozku
	private:
		unsigned int cislo; // cislo vagonu
		unsigned int pocetPolozek; // pocet polozek ve vagonu
		unsigned int cenaPolozky; // cena jedne polozky
};

/*Vagon::Vagon(void) { //konstruktor bez parametru nic neinicializuje, takze je prazdny
} */

Vagon::Vagon(unsigned int cislo) { /* konstruktor s parametrem priradi vagonu cislo z parametru 
	a nahodne vygeneruje pocet polozek, cenu jedne polozky a vypise info o vygenerovani */
	this->cislo = cislo; // cislo vagonu se nastavi na hodnotu z parametru
	this->pocetPolozek = (rand() % P) + 1; // pocet polozek se nahodne vygeneruje od 1 do P
	this->cenaPolozky = (rand() % (C2 - C1 + 1)) + C1; // vygeneruje se nahodne cena, aby byla od C1 do C2

	cout << "G" << this->cislo << "[" << this->pocetPolozek << "][" << this->cenaPolozky << "] "; // vypise info o vygenerovani
}

unsigned int Vagon::Cislo(void) { // vrati cislo vagonu
	return this->cislo;
}

unsigned int Vagon::PocetPolozek(void) { // vrati pocet polozek ve vagonu
	return this->pocetPolozek;
}

unsigned int Vagon::CenaNakladu(void) { // vrati cenu nakladu ve vagonu
	return this->pocetPolozek * this->cenaPolozky; // cena nakladu je pocet polozek * cena jedne polozky
}

void Vagon::VylozitPolozku(void) { // vylozi jednu polozku z vagonu
	this->pocetPolozek--; // pocet polozek se zmensi o jednicku
}


class Zasobnik{
	public:
		Zasobnik(void); // konstruktor
		~Zasobnik(void); // destruktor
		void Pridat(Vagon vagon); // prida vagon do zasobnku
		Vagon Odebrat(void); // odebere vagon ze zásobníku
		bool JeNeprazdny(void); // zjisti, zda je zasobnik neprazdny (obsahuje nejaky vagon/vagony )
	private:
		class Prvek{ // Trida prvek, predstavuje prvek zásobníku, ve kterem je ulozen vagon
			public:
				Vagon vagon; // vagon
				Prvek *dalsi; // ukazatel na dalsi prvek
		};
		Prvek *vrchol; // ukazatel na vrchol zásobníku
};

Zasobnik::Zasobnik(void) {  // konstruktor
	vrchol = NULL; // nastavi vrchol zásobníku na NULL (= Zasobnik je prazdny)
}

Zasobnik::~Zasobnik(void) { // destruktor
	Prvek *temp;
	while (this->vrchol != NULL) { // dokud zasobnik neni prazdny
		temp = this->vrchol; //polozka na vrcholu zásobníku se ulozi do temp
		this->vrchol = this->vrchol->dalsi; // vrchol zásobníku se presune na dalsi polozku
		delete temp; // vrchol ulozeny v temp se dealokuje
	}
}

void Zasobnik::Pridat(Vagon vagon) { // prida vagon do zasobnku
	Prvek *prvek = new Prvek; // vytvori se novy prvek zásobníku
	
	prvek->vagon = vagon; // do tohoto prvku se ulozi vagon
	prvek->dalsi = this->vrchol; // ukazatel dalsi toho prvku se nastavi na vrchol zásobníku

	this->vrchol = prvek; // vrchol zásobníku se nastavi na nove vytvoreny prvek
}

Vagon Zasobnik::Odebrat(void) { // odebere vagon ze zásobníku
	Vagon vagon = this->vrchol->vagon; // ulozi vagon na vrcholu zásobníku (vagon, ktery chci odebrat) do promenne vagon

	Prvek *docasnaProm = this->vrchol; // ulozi do docasne promenne prvek zásobníku, ve kterem je tento vagon ulozen
	this->vrchol = this->vrchol->dalsi; // vrchol zásobníku nastavi na dalsi prvek zásobníku

	delete docasnaProm; // dealokuje odebrany prvek zásobníku

	return vagon; // vrati vagon
}

bool Zasobnik::JeNeprazdny(void) { // zjisti, zda je zasobnik neprazdny (obsahuje nejaky vagon/vagony )
	if (this->vrchol == NULL) // pokud je vrchol NULL
		return false; // pak je zasobnik prazdny a vrat FALSE
	else // jinak zasobnik neco obsahuje (=neni prazdny)
		return true; // a vrat TRUE
} 


int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL)); // inicializace generatoru nahodnych cisel

	Zasobnik vlak; // zasobnik reprezentujici vlak
	Zasobnik zasobnik[3]; // zasobnik[0] = 1.kolej, zasobnik[1] = 2.kolej, zasobnik[2] = 3.kolej
	Vagon vagon;

	for (unsigned int i=1; i<=N; i++) {
		vlak.Pridat(Vagon(i)); // Prida do vlaku N vagonu s čísly od 1 do N
	}

	for (unsigned int i=1; i<=N; i++) { //for cyklus probiha tolikrat, kolik mam vygenerovanych vagonu
		
		vagon = vlak.Odebrat(); // odebere z vlaku vagon a ulozi jej do promenne vagon

		if (vagon.CenaNakladu() <= (1.0/3.0 * MAXCENA)){ // pokud je cena nakladu vagonu mensi nebo rovna 1/3 max. možné ceně
			zasobnik[0].Pridat(vagon); // vlož vagon do 1. zásobníku
			cout << "Z" << vagon.Cislo() << "[1] "; // vypis info o zarazeni do zásobníku
		}
		else if (vagon.CenaNakladu() <= (2.0/3.0 * MAXCENA)) { // jinak pokud je cena nakladu vagonu větší než 1/3 max. možné ceny a menší nebo rovna 2/3 max. možné ceny
			zasobnik[1].Pridat(vagon); // vlož vagon do 2. zásobníku
			cout << "Z" << vagon.Cislo() << "[2] "; // vypis info o zarazeni do zásobníku
		}
		else { // jinak
			zasobnik[2].Pridat(vagon); // vloz vagon do 3. zásobníku
			cout << "Z" << vagon.Cislo() << "[3] "; // vypis info o zarazeni do zásobníku
		}
	}

	while (zasobnik[0].JeNeprazdny() || zasobnik[1].JeNeprazdny() || zasobnik[2].JeNeprazdny()) { 
	// cyklus jde do te doby, dokud nejsou vsechny zasobniky (koleje) prazdne
		
		for (int i=0; i<=2; i++) { // cyklus jde 3x (každá iterace pro jeden zasobník)
			if (zasobnik[i].JeNeprazdny()) { // pokud je zasobnik neprazdny ( =obsahuje vagon(y) )
				
				vagon=zasobnik[i].Odebrat(); //odeber vagon ze zasobniku a uloz jej do promenne vagon
				
				vagon.VylozitPolozku(); //vyloz z vagonu jednu polozku
				cout << "V" << vagon.Cislo() << "[" << vagon.PocetPolozek() << "] "; // vypis info o vyložení položky

				if (vagon.CenaNakladu() == 0) { // pokud je cena nákladu po vyložení položky 0 (=ve vagonu uz zadne dalsi položky nejsou)
					cout << "X" << vagon.Cislo() << "[" << i+1 << "] "; // vypis info o odstranění vagonu ze zásobníku
					vlak.Pridat(vagon); // připoj vagon zpět k vlaku
				}
				else // jinak
					zasobnik[i].Pridat(vagon); // vloz vagon zpátky do zásobníku
			}
		}
	}


	return 0;
}