#include <iostream>
#include <ctime>
#include <cstdlib>

#include "elektromer.h"
#include "seznam.h"

using namespace std;

const int A = 10; // 
const int H = 100;
const int M = 1440;
const int N = 20; // pocet elektromeru //paendro


int main(void)
{
    srand (time(NULL));
    Seznam seznam;

    for (unsigned int i = 1; i <= N; ++i)
    {
        Elektromer *elektromer = new Elektromer(i);
        seznam.Vlozit(elektromer);
    }


    for (unsigned int i = 1; i <= M; ++i)
    {
        for (seznam.IteratorReset(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi())
        {
            Elektromer *elektromer = seznam.VratElektromer();
            int operace = rand() % 3;

            if (operace == 0) 
                elektromer->ZvysitSpotrebu();
            else if (operace == 1)
                elektromer->SnizitSpotrebu();
            else if (operace == 2)
                elektromer->PonechatSpotrebu();
        }
    }


    for (seznam.IteratorReset(); !seznam.IteratorJeKonec(); seznam.IteratorDalsi())
    {
        Elektromer *elektromer = seznam.VratElektromer();
        cout << "S" << elektromer->Cislo() << "[" << elektromer->CelkovaSpotreba() << "]" << endl;
    }


    return 0;
}
