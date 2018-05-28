#pragma once
#include "BaseCalcObj.h"
class CalcObj_plus :
	public BaseCalcObj
{
public:
	CalcObj_plus();
	~CalcObj_plus();
	CalcObj_plus(string);


	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
};

