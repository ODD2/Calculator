#include "CalcObj_num.h"
#include "BigNum.h"
#include "Decimal.h"


CalcObj_num::CalcObj_num()
{
}

CalcObj_num::CalcObj_num(string identity) :BaseCalcObj(identity) {};

CalcObj_num::~CalcObj_num()
{
}

bool CalcObj_num::isOperator() const {
	return false;
}

Decimal CalcObj_num::Operate(vector<BaseCalcObj*> * Origin, int index)const  {

	string id =  this->identity;
	

	if (index != 0) {
#if DEBUG >= 1
		cout << "Decimal::CalcObj_num(): index=" << index << endl;
		delete (*Origin)[0];
#endif // 0
		return Decimal(BigNum("0"));
	}
	delete (*Origin)[0];
	BigNum In(id);
	Decimal Result;

	if (In.floatPosition) {
		int swappos = In.floatPosition;
		
		Result.numerator[In.FloatPoint_Swap(0)] = 1;
		Result.denominator[BigNum("1").FloatPoint_Swap(swappos*(-1))] = 1;
	}

	else {
		Result.numerator[In] = 1;
		Result.denominator[BigNum("1")] = 1;
	}

	return move(Result);
}