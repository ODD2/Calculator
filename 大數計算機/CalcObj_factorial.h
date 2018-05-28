#pragma once
#include "BaseCalcObj.h"
class CalcObj_factorial :
	public BaseCalcObj
{
public:
	CalcObj_factorial();
	~CalcObj_factorial();
	CalcObj_factorial(string);

	virtual Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
	virtual void Divide(vector<BaseCalcObj*> *, int, vector<BaseCalcObj*> *&, vector<BaseCalcObj*>*&) const;
};

