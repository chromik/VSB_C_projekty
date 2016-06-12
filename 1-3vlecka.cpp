#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int M = 50; //pocet vagonu
const int N = 4; //pocet slepych kolejist / rozsah ciselnych kodu vagonu

struct Vagon // struktura reprezentujici vagon
{
	int identifikator; // identifikator Vagonu (poradi, ve kterem byl vygenerovan)
	int kod; // ciselny kod vagonu (dle tohoto kodu pak bude prirazen do kolejiste, kteremu nalezi)
};


struct pZasobnik //polozka Zasobniku
{ 
	pZasobnik *dalsi; // ukazatel na dalsi polozku v Zasobniku
	Vagon vagon; // vagon
};

struct Zasobnik
{
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
	if (zasobnik->vrchol==NULL) { //pokud je vrchol zasobniku NULL
		return true; //vratit TRUE -> zasobnik je prazdny
	}
	else {
		return false; //vratit FALSE -> zasobnik neni prazdny
	}
}

/**
@brief Funkce, ktera vlozi prvek do zasobniku
@param zasobnik Ukazatel na zasobnik
@param vagon prvek, který bude vložen
*/
void vlozit(Zasobnik *zasobnik,Vagon vagon)
{
	pZasobnik *polozka=new pZasobnik;	//dynamicky se vytvori prvek zasobniku
	polozka->vagon=vagon;	//tomuto prvku se priradi vagon
	polozka->dalsi=zasobnik->vrchol;	// nove vytvorenemu prvku se jako nasledovnik nastavi prvek, ktery byl dosud v zasobniku na vrcholu
	zasobnik->vrchol=polozka; //vrchol zasobniku ukazuje na nove vlozeny prvek
}

/**
@brief Funkce, ktera vyjme prvek ze zasobniku
@param zasobnik Ukazatel na zasobnik
@return Vrazi odebrany prvek
*/
Vagon odeber(Zasobnik *zasobnik)
{
		pZasobnik *ukazatel=zasobnik->vrchol; //nastavi ukazatel na prvek, ktery se nachazi na vrcholu zasobniku (na prvek, ktery bude odebran)
		zasobnik->vrchol=zasobnik->vrchol->dalsi; //vrchol zasobniku se posune na dalsi prvek (prvek, ktery vyjimame se tedy uz v zasobniku nenachazi)

		Vagon vagon=ukazatel->vagon; //docasna promenna pro ulozeni vagonu
		delete ukazatel; //dealokuje prave vyjmuty prvek ze zasobniku
		return vagon; //vrati vyjmuty vagon
}


/**
@brief Funkce, ktera zasobnik (vlak) naplni nahodne vygenerovanymi vagony
@param vlak Ukazatel na zasobnik reprezentujici vlak
*/
void naplnVlak(Zasobnik* vlak)
{
	Vagon vagon;
	for (unsigned int i=1;i<=M;++i) {// cyklus jde od 1 do M (vygeneruje se behem nej tedy M vagonu a ty se priradi k vlaku)
		vagon.identifikator=i; //nastavi mu identifikaci z parametru teto funkce
		vagon.kod=rand()%N; //nahodne vygeneruje kod od 0 do N-1
		cout << "G" << vagon.identifikator << "[K" << vagon.kod << "] " << endl; //vypise info o vygenerovani vlaku
		vlozit(vlak,vagon); //tento vagon se vlozi do zasobniku valku
	}
}


/**
@brief Funkce, ktera z vlaku odebira vagony a vklada je na koleje dle kodu vagonu
@param vlak Ukazatel na zasobnik reprezentujici vlak
@param koleje Ukazatel na pole zasobniku reprezentujici koleje
*/
void odstavVagony(Zasobnik* vlak,Zasobnik* koleje)
{
	while (!jePrazny(vlak)) { //dokud neni vlak prazdny
		Vagon vagon=odeber(vlak); //odeber z nej vagon a uloz jej do docasne promenne vagon
		vlozit(&koleje[vagon.kod],vagon); //vloz tento vagon do dane koleje dle ciselneho kodu vagonu
		cout << "O" << vagon.identifikator << "[K" << vagon.kod << "] " << endl; //vypis info o odstaveni vagonu do kolejnice
	}
}



/**
@brief Funkce, ktera z koleji postupne odebira vagony a pripojuje je k vlaku vzestupne dle kodu vagonu
@param vlak Ukazatel na zasobnik reprezentujici vlak
@param koleje Ukazatel na pole zasobniku reprezentujici koleje
*/
void pripojVagony(Zasobnik* vlak,Zasobnik* koleje)
{
	for (unsigned int i=0;i<N;++i) { // ze vsech kolejnic od první (koleje[0]) po poslední (koleje[N-1])
		while(!jePrazny(&koleje[i])) {  // dokud neni dana kolejnice prazdna
			Vagon vagon=odeber(&koleje[i]); //odeber z kolejnice vagon a uloz jej do docasne promenne vagon
			vlozit(vlak,vagon);	//pripoj odebrany vagon zpatky na vlak
			cout << "P" << vagon.identifikator << "[K" << vagon.kod << "] " << endl; //vypis info o pripojeni vagonu zpet k vlaku
		}
	}
}


//odebere ze zasobniku vsechny prvky, cimz je vlastne i dealokuje
void dealokacePameti(Zasobnik *vlak)
{
	while (!jePrazny(vlak)) {
		odeber(vlak);
	}
}

int main()
{
	srand(time(NULL)); //inicializace generatoru nahodnych cisel

	cout << "Generovani vagonu u vlaku:" << endl;
	Zasobnik vlak;	//zasobnik reprezentujici vlak
	inicializaceZasobniku(&vlak); //inicializace tohoto zasobniku v
	naplnVlak(&vlak); //pripojeni nahodne vygenerovanych vagonu k valku

	cout << endl << "Odstavovani vagonu do koleji:" << endl;
	Zasobnik koleje[N]; //zasobniky reprezentujici kolejnice
	for (int i=0;i<N;++i)
		inicializaceZasobniku(&koleje[i]); //inicializace techto zasobniku
	odstavVagony(&vlak,koleje); //odebira vagony z vlaku a odklada je na kolejnice dle kodu vagonu

	cout << endl << "Pripojovani vagonu k vlaku:" << endl;
	pripojVagony(&vlak,koleje); //pripojuje vagony z kolejnic zpet k vlaku dle kodu vagonu

	dealokacePameti(&vlak);


	return 0;
}