#pragma once
#include "BaseCalcObj.h"
class CalcObj_assign :
	public BaseCalcObj
{
public:
	CalcObj_assign();
	~CalcObj_assign();
	CalcObj_assign(string);
	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;

};

