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

	//BigNum In(str);

	//if (In.floatPosition) {
	//	int swappos = In.floatPosition;
	//	numerator[In.FloatPoint_Swap(0)]=1;
	//	denominator[BigNum("1").FloatPoint_Swap(swappos*(-1))] = 1;
	//}
	//else {
	//	numerator[In] = 1;
	//	denominator[BigNum("1")] = 1;
	//}
}

Decimal::Decimal(vector<BaseCalcObj*>* rhs) {
	int Start_Index = BaseCalcObj::Prior_FindLowest(rhs);
	*this  = (*rhs)[Start_Index]->Operate(rhs, Start_Index);
}

Decimal::Decimal(const Decimal& Dec) {
	numerator = Dec.numerator;
	denominator = Dec.denominator;
}

Decimal::Decimal(Decimal&& Dec)  {
	numerator = move(Dec.numerator);
	denominator = move(Dec.denominator);
}

Decimal::Decimal(const BigNum& rhs) {

	if (rhs.floatPosition) {
		int swappos = rhs.floatPosition;
		numerator[rhs.FloatPoint_Swap(0)] = 1;
		denominator[BigNum("1").FloatPoint_Swap(swappos*(-1))] = 1;
	}
	else {
		numerator[rhs] = 1;
		denominator[BigNum("1")] = 1;
	}

}


/* ========== Assignment =========== */

Decimal& Decimal::operator=(const Decimal& Dec) {
	numerator = Dec.numerator;
	denominator = Dec.denominator;
	return *this;
}


/* ========== Arithmetics ========== */
Decimal operator+(const Decimal& lhs, const Decimal& rhs) {

	Decimal cpLhs = lhs;
	Decimal cpRhs = rhs;

	map<BigNum , double>::const_iterator it;

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
		for (int i = 0; i<it->second; i++)
			L_NUMERATOR = L_NUMERATOR * it->first;
	}

	for (it = cpRhs.numerator.begin(); it != cpRhs.numerator.end(); it++) {
		for(int i=0;i<it->second;i++)
			R_NUMERATOR = R_NUMERATOR * it->first;
	}

	Result.numerator[L_NUMERATOR + R_NUMERATOR] = 1;
	Result.denominator = move(cpLhs.denominator);



	Result.Simplification();

	return move(Result);
}

Decimal operator-(const Decimal& lhs, const Decimal& rhs) {
	
	Decimal cpLhs = lhs;
	Decimal cpRhs = rhs;

	map<BigNum , double>::const_iterator it;

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
		for (int i = 0; i<it->second; i++)
			L_NUMERATOR = L_NUMERATOR * it->first;
	}

	for (it = cpRhs.numerator.begin(); it != cpRhs.numerator.end(); it++) {
		for (int i = 0; i<it->second; i++)
			R_NUMERATOR = R_NUMERATOR * it->first;
	}

	Result.numerator[L_NUMERATOR - R_NUMERATOR] = 1;
	Result.denominator = move(cpLhs.denominator);



	Result.Simplification();

	return move(Result);
}

Decimal operator*(const Decimal& lhs, const Decimal& rhs) {
	Decimal Result = lhs;

	map<BigNum , double>::const_iterator rhs_numerator_it = rhs.numerator.begin();
	for (; rhs_numerator_it != rhs.numerator.end(); rhs_numerator_it++) {
		Result.numerator[rhs_numerator_it->first] += rhs_numerator_it->second;
	}

	map<BigNum , double>::const_iterator rhs_denominator_it = rhs.denominator.begin();
	for (; rhs_denominator_it != rhs.denominator.end(); rhs_denominator_it++) {
		Result.denominator[rhs_denominator_it->first] += rhs_denominator_it->second;
	}

	Result.Simplification();
	return  move(Result);
}

Decimal operator/(const Decimal& lhs,const Decimal& rhs) {
	Decimal Result = lhs;

	map<BigNum , double>::const_iterator rhs_numerator_it = rhs.numerator.begin();
	for (; rhs_numerator_it != rhs.numerator.end(); rhs_numerator_it++) {
		Result.denominator[rhs_numerator_it->first] += rhs_numerator_it->second;
	}

	map<BigNum , double>::const_iterator rhs_denominator_it = rhs.denominator.begin();
	for (; rhs_denominator_it != rhs.denominator.end(); rhs_denominator_it++) {
		Result.numerator[rhs_denominator_it->first] += rhs_denominator_it->second;
	}

	Result.Simplification();
	return  move(Result);
}

