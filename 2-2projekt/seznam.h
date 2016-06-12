#ifndef _SEZNAM_H_
#define _SEZNAM_H_

class Seznam{
	public:
		Seznam(); // konstruktor
		~Seznam(); // destruktor
		
		void Pridat(Student *student); // prida studenta do seznamu
		void VlozeniZnamek(); // vlozi kazdemu studentovi v seznamu znaku
		void VypsatPrumernyProspech(); // vypise prumerny prospech studentu
	private:
		struct SeznamPolozka{ //definice struktury prestavujici jednu polozku v seznamu
			Student *student; // struktura obsahuje ukazatel na studenta
			SeznamPolozka *dalsi; //a ukazatel na dalsi strukturu stejneho typu (dalsi polozka v seznamu)
		};

		SeznamPolozka *hlava; // ukazatel na zacatek seznamu (ukazatel na prvni polozku seznamu)
		SeznamPolozka *iterator; /* ukazatel na libovolnou polozku v seznamu, na kterou polozku bude
		 ukazovat lze nastavit pomocí metod  IteratorReset a IteratorDalsi;
		*/


		/* nasledujici tri metody jsou PRIVATE, protoze k nim pristupu pouze
			ze Seznamu (z venku se k nim v tomto programu nikde nepristupuje)

		Samozrejme by mohli byt i PUBLIC a na programu by to nic nezmenilo... */
		void IteratorReset(); // nastavi ukazatel iterator na zacatek seznamu
		void IteratorDalsi(); // nastavi ukazatel iterator na dalsi polozku v seznamu
		bool IteratorJeKonec(); // zjisti zda iterator jiz neni mimo seznam
};

#endif
