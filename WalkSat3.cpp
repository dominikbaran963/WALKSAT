// Walksat poprawa.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <Windows.h>
using namespace std;
fstream file;
struct variable {
	string name;
	bool value;
	bool negation = false;
};

struct implicentVariable {
	int id;
	bool negation = false;
};
int t;
int iterations = 1;

int tab[50], xxx[50];
void generateImplicents(int n, int m, int k_min, int k_max, vector<vector<implicentVariable> >& implicents, vector<variable>& variables);
void showImplicents(vector<vector<implicentVariable> > implicents, vector<variable>  variables, ofstream& file);
void walkSAT(vector<vector<implicentVariable> >& implicents, vector<variable>& variables, ofstream& file);
void showResults(vector<variable>& variables, ofstream& file);
int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "pl");
	for (;;) {
		cout << "Algorytm WALKSAT dla problemu spełnialnośći (SAT)" << endl;
		cout << "Wybierz opcję\n1.Wygeneruj funkcję\n2.Wyjdź: "; int opcja; cin >> opcja;
		switch (opcja) {
		case 1: {
			int k_max;
			iterations = 1;
			ofstream file;
			file.open("zapis.txt", ios::out);
			cout << "Podaj liczbe zmiennych n =";
			int n; cin >> n;
			cout << "Podaj liczbe implicentow m= ";
			int m; cin >> m;
			cout << "Podaj minimalny rozmiar implicentow min= ";
			int k_min; cin >> k_min;
			cout << "Podaj maksymalny rozmiar implicentow max= ";
			cin >> k_max;
			cout << "Podaj ilość iteracji t= "; cin >> t;
			for (int i = 0; i < 50; i++) {
				tab[i] = 0;
			}
			vector <vector<implicentVariable> > implicents;
			vector<variable> variables;
			generateImplicents(n, m, k_min, k_max, implicents, variables);
			showImplicents(implicents, variables, file);
			walkSAT(implicents, variables, file);
			showResults(variables, file);
			getchar(); getchar(); system("cls");
			break;
		}
		case 2: {
			return 0;
		}
		}
	}
}



void generateImplicents(int n, int m, int k_min, int k_max, vector<vector<implicentVariable> > &implicents, vector<variable> &variables)
{

	vector<implicentVariable> temp;

	int x, size, varia;


	//give varialbes values true(1) or false(0)
	for (int i = 0; i < n; i++) {
		x = rand() % 2;

		variable tempvar;
		tempvar.name = "x" + to_string(i + 1);
		tempvar.value = x;
		tempvar.negation = false;

		variables.push_back(tempvar);
	}


	//make implicents
	for (int i = 0; i < m; i++) {
		temp.clear();
		size = rand() % (k_max + 1 - k_min) + k_min;	//number of variables in implicent
		for (int j = 0; j < size; j++) {
			varia = rand() % variables.size();
			bool neg = rand() % 2;
			implicentVariable tempimvar;
			if (j == 0) {
				tempimvar.id = varia;
				tempimvar.negation = neg;
				// dopierdol for z if i gitara!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 j++
				temp.push_back(tempimvar);
			}
			else {
				tempimvar.id = varia;
				tempimvar.negation = neg;
				int licznik = 0;
				for (int k = 0; k < j; k++) {
					if (temp[k].id == varia) {
						licznik++;
					}
				}
				if (licznik == 0) {
					temp.push_back(tempimvar);

				}
				else {
					j--;
				}
			}
		}

		implicents.push_back(temp);

	}

}





void showImplicents(vector<vector<implicentVariable> > implicents, vector<variable>  variables, ofstream &file)
{
	cout << endl << "Wygenerowana funkcja:" << endl << endl;
	cout << "f = ";
	file << endl << "Wygenerowana funkcja:" << endl << endl;
	file << "f = ";
	for (size_t i = 0; i < implicents.size(); i++) {
		cout << "(";
		file << "(";
		for (size_t j = 0; j < implicents[i].size(); j++) {

			if (implicents[i][j].negation) {
				cout << '~';
				file << '~';
			}
			cout << variables[implicents[i][j].id].name;
			file << variables[implicents[i][j].id].name;
			if (j < (implicents[i].size() - 1)) {
				cout << " | ";
				file << " | ";
			}
		}
		cout << ")";
		file << ")";
		if (i < (implicents.size() - 1)) {
			cout << " & ";
			file << " & ";
		}
	}

	cout << endl << endl << "Legenda:" << endl;
	cout << "|   operator alternatywy (OR)" << endl;
	cout << "&   operator koniunkcji (AND)" << endl;
	cout << "~   operator negacji (NEG)" << endl;

	file << endl << endl << "Legenda:" << endl;
	file << "|   operator alternatywy (OR)" << endl;
	file << "&   operator koniunkcji (AND)" << endl;
	file << "~   operator negacji (NEG)" << endl;




	cout << endl << "Wygenerowane wartosci zmiennych startowych:" << endl;
	file << "Wygenerowane wartosci zmiennych startowych :" << endl;
	for (size_t i = 0; i < variables.size(); i++) {
		cout << variables[i].name << " = " << variables[i].value << endl;
		file << variables[i].name << " = " << variables[i].value << endl;
	}
}




