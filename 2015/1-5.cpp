#include <iostream>
#include <cstdlib>

using namespace std;

const int N = 7; // velikost jedne fronty
const int Z = 10; // maximalni pocet vygenerovanych vygenerovano_zakazniku v jedne casove jednotce
const int M = 1000; // udava, kolik bude celkove vygenerovano vygenerovano_zakazniku

struct Zakaznik {
	int id; // id zakaznika
};

int vygenerovano_zakazniku = 0;

Zakaznik VytvoritZakaznika(void)
{
	Zakaznik zakaznik; // deklarace zakaznika 
	zakaznik.id = ++vygenerovano_zakazniku; // zvetci pocitadlo vygenerovano_zakazniku a priradi toho cislo jako id nove vytvoreneho zakaznika

	return zakaznik; // vrati zakaznika
}

struct Fronta {
	int cislo_fronty; // cislo fronty
	bool otevrena; // udava, zda je fronta zrovna otevrena nebo uzavrena

	Zakaznik zakaznik[N]; // pole vygenerovano_zakazniku
	int pocet_prvku; // pocet prvku ve fronte
	int zacatek; // zacatek fronty (kde je umisten 1. prvek)

	Fronta *dalsi; //ukazatel na dalsi frontu
};

struct Seznam {
	Fronta *zacatek; // ukazatel na prvni frontu v seznamu
	int pocet_front; // celkovy pocet front, ktere se nachazeji v tomto seznamu
};


void FrontaInicial(Fronta *fronta, int cislo) // pocatecni inicializace fronty
{
	fronta->zacatek = 0; // zacatek fronty je v poli na indexu 0;
	fronta->pocet_prvku = 0; // fronta se inicializaci nastavuje na prazdnou, pocet jejich prvku bude tedy 0
	fronta->dalsi = NULL; // ukazatel na dalsi frontu se nastavi na NULL
	fronta->cislo_fronty = cislo; // cislo fronty
	fronta->otevrena = false; // udava, zda je fronta zrovna otevrena nebo uzavrena
}

bool FrontaJePrazdna(Fronta *fronta)
{
	if (fronta->pocet_prvku == 0) //pokud ma 0 prvku je prazdna
		return true;
	else
		return false;
}

void FrontaVlozit(Fronta *fronta, Zakaznik zakaznik)
{
	fronta->zakaznik[(fronta->zacatek + fronta->pocet_prvku) % N] = zakaznik;
	++fronta->pocet_prvku;
}

Zakaznik FrontaOdebrat(Fronta *fronta)
{
	--fronta->pocet_prvku; //zmensi se pocet prvku ve fronte
	fronta->zacatek = fronta->zacatek % N; //aby zacatek nemohl dosahnout hodnoty N, ale jen 0 až N-1
	return fronta->zakaznik[fronta->zacatek++]; //vrati se zakaznik a zaroven se zacatek posune na dalsiho zakaznika
}

bool FrontaJePlna(Fronta *fronta)
{
	if (fronta->pocet_prvku == N) // pokud je velikost fronty rovno N
		return true; // Fronta je plna => vrati TRUE
	else // jinak
		return false; // Fronta je prazdna => vrati FALSE;
}

void SeznamInicial(Seznam *seznam) //inicializuje seznam (=nastavi jej na prazdny)
{
	seznam->zacatek = NULL; // seznam je prazdny => jeho ukazatel na zacatek tedy ukazuje na NULL
	seznam->pocet_front = 0; // nastav pocet front v seznamu na 0
}

void DealokaceSeznamu(Seznam *seznam)
{
	while (seznam->zacatek != NULL) //pokud v seznamu neco je
	{
		Fronta *temp = seznam->zacatek; // nastav docasnou promennou na zacatek seznamu
		seznam->zacatek = seznam->zacatek->dalsi; // zacatek seznamu nastav na dalsi polozku (nebo pokud dalsi neni, tak na NULL)
		delete temp; //polozku, ktera je ulozena v docasne promenne, smaz
	}
}

Fronta* PridatFrontu(Seznam *seznam)
{
	Fronta *konec = seznam->zacatek; //vytvori se ukazatel, ktery ted ukazuje na prvni frontu v seznamu
	++seznam->pocet_front; // zvetsi pocet front
	Fronta *fronta = new Fronta; // vytvori novou frontu (dynamicky)
	FrontaInicial(fronta, seznam->pocet_front); // provede pocatecni inicializaci teto fronty (=nastavi ji na prazdnou)

	if (konec == NULL) // pokud v seznamu dosud zadna fronta nebyla
	{
		seznam->zacatek = fronta; // nastav zacatek seznamu na tuto (nove vytvorenou) frontu
		return fronta; // vrat tuhle frontu
	}


	while (konec->dalsi != NULL) // dokud neukazuje konec na posledni frontu v seznamu
		konec=konec->dalsi; // posun ukazatel konec na dalsi frontu
		// po skonceni tohoto while cyklu bude konec ukazovat na posledni frontu v seznamu

	konec->dalsi = fronta; // na konec seznamu pridej nyni vytvorenou frontu
	return fronta; // vrat adresu nove vytvorene fronty
}


bool JeNeplnaOtevrena(Fronta *fronta)
{
	if (!FrontaJePlna(fronta) && !FrontaJePrazdna(fronta))
		return true;
	else
		return false;
}

bool JeNeplnaUzavrena(Fronta *fronta)
{
	if (!FrontaJePlna(fronta) && FrontaJePrazdna(fronta))
		return true;
	else
		return false;
}

