#include "CalcObj_plus.h"
#include "Decimal.h"


CalcObj_plus::CalcObj_plus()
{
}


CalcObj_plus::~CalcObj_plus()
{
}


CalcObj_plus::CalcObj_plus(string identity) :BaseCalcObj(identity) {};

Decimal CalcObj_plus::Operate(vector<BaseCalcObj*> * Obj_list, int index) const {
	vector<BaseCalcObj*> * lhs;
	vector<BaseCalcObj*> * rhs;
	this->Divide(Obj_list, index, lhs, rhs);
	int lhs_next_index = Prior_FindLowest(lhs);
	int rhs_next_index = Prior_FindLowest(rhs);
	return move((*lhs)[lhs_next_index]->Operate(lhs, lhs_next_index) + (*rhs)[rhs_next_index]->Operate(rhs, rhs_next_index));
}