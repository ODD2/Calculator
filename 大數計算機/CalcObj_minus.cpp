#include "CalcObj_minus.h"
#include "Decimal.h";

CalcObj_minus::CalcObj_minus()
{
	priority = ADD;
}

CalcObj_minus::~CalcObj_minus()
{
}

CalcObj_minus::CalcObj_minus(string identity) :BaseCalcObj(identity) {
	priority = ADD;
};


Decimal CalcObj_minus::Operate(vector<BaseCalcObj*> * Obj_list, int index) const {
	vector<BaseCalcObj*> * lhs;
	vector<BaseCalcObj*> * rhs;
	this->Divide(Obj_list, index, lhs, rhs);
	int lhs_next_index = Prior_FindLowest(lhs);
	int rhs_next_index = Prior_FindLowest(rhs);
	return move((*lhs)[lhs_next_index]->Operate(lhs, lhs_next_index) - (*rhs)[rhs_next_index]->Operate(rhs, rhs_next_index));
}