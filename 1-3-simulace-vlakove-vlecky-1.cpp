#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const unsigned int M = 50; //pocet vagonu
const unsigned int N = 4; //pocet slepych koleji

struct Vagon
{
	int id; // poradi vagonu
	int kod; // ciselny kod vagonu
};

struct Vagon* generujVagon(const int id) 
{
	struct Vagon *vagon=new struct Vagon; //dynamicky alokuje novy vagon
	vagon->id=id; //nastavi mu id podle parametru funkce
	vagon->kod=rand()%N; //nahodne vygeneruje kod od 0 do N-1
	cout << "G" << vagon->id << "[K" << vagon->kod << "] "; //vypise info o vygenerovani vlaku
	return vagon; //vrati adresu vygenerovaneho vagonu
}

struct pZasobnik //polozka Zasobniku
{ 
	pZasobnik *dalsi; //ukazatel na dalsi polozku v Zasobniku
	Vagon *vagon; //ukazatel na vagon
};

struct Zasobnik{
	pZasobnik *vrchol; //ukazatel na vrchol zasobniku
};


/**
@brief Funkce, ktera inicializuje pocatecni promennou zasobniku
@param zasobnik Ukazatel na zasobnik
*/
void inicializaceZasobniku(Zasobnik *zasobnik)
{
	zasobnik->vrchol=NULL; //nasavi zasobnik na prazdny
}


/**
@brief Funkce, ktera kontroluje, zda je zasobnik prazdny
@param zasobnik Ukazatel na zasobnik
@return V pripade prazdneho zasobniku vrati true, v pripade neprazdneho false
*/
bool jePrazny(Zasobnik *zasobnik)
{
	if (zasobnik->vrchol==NULL) //pokud je vrchol zasobniku NULL
		return true; //vratit TRUE -> zasobnik je prazdny
	else
		return false; //vratit FALSE -> zasobnik neni prazdny
}

/**
@brief Funkce, ktera vlozi prvek do zasobniku
@param zasobnik Ukazatel na zasobnik
@param vagon Ukazatel na prvek, který bude vložen
*/
void Vlozit(Zasobnik *zasobnik,Vagon *vagon)
{
	pZasobnik *temp=new pZasobnik;	//dynamicky se vytvori prvek zasobniku
	temp->vagon=vagon;	//tomuto prvku se priradi vagon
	temp->dalsi=zasobnik->vrchol;	// nove vytvorenemu prvku se jako nasledovnik nastavi prvek, ktery byl dosud v zasobniku na vrcholu
	zasobnik->vrchol=temp; //vrchol zasobniku ukazuje na nove vlozeny prvek
}

/**
@brief Funkce, ktera vyjme prvek ze zasobniku
@param zasobnik Ukazatel na zasobnik
@return Vrazi ukazatel na odebrany prvek
*/
Vagon * Odeber(Zasobnik *zasobnik)
{
	if (!jePrazny(zasobnik)) //pokud je zasobnik neprazdny
	{
		pZasobnik *ukazatel=zasobnik->vrchol; //nastavi ukazatel na prvek, ktery se nachazi na vrcholu zasobniku (na prvek, ktery bude odebran)
		zasobnik->vrchol=zasobnik->vrchol->dalsi; //vrchol zasobniku se posune na dalsi prvek (prvek, ktery vyjimame se tedy uz v zasobniku nenachazi)

		Vagon *vagon=ukazatel->vagon; //docasna promenna pro ulozeni vagonu
		delete ukazatel; //dealokuje prave vyjmuty prvek ze zasobniku
		return vagon; //vrati adresu, na ktere se nachazi vyjmuty vagon
	}
	else return NULL; //pokud je zasobnik prazdny, nelze nic odebrat
}


/**
@brief Funkce, ktera zasobnik (vlak) naplni nahodne vygenerovanymi vagony
@param vlak Ukazatel na zasobnik reprezentujici vlak
*/
void naplnVlak(Zasobnik* vlak)
{
	for (unsigned int i=1;i<=M;++i) // cyklus jde od 1 do M
		Vlozit(vlak,generujVagon(i)); //vygeneruje se M vagonu s id od 1 do M
}


/**
@brief Funkce, ktera ze zasobniku odebere a dealokuje vsechny prvky zasobniku.
*/
void dealokacePrvkuZasobniku(Zasobnik* vlak)
{
	while (!jePrazny(vlak))	//dokud zasobnik neni prazdny
		delete Odeber(vlak); //vyjmi z nej prvek a ten dealokuj
}


/**
@brief Funkce, ktera z vlaku odebira vagony a vklada je na koleje dle kodu vagonu
@param vlak Ukazatel na zasobnik reprezentujici vlak
@param koleje Ukazatel na pole zasobniku reprezentujici koleje
*/
void odstavVagony(Zasobnik* vlak,Zasobnik* koleje)
{
	while (!jePrazny(vlak)) //dokud neni vlak prazdny
	{
		struct Vagon* temp=Odeber(vlak); //odeber z nej vagon a uloz jej do docasne promenne temp
		Vlozit(&koleje[temp->kod],temp); //vloz tento vagon do dane koleje dle ciselneho kodu vagonu
		cout << "O" << temp->id << "[K" << temp->kod << "] "; //vypis info o odstaveni vagonu do kolejnice
	}
}



/**
@brief Funkce, ktera z koleji postupne odebira vagony a pripojuje je k vlaku vzestupne dle kodu vagonu
@param vlak Ukazatel na zasobnik reprezentujici vlak
@param koleje Ukazatel na pole zasobniku reprezentujici koleje
*/
void pripojVagony(Zasobnik* vlak,Zasobnik* koleje)
{
	for (unsigned int i=0;i<N;++i) // ze vsech kolejnic od první (koleje[0]) po poslední (koleje[N-1])
		while(!jePrazny(&koleje[i]))  // dokud neni dana kolejnice prazdna
		{
			struct Vagon* temp=Odeber(&koleje[i]); //odeber z kolejnice vagon a uloz jej do docasne promenne temp
			Vlozit(vlak,temp);	//pripoj odebrany vagon zpatky na vlak
			cout << "P" << temp->id << "[K" << temp->kod << "] "; //vypis info o pripojeni vagonu zpet k vlaku
		}
}

int main()
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel

	cout << "Generovani vagonu u vlaku:" << endl;
	Zasobnik vlak;	//zasobnik reprezentujici vlak
	inicializaceZasobniku(&vlak); //inicializace tohoto zasobniku
	naplnVlak(&vlak); //pripojeni nahodne vygenerovanych vagonu k vlaku

	cout << endl << endl << "Odstavovani vagonu do koleji:" << endl;
	struct Zasobnik koleje[N]; //zasobniky reprezentujici kolejnice
	for (int i=0;i<N;++i)
		inicializaceZasobniku(&koleje[i]); //inicializace techto zasobniku
	odstavVagony(&vlak,koleje); //odebira vagony z vlaku a odklada je na kolejnice dle kodu vagonu

	cout << endl << endl << "Pripojovani vagonu k vlaku:" << endl;
	pripojVagony(&vlak,koleje); //pripojuje vagony z kolejnic zpet k vlaku dle kodu vagonu
	cout << endl;

	dealokacePrvkuZasobniku(&vlak); //kolejnice byly vytvorene dynamicky, takze pred ukoncenim programu je treba je dealokovat

	return 0;
}