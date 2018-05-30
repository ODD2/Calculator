#include "Integer.h"



Integer::Integer() : Decimal()
{
	pureInt = true;
}

Integer::~Integer()
{
}

Integer::Integer(string command):Integer(Decimal(command).Evaluate().PureInt()){
	pureInt = true;
}

Integer::Integer(const Decimal& rhs):Integer(rhs.Evaluate().PureInt()){
	pureInt = true;
}

Integer::Integer(const BigNum& rhs) : Decimal(rhs.PureInt()) {
	pureInt = true;
}

istream& operator >> (istream& is, Integer& rhs) {
	BigNum Result("0");
	is >> Result;
	rhs = Result;
	return is;
}