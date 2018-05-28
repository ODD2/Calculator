#pragma once
#include "BaseCalcObj.h"
class CalcObj_div :
	public BaseCalcObj
{
public:
	CalcObj_div();
	CalcObj_div(string);
	~CalcObj_div();
	
	virtual Decimal Operate(vector<BaseCalcObj*> *, int) const;
};

