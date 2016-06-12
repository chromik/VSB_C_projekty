#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 5; //pocet prepazek (=pocet typu transakci)
const int P = 3; //maximalni pocet transakci pro jednoho zakaznika
const int M = 100; //uvada, kolik zakazniku bude celkem vygenerovano

struct Zakaznik{
	int id; //id zakaznika (prvni bude 1, posledni M)
	int pocet_transakci; //pocet transakci, ktere potrebuje tento zakaznik provest
	int typ_transakce; // typ transakce, kterou (ktere) potrebuje zakaznik provest
};  //struktrura reprezentujici Zakaznika

struct pFronta {
	Zakaznik zakaznik; //v této struktuře bude uložen zákazník
	struct pFronta *dalsi; //ukazatel na dalsi polozku ve fronte
	struct pFronta *predchozi; //ukazatel na predchozi polozku ve fronte
}; // struktura reprezentujici polozku ve Fronte

struct Fronta{
	pFronta *konec; //ukazatel na poslední prvek ve fronte
}; // struktura reprezentujici Frontu

// Nastaví frontu jako prázdnou
void InitialFronta(Fronta *fronta)
{
	fronta->konec = NULL; //ukazatel na konec fronty na nic neukazuje (=fronta je tedy prazdna)
}

// vlozi zakaznika do fronty
void Vlozit(Fronta *fronta,Zakaznik zakaznik)
{
	pFronta *temp = new pFronta;	//dynamicky se vytvori novy prvek fronty
	temp->zakaznik = zakaznik;	//tomuto prvku se priradi zakaznik

	temp->predchozi = NULL;	//za timto prvkem uz zadny jiny neni (tenhle bude na konci fronty), tak se ukazatel na predchozi nastavi na NULL
	temp->dalsi=fronta->konec;	//ukazatel na nasledovnika se nastavi na zakaznika, ktery je pred timto (pokud nikdo pred nim neni, tak na NULL)

	if (fronta->konec != NULL)	//pokud ve fronte je jeste nejaky prvek mimo aktualne pridaneho
		fronta->konec->predchozi = temp;	 //bude prvek, ktery byl pred vlozenim noveho zakaznika posledni ukazovat na nove pridane prvek jako na sveho predchudce
	fronta->konec = temp;	// konec fronty se nastavi na prvek nove pridaneho zakaznika
}


Zakaznik VytvorZakaznika(int id)
{
	Zakaznik zakaznik; //Vytvor promennou pro zakaznika
	zakaznik.id = id;	//nastav id z parametru funkce
	zakaznik.pocet_transakci = (rand() % P) + 1; //nahodne prirad číslo od 1 do P
	zakaznik.typ_transakce = rand() % N; //nahodne prirad cislo od 0 do N-1
	return zakaznik;	// vraci vytvoreneho zakaznika
}

Zakaznik VratPrvniBezOdberu(Fronta *fronta)
{
	pFronta *ukazatel = fronta->konec; //ukazatel na konec fronty;
	return ukazatel->zakaznik; // vrati zakaznika, jenz byl na konci fronty
}

Zakaznik ZpracujZakaznika(Fronta *fronta)
{
	pFronta *ukazatel = fronta->konec; //ukazatel na konec fronty;
	--ukazatel->zakaznik.pocet_transakci; //zmensi pocet transakci, ktere jsou u zakaznika treba provest (=vykonani jedne transakce)
	return ukazatel->zakaznik; //vrat zakaznika
}

Zakaznik Odeber(Fronta *fronta)
{
	pFronta *ukazatel=fronta->konec; //ukazatel na konec fronty;

	while(ukazatel->dalsi != NULL)	//posunuje ukazatel tak dlouho
		ukazatel=ukazatel->dalsi;	// dokud jej nenastavi na prvni prvek fronty (ten, ktery chci vyjmout)

	if (ukazatel->predchozi != NULL)	//Pokud není prvni prvek fronty jejim jedinym prvkem
		ukazatel->predchozi->dalsi=NULL; //Nastav druhy prvek tak, aby ukazoval na NULL (=nyní už není druhý prvek ve frontě, ale první)
	else 
		fronta->konec=  NULL; //jinak nastav ukazatel na konec fronty na NULL (=fronta je prázdná)
	
	Zakaznik zakaznik = ukazatel->zakaznik; //docasna promenna pro ulozeni zakaznika, ktereho jsem vyjmul z fronty
	delete ukazatel;	//smazani dynamicky alokovane pameti pro prvek fronty;
	return zakaznik; //vrat zakaznika
}

// zjisti, zda je fronta prazdna. Pokud ano - vrati TRUE, jinak vrátí FALSE;
bool JePrazdna(Fronta *fronta)
{
	if (fronta->konec == NULL)	//pokud ukazuje ukazatel na konec fronty na NULL
		return true;
	else
		return false;
}

bool UkoncitOdebirani(Fronta *fronta) //zjisti, zda jsou vsechny fronty prazdne a pokud ano vrati TRUE
{
	for (int i = 0; i < N; ++i)
		if (!JePrazdna(&fronta[i])) //pokud je fronta plna, vrati FALSE
			return false;
	return true; //pokud se neprovedlo FALSE o radek vyse, tak vsechny fronty v cyklu byly prazdne a tak se nyni vrati true
}


int main()
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	Fronta fronta[N];
	for (int i = 0; i < N; ++i)  //pro vsechny fronty
		InitialFronta(&fronta[i]);	//inicializace promennych ve fronte

	int klientu = 0; //pocet vygenerovanych klientu (pri zacatku programu je to 0)
	Zakaznik zakaznik; // deklarace struktury pro ulozeni zakaznika
	do
	{
		if (klientu<M) {// vytvarim kazazniky, dokud jich nevytvorim M
			// vygenerovani zakaznika a vypis o vygenerovani
			zakaznik = VytvorZakaznika(++klientu);
			cout << "G" << zakaznik.id << "[F" << zakaznik.typ_transakce << "][" <<zakaznik.pocet_transakci << "]" << endl;
		
			// vlozeni zakaznika do fronty a vypis o vlozeni
			Vlozit(&fronta[zakaznik.typ_transakce],zakaznik);
			cout << "V" << zakaznik.id << "[F" << zakaznik.typ_transakce << "]" << endl;
			
		}

		for (int i = 0; i < N; ++i) { //pro všechny fronty
			if (!JePrazdna(&fronta[i])) { //jestliže je neprázdná (obsahuje nejake zakazniky)
				zakaznik = VratPrvniBezOdberu(&fronta[i]);	//tak zakaznika odeber a prirad k promenne zakaznik
				if (zakaznik.pocet_transakci > 0) {	//pokud je mozno provest nejakou transakci (zakaznik jich ma ke zpracovani vic nez 0)
					zakaznik = ZpracujZakaznika(&fronta[i]); //proved transakci
					cout << "T" << zakaznik.id << "[" << zakaznik.pocet_transakci << "]" << endl; //vypis info o zpracovani transakce
				}
				else {	// pokud jsou pro tohoto klienta provedeny vsechny transakce
					zakaznik = Odeber(&fronta[i]); //odeber zakaznika z fronty
					cout << "O" << zakaznik.id << "[F" << i << "]" << endl; // vypis info o odebrani 
				}
			}
		}

	}
	while (!UkoncitOdebirani(fronta) || klientu < M);	// program běží dokud ještě nebylo vygenerováno M klientu nebo pokud jsou nektere fronty stale neprazdne

	return 0;
}