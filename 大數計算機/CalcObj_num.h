#pragma once
#include "BaseCalcObj.h"
class CalcObj_num :
	public BaseCalcObj
{
public:
	CalcObj_num();
	CalcObj_num(string);
	~CalcObj_num();

	virtual bool isOperator() const  final;
	virtual Decimal Operate(vector<BaseCalcObj*> *, int) const;

};

