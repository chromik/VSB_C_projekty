class VariableTable{
	public:
		VariableTable(const int N); // konstruktor s parametrem pro velikost pole
		~VariableTable(); // destruktor
		void SetVariable (const int IndexOfVariable , const bool NewValue); // nnastavi hodnotu prvku na danem indexu (=nastavi hodnotu promenne)
		bool GetVariableValue(const int IndexOfVariable); // vrati hodnotu promenne s danym indexem
	private:
		bool *variables; 
};

VariableTable::VariableTable(const int N) // konstruktor
{
	variables = new bool[N]; // dynamicky se alokuje pole o velikosti N (z parametru)
}

VariableTable::~VariableTable() // destruktor
{
	delete [] variables; // dealokuje se dynamicky vytvorene pole
}

void VariableTable::SetVariable (const int IndexOfVariable , const bool NewValue) // nastavi se promenna na danem indexu (hodnota danne promenne)
{
	variables[IndexOfVariable] = NewValue;
}

bool VariableTable::GetVariableValue(const int IndexOfVariable) // vrati se hodnota danne promenne 
{
	return variables[IndexOfVariable];
}