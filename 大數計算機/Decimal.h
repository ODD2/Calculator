#pragma once

#include <map>
#include <vector>
#include "GLOBALSETTING.h"
#include "BigNum.h"


//�]�����class���ۤޥΡA���ઽ���ޥμ��Y��
//��Ӥ��ۤޥΪ�class�u��غc���class�����СC
//���M�p�Gclass A�غc class B�Aclass B�S�غc class A�A�h�|�L���غc�C
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

