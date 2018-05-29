#include "CalcObj_pos.h"
#include "Decimal.h"


CalcObj_pos::CalcObj_pos()
{
	priority = SIGN;
}

CalcObj_pos::~CalcObj_pos()
{
}

CalcObj_pos::CalcObj_pos(string identity) :BaseCalcObj(identity) {
	priority = SIGN;
};

Decimal CalcObj_pos::Operate(vector<BaseCalcObj*> * Obj_list, int index) const {
	vector<BaseCalcObj*> * lhs;
	vector<BaseCalcObj*> * rhs;
	this->Divide(Obj_list, index, lhs, rhs);
	int rhs_next_index = Prior_FindLowest(rhs);
	return move(+(*rhs)[rhs_next_index]->Operate(rhs, rhs_next_index));
}


void CalcObj_pos::Divide(vector<BaseCalcObj*> * Origin, int delimiter, vector<BaseCalcObj*> *& lhs, vector<BaseCalcObj*>*& rhs) const {
	delete (*Origin)[delimiter];
	Origin->erase(Origin->begin() + delimiter);
	lhs = nullptr;

	if (Origin->size() == 1) {
		rhs = Origin;
		Origin = nullptr;
		return;
	}

	vector<BaseCalcObj*>::iterator lhs_end = Origin->begin() + delimiter;
	rhs = new vector<BaseCalcObj*>(lhs_end, Origin->end());

	delete Origin; Origin = nullptr;
}