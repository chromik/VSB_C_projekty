#include <iostream>	
#include <cstdlib> // kvuli funkci rand()
#include <ctime> // kvuli inicializaci generatoru nahodnych cisel

using namespace std;

/* Globalni konstanty */
const int N = 50; // pocet zaku
const int Z = 100; // pocet znamek u jednoho zaka

/* Include hlavickovych souboru */
#include "student.h" 
#include "seznam.h"


int main(void)
{
	srand((unsigned)time(NULL)); // inicializace generatoru nahodnych cisel (pouziva se, aby rand() negenerovalo stale stejna cisla)
	Seznam seznam; // vytvoreni objektu seznam

	for (int i = N; i > 0; --i) // for cyklus od 1 do N (=generuje se N studentu)
	{
		Student *student = new Student(i, Z); // studentovi se priradi cislo (i) a pocet znamek (Z)
		cout << "G" << student->Cislo() << "[" << Z << "]" << endl; // vypise se Info o vygenerovani, cislo studenta a pocet znamek
		seznam.Pridat(student); // student se prida do seznamu
	}


	for (int i = 0; i < Z; ++i) // for cyklus jde Z krát (přidává se Z známek každému studentovi)
	{
		seznam.VlozeniZnamek(); // nad seznamem se zavola metoda VlozeniZnamek(), ktera kazdemu studentovi v seznamu vlozi jednu znamku

	}

	seznam.VypsatPrumernyProspech(); /* nad seznamem se zavola metoda VypsatPrumernyProspech(), která vypíše prumerny prospech
		kazdeho zaka ze seznamu*/
	char end;
	cin >> end;
	return 0;
}