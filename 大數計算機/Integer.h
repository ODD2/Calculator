#pragma once
#include "Decimal.h"
class Integer :
	public Decimal
{
public:
	Integer();
	Integer(string);
	Integer(const Decimal&);
	Integer(const BigNum& rhs);
	~Integer();

	Integer& operator=(const Decimal& rhs) {
		Init(rhs.Evaluate().PureInt());
		return *this;
	}

	Integer& operator=(const Integer& rhs) {
		numerator = rhs.numerator;
		denominator = rhs.denominator;
		pureInt = true;
		return *this;
	}

	Integer& operator=(const BigNum& rhs) {
		*this = Integer(rhs);
		return *this;
	}

	friend istream& operator >> (istream&, Integer&);

};