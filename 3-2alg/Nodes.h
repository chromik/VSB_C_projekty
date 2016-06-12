class Node{ // abstraktni trida - slouzi jen jako predek pro dalsi tridy. Nelze vytvorit instanci (=objekt) této třídy, pouze ukazatel
	public:
		virtual bool Evaluate() = 0; // virtualni metoda, ktera je nedefinovaná (každý potomek třídy si jí definuje jinak)
		virtual ~Node() {}; // virtualni prázdný destruktor, musi zde byt, protoze se vola pred ukoncenim programu destruktor na koren stromu, což je ukazatel na Node
	private:
		Node *left;
		Node *right;
};


class And : public Node{ // třída And, která dědí z Node
	public:
		And(Node *left, Node *right); // konstruktor s pamrametry, pomoci parametru se nastavuje levy a pravy potomek (což může být další logická operace nebo přímo proměnná)
		virtual ~And(); // destruktor
		virtual bool Evaluate(); // metoda, která vyhodnotí výsledek
	private:
		Node *left; // ukazatel na levy potomek
		Node *right; // ukazatel na pravy potomek
};

And::And(Node *left, Node *right) // konstruktor s parametry
{
	this->left = left; // ukazatel na levy potomek se nastavi z parametru metody
	this->right = right; // ukazatel na pravy potomek se nastavi z parametru metody
}

And::~And() // destruktor
{
	delete left; // smaze levý potomek
	delete right; // smaze pravy potomek
}

bool And::Evaluate() // vyhodnoti vyraz
{
	return left->Evaluate() && right->Evaluate();  // vyhodnoti leveho potomka a da jej do ANDu s pravym potomkem (rekurze)
}


// OR je úplně to samé jako AND, akorát místo && je tu ||
class Or : public Node{
	public:
		Or(Node *left, Node *right);
		virtual ~Or();
		virtual bool Evaluate();
	private:
		Node *left;
		Node *right;
};

Or::Or(Node *left, Node *right)
{
	this->left = left;
	this->right = right;
}

Or::~Or()
{
	delete left;
	delete right;
}


bool Or::Evaluate()
{
	return left->Evaluate() || right->Evaluate();
}


// Xor to samé jako And a OR, akorát je tu != což je logický XOR
class Xor : public Node{
	public:
		Xor(Node *left, Node *right);
		virtual ~Xor();
		virtual bool Evaluate();
	private:
		Node *left;
		Node *right;
};

Xor::Xor(Node *left, Node *right)
{
	this->left = left;
	this->right = right;
}

Xor::~Xor()
{
	delete left;
	delete right;
}

bool Xor::Evaluate()
{
	return left->Evaluate() != right->Evaluate();
}


// taktéž to samé, ale už má jen jednoho potomka, takže má ukazatel jen next (ne left a right)
class Not : public Node{
	public:
		Not(Node *next);
		virtual ~Not();
		virtual bool Evaluate();
	private:
		Node *next;
};

Not::Not(Node *next)
{
	this->next = next;
}

Not::~Not()
{
	delete next;
}

bool Not::Evaluate()
{
	return !next->Evaluate();
}

// To same, akorat bez potomku a pri vyhodnoceni vrati hodnotu uzlu
class Variable : public Node{
	public:
		Variable(int index, VariableTable *table);
		virtual bool Evaluate();
	private:
		int index; // index (promenna) uzlu.. pokud je index 0, tak je promenna A, pokud 1 tak B, pokud 2 tak C....
		VariableTable *table;
};

Variable::Variable(int index, VariableTable *table) //konstruktor
{
	this->index = index; //nastavi se index z parametru
	this->table = table; // nastavi se ukazatel na tabulku z parametru
}

bool Variable::Evaluate()
{
	return table->GetVariableValue(index); // vrati hodnotu uzlu
}