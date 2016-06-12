#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int const N=5;
int const M=500;
int const P=7;

struct Pozadavek{
	int identifikace; //id pozadavku
	int provoz; //urcite, na ktere provozne se ma pozadavek vykonat
	bool priorita; //urcuje, zda ma pozadavek prioritu a ma byt zarazen na zacatek seznamu
	static int pocet_pozadavku; //staticka promenna - je v ni ulozeny pocet jit vygenerovanych pozadavku
};

int Pozadavek::pocet_pozadavku=0; // inicializace staticke promenne na 0


Pozadavek * VytvoritPozadavek()
{
	Pozadavek *pozadavek=new Pozadavek;  //dynamicka alokace struktury pro pozadavek
	pozadavek->identifikace=++Pozadavek::pocet_pozadavku; //inkrementuje pocitadlo vytvorenych pozadavku a dle tohoto cisla zaroven nastavi identifikaci
	pozadavek->provoz=rand()%N; //nahodne vygeneruje provoz od 0 do N-1;
	pozadavek->priorita=rand()%2; //nahodne nastavi prioritu na 0 nebo 1 (=false nebo true)
	return pozadavek;	//vrati adresu na aktualne vytvorenou strukturu

}

struct pSeznam{
	pSeznam *dalsi; //ukazatel na dalsi polozku v seznamu
	Pozadavek *pozadavek; //ukazatel na pozadavek
};

struct Seznam{
	pSeznam *hlava; //ukazatel na zacatek seznamu
};

void SeznamInit(Seznam *seznam)
{
	seznam->hlava=NULL; //inicializace seznamu - nastaveni na prazdny seznam
}

bool jePrazny(Seznam *seznam)
{
	if (seznam->hlava==NULL) //pokud je hlava seznamu rovna NULL
		return true; //vratit TRUE -> seznam je prazdny
	else
		return false; //vratit FALSE -> seznam neni prazdny
}

pSeznam * UkazatelNaPosledniPrvek(Seznam *seznam)
{
	if (jePrazny(seznam)) //pokud je seznam prazdny
		return NULL;	//vrat NULL

	pSeznam *ukazatel=seznam->hlava; //nastav ukazatel na prvni prvek
	while(ukazatel->dalsi!=NULL) //dokud ma prvek, na ktery ukazatel ukazuje, nejake nasledovnkiky
		ukazatel=ukazatel->dalsi; //tak prenastav ukazatel na tohoto nasledovnika => po skonceni while cyklu bude ukazatel ukazovat na posledni prvek v seznamu
	return ukazatel; //vrat ukazatel na posledni prvek
}


void VlozitNaZacatek(Seznam *seznam,Pozadavek *pozadavek)
{
	pSeznam *prvSeznam=new pSeznam;	//dynamicky se vytvori prvek seznamu
	prvSeznam->pozadavek=pozadavek;	//tomuto prvku se priradi pozadavek (parametr funkce)
	prvSeznam->dalsi=seznam->hlava;	// nove vytvorenemu prvku se jako nasledovnik nastavi prvek, ktery byl dosud v seznamu na prvnim miste
	seznam->hlava=prvSeznam; //hlava seznamu ukazuje na nove vlozeny prvek -> nove vlozeny prvek je v seznamu na 1. miste
}


void VlozitNaKonec(Seznam *seznam,Pozadavek *pozadavek)
{
	if (jePrazny(seznam)) // pokud je seznam prazdny
	{
		VlozitNaZacatek(seznam,pozadavek); //tak vloz pozadavek na zacatek (v seznamu totiz bude jen jeden pozadavek, tak bude zaroven prvni i posledni)
	}
	else
	{
		pSeznam *ukazatel=UkazatelNaPosledniPrvek(seznam); //nastavi ukazatel na posledni prvek v seznamu
		ukazatel->dalsi=new pSeznam; //tomuto poslednimu prvku se vygeneruje jako nasledovnik novy prvek;
		ukazatel=ukazatel->dalsi; //nastavi ukazatel na tento nove vygenerovany prvek;
		ukazatel->pozadavek=pozadavek; //priradi tomuto prvku pozadavek z parametru funkce
		ukazatel->dalsi=NULL; //nastavi tomuto prvkovi jako nasledovnika NULL (zadneho nasledovnika nema = je na konci seznamu)
	}
}



