#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int A = 10; // maximalni vychozi hodnota aktualni spotreby
const int H = 100; // maximalni vychozi hodnota celkove spotreby
const int M = 1440; // pocet casovych jednotek pro ktere ma probihat simulace
const int N = 20; // pocet elektromeru

class Elektromer{
    public:
        Elektromer(int cislo);
        unsigned int Cislo();
        void ZvysitSpotrebu();
        void SnizitSpotrebu();
        void PonechatSpotrebu();
        unsigned int CelkovaSpotreba();

    private:
        unsigned int celkovaSpotreba; 
        unsigned int aktualSpotreba;
        unsigned int cislo;  
};

Elektromer::Elektromer(int cislo)
{
    this->celkovaSpotreba = rand() % (H - 1) + 1; // celkova spotreba bude cislo od 1 do H-1
    this->aktualSpotreba = rand() % (A - 1) + 1; // akktualni spotreba bude cislo od 1 do A-1
    this->cislo = cislo; // cislo elektromeru se priradi z parametru funkce (pro 1. to bude 1, pro druhy 2, atd...)

    cout << "G" << this->cislo << "[" << this->celkovaSpotreba << "][" << this->aktualSpotreba << "]" << endl; // vypise info o vygenerovani
}

unsigned int Elektromer::Cislo() // funkce vrati cislo elektromeru
{
    return this->cislo; 
}

void Elektromer::ZvysitSpotrebu()  // funkce zvysi aktualni spotrebu
{
    cout << "Z" << this->cislo << "[" << this->aktualSpotreba << "]" << endl; // vypise se info před zvýšení spotřeby
    ++this->aktualSpotreba; // zvýší se aktuální spotřeba o 1


    celkovaSpotreba += (aktualSpotreba/60); // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)
}

void Elektromer::SnizitSpotrebu()
{
    cout << "N" << this->cislo << "[" << this->aktualSpotreba << "]" << endl; // vypise se info před snizenim spotřeby
    
    if (aktualSpotreba > 0) // spotreba lze snizit pouze, pokud je vetsi nez 0, pokud je nulova uz snizit nelze (=nelze mit zapornou spotrebu)
        --this->aktualSpotreba; // snizi se aktualni spotreba o 1

    celkovaSpotreba += (aktualSpotreba/60); // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)
}

void Elektromer::PonechatSpotrebu() //nedela nic s aktualni spotrebo
{
    celkovaSpotreba += (aktualSpotreba/60); // celkova spotreba se zvysi o aktualni spotrebu delenou 60ti (protoze aktualni spotreba je v kWhodinach a simulace jde po minutach)

}

unsigned int Elektromer::CelkovaSpotreba() // vrati celkovou spotrebu elektromeru
{
    return celkovaSpotreba;
}

class Seznam{
    public:
        Seznam(); 
        ~Seznam();
        void Vlozit(Elektromer *elektromer);

        void IteratorReset();  // nastavi iterator na 1. prvek v seznamu
        void IteratorDalsi(); // iterator presune na dalsi prvek v seznamu
        bool IteratorJeKonec(); // vrati true, jestlize je iterator na konci seznamu
        Elektromer *VratElektromer();
    private:
        struct SeznamItem{ // struktru pro polozku seznamu
            Elektromer *elektromer; // ukazatel na elektromer
            SeznamItem *dalsi; // ukazatel na dalsi polozku seznamu
        };
        SeznamItem *hlava; // ukazatel na hlavu seznamu (=zacatek seznamu)
        SeznamItem *iterator; // iterat - coz je ukazatel na polozku (muze byt libovolna) v seznamu
};

Seznam::~Seznam()
{
    for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi()) // projdi cely seznam
    {
        delete iterator->elektromer; // a dealokuje vsechny Elektromery v seznamu
    }

    while (hlava != NULL) // dokud na zacatku seznamu je nejaka polozka
    {
        iterator = hlava; // nastav iterator na tuto polozku
        hlava = hlava->dalsi; // zacatek seznamu nastav na dalsi polozku (nebo v pripade ze uz zadna neni na NULL)
        delete iterator; // smaz tuhle polozku
    }

}

