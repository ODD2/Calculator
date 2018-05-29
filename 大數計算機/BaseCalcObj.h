#pragma once
#define PRIORITY_INTERVAL 6
#include <vector>
#include <string>
#include <iostream>
#include "GLOBALSETTING.h"
using namespace std;

class Decimal;

class BaseCalcObj
{
	friend class Decimal;

public:
	enum CalcType
	{
		ASSIGN = -1,
		NUM = 0,
		ADD,
		SIGN,
		MULTIPLY,
		POWER,
		FACTORIAL
	};

	BaseCalcObj();
	BaseCalcObj(string);
	virtual ~BaseCalcObj();
	BaseCalcObj& operator= (BaseCalcObj&);

	void PriorFall();
	void PriorRise();
	void setPrior(int prior) ;
	int getPrior();
	string getIdentity();

	virtual bool isOperator() const;


	virtual Decimal Operate(vector<BaseCalcObj*> *, int) const = 0;
	virtual void Divide(vector<BaseCalcObj*> *, int, vector<BaseCalcObj*> *&, vector<BaseCalcObj*>*&) const;
	static int Prior_FindLowest(const vector<BaseCalcObj*> *) ;

protected:
	int priority = 0;
	string identity="default";
};

