#include <cmath>
#include "Decimal.h"
#include "BaseCalcObj.h"
#include "Interpreter.h"


Decimal::Decimal()
{
	numerator[BigNum("1")]=1;
	denominator[BigNum("1")]=1;
}

Decimal::~Decimal()
{
	numerator.clear();
	denominator.clear();
}

Decimal::Decimal(string str) {
	*this = Decimal(Interpreter::Converter(str));
}

Decimal::Decimal(vector<BaseCalcObj*>* rhs) {
	int Start_Index = BaseCalcObj::Prior_FindLowest(rhs);
	*this  = (*rhs)[Start_Index]->Operate(rhs, Start_Index);
}

Decimal::Decimal(const Decimal& Dec) {
	numerator = Dec.numerator;
	denominator = Dec.denominator;
	pureInt = Dec.pureInt;
}

Decimal::Decimal(Decimal&& Dec)  {
	numerator = move(Dec.numerator);
	denominator = move(Dec.denominator);
	pureInt = Dec.pureInt;
}

Decimal::Decimal(const BigNum& rhs) {
	BigNum ONE("1");
	if (rhs.floatPosition) {
		int swappos = rhs.floatPosition;
		numerator[rhs.FloatPoint_Swap(0)] = ONE;
		denominator[ONE.FloatPoint_Swap(swappos*(-1))] = ONE;
	}
	else {
		numerator[rhs] = ONE;
		denominator[ONE] = ONE;
	}
	pureInt = rhs.pureInt;
}


/* ========== Assignment =========== */

Decimal& Decimal::operator=(const Decimal& Dec) {
	numerator = Dec.numerator;
	denominator = Dec.denominator;
	pureInt = Dec.pureInt;
	return *this;
}

Decimal& Decimal::operator=(const BigNum & BN) {
	*this = move(Decimal(BN));
	return *this;
}


/* ========== Arithmetics ========== */
Decimal operator+(const Decimal& lhs, const Decimal& rhs) {

	Decimal cpLhs = lhs;
	Decimal cpRhs = rhs;

	map<BigNum , BigNum>::const_iterator it;

	for (it = rhs.denominator.begin(); it != rhs.denominator.end(); it++) {
		cpLhs.numerator[it->first] += it->second;
		cpLhs.denominator[it->first] += it->second;
	}
		
	//cpLhs.Simplification();

	for (it = lhs.denominator.begin(); it != lhs.denominator.end(); it++) {
		cpRhs.numerator[it->first] += it->second;
		cpRhs.denominator[it->first] += it->second;
	}

	//cpRhs.Simplification();

	Decimal Result;

	BigNum L_NUMERATOR("1");
	BigNum R_NUMERATOR("1");


	for (it = cpLhs.numerator.begin(); it != cpLhs.numerator.end(); it++) {
			L_NUMERATOR = L_NUMERATOR * (it->first ^ it->second);
	}

	for (it = cpRhs.numerator.begin(); it != cpRhs.numerator.end(); it++) {
		
			R_NUMERATOR = R_NUMERATOR * (it->first ^ it->second);
	}

	Result.numerator[L_NUMERATOR + R_NUMERATOR] = 1;
	Result.denominator = move(cpLhs.denominator);



	Result.Simplification();

	Result.pureInt = (lhs.pureInt && rhs.pureInt);
	return move(Result);
}

Decimal operator-(const Decimal& lhs, const Decimal& rhs) {
	
	Decimal cpLhs = lhs;
	Decimal cpRhs = rhs;

	map<BigNum , BigNum>::const_iterator it;

	for (it = rhs.denominator.begin(); it != rhs.denominator.end(); it++) {
		cpLhs.numerator[it->first] += it->second;
		cpLhs.denominator[it->first] += it->second;
	}

	//cpLhs.Simplification();

	for (it = lhs.denominator.begin(); it != lhs.denominator.end(); it++) {
		cpRhs.numerator[it->first] += it->second;
		cpRhs.denominator[it->first] += it->second;
	}

	//cpRhs.Simplification();

	Decimal Result;

	BigNum L_NUMERATOR("1");
	BigNum R_NUMERATOR("1");


	for (it = cpLhs.numerator.begin(); it != cpLhs.numerator.end(); it++) {
		L_NUMERATOR = L_NUMERATOR * (it->first ^ it->second);
	}

	for (it = cpRhs.numerator.begin(); it != cpRhs.numerator.end(); it++) {
		R_NUMERATOR = R_NUMERATOR * (it->first ^ it->second);
	}

	Result.numerator[L_NUMERATOR - R_NUMERATOR] = 1;
	Result.denominator = move(cpLhs.denominator);



	Result.Simplification();


	Result.pureInt = (lhs.pureInt && rhs.pureInt);


	return move(Result);
}