Pozadavek * Odeber(Seznam *seznam)
{
	if (!jePrazny(seznam)) //pokud je seznam neprazdny
	{
		pSeznam *ukazatel=seznam->hlava; //nastavi ukazatel na 1. prvek v seznamu
		seznam->hlava=seznam->hlava->dalsi; //hlavu seznamu nastavi na 2. prvek (takze 1. prvek v seznamu jiz neni - prave byl vyjmut)

		Pozadavek *pozadavek=ukazatel->pozadavek; //docasna promenna pro ulozeni pozadavku z prave vyjmuteho prvku seznamu
		delete ukazatel; //dealokuje tento vyjmuty prvek seznamu
		return pozadavek; //vrati adresu prave vyjmuteho prvku
	}
	else return NULL; //pokud je seznam prazdny, nelze nic odebrat
}



bool VsechnyPrazdne(Seznam *seznam,int pocet) //pocet je velikost pole seznamu
{
	int prazdnych=0;
	for (int i=0;i<pocet;++i) //projde vsechny seznamy v poli
	{
		if (jePrazny(&seznam[i]))  //pokud je dany seznam prazdny
			++prazdnych; //inkrementuje promenou prazdnych
	}
	if (prazdnych==pocet) //pokud je pocet prazdnych seznamu stejny jako velikost pole
		return true; //vsechny seznamy jsou prazdny 
	else
		return false; //alespon jeden ze seznamu neni prazdny


}



int main(int argc, char *agv[])
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel
	Pozadavek *pozadavek;
	Seznam seznam[N];
	for (int i=0;i<N;++i)
		SeznamInit(&seznam[i]); //pocatecni inicializace seznamu (nastaveni na prazdny seznam)

	do
	{
		for (int i=0;i<rand()%P+1;++i) //cyklus probehne 1x až Px
		{
			if (Pozadavek::pocet_pozadavku<M) //nove pozadavky se vytvari pouze, pokud jich zatim nebylo jeste vytvoreno M
			{
				pozadavek=VytvoritPozadavek(); // vytvoreni pozadavku
				cout << "G" << pozadavek->identifikace << "[S" << pozadavek->provoz << "][" << pozadavek->priorita << "]" << endl;

				if (pozadavek->priorita) // pokud ma pozadavek prioritu
					VlozitNaZacatek(&seznam[pozadavek->provoz],pozadavek); // vlozeni pozadavku na zacatek seznamu (prioritne)
				else
					VlozitNaKonec(&seznam[pozadavek->provoz],pozadavek); // normalni vlozeni pozadavku na konec seznamu

				cout << "V" << pozadavek->identifikace << "[S" << pozadavek->provoz << "][" << pozadavek->priorita << "]" << endl;
			}
		}


		for (int i=0;i<N;++i) //projdi vsechny provozy
		{
			if (!jePrazny(&seznam[i])) //pokud neni dany provoz prazdny
			{
				pozadavek=Odeber(&seznam[i]); //odeber pozadavek, ktery je prvni v poradi
				cout << "O" << pozadavek->identifikace << "[S" << pozadavek->provoz << "]" << endl; //vypis info o realizaci opravy
				delete pozadavek; //dealokuj pozadavek
			}
		}

	}
	while (!VsechnyPrazdne(seznam,N) || Pozadavek::pocet_pozadavku<M); 
	//do - while cyklus probiha tak dlouho, dokud nejsou vsechny seznamy vyprazdnene a dokud pocet vygenerovanych pozadavku nedosahne M


	return 0;
}