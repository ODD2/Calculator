#pragma once
#include "BigNum.h"
#include "Decimal.h"
#include "BaseCalcObj.h"
#include "IncludeCalcObj.h"

class Interpreter
{
public:
	static map<string, Decimal> GLOBAL_VAR_MAP;
	static  vector<BaseCalcObj*>* Converter(string);
	static Decimal& getGVM(string);

	Interpreter();
	~Interpreter();

	operator vector<BaseCalcObj*>*() {
		vector<BaseCalcObj*> *ret = new vector<BaseCalcObj*>;
		*ret = *obj_list;
		return ret;
	}

	vector<BaseCalcObj*> Converter_Save(string);

protected:
	vector<BaseCalcObj*> * obj_list;

	/* ==== tools === */
	static bool checkBrackets(string);

	static int checkMode(string, string &, string &);

	static bool checkright(string);

	static 	bool checkoperator(char);

	static bool ischar(char);

	static bool isnum(char);

	static bool is_plus_minus(char);

	static bool is_operator(char);

	static void changeSign(string&);

	static int getPriority(char,int&);

	static void toUnit(string, vector<BaseCalcObj*>*);
};