Fronta* VratitNeplnouFrontu(Seznam *seznam)
{
	Fronta *ukazatel = seznam->zacatek; // nastavi ukazatel na prvni frontu (v pripade prazdneho seznamu na NULL)
	
	if (ukazatel == NULL) // pokud seznam neobsahuje zatim zadnou frontu (je prazdny)
		return NULL; //vrati se NULL


	for (int i = 0; i < seznam->pocet_front; ++i)  // pro vsechny fronty v seznamu
	{
		if (JeNeplnaOtevrena(ukazatel)) //pokud neni fronta plna a zaroven je tato fronta otevrena
			return ukazatel; //vrat na ni ukazatel
		else
			ukazatel = ukazatel->dalsi; // jinak prejdi na dalsi frontu v seznamu
	}


	/* tenhle blok se provadi, pokud nebyla nalezena fronta, co je otevrena a zaroven neni plna.. 
	Nyni se hleda, zda je dostupna alespon nejaka uzavrena fronta, ktera by se mohla otevrit */
	ukazatel = seznam->zacatek;   //nastaveni ukazatele na prvni frontu v seznamu
	for (int i = 0; i < seznam->pocet_front; ++i)  //pro vsechny fronty seznamu
	{
		if (JeNeplnaUzavrena(ukazatel)) //pokud neni fronta plna, ale je jen uzavrena
			return ukazatel; //vrat na ni ukazatel
		else
			ukazatel = ukazatel->dalsi; // jinak prejdi v cyklu na dalsi frontu v seznamu
	}
	
	return NULL; // pokud nenalezena zadna prazdna fronta v seznamu, vrati se NULL
}

bool JsouFrontyPrazdne(Seznam *seznam)
{
	Fronta *ukazatel = seznam->zacatek; // ukazatel se nastavi na prvni frontu v seznamu
	
	if (ukazatel == NULL) // pokud je ukazatel == NULL, tak je seznam prazdny
		return false; //takze se vrati false, protoze v prazdnem seznamu nemuze byt zadna prazdna fronta

	for (int i = 0; i < seznam->pocet_front; ++i) //pro vsechny fronty v seznamu
		if (!FrontaJePrazdna(ukazatel)) // pokud fronta neni prazdna
			return false; //vrat false
		else  // jinak
			ukazatel = ukazatel->dalsi; //prejdi v cyklu na dalsi polozku
	return true; // nebyla nalezena zadna fronta, co by nebyla prazdna == vsechny jsou prazdne == vrat TRUE
}

void GenerovaniZakazniku(Seznam *seznam)
{
	int pocet_vygenerovano_zakazniku = (rand() % Z) + 1;
	for (int i = 0; i < pocet_vygenerovano_zakazniku; ++i)  // cyklus muze jit 1 krát až Z krát (=může se v něm vytvořit 1 až Z zákazníku)
	{
		Zakaznik zakaznik = VytvoritZakaznika(); //vygenerovaní zákazníka a uložení jej do proměnné zakaznik
		cout << "G" << zakaznik.id << endl; // vypsat info o vygenerovani zakaznika
		
		Fronta *volna = VratitNeplnouFrontu(seznam); // do ukazatele prazdna ulozi prvni frontu ze seznamu, ktera neni plna
		if (volna == NULL) // pokud je volna rovna NULL (=to znamena, ze nebyla v seznamu nalezena zadna fronta s volnym mistem)
			volna = PridatFrontu(seznam); // vytvori v seznamu dalsi frontu a nastavi ukazatel volna na ni
				
		if (volna->otevrena == false)  // jestlize je volna fronta uzavrena
		{
			volna->otevrena = true; // otevri ji
			cout << "F" << seznam->pocet_front << endl; // vypis info o otevreni nove fronty (=pokladny)
		}
		
		FrontaVlozit(volna, zakaznik); //vytvoreneho zakaznika vlozi do volne fronty
		cout << "V" << zakaznik.id << "[F" << volna->cislo_fronty << "]" << endl; // vypise info o vlozeni zakaznika do fronty
	}
}

void ZpracovaniFront(Seznam *seznam)
{
	Fronta *fronta = seznam->zacatek; // nastavi ukazatel fronta na zacatek seznamu (tedy na prvni frontu)
	for (int i = 0; i < seznam->pocet_front; ++i) // pro vsechny prvky seznamu (=pro vsechny fronty)
	{
		if (!FrontaJePrazdna(fronta)) // pokud je ve fronte nejaky zakaznik (jinak se prejde na dalsi frontu)
		{
			Zakaznik zakaznik = FrontaOdebrat(fronta); // tak tohoto zakaznika z fronty odeber a uloz jej do promenne zakaznik
			cout << "O" << zakaznik.id << "[" << fronta->cislo_fronty << "]" << endl; // vypis info o odebrani zakaznika
 
			if (FrontaJePrazdna(fronta)) // jestlize ve Fronte po odebrani zakaznika jiz nic neni
			{
				fronta->otevrena = false; // nastav ji jako uzavrenou
				cout << "U" << fronta->cislo_fronty << endl; // vypis info o uzavreni fronty (=pokladny)
			}
		}

		fronta = fronta->dalsi; // prejdi na dalsiho zakaznika
	}

}

int main(void)
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	
	Seznam seznam;
	SeznamInicial(&seznam); //pocatecni inicializace seznamu (nastaveni jej na prazdny)


	while (!JsouFrontyPrazdne(&seznam) || vygenerovano_zakazniku < M)  // program neukoncuj, dokud nejsou vsechny fronty prazdne nebo dokud nebylo vygenerovano M vygenerovano_zakazniku
	{

		if (vygenerovano_zakazniku < M) // Generovani probiha, dokud jeste nebylo vygenerovano M vygenerovano_zakazniku
		{
			GenerovaniZakazniku(&seznam);
		}

		ZpracovaniFront(&seznam);

	} 

	DealokaceSeznamu(&seznam); // dealokuj pamet pro seznam
	return 0;
}