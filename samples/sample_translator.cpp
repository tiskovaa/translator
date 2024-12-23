#include <iostream>
#include "translator.h"

using namespace std;

int main()
{
	setlocale(2, "rus");
	try
	{
		string temp;
		cout << "Введите арифметическое выражение - ";
		cin >> temp;
		Translator t(temp);
		cout << "Ответ = " << t.solve();
	}
	catch (const char* error_message)
	{
		cout <<error_message<<endl;
	}
}