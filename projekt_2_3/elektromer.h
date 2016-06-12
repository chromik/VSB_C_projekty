#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;


extern const int A;
extern const int H;
extern const int M;
extern const int N;

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
    this->celkovaSpotreba = rand() % H;
    this->aktualSpotreba = rand() % A;
    this->cislo = cislo;

    cout << "G" << this->cislo << "[" << this->celkovaSpotreba << "][" << this->aktualSpotreba << "]" << endl;
}

unsigned int Elektromer::Cislo()
{
    return this->cislo;
}

void Elektromer::ZvysitSpotrebu()
{
    cout << "Z" << this->cislo << "[" << this->aktualSpotreba << "]" << endl;
    ++this->aktualSpotreba;


    celkovaSpotreba += (aktualSpotreba / 60);
}

void Elektromer::SnizitSpotrebu()
{
    cout << "N" << this->cislo << "[" << this->aktualSpotreba << "]" << endl;
    
    if (aktualSpotreba > 0)
        --this->aktualSpotreba;

    celkovaSpotreba += (aktualSpotreba / 60);
}

void Elektromer::PonechatSpotrebu()
{
    celkovaSpotreba += (aktualSpotreba / 60);

}

unsigned int Elektromer::CelkovaSpotreba()
{
    return celkovaSpotreba;
}