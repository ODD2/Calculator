#include "CalcObj_assign.h"
#include "Decimal.h"
#include "Interpreter.h"

CalcObj_assign::CalcObj_assign()
{
}

CalcObj_assign::~CalcObj_assign()
{
}

CalcObj_assign::CalcObj_assign(string identity) :BaseCalcObj(identity) {};

Decimal CalcObj_assign::Operate(vector<BaseCalcObj*> * Obj_list, int index) const {
	if (index != 1) {
#if DEBUG >=2
		cout << "CalcObj_assign::Operate(): Printing Formula:" << endl;
		for (int i = 0; i <= index; i++) {
			cout << (*Obj_list)[i]->getIdentity();
		}
		cout << endl;
#endif // DEBUG
		cout << "Input Formula Error!" << endl;
		return Decimal(BigNum("0"));
	}
	vector<BaseCalcObj*> * lhs;
	vector<BaseCalcObj*> * rhs;
	this->Divide(Obj_list, index, lhs, rhs);

	int rhs_next_index = Prior_FindLowest(rhs);
	string variable = (*lhs)[0]->getIdentity();
	return Interpreter::GLOBAL_VAR_MAP[variable] = (*rhs)[rhs_next_index]->Operate(rhs, rhs_next_index);
}