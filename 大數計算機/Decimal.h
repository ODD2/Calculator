#pragma once

#include <map>
#include <vector>
#include "GLOBALSETTING.h"
#include "BigNum.h"


//因為兩個class互相引用，不能直接引用標頭黨
//兩個互相引用的class只能建構對方class的指標。
//不然如果class A建構 class B，class B又建構 class A，則會無限建構。
class BaseCalcObj;

class Decimal 
{
	friend class CalcObj_num;
public:
	Decimal();
	Decimal(const Decimal&);
	Decimal(Decimal&&);
	Decimal(string);
	Decimal(vector<BaseCalcObj*>*);
	virtual ~Decimal();
	Decimal(const BigNum &);
	Decimal& operator=(const Decimal&);
	Decimal& operator=(const BigNum & BN);

	friend Decimal operator+(const Decimal&,const Decimal&);
	friend Decimal operator-(const Decimal&, const Decimal&);
	friend Decimal operator*(const Decimal&, const Decimal&);
	friend Decimal operator/(const Decimal&, const Decimal&);
	friend Decimal operator^(const Decimal&, const Decimal&);
	friend Decimal operator!(const Decimal &);
	friend Decimal operator-(const Decimal &);
	friend Decimal operator+(const Decimal &);


	friend ostream& operator << (ostream&, const Decimal&);
	friend istream& operator >> (istream&, Decimal&);


	BigNum Evaluate() const;
	void Combination(ostream&) const;
protected:
	map<BigNum , BigNum> numerator;
	map<BigNum , BigNum> denominator;

	void Init(const BigNum& In) {
		Clear();
		numerator[In] = 1;
		denominator[BigNum("1")] = 1;
	}

	void Clear() {
		numerator.clear();
		denominator.clear();
	}

	void Simplification();

	
};

