#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const unsigned int N = 7;
const unsigned int M = 1000;
const unsigned int Z = 10; 

struct Zakaznik{ // struktura predstavujici Zakaznika
	int id; // id zakaznika
};

unsigned int generovano_zakazniku = 0; // globalni promenna, ukazujici kolik bylo vygenerovano uz zakazniku

struct Fronta{ // struktura predstavujici frontu
	struct Zakaznik pole[N]; // pole zakazniku
	unsigned int zacatek; // udava, na kterem indexu v poli je prvni zakaznik ve fronte
	unsigned int prvku; // udava, kolik je ve fronte zakazniku
};


struct Fronta* Fronta_Vytvorit(void) // funkce vytvori novou frontu, inicializuje ji a vrati adresu na ni
{
	struct Fronta *fronta = new struct Fronta; // dynamicky vytvorena fronta
	fronta->zacatek = 0; // pocatecni index bude 0
	fronta->prvku = 0; // pocet zakazniku ve fronte taky 0  -> pocatecni nastaveni (=fronta je ted prazdna)
	return fronta; // vrat adresu vyvorene fronty
}

void Fronta_Vlozit(struct Fronta *fronta, struct Zakaznik zakaznik) // vlozi do Fronty zakazaznika z parametry
{
	fronta->pole[(fronta->zacatek + fronta->prvku) % N] = zakaznik; /* zakaznik se vlozi do pole na zacatek a posune se doprava o pocet prvku,
	 co v poli uz jsou (=zaradi se tedy nakonec pole), %N je proto, ze indexace muze prekrocit velikost pole a v tom pripade se zacina zase znova vlevo */
	++fronta->prvku; // zvysi pocitadlo zakazniku ve fronte

}

struct Zakaznik Fronta_Odebrat(struct Fronta *fronta) // vrati Zakaznika z fronty
{
	Zakaznik tmp = fronta->pole[fronta->zacatek]; // zakaznika ze zacatku fronty ulozi do 'tmp'
	--fronta->prvku; // zmensi pocitadlo zakazniku ve fronte
	++fronta->zacatek;
	fronta->zacatek = fronta->zacatek % N; // posune zacatek fronty doprava, pripadne (pri prekroceni velikosti zprava) ho posune zase nalevo (proto opet % N)
	return tmp;
}

bool Fronta_JePrazdna(struct Fronta *fronta)
{
	return fronta->prvku == 0; // fronta je prazdna, pokud ma 0 prvku
}

bool Fronta_JePlna(struct Fronta *fronta)
{
	return fronta->prvku == N; // fronta je plna, pokud ma N prvku
}


struct pSeznam{ // struktura reprezentujici prvek seznamu
	struct Fronta *fronta; // ukazatel na frontu
	struct pSeznam *dalsi; // ukazatel da dalsi prvek seznamu
};

struct Seznam{ // struktura reprezentujici seznam
	struct pSeznam *hlava; // ukazatel na zacatek seznamu (=hlava seznamu)
	struct pSeznam *iterator; // iterator, da se nastavovat pres funkce Seznam_iterator_XXXX a ukazuje vzdycky na jeden z prvku v seznamu
	unsigned int iterator_index; // promenna, ktera uchovava cislo prvku (=cislo fronty), na ktere iterator ukazuje
};

struct Seznam* Seznam_Vytvorit(void)
{
	struct Seznam *seznam = new struct Seznam;
	
	seznam->hlava = NULL;
	
	seznam->iterator = seznam->hlava;
	seznam->iterator_index = 0;
	return seznam;
}

struct Fronta* Seznam_VratNeobsazenouFrontu(struct Seznam *seznam, unsigned int *front_id)
{
	*front_id = 0;
	struct pSeznam *pointer = seznam->hlava;
	if (pointer) {
		*front_id = 1;
		while (Fronta_JePlna(pointer->fronta) && pointer->dalsi != NULL) {
			pointer = pointer->dalsi;
			++(*front_id);
		}
		if (Fronta_JePlna(pointer->fronta))
			return NULL;
		else
			return pointer->fronta;
	} else {
		return NULL;
	}
}


// nastavi iterator na zacatek seznamu
void Seznam_iterator_Begin(struct Seznam *seznam) {
	seznam->iterator = seznam->hlava; // iterator ukazuje na hlavu (1. prvek v seznamu)
	seznam->iterator_index = 1; // index toho prvku je 1
}


// prejde iteratorem na dalsi prvek v seznamu
void Seznam_iterator_Next(struct Seznam *seznam) {
	seznam->iterator = seznam->iterator->dalsi; // iterator nastavuju na dalsi prvek
	++(seznam->iterator_index); // index se zvysuje o +1
}

// zjisti, zda iterator ukazuje na konec
bool Seznam_iterator_IsEnd(struct Seznam *seznam) {
	return seznam->iterator == NULL; // iterator ukazuje na NULL -> jsem na konci seznamu
}

// vrati frontu, na kterou ukazuje iterator
struct Fronta* Seznam_iterator_GetFront(struct Seznam *seznam, unsigned int *front_id) {
	*front_id = seznam->iterator_index; // nastavi index v parametru na index podle iteratoru
	return seznam->iterator->fronta; // vrati frontu, na kterou ukazuje iterator
}


// zjisti, zda je seznam prazdny
bool Seznam_JePrazdny(struct Seznam *seznam) {
	return seznam->hlava == NULL; // kdyz je seznam prazdny, hlava je NULL
}


