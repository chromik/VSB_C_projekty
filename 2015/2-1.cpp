#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>

using namespace std;

const int A = 10; // maximalni vychozi hodnota aktualni spotreby
const int H = 100; // maximalni vychozi hodnota celkove spotreby
const int M = 100; // pocet casovych jednotek pro ktere ma probihat simulace
const int N = 2; // pocet elektromeru

class Elektromer{
    public:
        Elektromer(unsigned int cislo, unsigned int celkovaSpotreba, unsigned int aktualniSpotreba); // konstruktor
        unsigned int Cislo();
        void ZvysitSpotrebu();
        void SnizitSpotrebu();
        void PonechatSpotrebu();
        unsigned int CelkovaSpotreba();

    private:
        float celkovaSpotreba; 
        float aktualSpotreba;
        unsigned int cislo;  
};

Elektromer::Elektromer(unsigned int cislo, unsigned int celkovaSpotreba, unsigned int aktualniSpotreba)
{
    
    this->cislo = cislo; // cislo elektromeru se priradi z parametru funkce, 'this' pouzito, aby se odlisila promenna tridy od promenne z parametru
    this->celkovaSpotreba = celkovaSpotreba; // priradi celkovou spotrebu z parametru
    this->aktualSpotreba = aktualniSpotreba; // priradi aktualni spotrebu z parametru

    cout << "G" << this->cislo << "[" << this->celkovaSpotreba << "][" << this->aktualSpotreba << "]" << endl; // vypise info o vygenerovani
}

unsigned int Elektromer::Cislo() // funkce vrati cislo elektromeru
{
    return cislo; 
}

void Elektromer::ZvysitSpotrebu()  // funkce zvysi aktualni spotrebu
{
    cout << "Z" << cislo << "[" << aktualSpotreba << "]" << endl; // vypise se info před zvýšení spotřeby
    ++aktualSpotreba; // zvýší se aktuální spotřeba o 1


    celkovaSpotreba += aktualSpotreba / 60; // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)
}

void Elektromer::SnizitSpotrebu()
{
    cout << "N" << cislo << "[" << aktualSpotreba << "]" << endl; // vypise se info před snizenim spotřeby
    if (aktualSpotreba > 0) // spotreba lze snizit pouze, pokud je vetsi nez 0, pokud je nulova uz snizit nelze (=nelze mit zapornou spotrebu)
        --aktualSpotreba; // snizi se aktualni spotreba o 1
    celkovaSpotreba += aktualSpotreba / 60; // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)
}

void Elektromer::PonechatSpotrebu() //nedela nic s aktualni spotrebo
{
    celkovaSpotreba += aktualSpotreba / 60; // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)

}

unsigned int Elektromer::CelkovaSpotreba() // vrati celkovou spotrebu elektromeru
{
    return celkovaSpotreba;
}

class Seznam{
    public:
        Seznam(); 
        ~Seznam();
        void Vlozit(Elektromer *elektromer); // vlozi elektromer z parametru do seznamu
        void IteratorZacatek();  // nastavi iterator na 1. prvek v seznamu
        void IteratorDalsi(); // iterator presune na dalsi prvek v seznamu
        bool IteratorJeKonec(); // vrati true, jestlize je iterator na konci seznamu
        Elektromer *VratElektromer();// vrati elektromer, na ktery ukazuje iterator
    private:
        struct polozkaSez{ // struktru pro polozku seznamu
            Elektromer *elektromer; // ukazatel na elektromer
            polozkaSez *dalsi; // ukazatel na dalsi polozku seznamu
        };
        polozkaSez *hlava; // ukazatel na hlavu seznamu (=zacatek seznamu)
        polozkaSez *iterator; // iterat - coz je ukazatel na polozku (muze byt libovolna) v seznamu
};

Seznam::~Seznam() // destruktor
{
    // dealokace elektromeru v seznamu
    for (IteratorZacatek(); !IteratorJeKonec(); IteratorDalsi()) // projdi cely seznam
        delete iterator->elektromer; // a dealokuje vsechny Elektromery v seznamu
	
    // dealokace samotnych prvku seznamu
    while (hlava != NULL) { // dokud na zacatku seznamu je nejaka polozka
        iterator = hlava; // nastav iterator na tuto polozku
        hlava = hlava->dalsi; // zacatek seznamu nastav na dalsi polozku (nebo v pripade ze uz zadna neni na NULL)
        delete iterator; // smaz tuhle polozku
    }

}

