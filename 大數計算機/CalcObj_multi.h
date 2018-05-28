#pragma once
#include "BaseCalcObj.h"
class CalcObj_multi :
	public BaseCalcObj
{
public:
	CalcObj_multi();
	~CalcObj_multi();
	CalcObj_multi(string);

	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
};

