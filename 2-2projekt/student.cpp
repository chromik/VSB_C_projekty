#include <iostream>

#include "student.h"

using namespace std;

Student::Student(int cislo, int pocet_znamek) // kostruktor studenta s parametry
{
	this->cislo = cislo; // studentovi se priradi cislo z parametru
	this->pocet_znamek = pocet_znamek;
	this->znamky = new int[this->pocet_znamek]; // vytvori se pole pro ulozeni znamek o velikosti, ktera je dana parametrem pocet_znamek
	this->aktualni_pocet_znamek = 0; /// aktualni pocet znamek se nastavi na 0, protoze objekt student pri vytvoreni zadne znamky nema
}

Student::~Student() // destruktor studenta
{
	delete [] this->znamky; // smaze se dynamicky alokovane pole znamek
}

int Student::Cislo() // vrati cislo studenta
{
	return this->cislo;
}

void Student::VlozitZnamku(int znamka) // vlozi studentovi znamku z parametru
{	
	this->znamky[this->aktualni_pocet_znamek++] = znamka; /* znamka se ulozi do pole,
		vlozeni prvni znamky se ulozi na index 0, druhe na index 1, treti na index 2, atd.. 
		Protoze pole indexuji pomoci promenne "aktualni_pocet_znamek" ktera je pred vlozenim 
		prvni znamky rovna 0 a po vlozeni znamky se inkrementuje o jednicku
		*/
}

float Student::PrumernyProspech() //spocita prumerny prospech studenta
{
	float suma = 0; // vytvori se promenna, ktera se nastavi na 0
	for (int i = 0; i < this->pocet_znamek; ++i) // cyklus jde tolikrat, kolik ma student znamek
	{
		suma += this->znamky[i]; // k promenne "suma" se pricte "i"-tá známka
	} // takze po skonceni cyklu je v "suma" ulozen soucet vsech znamek
	return suma / this->pocet_znamek; // tento soucet se vydeli počtem známek, tak se získá průměrná známka a ta se vrátí
}
