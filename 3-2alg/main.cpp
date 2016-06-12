#include <iostream>

using namespace std; 

const int N = 3; // počet proměnných 


#include "VariableTable.h"
#include "Nodes.h"

int main(void)
{
	VariableTable *table = new VariableTable(N); // vytvoreni tabulky promennych
	
	const char *root1_task = "(A and B) or (B and (not C))";
	Node* root1 = new Or(
		new And(
			new Variable(0, table ), // A
			new Variable(1, table ) // B
		),
		new And(
			new Variable(1, table ), // B
			new Not(
				new Variable(2, table ) // C
			)
		)
	);

	const char *root2_task = "(A xor B) and (not C)";
	Node *root2 = new And(
		new Xor(
			new Variable(0, table ), // A
			new Variable(1, table ) // B
		),
		new Not(
			new Variable(2, table ) // C
		)
	);


	const char *root3_task = "A and (B or (not C))";
	Node *root3 = new And(
		new Variable(0, table ), //A
		new Or(
			new Variable(1, table), //B
			new Not(
				new Variable(2, table) //C
			)
		)
	);

	const char *root4_task = "A or (B and (not C))";
	Node *root4 = new Or(
		new Variable(0, table ), //A
		new And(
			new Variable(1, table), //B
			new Not(
				new Variable(2, table) //C
			)
		)
	);
	
	table->SetVariable (0 , false); 
	table->SetVariable (1 , false); 
	table->SetVariable (2 , false); 
	cout << "A = 0, B = 0, C = 0" << endl;
	cout << root1_task << " = " << root1->Evaluate() << endl; // vypise vzorec 1 a jeho vysledek
	cout << root2_task << " = " << root2->Evaluate() << endl; // vypise vzorec 2 a jeho vysledek
	cout << root3_task << " = " << root3->Evaluate() << endl; // vypise vzorec 3 a jeho vysledek
	cout << root4_task << " = " << root4->Evaluate() << endl << endl; // vyspise vzorec 4 a jeho vysledek


	table->SetVariable (0 , false); 
	table->SetVariable (1 , false); 
	table->SetVariable (2 , true); 
	cout << "A = 0, B = 0, C = 1" << endl;
	cout << root1_task << " = " << root1->Evaluate() << endl; // vypise vzorec 1 a jeho vysledek (uz s novymi hodnotami)
	cout << root2_task << " = " << root2->Evaluate() << endl; // vypise vzorec 2 a jeho vysledek..............
	cout << root3_task << " = " << root3->Evaluate() << endl; // ...... ard atd...
	cout << root4_task << " = " << root4->Evaluate() << endl << endl;

	
	table->SetVariable (0 , true); 
	table->SetVariable (1 , true); 
	table->SetVariable (2 , false); 
	cout << "A = 1, B = 1, C = 0" << endl;
	cout << root1_task << " = " << root1->Evaluate() << endl;
	cout << root2_task << " = " << root2->Evaluate() << endl;
	cout << root3_task << " = " << root3->Evaluate() << endl;
	cout << root4_task << " = " << root4->Evaluate() << endl << endl;


	delete root1; // smaze se strom pro 1. vzorec
	delete root2; // ----||----- pro 2. 
	delete root3; // ____|| ------ 3.
	delete root4;

	delete table; // smaze se tabulka promennych

	return 0;
}