Decimal operator^(const Decimal& lhs, const Decimal& rhs) {
	double power = rhs.Evaluate();

	if (power == 0) {
		return move(Decimal());
	}
	else if (fmod(power,0.5) != 0 ) {
#if DEBUG >= 2
		cout << "Decimal::operator^,rhs/0.5 != 0 , rhs=" << power << endl;
		cout << "Decimal::operator^, Returning 1" << endl;;
#endif
		return move(Decimal());
	}
	else {
		Decimal Result = lhs;
		if (power < 0) {
		}
		else if (power == 0) {
			return move(Decimal());
		}
		else {
		}

		map<BigNum, double>::iterator Result_it = Result.numerator.begin();
		//power to numerators;
		for (; Result_it != Result.numerator.end(); Result_it++)
		{
			(Result_it->second) *= power;
		}

		Result_it = Result.denominator.begin();
		//power to denominators;
		for (; Result_it != Result.denominator.end(); Result_it++)
		{
			(Result_it->second) *= power;
		}
		Result.Simplification();
		return move(Result);
		
	}
}

Decimal operator!(const Decimal &lhs) {
	return move(Decimal((!lhs.Evaluate())));
}

Decimal operator-(const Decimal & rhs) {
	Decimal Result(rhs);
	Result.numerator[BigNum("-1")]+=1;
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
	os <<  rhs.Evaluate();

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

	map<BigNum , double>::const_iterator it = numerator.begin();
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

	map<BigNum , double>::const_iterator it = numerator.begin();
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

	//��1���R��(�ٮĲv)
	denominator.erase(BigNum("1"));
	numerator.erase(BigNum("1"));


	if (denominator.size() == 0 || numerator.size() == 0) {
		//noting
	}
	else {

		BigNum O("0");
		map<BigNum, double>::iterator it;


		//�ˬd���l�������S��0
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






		//�s��n�Q�R�������l
		vector< map<BigNum, double>::iterator > erase_numerator;
		//�s��n�Q�R��������
		vector< map<BigNum, double>::iterator > erase_denominator;


		it = denominator.begin();

		for (; it != denominator.end(); it++) {
			//���l��
			if (numerator.count(it->first)) {

				if (it->second > numerator[it->first]) {
					//���l�h
					it->second -= numerator[it->first];

					//�R���l
					erase_numerator.push_back(numerator.find(it->first));
				}
				else if (it->second < numerator[it->first]) {
					//�����h
					numerator[it->first] -= it->second;

					//�R����
					erase_denominator.push_back(it);
				}
				else {
					//�@�˦h�A���R
					erase_numerator.push_back(numerator.find(it->first));
					erase_denominator.push_back(it);
				}

			}
		}

		//�R����
		for (int i = 0; i < erase_denominator.size(); i++) denominator.erase(erase_denominator[i]);
		for (int i = 0; i < erase_numerator.size(); i++) numerator.erase(erase_numerator[i]);


#if DEBUG >= 2
		bool print_origin = true;
#endif

		//��²����
#if SIMPLIFY_EXTREME

		BigNum ONE("1");
		while (1) {

			for (map<BigNum, double>::iterator numerator_it = numerator.begin();
				numerator_it != numerator.end();
				numerator_it++)
			{
				if (fmod(numerator_it->second, 1) != 0)continue;



				for (map<BigNum, double>::iterator denominator_it = denominator.begin();
					denominator_it != denominator.end();
					denominator_it++)
				{
					if (fmod(denominator_it->second, 1) != 0)continue;



					BigNum GCD = denominator_it->first.GCD(numerator_it->first);
					if (GCD.Sign_Pos() != ONE) {

#if DEBUG >=2
						if (print_origin)
							cout << "Decimal::Simplification(): "; Combination(cout); cout << endl;
#endif // DEBUG >=5
						int num_simplify = denominator_it->second < numerator_it->second ?
							denominator_it->second : numerator_it->second;


						//�[�W�h���̤j���]�ƫ᪺��
						denominator[denominator_it->first / GCD] += num_simplify;
						numerator[numerator_it->first / GCD] += num_simplify;

						//�R���������Ӽ�
						denominator_it->second -= num_simplify;
						numerator_it->second -= num_simplify;

						if (denominator_it->second == 0) { denominator.erase(denominator_it); }
						if (numerator_it->second == 0) { numerator.erase(numerator_it); };


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

	//�p�G���l�Τ����O�Ū��A�� 1
	if (denominator.size() == 0) denominator[BigNum("1")] = 1;
	if (numerator.size() == 0) numerator[BigNum("1")] = 1;
}