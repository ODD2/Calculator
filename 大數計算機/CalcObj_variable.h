#pragma once
#include "BaseCalcObj.h"
class CalcObj_variable :
	public BaseCalcObj
{
public:
	CalcObj_variable();
	CalcObj_variable(string);
	~CalcObj_variable();

	virtual bool isOperator() const  final;
	virtual Decimal Operate(vector<BaseCalcObj*> *, int) const;
};

