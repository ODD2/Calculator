#include "CalcObj_assign.h"
#include "Decimal.h"
#include "Interpreter.h"

CalcObj_assign::CalcObj_assign()
{
	priority = ASSIGN;
}

CalcObj_assign::~CalcObj_assign()
{
}

CalcObj_assign::CalcObj_assign(string identity) :BaseCalcObj(identity) {
	priority = ASSIGN;
};

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
	string id = (*lhs)[0]->getIdentity();


	//防止未定義變數
	if (!Interpreter::GLOBAL_VAR_MAP.count(id)) {
		cout << "Error! Assignment to Unknown Variable:" << id << endl;
		return Decimal();
	}


	//計算算式的值
	Decimal value= (*rhs)[rhs_next_index]->Operate(rhs, rhs_next_index);

	//輸入值到變數
	if (Interpreter::GLOBAL_VAR_MAP[id].isPureInt()) {
		Interpreter::GLOBAL_VAR_MAP[id] = value.Evaluate().PureInt();
	}
	else {
		value.setPureInt(false);
		Interpreter::GLOBAL_VAR_MAP[id] = move(value);
	}

	//回傳變數
	return Interpreter::GLOBAL_VAR_MAP[id];
}