void Seznam_OdebratFrontu(struct Seznam *seznam, struct Fronta *fronta)
{
	struct pSeznam *tmp = seznam->hlava; // nastavi tmp na 1. prvek (hlavu) seznamu

	if (tmp->fronta == fronta) { // pokud je fronta v 1. prvku frontou, kterou chci smazat
		seznam->hlava = tmp->dalsi; // nastav hlavu seznamu na  2. prvek (posun ji doprava)
		delete tmp->fronta; // smaz frontu z prvniho prvku seznamu
		delete tmp; // smaz prvni prvek seznamu
	} else { // jinak
		while (tmp->dalsi->fronta != fronta && tmp->dalsi->fronta != NULL) { // dokud dalsi prvek v seznamu nenoi fronta ke smazani a ani neni NULL
			tmp = tmp->dalsi; // prechazim na dalsi prvek
		}

		if (tmp->dalsi != fronta) {
			return; // pokud nemame po skonceni WHILE cyklu v tmp ulozenou polozku, ktera je pred frontou k smazani, tap fronta neni v seznamu a ukoncim funkci
		} else { // jinak
			struct pSeznam *smazat = tmp->dalsi;  // nastav dalsi polozku ke smazani
			tmp->dalsi = tmp->dalsi->dalsi; // zmen ukazatel na polozku objedno dal
			delete smazat->fronta; // smaz frontu z polozky ke smazani
			delete smazat; // zmas polozku
		}
	}
}

void Seznam_VlozitFrontu(struct Seznam *seznam, struct Fronta *fronta)
{
	struct pSeznam *seznam_item = new struct pSeznam; // vytvori novou polozku seznamu
	seznam_item->fronta = fronta; // priradi ji frontu z parametru
	seznam_item->dalsi = NULL; // nastavi ji ukazatel dalsi na NULL (nema nasledovnika -> bude v seznamu jako posledni)

	if (!seznam->hlava) { // pokud je seznam prazdny
		seznam->hlava = seznam_item; // nastavim hlavu na novou polozku
	} else { // jinak
		struct pSeznam *pointer = seznam->hlava; // ukazatel nastavim na hlavu
		while (pointer->dalsi)  // a dokud mam kde pokracovat dal
			pointer = pointer->dalsi; // tak posunuju ukazatel doprava

		// po skonceni WHILE  nam pointer ukazuje na posledni prvek....
		pointer->dalsi = seznam_item; // a jeho dalsi nastavim na nove vytvoreny prvek (=pridani prvku na konec seznamu)
	}
}

int main(void)
{
	struct Seznam *seznam = Seznam_Vytvorit();
	unsigned int front_id;

	srand(time(NULL));
	while(!Seznam_JePrazdny(seznam) || generovano_zakazniku < M) // dokud je seznam neprazdny (v nejake fronte jsou zakaznici) nebo nebylo jeste vygenerovano M zakazniku
	{


		/* ZPRACOVAVANI POKLADEN */
		for (Seznam_iterator_Begin(seznam); !Seznam_iterator_IsEnd(seznam); Seznam_iterator_Next(seznam)) { //  iteratorem prochazim seznam
			
			struct Fronta *fronta = Seznam_iterator_GetFront(seznam, &front_id);

			if (Fronta_JePrazdna(fronta)) {
				Seznam_OdebratFrontu(seznam, fronta);
				cout << "U" << front_id << endl;
			} else {
				struct Zakaznik zakaznik = Fronta_Odebrat(fronta);
				cout << "O" << zakaznik.id << "[F" << front_id << "]" << endl;
			}
		}


		/* GENEROVANI ZAKAZNIKU */
		unsigned int generovat_ted = random() % Z + 1; // zvolim nahodne cislo od 1 do Z, ktere udava pocet vygenerovanych zakazniku v teto casove jednotce
		
		if (generovano_zakazniku + generovat_ted > M) { // pokub by se melo vygenerovat v tehle casove jednotce tolik zakazniku, ze by to melo dohromady presahnout M
			generovat_ted = M - generovano_zakazniku; // tak se vygeneruje jen tolik, aby to M nepresahlo
		}
		
		for (unsigned int i = 0; i < generovat_ted; ++i) {
			struct Zakaznik zakaznik; // vytvori noveho zakaznika
			zakaznik.id = ++generovano_zakazniku; // zvysi globalni promennou generovano_zakazniku a ulozi hodnotu do zakaznik.id
			cout << "G" << zakaznik.id << endl; // vypise informaci o vygenerovani zakaznika


			struct Fronta *fronta = Seznam_VratNeobsazenouFrontu(seznam, &front_id); // vrati prvni neobsazenou frontu ze seznamu (v pripade, ze zadna neni, tak NULL) a cislo teto fronty do promenne cislo_fronty
			
			if (!fronta) { // pokud neni zadna fronta v seznamu dostupna
				Seznam_VlozitFrontu(seznam, Fronta_Vytvorit()); // vytvori se nova fronta a vlozi do seznamu
				fronta = Seznam_VratNeobsazenouFrontu(seznam, &front_id); // vytahnu tuhle frontu ze seznamu a zjistim jeji cislo
				cout << "F" << front_id << endl;
			}

			Fronta_Vlozit(fronta, zakaznik); // vlozi zakaznika do teto fronty
			cout << "V" << zakaznik.id << "[F" << front_id << "]" << endl;



		}

	}
	delete seznam;

	return 0;
}