void walkSAT(vector<vector<implicentVariable> > &implicents, vector<variable>  &variables, ofstream &file)
{
	int xx;		int pomoc = 0;
	vector<vector<implicentVariable> > negativeImplicents;
	negativeImplicents.clear();
	bool impltrue;
	for (size_t i = 0; i < implicents.size(); i++) {
		impltrue = false;
		for (size_t j = 0; j < implicents[i].size(); j++) {

			bool val;
			if (implicents[i][j].negation)
				val = !(variables[implicents[i][j].id].value);
			else
				val = (variables[implicents[i][j].id].value);


			if (val) {
				impltrue = true;
				break;
			}

		}
		if (!impltrue) {
			negativeImplicents.push_back(implicents[i]);
			xxx[pomoc] = i;
			pomoc++;
		}

	}
	if (negativeImplicents.size() > 0) {
		int x = rand() % negativeImplicents.size();
		int y = rand() % negativeImplicents[x].size();
		variables[negativeImplicents[x][y].id].value = !(variables[negativeImplicents[x][y].id].value);
		variables[negativeImplicents[x][y].id].negation = !(variables[negativeImplicents[x][y].id].negation);
		file << "Nowe zmienne:" << endl;
		cout << "Nowe zmienne: " << endl;
		cout << "Została zanegowana zmienna numer : " << variables[negativeImplicents[x][y].id].name << ", została ona wylosowana z implicentu numer: " << xxx[x] + 1 << ", którego wartość była równa 0 " << endl;
		file << "Została zanegowana zmienna numer : " << variables[negativeImplicents[x][y].id].name << ", została ona wylosowana z implicentu numer: " << xxx[x] + 1 << ", którego wartość była równa 0 " << endl;
		for (size_t i = 0; i < variables.size(); i++) {
			cout << variables[i].name << " = " << variables[i].value << endl;
			file << variables[i].name << " = " << variables[i].value << endl;
		}
		iterations++;
		if (iterations <= t)
			walkSAT(implicents, variables, file);
		else {
			cout << "Przekroczono dozwolona liczbe " << t << " iteracji. Nie znaleziono rozwiazania spelniajacego podane parametry!";
			file << "Przekroczono dozwolona liczbe " << t << " iteracji. Nie znaleziono rozwiazania spelniajacego podane parametry!";
			iterations--;
		}
	}
}
void showResults(vector<variable>  &variables, ofstream &file)
{
	vector<variable> negations;

	for (size_t i = 0; i < variables.size(); i++) {
		if (variables[i].negation)
			negations.push_back(variables[i]);
	}
	cout << endl << "---------------------------------------" << endl;
	if (iterations != 1001)
		cout << endl << "Algorytm pozytywnie zakończył pracę, wyniki pracy algorytmu walkSAT:" << endl;
	else {
		cout << "Nie znaleziono rozwiązania" << endl;
	}
	file << endl << "---------------------------------------" << endl;
	file << endl << "Wyniki pracy algorytmu walkSAT:" << endl;
	if (negations.size() > 0) {
		cout << endl << "Wartosci zmiennych zanegowanych:" << endl;
		file << endl << "Wartosci zmiennych zanegowanych:" << endl;
		for (size_t i = 0; i < negations.size(); i++) {
			cout << negations[i].name << " = " << negations[i].value << endl;
			file << negations[i].name << " = " << negations[i].value << endl;
		}
	}
	else
	{
		cout << endl << "Nie zmieniono wartosci zmiennych";
		file << endl << "Nie zmieniono wartosci zmiennych";
	}

	cout << endl << "Liczba iteracji: " << iterations;
	cout << endl << endl << "---------------------------------------";

	file << endl << "Liczba iteracji: " << iterations;
	file << endl << endl << "---------------------------------------";

}