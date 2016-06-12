#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N=5; // pocet Front, Pocet typu transakci
const int P=3; // Maximalni pocet transakci pro Zakaznika
const int M=100; // Pocet vygenerovanych Zakazniku

struct Zakaznik{
	int id; // identifikace zakaznika
	int pocet_transakci; // pocet transakci k provedeni
	int typ_transakce; // typ transakce

	Zakaznik *dalsi; // ukazatel na dalsiho Zakaznika
	Zakaznik *predchozi; // ukazatel na predchoziho Zakaznika
};

int vygZakazniku = 0; //pocet vygenerovanych Zakazniku

struct Fronta{
	Zakaznik *konec; //ukazatel na posledního Zakaznika ve fronte
};

// Nastaví frontu jako prázdnou
void InitialFronta(Fronta *fronta)
{
	fronta->konec = NULL; //jeli Fronta prazdna, ukazatel konce fronty je NULL
}

// vlozi zakaznika do fronty
void Vlozit(Fronta *fronta, Zakaznik *zakaznik)
{

	zakaznik->dalsi = fronta->konec;	//ukazatel na nasledovnika se nastavi na zakaznika, ktery je pred timto (pokud nikdo pred nim neni, tak na NULL)

	if (fronta->konec != NULL)	//pokud ve fronte je jeste nejaky zakaznik (zakaznici) mimo aktualne pridaneho
		fronta->konec->predchozi = zakaznik;	 //nastav jeho ukazatel predchozi na aktualne vlozeneho zakaznika
	fronta->konec = zakaznik;	// konec fronty se nastavi na nove pridaneho zakaznika (nove pridany zakaznik je na konci Fronty)
}



Zakaznik* VratUkazatelNaZacatekFronty(Fronta *fronta)
{
	Zakaznik *ukazatel = fronta->konec; //nastav ukazatel na konec Fronty
	if (ukazatel == NULL)	//Pokud ve Fronte nic neni
		return NULL;	// vrat NULL
	while(ukazatel->dalsi != NULL) // Dokud nejsem na prvnim prvku ve Fronte
		ukazatel = ukazatel->dalsi; // Posunuji ukazatele dopredu
	return ukazatel; //vrati adresu prvniho Zakaznika ve Fronte
}

Zakaznik * VytvorZakaznika(int id)
{
	Zakaznik *zakaznik = new Zakaznik; //alokuj strukturu Zakaznik
	zakaznik->id = id;	//nastav id z parametru funkce
	zakaznik->pocet_transakci = rand()%P+1; //nahodne prirad číslo od 1 do P
	zakaznik->typ_transakce = rand()%N; //nahodne prirad cislo od 0 do N-1

	zakaznik->dalsi = NULL;
	zakaznik->predchozi = NULL;
	return zakaznik;	//funkce vraci adresu na vytvoreneho Zakaznika
}

Zakaznik * Odeber(Fronta *fronta)
{
	Zakaznik *zakaznik = VratUkazatelNaZacatekFronty(fronta); //nastav ukazatel na prvniho Zakaznika ve Fronte
	if (zakaznik == NULL)
	{
		cout << "Fronta je prazdna. Nelze z ni nic odebrat." << endl;
		return NULL;
	}

	if (zakaznik->predchozi == NULL)	// Pokud je ve Fronte jen jeden Zakaznik (odebirany Zakaznik nema zadneho predchudce)
		fronta->konec = NULL; // nastav konec Fronty na NULL (fronta po odebrani toho Zakaznika bude prazdna)
	else 
		zakaznik->predchozi->dalsi = NULL; // jinak Nastav jako prvniho Zakaznika ve Fronte toho Zakaznika, ktery byl doted na druhem miste
	
	return zakaznik; //vrat adresu na zakaznika, jenz byl vyjmout z fronty
}


Zakaznik * Ziskej(Fronta *fronta)
{
	return VratUkazatelNaZacatekFronty(fronta); // vrati adresu prvniho Zakaznika ve Fronte
}

// zjisti, zda je fronta prazdna. Pokud ano - vrati TRUE, jinak vrátí FALSE;
bool JePrazdna(Fronta *fronta)
{
	if (fronta->konec == NULL)	//pokud ukazuje ukazatel na konec fronty na NULL
		return true;
	else
		return false;
}

bool JsouVsechnyPrazdne(Fronta *fronta)
{
	for (int i=0; i<N; ++i)	
		if (!JePrazdna(&fronta[i]))	//pokud alespon jedna Fronta neni prazdna
			return false;  // vrat FALSE
	return true; // jinak vrat TRUE
}

int main()
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	Fronta fronta[N]; //deklarace pole Fronty o velikost N
	for (int i=0; i<N; ++i)
		InitialFronta(&fronta[i]);	//pocatecni inicializace promennych Fronty

	Zakaznik *zakaznik; // ukazatel na strukturu Zakaznik
	do
	{
		if (vygZakazniku < M) // vytvarim kazazniky, dokud jich nevytvorim M
		{

			// vygenerovani zakaznika a vypis o vygenerovani
			zakaznik = VytvorZakaznika(++vygZakazniku); //vytvor zakaznika a zvetsi pocitadlo vygenerovanych zakazniku
			cout << "G" << zakaznik->id << "[F" << zakaznik->typ_transakce << "][" <<zakaznik->pocet_transakci << "]" << endl;
		
			// vlozeni zakaznika do fronty a vypis o vlozeni
			Vlozit(&fronta[zakaznik->typ_transakce], zakaznik);
			cout << "V" << zakaznik->id << "[F" << zakaznik->typ_transakce << "]" << endl;
		}

		for (int i=0;i<N;++i) //pro všechny Fronty (Front máme N)
		{
			if (!JePrazdna(&fronta[i])) //jestliže není prázdná (=nachází se v ní aspoň jeden zákazník)
			{
				zakaznik = Ziskej(&fronta[i]); //nastav ukazatel na tohoto zakaznika
				
				if (zakaznik->pocet_transakci > 0)	//pokud je u Zakaznika nejaka transakce k provedeni
				{
					--zakaznik->pocet_transakci;	//proved ji
					cout << "T" << zakaznik->id << "[" << zakaznik->pocet_transakci << "]" << endl; //vypis info o zpracovani transakce Zakaznika
				}
				else	// pokud neni transakce k provedeni
				{
					Odeber(&fronta[i]);  //odeber zakaznika z fronty
					cout << "O" << zakaznik->id << "[F" << i << "]" << endl; // vypis info o odebrani Zakaznika
					delete zakaznik;	// dealokuj zakaznika
				}
			}
		}
		system("sleep 10");
	}
	while (!JsouVsechnyPrazdne(fronta) || vygZakazniku < M);	// program běží dokud ještě nebylo vygenerováno M klientu nebo pokud jsou nektere fronty stale neprazdne
	return 0;
}
