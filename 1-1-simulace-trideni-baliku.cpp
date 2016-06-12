#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int N=5; // pocet vstupnich pasu
const int K=100; // celkovy pocet baliku
const int M=10; // pocet kontejneru
const int L=20; // maximalni nosnost jednoho kontejneru

struct Balik{
	int id; 
	int smer_cislo;
	int hmostnost;
};

struct pFronta{
	Balik balik; // balik
	pFronta *dalsi; // ukazatel na dalsi prvek fronty
	pFronta *predchozi; // ukazatel na predchozi prvek fronty
};

struct Fronta{
	pFronta *konec_fronty; // ukazatel na konec fronty (ukazuje tedy na posledni prvek fronty)
};


struct Kontejner // struktura reprezentujici kontejner
{
	int poradove_cislo; // poradove cislo kontejneru (může být 1 až M-1)
	int celkova_hmotnost; // celkova hmostnost baliku v tomto kontejneru
};

void frontaInicial(Fronta *fronta) // pocatecni inicializace fronty
{
	fronta->konec_fronty = NULL;  // konec fronty bude ukazovat na NULL (coz znamena, ze fronta je po pocatecni inicializaci prazdna)
}

void kontejnerInicial(Kontejner *kontejner, int poradi) // pocatecni inicializace kontejneru
{
	kontejner->poradove_cislo=poradi; // kontejneru se priradi poaradove cislo z parametru funkce => pro první kontejner to bude 1, pro druhy 2,...., až po M-1
	kontejner->celkova_hmotnost=0; // pocatecni vaha baliku v kontejneru je 0 => pri inicializaci kontejneru v nem nic neni, takze je hmostnost 0
}

bool frontaJePrazdna(Fronta *fronta) //zjisti zda je fronta prazdna
{
	if (fronta->konec_fronty == NULL)  // pokud konec fronty ukazuje na NULL
		return true; // znamena to, ze ve fronte nic neni a tak se vrati TRUE => Fronta je prazdna
	else // jinak 
		return false; // vrati se FALSE => Fronta neni prazdna
}


void frontaVlozit(Fronta *fronta, Balik balik) //Vlozi balik do fronty
{
	pFronta *polozka = new pFronta; //vytvori novy prvek fronty
	polozka->balik = balik; // tomuto prvku se priradi balik z parametru funkce

	polozka->predchozi = NULL; // predchudce tohoto prvku se nastavi na NULL (= tento prvek byl vlozen na konec fronty, takze zadneho predchudce nema)
	polozka->dalsi = fronta->konec_fronty; // ukazatel dalsi se nastavi na prvek, ktery byl dosud ve fronte na konci

	if (fronta->konec_fronty != NULL) // pokud ve fronte dosud neco bylo (=fronta nebyla pred vlozenim baliku prazdna)
		fronta->konec_fronty->predchozi = polozka; // nastavi se prvku, ktery byl doposud na konci fronty, jeho ukazatel predchozi na aktualne vlozeny prvek
	fronta->konec_fronty = polozka; // konec fronty se nastavi na aktualne vlozeny prvek
}

Balik frontaOdebrat(Fronta *fronta) //odebere z fronty balik
{
	pFronta *ukazatel = fronta->konec_fronty; // nastavi ukazatel na konec fronty

	while(ukazatel->dalsi != NULL)  // dokud ukazatel neukazuje na zacatek fronty (kdyby ukazoval na zacatek, tak by while cyklus neprobehl a pokracovalo se dale..)
		ukazatel = ukazatel->dalsi;  // prenastav ukazatel na dalsi prvek ve fronte

	if (ukazatel->predchozi != NULL) // pokud u prvniho prvku ve fronte neni ukazatel predchozi nastaven na NULL (to znamena, ze za timto prvkem je/jsou jeste nejake dalsi prvky)
		ukazatel->predchozi->dalsi = NULL; //nastavi u prvku, ktery je ve fronte nyni na druhem miste ukazatel dalsi na NULL (=takze tento prvek je nyni ve fronte jiz na prvnim miste)
	else 
		fronta->konec_fronty = NULL; // jinak (to znamena, ze ve fronte po odebrani prvku uz nic nezustane) nastav konec fronty na NULL (= fronta je nyni prazdna)

	Balik balik = ukazatel->balik; //uloz docasne balik do promenne balik
	delete ukazatel; // smaz dynamicky alokovany prvek fronty
	return balik; // vrat balik

}

