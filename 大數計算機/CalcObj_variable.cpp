#include "CalcObj_variable.h"
#include "BigNum.h"
#include "Decimal.h"
#include "Interpreter.h"

CalcObj_variable::CalcObj_variable()
{
	priority = NUM;

}

CalcObj_variable::~CalcObj_variable()
{
}

CalcObj_variable::CalcObj_variable(string identity) :BaseCalcObj(identity) {
	priority = NUM;
};

bool CalcObj_variable::isOperator() const {
	return false;
}

Decimal CalcObj_variable::Operate(vector<BaseCalcObj*> * Origin, int index)const {
	string id = this->identity;

	if (index != 0) {
#if DEBUG >= 1
		cout << "Decimal::CalcObj_variable(): index=" << index << endl;
		for (int i = 0; i < Origin->size(); i++) {
			delete (*Origin)[i];
		}
#endif // 0
		return Decimal(BigNum("0"));
	}
	
	delete (*Origin)[0];
	
	return Interpreter::getGVM(id);
}

