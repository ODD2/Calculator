#pragma once
#include "BaseCalcObj.h"
class CalcObj_power :
	public BaseCalcObj
{
public:
	CalcObj_power();
	~CalcObj_power();
	CalcObj_power(string);
	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
};

