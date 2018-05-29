#include "CalcObj_num.h"
#include "BigNum.h"
#include "Decimal.h"


CalcObj_num::CalcObj_num()
{
	priority = NUM;

}

CalcObj_num::~CalcObj_num()
{
}

CalcObj_num::CalcObj_num(string identity) :BaseCalcObj(identity) {
	priority = NUM;
};

bool CalcObj_num::isOperator() const {
	return false;
}

Decimal CalcObj_num::Operate(vector<BaseCalcObj*> * Origin, int index)const  {

	string id =  this->identity;
	

	if (index != 0) {
#if DEBUG >= 1
		cout << "Decimal::CalcObj_num(): index=" << index << endl;
		for (int i = 0; i < Origin->size(); i++) {
			delete (*Origin)[i];
		}
#endif // 0
		return Decimal(BigNum("0"));
	}

	delete (*Origin)[0];
	

	return move(Decimal(BigNum(id)));
}