Decimal operator*(const Decimal& lhs, const Decimal& rhs) {
	
	Decimal Result = lhs;

	map<BigNum , BigNum>::const_iterator rhs_numerator_it = rhs.numerator.begin();
	for (; rhs_numerator_it != rhs.numerator.end(); rhs_numerator_it++) {
		Result.numerator[rhs_numerator_it->first] += rhs_numerator_it->second;
	}

	map<BigNum , BigNum>::const_iterator rhs_denominator_it = rhs.denominator.begin();
	for (; rhs_denominator_it != rhs.denominator.end(); rhs_denominator_it++) {
		Result.denominator[rhs_denominator_it->first] += rhs_denominator_it->second;
	}

	Result.Simplification();

	Result.pureInt = (lhs.pureInt && rhs.pureInt);


	return  move(Result);
}

Decimal operator/(const Decimal& lhs,const Decimal& rhs) {

	if (lhs.pureInt && rhs.pureInt) 
	{
		cout << "Warning: Calculating Division of 2 Pure Integers." << endl;
		return Decimal((lhs.Evaluate()/rhs.Evaluate()).PureInt());
	}
	else {

		Decimal Result = lhs;

		map<BigNum, BigNum>::const_iterator rhs_numerator_it = rhs.numerator.begin();
		for (; rhs_numerator_it != rhs.numerator.end(); rhs_numerator_it++) {
			Result.denominator[rhs_numerator_it->first] += rhs_numerator_it->second;
		}

		map<BigNum, BigNum>::const_iterator rhs_denominator_it = rhs.denominator.begin();
		for (; rhs_denominator_it != rhs.denominator.end(); rhs_denominator_it++) {
			Result.numerator[rhs_denominator_it->first] += rhs_denominator_it->second;
		}

		Result.Simplification();
		Result.pureInt = false;
		return  move(Result);
	}
}

Decimal operator^(const Decimal& lhs, const Decimal& rhs) {
	BigNum power = rhs.Evaluate();
	BigNum O("0");
	BigNum O_5("0.5");
	
	if (power.Sign_Pos() == O) {
		return move(Decimal());
	}
	else if ( power.Sign_Pos() % O_5 != O ) {
#if DEBUG >= 2
		cout << "Decimal::operator^,rhs/0.5 != 0 , rhs=" << power << endl;
		cout << "Decimal::operator^, Returning 1" << endl;;
#endif
		cout << "Error! Power Must be Multiple of 0.5" << endl;
		return move(Decimal());
	}
	else {
		Decimal Result = lhs;

		if (power < O && lhs.pureInt) {
			cout << "Warning: Calculating Negative Power of a Pure Integer." << endl;
			return move(O);
		}
		else if (lhs.pureInt&&rhs.pureInt) {
		}
		else {
			Result.pureInt = false;
		}


		

		
		
		map<BigNum , BigNum>::iterator Result_it = Result.numerator.begin();
		//power to numerators;
		for (; Result_it != Result.numerator.end(); Result_it++)
		{
			Result_it->second = Result_it->second * power;
		}

		Result_it = Result.denominator.begin();
		//power to denominators;
		for (; Result_it != Result.denominator.end(); Result_it++)
		{
			Result_it->second = Result_it->second * power;
		}
		Result.Simplification();
		return move(Result);
		
	}
}

Decimal operator!(const Decimal &lhs) {
	Decimal Result = move(Decimal((!lhs.Evaluate()).PureInt()));
	Result.pureInt = lhs.pureInt;
	return move(Result);
}

Decimal operator-(const Decimal & rhs) {
	Decimal Result(rhs);
	Result.numerator[BigNum("-1")] += 1;
	return move(Result);
}

Decimal operator+(const Decimal & rhs) {
	return rhs;
}


/* ========== IN/OUT ========== */
ostream& operator << (ostream& os, const Decimal& rhs) {
	
#ifdef DEBUG
	rhs.Combination(os);
	os << ",Evaluate:";
#endif // 
	BigNum Result = rhs.Evaluate().AllCheck();
	os <<  Result;

	if (!rhs.isPureInt()) {
		int cover = FLOAT_EFFECTIVE_RANGE - Result.getFloatPosition();
		if (cover == FLOAT_EFFECTIVE_RANGE) {
			cout << ".";
		}
		for (int i = 0; i < cover; i++) {
			os << "0";
		}
	}
	

	return os;
}

istream& operator >> (istream& is, Decimal& rhs) {
	BigNum StartUp;
	is >> StartUp;
	rhs.Init(StartUp);
	return is;
}





