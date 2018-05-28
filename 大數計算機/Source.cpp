#include <iostream>
#include <string>
#include <vector>
#include "IncludeCalcObj.h"
#include "BigNum.h"
#include "BaseCalcObj.h"
#include "Decimal.h"
#include "Interpreter.h"

using namespace std;

int main() {
	string command;
	while (1) {
		getline(cin, command);
		if (!cin) break;
		vector<BaseCalcObj*> *lol = Interpreter::Converter(command);

		int index = CalcObj_num().Prior_FindLowest(lol);
		cout << (*lol)[index]->Operate(lol, index).Evaluate() << endl;
	}
	
	//cout << (Decimal("123") / Decimal("0")).Evaluate() << endl;
	//cout << (-Decimal("3")).Evaluate() << endl;
	//vector<BaseCalcObj*> *lol = new vector<BaseCalcObj*>;
	//lol->push_back(new CalcObj_num("1"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_plus ("+"));
	//lol->back()->setPrior(7);
	//lol->push_back(new CalcObj_num("3"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_multi("*"));
	//lol->back()->setPrior(8);
	//lol->push_back(new CalcObj_num("8"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_minus("-"));
	//lol->back()->setPrior(7);
	///*lol->push_back(new CalcObj_num("-1"));
	//lol->back()->setPrior(0);*/
	//lol->push_back(new CalcObj_neg("@"));
	//lol->back()->setPrior(9);
	///*lol->push_back(new CalcObj_multi("*"));
	//lol->back()->setPrior(8);*/
	//lol->push_back(new CalcObj_num("4"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_plus("+"));
	//lol->back()->setPrior(13);
	//lol->push_back(new CalcObj_num("6"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_multi("*"));
	//lol->back()->setPrior(14);
	//lol->push_back(new CalcObj_num("7"));
	//lol->back()->setPrior(0);
	//lol->push_back(new CalcObj_factorial("!"));
	//lol->back()->setPrior(5);

	//cout << (*lol)[12]->Operate(lol, 12).Evaluate() << endl;


	////cout << (BigNum("2") ^ BigNum("0.5")) << endl;
	//cout << (Decimal("2") ^ (Decimal("9")).Evaluate()) << endl;


	//cout << (Decimal("0") / Decimal("9")).Evaluate() << endl;
	//cout << ((Decimal("1") / Decimal("9")) ^ (Decimal("1234") / Decimal("2468"))) << endl;

	//
	//map<string, int> test;
	//test["1"]; test["2"]; test["3"];

	///*Testing Part*/
	//BigNum G;
	//BigNum B;
	//BigNum D;
	//
	//
	//B = BigNum("81");
	//Decimal A = Decimal("2") * Decimal("5") * Decimal("7") / Decimal("13") / Decimal("17") / Decimal("23");
	//Decimal C = Decimal("3") * Decimal("29") * Decimal("31") / Decimal("13") / Decimal("19") / Decimal("23");


	//cout << A + C << endl;
	//cout << A - C << endl;
	//cout << Decimal("65635653353332833") / Decimal("123456789") <<endl;

	//cout << B.GCD(BigNum("9")) << endl;
	//cout << Decimal("4") / Decimal("100") << endl;

	//cout << Decimal("0.123124123124") << endl;
	//cout << Decimal("123456") / Decimal("123456") << endl;
	//
	//cout << (BigNum("-2") ^ BigNum("-0.5")) << endl;
 //	cout << (((BigNum("2") ^ BigNum("3")) ^ BigNum("0.5")) ^ (BigNum("2") * BigNum("2"))) << endl;
	//
	//cout << !BigNum("100") << endl;
	//cout << !!BigNum("5") << endl;
	//cout << !BigNum("120") << endl;
	//cout << (BigNum("123456789") ^ BigNum("0.5")) << endl;

	

	
	/*bool test = false;
	cout << (true ^ test);*/
	/*string expression;
	while (cin >> expression)
	{
		calculator.parser();
		calculator.exec();
	}*/

	system("pause");
	return 0;
}