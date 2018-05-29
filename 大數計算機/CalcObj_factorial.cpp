#include "CalcObj_factorial.h"
#include "Decimal.h"



CalcObj_factorial::CalcObj_factorial()
{
	priority = FACTORIAL;
}


CalcObj_factorial::~CalcObj_factorial()
{

}


CalcObj_factorial::CalcObj_factorial(string identity) :BaseCalcObj(identity) {
	priority = FACTORIAL;
};

Decimal CalcObj_factorial::Operate(vector<BaseCalcObj*> * Obj_list, int index) const {
	vector<BaseCalcObj*> * lhs;
	vector<BaseCalcObj*> * rhs;
	this->Divide(Obj_list, index, lhs, rhs);
	int lhs_next_index = Prior_FindLowest(lhs);
	return move(!((*lhs)[lhs_next_index]->Operate(lhs, lhs_next_index)));
}


void CalcObj_factorial::Divide(vector<BaseCalcObj*> * Origin, int delimiter, vector<BaseCalcObj*> *& lhs, vector<BaseCalcObj*>*& rhs) const {
	delete (*Origin)[delimiter];
	Origin->erase(Origin->begin() + delimiter);
	rhs = nullptr;

	if (Origin->size() == 1) {
		lhs = Origin;
		Origin = nullptr;
		return;
	}

	vector<BaseCalcObj*>::iterator lhs_end = Origin->begin() + delimiter;
	lhs = new vector<BaseCalcObj*>(Origin->begin(), lhs_end);

	delete Origin; Origin = nullptr;
}