/* ========== TOOLS ========== */
BigNum Decimal::Evaluate() const {
	
	BigNum Result("1");
	BigNum ONE("1");

	map<BigNum , BigNum>::const_iterator it = numerator.begin();
	for (; it != numerator.end(); it++) {
			Result = Result * (it->first ^ it->second);
	}

	it = denominator.begin();

	for (; it != denominator.end(); it++) {
		if (it->first == ONE) continue;
		Result = Result / (it->first^ it->second);
	}

	return move(Result);
}

void Decimal::Combination(ostream& os=cout) const {

	map<BigNum , BigNum>::const_iterator it = numerator.begin();
	for (; it != numerator.end();) {
		os << "(" << it->first << "^" << it->second << ")";
		if (++it != numerator.end())os << "*";
	}

	os << "/";

	it = denominator.begin();
	for (; it != denominator.end();) {
		os << "(" << it->first << "^" << it->second << ")";
		if (++it != denominator.end())os << "*";
	}
}

void Decimal::Simplification() {

	//р1常奔(瞯)
	denominator.erase(BigNum("1"));
	numerator.erase(BigNum("1"));


	if (denominator.size() == 0 || numerator.size() == 0) {
		//noting
	}
	else {

		BigNum O("0");
		BigNum ONE("1");
		map<BigNum , BigNum>::iterator it;


		//浪琩だだダΤ⊿Τ0
		it = denominator.begin();
		for (; it != denominator.end(); it++) {
			if (it->first == O) {
				cout << "Error! Decimal Denominator has 0, Returning 0." << endl;

				numerator.clear();
				denominator.clear();

				numerator[O] = 1;
				denominator[BigNum("1")] = 1;
				return;
			}
		}
		it = numerator.begin();
		for (; it != numerator.end(); it++) {
			if (it->first == O) {
				cout << "Decimal Numerator Has 0, Returning 0." << endl;


				numerator.clear();
				denominator.clear();

				numerator[O] = 1;
				denominator[BigNum("1")] = 1;

				return;
			}
		}






		//璶砆奔だ
		vector< map<BigNum , BigNum>::iterator > erase_numerator;
		//璶砆奔だダ
		vector< map<BigNum , BigNum>::iterator > erase_denominator;


		it = denominator.begin();

		for (; it != denominator.end(); it++) {
			//だΤ
			if (numerator.count(it->first)) {

				if (it->second > numerator[it->first]) {
					//だ
					it->second -= numerator[it->first];

					//だ
					erase_numerator.push_back(numerator.find(it->first));
				}
				else if (it->second < numerator[it->first]) {
					//だダ
					numerator[it->first] -= it->second;

					//だダ
					erase_denominator.push_back(it);
				}
				else {
					//妓常
					erase_numerator.push_back(numerator.find(it->first));
					erase_denominator.push_back(it);
				}

			}
		}

		//
		for (int i = 0; i < erase_denominator.size(); i++) denominator.erase(erase_denominator[i]);
		for (int i = 0; i < erase_numerator.size(); i++) numerator.erase(erase_numerator[i]);


#if DEBUG >= 2
		bool print_origin = true;
#endif

		//程虏だ计
#if SIMPLIFY_EXTREME

		while (1) {

			for (map<BigNum , BigNum>::iterator numerator_it = numerator.begin();
				numerator_it != numerator.end();
				numerator_it++)
			{
				if ((numerator_it->second.Sign_Pos() % ONE) != O )continue;



				for (map<BigNum , BigNum>::iterator denominator_it = denominator.begin();
					denominator_it != denominator.end();
					denominator_it++)
				{
					if ((numerator_it->second.Sign_Pos() % ONE) != O)continue;



					BigNum GCD = denominator_it->first.GCD(numerator_it->first);
					if (GCD.Sign_Pos() != ONE) {

#if DEBUG >=2
						if (print_origin)
							cout << "Decimal::Simplification(): "; Combination(cout); cout << endl;
#endif // DEBUG >=5
						BigNum
							num_simplify = denominator_it->second < numerator_it->second ?
							denominator_it->second : numerator_it->second;


						//奔程そ计计
						denominator[denominator_it->first / GCD] += num_simplify;
						numerator[numerator_it->first / GCD] += num_simplify;

						//奔だ计
						denominator_it->second -= num_simplify;
						numerator_it->second -= num_simplify;

						if (denominator_it->second == O) { denominator.erase(denominator_it); }
						if (numerator_it->second == O) { numerator.erase(numerator_it); };


#if DEBUG >=2
						cout << "Decimal::Simplification(): "; Combination(cout); cout << endl;
#endif // DEBUG >=5

						goto SIMPLIFY_RETRY;
					}
				}

			}

			break;
		SIMPLIFY_RETRY:
			continue;
		}
#endif // SIMPLIFY_EXTREME
	}

	//狦だ┪だダ琌干 1
	if (denominator.size() == 0) denominator[BigNum("1")] = 1;
	if (numerator.size() == 0) numerator[BigNum("1")] = 1;
}