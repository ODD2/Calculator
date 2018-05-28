#pragma once
#include "BigNum.h"
#include "Decimal.h"
#include "BaseCalcObj.h"

class Interpreter
{
public:
	static map<string, Decimal> GLOBAL_VAR_MAP;
	static  vector<BaseCalcObj*>* Converter(string);


	Interpreter();
	~Interpreter();
	operator vector<BaseCalcObj*>*();
	vector<BaseCalcObj*> Converter_Save(string);

private:
	vector<BaseCalcObj*>* obj_list;
};

