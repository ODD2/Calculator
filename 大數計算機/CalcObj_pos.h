#pragma once
#include "BaseCalcObj.h"
class CalcObj_pos :
	public BaseCalcObj
{
public:
	CalcObj_pos();
	~CalcObj_pos();
	CalcObj_pos(string);

	Decimal Operate(vector<BaseCalcObj*> * Obj_list, int index) const;
	virtual void Divide(vector<BaseCalcObj*> *, int, vector<BaseCalcObj*> *&, vector<BaseCalcObj*>*&) const;
};