void Seznam::IteratorReset() // nastavi ukazatel iterator na 1. polozku v seznamu
{
    iterator = hlava; // na 1. polozku v seznamu ukazuje vzdy "hlava", takze se iterator nastavuje na hlavu
}

void Seznam::IteratorDalsi() // nastavi ukazatel na dalsi polozku v seznamu (pokud jsem byl na 1. polozce, prejde se na 2... pokud jsem byl na 2., tak na treti, atd....)
{
    iterator = iterator->dalsi;
}

bool Seznam::IteratorJeKonec() //zjisti, zda iterator uz je za koncem seznamu
{
    if (iterator == NULL) // pokud je iterator NULL, znamena to, ze uz jsem prosel cely seznam a nyni jsem za nim
        return true; // pokud jsem tedy prosel cely seznam, vrati se TRUE
    else
        return false; // jinak se vrati FALSE a to znamena, ze iterator jeste ukazuje na nejaky prvek v seznamu
}

Elektromer* Seznam::VratElektromer() // vrati elektromer, na ktery ukazuje zrovna iterator
{
    return iterator->elektromer; // vrati se elektromer z polozky seznamu, na kterou ukazuje iterator
}

Seznam::Seznam()  // konstruktor Seznamu
{
    hlava = NULL; // seznam je ze zacatku prazdny, tak se jeho hlava nastavi na NULL
}

void Seznam::Vlozit(Elektromer *elektromer) // vlozi elektromer do seznamu
{
    SeznamItem *item = new SeznamItem; // vytvori novou polozku seznamu

    item->dalsi = this->hlava; /* ukazatel dasli nastavi teto polozce na hlavu seznamu
         (to znamena, ze pokud v seznamu neco je, tak ukazatel dalsi bude ukazovat na 1. polozku v seznamu) 
            pokud vsak v seznamu nic neni, bude ukazatel dalsi ukazovat na NULL
         */
    item->elektromer = elektromer; // do nove vytvorene polozky seznamu se priradi elekromer z parametru funkce
    this->hlava = item; // hlava seznamu se nastavi na nove vlozenou polozku (polozka tedy byla vlozena na zacatek seznamu)
}

int main(void)
{
    srand (time(NULL)); // inicializace generatoru nahodnych cisel
    Seznam seznam; // vytvoreni seznamu

    for (unsigned int i = 1; i <= N; ++i) // cyklus jde od 1 do N - probiha teda N-krát (vlozi se behem nej tedy do seznamu N elektromeru)
    {
        Elektromer *elektromer = new Elektromer(i); //vytvorí se novy elektromer (priradi se mu cislo i - pro prvni elektromer 1, pro druhý 2, ...)
        seznam.Vlozit(elektromer); // elektromer se vlozi do seznamu
    }


    for (unsigned int i = 1; i <= M; ++i) // cyklus probiha M-krát, tedy pro M časových jednotek
    {
        for (seznam.IteratorReset(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi()) // projdi celý seznam
        {
            Elektromer *elektromer = seznam.VratElektromer(); // do promenne elektromer uloz elektromer, na ktery v teto iteraci cyklu ukazuje iterator
            int operace = rand() % 3; // do operace prirad nahodne cislo od 0 do 2

            if (operace == 0)  // pokud byla nahodne vygenerovana 0
                elektromer->ZvysitSpotrebu(); // zvetsi spotrebu elektromeru
            else if (operace == 1) // pokud byla nahodne vygenerova 1
                elektromer->SnizitSpotrebu(); // sniz spotrebu elektromeru
            else if (operace == 2) // pokud byla nahodne vygenerovana 2
                elektromer->PonechatSpotrebu(); // ponech spotrebu elektromeru
        }
    }


    for (seznam.IteratorReset(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi()) // projdi cely seznam
    {
        Elektromer *elektromer = seznam.VratElektromer(); // do promenne elektromer uloz elektromer, na ktery v teto iteraci cyklu ukazuje iterator
        cout << "S" << elektromer->Cislo() << "[" << elektromer->CelkovaSpotreba() << "]" << endl; // vypis celkovou spotrebu tohoto elektromeru
    }


    return 0;
}