void Seznam::IteratorZacatek() // nastavi ukazatel iterator na 1. polozku v seznamu
{
    iterator = hlava; // na 1. polozku v seznamu ukazuje vzdy "hlava", takze se iterator nastavuje na hlavu
}

void Seznam::IteratorDalsi() // nastavi ukazatel na dalsi polozku v seznamu
{
    iterator = iterator->dalsi; // posune iterator dal v seznamu
}

bool Seznam::IteratorJeKonec() // zjisti, zda iterator uz je za koncem seznamu
{
    if (iterator) // pokud iterator neni NULL, znamena to, ze nejsem na konci seznamu
        return false;
    else // jinak jsem na konci seznamu
        return true;
}

Elektromer* Seznam::VratElektromer() // vrati elektromer, na ktery ukazuje zrovna iterator
{
    return iterator->elektromer;
}

Seznam::Seznam()  // konstruktor Seznamu
{
    hlava = NULL; // seznam je ze zacatku prazdny, tak se jeho hlava nastavi na NULL
}

void Seznam::Vlozit(Elektromer *e) // vlozi elektromer z parametru do seznamu
{
    polozkaSez *item = new polozkaSez; // vytvori novou polozku seznamu (struktura)

    item->dalsi = hlava; /* ukazatel dasli nastavi teto polozce na hlavu seznamu
         (to znamena, ze pokud v seznamu neco je, tak ukazatel dalsi bude ukazovat na 1. polozku v seznamu) 
            pokud vsak v seznamu nic neni, bude ukazatel dalsi ukazovat na NULL
         */
    item->elektromer = e; // do nove vytvorene polozky seznamu se priradi elekromer z parametru funkce
    hlava = item; // hlava seznamu se nastavi na nove vlozenou polozku (polozka tedy byla vlozena na zacatek seznamu)
}

int main(void)
{
    srand(time(NULL)); // inicializace generatoru nahodnych cisel
    Seznam seznam; // vytvoreni seznamu

    for (unsigned int cislo = 1; cislo <= N; ++cislo) {// cyklus jde od 1 do N - probiha teda N-krát (vlozi se behem nej tedy do seznamu N elektromeru)
	unsigned int celkovaSpotreba = rand() % (H - 1) + 1; // celkova spotreba bude cislo od 1 do H-1
    	unsigned int aktualSpotreba = rand() % (A - 1) + 1; // akktualni spotreba bude cislo od 1 do A-1
        Elektromer *e = new Elektromer(cislo, celkovaSpotreba, aktualSpotreba); // vytvorí se novy elektromer (priradi se mu cislo, celkova spotreba a aktualni spotreba)
        seznam.Vlozit(e); // elektromer se vlozi do seznamu
    }
    for (unsigned int i = 1; i <= M; ++i) {// cyklus probiha M-krát, tedy pro M časových jednotek
        for (seznam.IteratorZacatek(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi()) { // projdi celý seznam
            Elektromer *e = seznam.VratElektromer(); // do promenne e uloz elektromer, na ktery v teto iteraci cyklu ukazuje iterator
            int operace = rand() % 3; // do operace prirad nahodne cislo od 0 do 2
            if (operace == 0)  // pokud byla nahodne vygenerovana 0
                e->ZvysitSpotrebu(); // zvetsi spotrebu elektromeru
            else if (operace == 1) // pokud byla nahodne vygenerova 1
                e->SnizitSpotrebu(); // sniz spotrebu elektromeru
            else // pokud byla nahodne vygenerovana 2
                e->PonechatSpotrebu(); // ponech spotrebu elektromer
        }
    }
    for (seznam.IteratorZacatek(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi()) { // projdi cely seznam
        Elektromer *e = seznam.VratElektromer(); // do promenne e uloz elektromer, na ktery v teto iteraci cyklu ukazuje iterator
        cout << "S" << e->Cislo() << "[" << e->CelkovaSpotreba() << "]" << endl; // vypis celkovou spotrebu tohoto elektromeru
    }

    getchar(); // ceka na stisk ENTER
    return 0; // pote ukonci program
}
