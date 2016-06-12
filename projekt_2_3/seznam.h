#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

extern const int A;
extern const int H;
extern const int M;
extern const int N;

class Seznam{
    public:
        Seznam(); 
        ~Seznam();
        void Vlozit(Elektromer *elektromer);

        void IteratorReset();
        void IteratorDalsi();
        bool IteratorJeKonec();
        Elektromer *VratElektromer();
    private:
        struct SeznamItem{
            Elektromer *elektromer;
            SeznamItem *dalsi;
        };
        SeznamItem *hlava;
        SeznamItem *iterator;
};



void Seznam::IteratorReset()
{
    iterator = hlava;
}

void Seznam::IteratorDalsi()
{
    iterator = iterator->dalsi;
}

bool Seznam::IteratorJeKonec()
{
    if (iterator == NULL)
        return true;
    else
        return false;
}

Elektromer* Seznam::VratElektromer()
{
    return iterator->elektromer;
}

Seznam::Seznam()
{
    hlava = NULL;
}

void Seznam::Vlozit(Elektromer *elektromer)
{
    SeznamItem *item = new SeznamItem;

    item->dalsi = this->hlava;
    item->elektromer = elektromer;
    this->hlava = item;
}

Seznam::~Seznam()
{
    for (this->IteratorReset(); !this->IteratorJeKonec(); this->IteratorDalsi())
    {
        delete iterator->elektromer; // Dealokuje vsechny Elektromery v seznamu
    }

    while (hlava != NULL)
    {
        iterator = hlava;
        hlava = hlava->dalsi;
        delete iterator;
    }

}