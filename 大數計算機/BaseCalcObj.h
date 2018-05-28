#pragma once
#define PRIORITY_INTERVAL 6
#include <vector>
#include <string>
#include "GLOBALSETTING.h"

using namespace std;

class Decimal;

class BaseCalcObj
{
	friend class Decimal;
public:
	BaseCalcObj();
	BaseCalcObj(string);
	~BaseCalcObj();

	void PriorFall();
	void PriorRise();
	void setPrior(int prior) ;
	int getPrior();
	string getIdentity();

	virtual bool isOperator() const;


	virtual Decimal Operate(vector<BaseCalcObj*> *, int) const = 0;
	virtual void Divide(vector<BaseCalcObj*> *, int, vector<BaseCalcObj*> *&, vector<BaseCalcObj*>*&) const;


protected:
	int Prior_FindLowest(const vector<BaseCalcObj*> *) const;
	int priority = -1;
	string identity;
};

