#ifndef _STUDENT_H_
#define _STUDENT_H_

class Student{
	public:
		Student(int cislo, int pocet_znamek); // konstruktor, parametry jsou cislo studenta a pocet znamek
		~Student(); // destruktor
		int Cislo(); // vraci cislo studenta
		void VlozitZnamku(int znamka); // vlozi studentovi znamku z parametru
		float PrumernyProspech(); // vrati prumerny prospech studenta
	private:
		int cislo; // cislo studenta

		int pocet_znamek;
		int aktualni_pocet_znamek; // pocet znamek, ktere jsou u studenta ulozene v poli
		int *znamky; // promenna typu ukazatel na int, pozdeji se do ni priradi dynamicky alokovane pole pro ulozeni znamek
};
#endif