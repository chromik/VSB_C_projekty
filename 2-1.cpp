#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int A = 5; // pocet aut
const int M = 10; // delka zavodu (v minutach)
const int P = 10; // procento snizeni po oprave
const int R1 = 150; // dolni limit rychlosti auta
const int R2 = 180; // horni limit rychlosti auta

class Auto{
    public:
        Auto();
        ~Auto();
        int Cislo();
        void Zastavit();
        void Opravit();
        void Pokracovat();
        int Vzdalenost();

    private:
        int cislo;
        int procento_snizeni;
        int prumer_rychlost;
        int ujeta_vzdalenost;

        static int pocet_aut; /*staticka promenna, udava kolik objektu typu Auto mam vytvoreno
            V konstruktoru tuto promennou inkrementuji, v destruktoru dekrementuji
        */
};

int Auto::pocet_aut = 0; //pocet vytvorenych aut je pri spusteni programu 0

Auto::Auto() {
     if ( (R2 - R1) < 2 )  // pokud jsou R1 a R2 nastavene tak, ze prumerna rychlost nelze vygenerovat tak, aby byla mezi nimi, vyhodi se chyba
        throw "CHYBA! Hodnoty konstant R1 a R2 jsou zvoleny nespravne.";


    cislo = ++Auto::pocet_aut; //inkrementace poctu vytvorenych aut (promenna static)
    procento_snizeni = rand() % (P - 1) + 1;
    prumer_rychlost = (R1 + 1) + rand() % (R2 - R1 - 1); // nahodne se zvoli prum_ryclost tak, aby platilo: R1 < prum_rychlost < R2
    ujeta_vzdalenost = 0
;
    // info o vygenerovani auta
    cout << "G" << cislo << "[" << prumer_rychlost << "][" << procento_snizeni << "]" << endl;
}

Auto::~Auto()
{
    --Auto::pocet_aut; //dekrementace poctu vytvorenych aut (promenna static)
}

void Auto::Pokracovat()
{
    ujeta_vzdalenost += (prumer_rychlost / 60); // zvetseni ujete vzdalenosti. tato vzdalenost se zvysuje kazdou minutu, proto se pripocitava [prumerna rychlost auta]/60
}

void Auto::Zastavit()
{
    cout << "Z" << cislo << endl; // vypise info o zastaveni
}

void Auto::Opravit() // Oprava auta
{
    prumer_rychlost -= prumer_rychlost * procento_snizeni / 100; //prumerna rychlost se zmensi o procento snizeni

    cout << "O" << cislo << endl; // vypise se info o zmenseni prumerne rychlosti
}

int Auto::Cislo() // vrati cislo Auta
{
    return cislo;
}

int Auto::Vzdalenost() // vrati ujetou vzdalenost Auta
{
    return ujeta_vzdalenost;
}


class Zavod{
    public:
        Zavod();
        ~Zavod();
        void VlozitAuto(Auto *automobil); //Vlozi auto do zavodu
        int Pozice(int poz); // vrati cislo auta na zadane pozici
        void DalsiMinuta();

        void SeraditPole(); // seradi pole Aut od vedouciho v zavode po posledniho
        void VypsatPoradi(int kolo);
    private:
        Auto **pole;
        int pocet_aut;
};

Zavod::Zavod()
{
	pole = new Auto*[A]; // alokuje pole ukazatelu na objekt Auto o velikosti A
    pocet_aut = 0; // pocet Aut v poli bude 0 => v poli zatim nic neni
}

Zavod::~Zavod()
{
    for (int i = 0; i < pocet_aut; ++i) //pro vsechny auta v poli
        delete pole[i]; //dealokuj tyto auta

    delete [] pole; //pote dealokuj i pole ukazatelu na auta
}

int Zavod::Pozice(int poz) // vrati cislo auta na zadane pozici
{
    return pole[poz-1]->Cislo();  // auto na prvnim miste je na indexu 0, proto se od promenne "poz" odecita jednička
}

void Zavod::SeraditPole() /* seradi pole tak, aby v poli na indexu 0 byl zavodnik s nejvetsi 
                            ujetou vzdalenosti, serazeni probiha Bubble Sortem */
{
    for (int i = 0; i < A - 1; i++)
    {
        for (int j = 0; j < A - i - 1; j++)
        {
            if (pole[j]->Vzdalenost() < pole[j+1]->Vzdalenost())
            {
                Auto *tmp = pole[j + 1];
                pole[j + 1] = pole[j];
                pole[j] = tmp;
            }  
        }  
    }  
}

void Zavod::DalsiMinuta() // funkce, ktera provede prubeh jedne minuty zavodu
{
    for (int i = 0; i < pocet_aut; ++i) {
        int random = rand() % 3; // vygeneruje se nahodne cislo od 0 do 2
        switch (random) {
            case 0: // pokud bylo nahodne cislo 0
                pole[i]->Pokracovat();  // auto bude v teto minute pokracovat v jizde
                break;

            case 1: // pokud bylo nahodne cislo 1
                pole[i]->Zastavit(); // auto v teto minute zastavi
                break;

            case 2: // pokud bylo nahodne cislo 2
                pole[i]->Opravit(); // auto se v teto minute opravi
                break;
        }
    }
}

void Zavod::VlozitAuto(Auto *automobil)
{
    if (pocet_aut >= A) // Tahle podminka se v programu sice nikdy nesplni, ale pro jistotu jsem to i tak ošetřil
        throw "CHYBA! Pole je jiz plne, takze do nej nelze vlozit dalsi auto.";


    pole[pocet_aut] = automobil; //Auto se vlozi na konec pole
    ++pocet_aut; // zvetsi se promenna, ktera udava pocet Aut v poli
}

void Zavod::VypsatPoradi(int kolo) // funkce vypisuje info o aktualnim poradi v zavode
{
    cout << "P" << kolo;  // vypise P#cislo_kola, kde cislo_kola je hodnota z parametru teto funkce
    for (int i = 1; i <= pocet_aut; ++i) // pro vsechny auta v zavode (od 1. v poradi po posledniho v poradi)
    {
        cout << "[" << Pozice(i) << "]"; //vypis jejich cislo
    }
    cout << endl;
}

int main(void)
{
    srand (time(NULL));
    Zavod *zavod = new Zavod(); // dynamicka alokace objektu zavod

    // Vytvoreni a vlozeni auto do zavodu
	for (int i = 0; i < A; ++i)
    {
        zavod->VlozitAuto(new Auto());
    }


    // Simulace zavodu, probiha M minut -> for jde od 1 do M
    for (int i = 1; i <= M; ++i)
    {
        zavod->DalsiMinuta(); // funkce, ktera provede prubeh jedne minuty zavod

        zavod->SeraditPole(); // Auta se v poli seradi podle jejich aktualni pozici v zavode

        zavod->VypsatPoradi(i); // funkce vypisuje info o aktualnim poradi v zavode

    }

    delete zavod; // dealokace objektu zavod

    return 0;
}
