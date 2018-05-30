#include <iostream>
#include <string>
#include <vector>
#include "IncludeCalcObj.h"
#include "BigNum.h"
#include "BaseCalcObj.h"
#include "Decimal.h"
#include "Integer.h"
#include "Interpreter.h"

using namespace std;

int main(){
	Interpreter::GLOBAL_VAR_MAP["0"] = BigNum("0");
	/*Integer i = "123";
	Decimal d = "123.3";
	vector<Decimal*> nums;
	nums.push_back(&i);
	nums.push_back(&d);
	for (const auto& num : nums)
		cout << *num << endl;

	Integer i2 = "4";
	Decimal d2 = "4.0";
	cout << i / i2 << endl;
	cout << i / d2 << endl;*/

	
	string command;
	while (getline(cin, command)) {
		if (command.size() == 0) {
			cout << "Command is Empty!!!" << endl;
			continue;
		}

		vector<BaseCalcObj*> * lol = Interpreter::Converter(command);
		int index = BaseCalcObj::Prior_FindLowest(lol);

		if (index == -1) {
		}
		else if((*lol)[index]->getIdentity()=="="){
			(*lol)[index]->Operate(lol, index);
		}
		else{
			cout << " = " << (*lol)[index]->Operate(lol, index) << endl;
		}
	}
	system("pause");
	return 0;
}