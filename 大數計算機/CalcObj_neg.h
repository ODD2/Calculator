#pragma once
#include "BaseCalcObj.h"
class CalcObj_neg :
	public BaseCalcObj
{
public:
	CalcObj_neg();
	~CalcObj_neg();
	CalcObj_neg(string);
	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
	virtual void Divide(vector<BaseCalcObj*> *, int, vector<BaseCalcObj*> *&, vector<BaseCalcObj*>*&) const;
};

