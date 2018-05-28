#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include "GLOBALSETTING.h"

using namespace std;

class BigNum
{
	friend class Decimal;
	friend class CalcObj_num;
public:
	BigNum();
	BigNum(double);
	BigNum(string);
	BigNum(const BigNum&);
	BigNum(BigNum&&);
	~BigNum();
	
	BigNum& operator=(const  BigNum&);
	BigNum& operator=(BigNum&&);


	friend istream& operator >> (istream& lhs,BigNum& rhs);
	friend ostream& operator << (ostream& lhs, const BigNum& rhs);


	void operator+=(const BigNum& rhs) {
		*this = *this + rhs;
	}
	void operator-=(const BigNum& rhs){
		*this = *this - rhs;
	}

	friend BigNum operator+(const BigNum&, const BigNum&);
	friend BigNum operator-(const BigNum&, const BigNum&);
	friend BigNum operator*(const BigNum&, const BigNum&);
	friend BigNum operator/(const BigNum&, const BigNum&);
	friend BigNum operator^(const BigNum&, const BigNum&);
	friend BigNum operator%(const BigNum&, const BigNum&);
	friend BigNum operator!(const BigNum&);
	friend BigNum operator-(const BigNum&);
	friend BigNum operator+(const BigNum&);
	

	friend bool operator==(const BigNum&,const BigNum&);
	friend bool operator>(const BigNum&, const BigNum&);
	friend bool operator<(const BigNum&, const BigNum&);
	friend bool operator>=(const BigNum&, const BigNum&);
	friend bool operator<=(const BigNum&, const BigNum&);
	friend bool operator!=(const BigNum&, const BigNum&);


	operator long long  int() const;
	operator int() const;
	operator double() const;


	int getFloatPosition() const { return floatPosition; }
	bool  getSign() const { return sign; }
	bool getPureInt() const { return pureInt; }

	BigNum Sign_Pos() const;
	BigNum Sign_Neg() const;
	BigNum Sign_Switch() const;
	void Sign_Set(bool);

	BigNum GCD(const BigNum&) const;

protected:
	vector<int> value;
	int floatPosition = 0;
	bool sign = true;
	bool pureInt = true;

	void Refine();

	void Vec_Multiply(const vector<int>&, const vector<int>&);
	vector<int> Vec_Cut(int, int);

	void Num_CheckFraction();
	void Num_CheckRedundant();
	bool Num_CheckClose();
	bool Num_CheckCloseTo1();
	bool Num_CheckCloseTo0();
	
	
	BigNum FloatPoint_Swap(int)const;
	BigNum FloatPoint_Cut(int)const;

};