void vlozeniNaPasy(Fronta *pasy)
{
	Balik balik;
	for (int i=1; i<=K; ++i) {  // cyklus jde K-krát - od 1 do K (vytvori se behem nej K baliku)
		balik.id = i; //prirad baliku id (pro prvni balik id=1, pro druhy balik id=2, ....)
		balik.smer_cislo = rand() % 1000; // nahodne prideli baliku smerovaci cislo
		balik.hmostnost = rand() % (L-1) + 1; // nahodne baliku prideli hmotnost od 1 do L-1

		int cislo_pasu=rand() % N; // zvol nahodne pas (nahodne cislo od 0 do N-1)
		frontaVlozit(&pasy[cislo_pasu],balik); // na pas s timto cislem vloz balik

		cout << "B" << cislo_pasu << "[" << balik.id << "][" << balik.smer_cislo << "][" << balik.hmostnost << "]" << endl; // vypis info o vlozeni baliku na pas
	}
}

bool jsouPasyPrazdne(Fronta *pasy) // zjisti, zda jsou vsechny pasy prazdne
{
	for (int i=0; i<N; ++i){ // vsechny pasy pasy[0] až pasy[N-1];
		if (!frontaJePrazdna(&pasy[i])) // jestlize je pas neprazdny
			return false; // vrat false
	}
	return true; // pokud nebyl nalezeny ani jeden neprazdny pas (=vsechny pasy jsou prazdne), vrat TRUE
}


int main(void)
{
	srand((unsigned)time(NULL)); // inicializace generatoru nahodnych cisel
	Fronta pasy[N]; // vytvori N pasu
	for (int i=0; i<N; ++i)
		frontaInicial(&pasy[i]); // techto N pasu se inicializuje

	vlozeniNaPasy(pasy); // vygeneruje nahodne baliky a vlozi jej nahodne do nejakeho pasu

	Kontejner kontejner[M]; // vytvori se M kontejneru
	for (int i=0; i<M; ++i)  // pro vsechny kontejnery
	{
		kontejnerInicial(&kontejner[i],i+1); // vsechny tyto konternery se inicializuji (nastavi se na prazdne a prideli se jim id)
		cout << "P" << i << "[" << kontejner[i].poradove_cislo << "]" << endl; // vypis info o pripojeni kontejneru k pripojnemu mist u (kontejnery se ihned pripoji)

	}

	while(!jsouPasyPrazdne(pasy)) // dokud nejsou vsechny pasy prazdne
	{
		Balik balik;
		for (int i=0; i<N; ++i) // pro vsechny pasy
		{
			if (frontaJePrazdna(&pasy[i])) //pokud je pas prazdny
				continue; //tak nic nedelej a skoc o jednu iteraci v cyklu dal
			
			balik = frontaOdebrat(&pasy[i]); // odebere balik z pasu a ulozi jej do promenne balik
			int m = balik.id % M; // zjisti podle ID tohoto baliku, do ktereho kontejneru se ma vlozit

			cout << "R" << i << "[" << balik.id << "]" << endl; // vypise info o odebrani baliku z pasu

			if (kontejner[m].celkova_hmotnost + balik.hmostnost > L) // pokud by balik vazil prilis a nevlezl se jiz do kontejneru (presahla by se tim kapacita kontejneru - L)
			{
				cout << "E" << m << "[" << kontejner[m].poradove_cislo << "][" << kontejner[m].celkova_hmotnost << "]" << endl; // vypise info o expedici kontejneru
				kontejner[m].celkova_hmotnost = 0; // po expedici v kontejneru jiz nic nebude, tak se jeho celkova hmotnost se nastavi tedy na 0
				cout << "P" << m << "[" << kontejner[m].poradove_cislo << "]" << endl; // po expedici je treba pripojit kontejner opet zpatky k pripojnemu mistu
			}
			
			kontejner[m].celkova_hmotnost+=balik.hmostnost; // pridej balik do kontejneru
			cout << "U" << m << "[" << balik.id << "][" << balik.smer_cislo << "][" << balik.hmostnost << "][" << kontejner[m].celkova_hmotnost << "]" << endl; // vypis info o ulozeni baliku do kontejneru
	
		}
	}


	// po vyprazdneni pasu se jeste v kontejnerech muzou nachazet nejake baliky
	for (int i=0; i<M; ++i) //takze pro vsechny kontejnery - kontejner[0] až kontejner[M-1]
	{
		if (kontejner[i].celkova_hmotnost != 0) // pokud jejich hmotnost neni 0 => v kontejneru jsou nejake baliky
		{
			cout << "E" << i << "[" << kontejner[i].poradove_cislo << "][" << kontejner[i].celkova_hmotnost << "]" << endl; // posli kontejner k expedici a vypis info
		}
	}


	return 0;
}
