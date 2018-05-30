#include "BaseCalcObj.h"



BaseCalcObj::BaseCalcObj()
{
}

BaseCalcObj::BaseCalcObj(string identity) {
	if(identity.size())
	this->identity = identity;
}

BaseCalcObj::~BaseCalcObj()
{
}

BaseCalcObj& BaseCalcObj::operator= (BaseCalcObj& rhs) {
	priority = rhs.priority;
	identity = rhs.identity;
	return *this;
}

void BaseCalcObj::PriorFall() {
	priority -= PRIORITY_INTERVAL;
}

void BaseCalcObj::PriorRise() {
	priority += PRIORITY_INTERVAL;
}

void BaseCalcObj::setPrior(int prior) {
	this->priority = prior;
}

int BaseCalcObj::getPrior() {
	return this->priority;
}

string BaseCalcObj::getIdentity() {
	return identity;
}

bool BaseCalcObj::isOperator() const {
	return true;
}

//devide the formula , remove operator
void BaseCalcObj::Divide(vector<BaseCalcObj*> * Origin, int delimiter, vector<BaseCalcObj*> *& lhs, vector<BaseCalcObj*>*& rhs) const	{
	delete (*Origin)[delimiter];
	Origin->erase(Origin->begin() + delimiter);

	if (Origin->size() == 1) {
		lhs = Origin;
		Origin = nullptr;
		rhs = nullptr;
		return;
	}

	vector<BaseCalcObj*>::iterator lhs_end= Origin->begin()+delimiter;
	vector<BaseCalcObj*>::iterator rhs_end = Origin->end(); 
	lhs = new vector<BaseCalcObj*>(Origin->begin(), lhs_end);
	rhs = new vector<BaseCalcObj*>(lhs_end, rhs_end);


	delete Origin;
}

int BaseCalcObj::Prior_FindLowest(const vector<BaseCalcObj*> * Origin ) {
	
	if (Origin->size() == 0) {
		cout << "Error! Formula is Empty!!" << endl;
		return -1;
	}



	int ret_priority = INT32_MAX;
	int ret_index = 0;

	for (int i = Origin->size()-1; i >= 0 ; i--) {
		if ((*Origin)[i]->isOperator())
		{
			if ((*Origin)[i]->priority < ret_priority) {
				ret_priority = (*Origin)[i]->priority;
				ret_index = i;
			}
		}
	}

	return ret_index;
}