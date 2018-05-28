#pragma once
#include "BaseCalcObj.h"
class CalcObj_minus :
	public BaseCalcObj
{
public:
	CalcObj_minus();
	CalcObj_minus(string);
	~CalcObj_minus();
	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
};

