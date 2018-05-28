#include "BigNum.h"



BigNum::BigNum()
{
	value.push_back(0);
}

BigNum::~BigNum()
{
	value.~vector<int>();
}

BigNum::BigNum(string value){
	//get sign
	if (value[0] == '-') {
		sign = false;
		value.erase(value.begin());
	}
	//get value
	for (int i = 0; i < value.size(); i++) {
		if (value[i] == '.') {
			pureInt = false;
			floatPosition = value.size() - 1 - i;
			continue;
		}
		this->value.insert(this->value.begin(), value[i] - '0');
	}

	Num_CheckFraction();
	Num_CheckRedundant();
	Num_CheckClose();
}

BigNum::BigNum(const BigNum& rhs) {
	this->value = rhs.value;
	this->floatPosition = rhs.floatPosition;
	this->sign = rhs.sign;
	this->pureInt = rhs.pureInt;
}

BigNum::BigNum(BigNum&& rhs) {
	this->value = move(rhs.value);
	this->floatPosition = rhs.floatPosition;
	this->sign = rhs.sign;
	this->pureInt = rhs.pureInt;
}

BigNum::BigNum(double rhs):BigNum(to_string(rhs)) {
	
}
/* ========== Assignments ========== */
BigNum& BigNum::operator=(const  BigNum& rhs) {
	this->value = rhs.value;
	this->floatPosition = rhs.floatPosition;
	this->sign = rhs.sign;
	this->pureInt = rhs.pureInt;
	return *this;
}

BigNum& BigNum::operator=(BigNum&& rhs) {
	this->value = move(rhs.value);
	this->floatPosition = rhs.floatPosition;
	this->sign = rhs.sign;
	this->pureInt = rhs.pureInt;
	return *this;
}


/* ========== In/Out ========== */

istream& operator >> (istream& is, BigNum& rhs) {
	string value;
	is >> value;

	//clean before initialization.
	rhs.value.clear();

	//get sign
	if (value[0] == '-') {
		rhs.sign = false;
		value.erase(value.begin());
	}

	//initialization
	for (int i = 0; i < value.size(); i++) {
		if (value[i] == '.') {
			rhs.pureInt = false;
			rhs.floatPosition = value.size()-1-i;
			continue;
		}
		rhs.value.insert(rhs.value.begin(), value[i] - '0');
	}

	rhs.Num_CheckRedundant();
	rhs.Num_CheckFraction();

	return is;
}

ostream& operator << (ostream& os, const BigNum& rhs) {
	BigNum OUT(rhs.FloatPoint_Cut(FLOAT_EFFECTIVE_RANGE));
	OUT.Num_CheckClose();
	OUT.Num_CheckRedundant();

	if (!(OUT.sign))cout << '-';

	int end = 0;

	if (OUT.floatPosition > FLOAT_EFFECTIVE_RANGE) {
		end = OUT.floatPosition - FLOAT_EFFECTIVE_RANGE;
	}

	for (int i = OUT.value.size() - 1; i >= end; i--) {
		os << OUT.value[i];
		if (OUT.floatPosition == i && OUT.floatPosition)cout << '.';
	}

	return os;
}



/* ========== Arithmetics ========== */
BigNum operator+(const BigNum& lhs, const BigNum& rhs) {
	BigNum Result;

	if (!lhs.sign&&rhs.sign) {

		return operator-(rhs,lhs.Sign_Pos());
	}
	else if (lhs.sign&&!rhs.sign) {
		
		return operator-(lhs,rhs.Sign_Pos());
	}
	else if (!lhs.sign && !rhs.sign) {
		Result.sign = false;
	}



	
	int lhsIntSize = lhs.value.size() - lhs.floatPosition;
	int rhsIntSize = rhs.value.size() - rhs.floatPosition;
	
	//т皚程疊翴/俱计
	int floatSize = lhs.floatPosition > rhs.floatPosition ? lhs.floatPosition : rhs.floatPosition;
	int intSize = lhsIntSize > rhsIntSize ? lhsIntSize : rhsIntSize;
	Result.floatPosition = floatSize;
	
	//硑计皚
	Result.value.resize(floatSize+intSize);

	//狡籹Resultじ
	for (int i = 0; i < rhs.value.size(); i++) {
		Result.value[floatSize -(rhs.floatPosition - i)] += rhs.value[i];
	}
	for (int i = 0; i < lhs.value.size(); i++) {
		Result.value[floatSize - (lhs.floatPosition - i)] += lhs.value[i];
	}

	//秈
	Result.Refine();
	Result.Num_CheckFraction();
	Result.Num_CheckRedundant();
	Result.Num_CheckClose();


#if DEBUG>=3
	cout << "BigNum::Operator+(): " << lhs << " + " << rhs << " = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator-(const BigNum& lhs, const BigNum& rhs) {
	
	BigNum Result;
	if (!lhs.sign&&rhs.sign) {
		return operator+(rhs, lhs.Sign_Pos()).Sign_Neg();
	}
	else if (lhs.sign && !rhs.sign) {
		return operator+(lhs, rhs.Sign_Pos()).Sign_Pos();
	}
	else if (!lhs.sign && !rhs.sign) {
		return operator-(rhs.Sign_Pos(), lhs.Sign_Pos());
	}


	int lhsIntSize = lhs.value.size() - lhs.floatPosition;
	int rhsIntSize = rhs.value.size() - rhs.floatPosition;

	//т皚程疊翴/俱计
	int floatSize = lhs.floatPosition > rhs.floatPosition ? lhs.floatPosition : rhs.floatPosition;
	int intSize = lhsIntSize > rhsIntSize ? lhsIntSize : rhsIntSize;
	

	//硑计皚
	Result.value.resize(floatSize + intSize);
	Result.floatPosition = floatSize;


	//狡籹Result
	for (int i = 0; i < lhs.value.size(); i++) {
		Result.value[floatSize - (lhs.floatPosition - i)] += lhs.value[i];
	}

	for (int i = 0; i < rhs.value.size(); i++) {
		Result.value[floatSize - (rhs.floatPosition - i)] -= rhs.value[i];
	}


	//Pre-clear up
	Result.Num_CheckRedundant();



	//Find Adjust Condition
	bool sign = true;

	for(int i = Result.value.size() - 1;i>=0; i--){
		if (Result.value[i] < 0) {
			sign = false;
			break;
		}
		else if (Result.value[i] > 0) {
			break;
		}
	}

	Result.sign = sign;

	if (!sign) {
		//Adjust to Positive Value
		for (int i = Result.value.size()-1; i>=0 ; i--) {
			Result.value[i] *= -1;
		}
	}

	//Calculate carries.
	for (int i = 0; i <Result.value.size()-1; i++) {
		if (Result.value[i] < 0) {
			Result.value[i + 1] -= 1;
			Result.value[i] = 10 + Result.value[i];
		}
	}

	//Post-clear up
	Result.Num_CheckFraction();
	Result.Num_CheckRedundant();
	Result.Num_CheckClose();


#if DEBUG>=3
	cout << "BigNum::Operator-(): " << lhs << " - " << rhs << " = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator*(const BigNum& lhs, const BigNum& rhs) {
	BigNum Result;
	Result.sign = !(lhs.sign ^ rhs.sign);
	//get floatpoint
	Result.floatPosition = lhs.floatPosition + rhs.floatPosition;

	//do multiplication.
	Result.Vec_Multiply(lhs.value, rhs.value);
	
	Result.Num_CheckFraction();
	Result.Num_CheckRedundant();
	Result.Num_CheckClose();

#if DEBUG>=3
	cout << "BigNum::Operator*(): " << lhs << " * " << rhs << " = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator/(const BigNum& lhs, const BigNum& rhs) {
	BigNum Zero("0");
	

	
	if (lhs == Zero) {
#if DEBUG >=5
		cout << "BigNum::operator/():" << endl << "lhs = 0 , rhs = " << rhs << endl;
#endif
		return move(Zero);
	}
	else if (rhs == Zero) {
#if DEBUG >=2
		cout << "BigNum::operator/():" << endl << "lhs = " << lhs << " , rhs = 0" << endl << "Return BigNum(\"0\")" << endl;
#endif
		cout << "Error! Denominator has 0, Returning BigNum(0)" << endl;
		return move(Zero);
	}

	BigNum Result;

	//タ璽腹タタ眔タ璽璽眔タ
	if (lhs.sign == rhs.sign)Result.sign = true;
	else Result.sign = false;

	//т程计计
	int swappos = lhs.floatPosition > rhs.floatPosition ? lhs.floatPosition : rhs.floatPosition;

	//р┮Τ计跑Θ俱计
	BigNum cplhs = move(lhs.FloatPoint_Swap(lhs.floatPosition - swappos));    cplhs.sign = true; cplhs.Num_CheckRedundant();
	BigNum cprhs = move(rhs.FloatPoint_Swap(rhs.floatPosition - swappos));    cprhs.sign = true; cprhs.Num_CheckRedundant();

	BigNum zero("0");
	BigNum numerator;

	if(cplhs > cprhs) {
		numerator.value = move(cplhs.Vec_Cut(cplhs.value.size() - cprhs.value.size() + 1, cprhs.value.size()));
		cplhs.value = move(cplhs.Vec_Cut(1, cplhs.value.size() - numerator.value.size()));
	}
	else {
		numerator = cplhs;
		cplhs.value.clear();
	}
	
	
	do{
		//刚埃计籔砆埃计―坝
		for (int i = 1; i <= 10; i++) {
			if (numerator < cprhs* BigNum(to_string(i))) {
				//块坝
				Result.value.insert(Result.value.begin(), i - 1);
				//緇计
				numerator = numerator - cprhs*BigNum(to_string(i - 1));
				break;
			}
		}

		if (cplhs.value.size() == 0 && numerator == zero) {
			//辣干簍衡猭簗衡礚计floatPosition穦1
			//┮讽笿Τ计琵floatPosition1
			Result.floatPosition += 1;
			break;
		}

		//緇计
		if (cplhs.value.size() == 0) {
			//(⊿緇计干碞干0)
			numerator.value.insert(numerator.value.begin(), 0);
			//盢璶璸衡疊翴计竚
			Result.floatPosition += 1;
		}
		else {
			numerator.value.insert(numerator.value.begin(), cplhs.value.back());
			numerator.Num_CheckRedundant();
			//埃干计
			cplhs.value.pop_back();
		}

		
	} while (Result.floatPosition <= FLOAT_MAX);

	//埃奔⊿Τ衡疊翴计竚
	Result.floatPosition -= 1;

	//浪琩
	Result.Num_CheckFraction();
	Result.Num_CheckRedundant();
	Result.Num_CheckClose();

#if DEBUG>=3
	cout << "BigNum::Operator/(): " << lhs << " / " << rhs << " = " << Result;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator%(const BigNum& lhs, const BigNum& rhs) {
	BigNum Quotient = lhs / rhs;
	BigNum Result = lhs - rhs * Quotient.FloatPoint_Cut(0);

#if DEBUG>=3
	cout << "BigNum::Operator%(): " << lhs << " % " << rhs << " = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator^(const BigNum& lhs, const BigNum& rhs) {
	
	
	BigNum Zero_5("0.5");
	BigNum Zero("0");
	BigNum One("1");
	BigNum _0_5_times = move((rhs / Zero_5).Sign_Pos());


	//Special Cases;
	if (lhs == Zero) {
		return move(BigNum("0"));
	}
	else if (rhs == Zero) {
		return move(BigNum("1"));
	}
	else if (lhs == One) {
		return move(BigNum("1"));
	}
	else if (rhs == One) {
		return lhs;
	}


	//ぃ琌0.5俱计
	else if (_0_5_times.floatPosition != 0) {
#if DEBUG>=5
		cout << "BigNum::operator^():Power(" << rhs << ") is not a multiplication of 0.5." << endl;
#endif // DEBUG
		return move(BigNum("0"));
	}
	//秨璽计キよ
	else if ( !lhs.sign && rhs.floatPosition != 0) {
#if DEBUG>=2
		cout << "BigNum::operator^(): Power(" << lhs << "," << rhs << ") is faulty." << endl;
#endif // DEBUG
		return move(BigNum("0"));
	}

	BigNum Two("2");
	BigNum Result("1");

	//惠璶腹
	if (_0_5_times % BigNum("2") != Zero)
	{
		/*-----------箉―猭-----------*/

		BigNum Value = lhs.Sign_Pos();//n

		BigNum Root = lhs.Sign_Pos();//x

		while (1)
		{
			/*
			f(x) = x^2 - n
			f'(x) = 2*x
			Xn =  x^2 - f(x)/f'(x)  =>  (x^2 + n)/2*n
			*/
			BigNum NewtonMethod = move(((Root*Root) + Value) / (BigNum("2")*Root));
			if (Root.FloatPoint_Cut(FLOAT_EFFECTIVE_RANGE) == NewtonMethod.FloatPoint_Cut(FLOAT_EFFECTIVE_RANGE))
			{
				break;
			}
			Root = NewtonMethod;
		}
		Result = Result *  Root;
	}
		
	//俱计Ωよ
	for (; _0_5_times >= Two ; ) {
		Result = Result * lhs;
		_0_5_times = _0_5_times - Two;
	}

	//璽Ωよ
	if (!rhs.sign) {
#if DEBUG >= 5
		cout << "BigNum::operator^(): working on power " << _0_5_times << "times of root" << endl;
#endif // DEBUG
		Result = move(BigNum("1") / Result);
	}
#if DEBUG>=3
	cout << "BigNum::Operator^(): " << lhs << " ^ " << rhs << " = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator!(const BigNum& lhs) {

	if (lhs.floatPosition != 0) {
		//獶俱计.
		return move(BigNum("0"));

#if DEBUG>=3
		cout << "BigNum::operator!(),lhs.floatPosition=" << lhs.floatPosition << endl;
		cout << "BigNum::operator!(),lhs=" << lhs << endl;
#endif // DEBUG>=3

	}
	else if (lhs < BigNum("0")) {
		return move(BigNum("0"));

#if DEBUG>=3
		cout << "BigNum::operator!(),lhs < 0" << lhs.floatPosition << endl;
		cout << "BigNum::operator!(),lhs=" << lhs << endl;
#endif // DEBUG>=3
	}

	BigNum Current = lhs;
	BigNum Result = BigNum("1");
	BigNum one = BigNum("1");
	while (Current != one) {
		Result = move(Result * Current);
		Current = move(Current - BigNum("1"));
	}

#if DEBUG>=3
	cout << "BigNum::Operator!(): " << lhs << "! = " << Result << endl;
#endif // DEBUG>=3
	return move(Result);
}

BigNum operator-(const BigNum& rhs) {
	BigNum Result = rhs;
	Result.sign ^= 1;
	return move(Result);
}

BigNum operator+(const BigNum& rhs) {
	return rhs;
}



//==============COMPARISON==============//


bool operator==(const BigNum& lhs, const BigNum& rhs) {
	if ((lhs.floatPosition != rhs.floatPosition )||(lhs.value.size()!=rhs.value.size())||(lhs.sign!=rhs.sign)) {
		return false;
	}
	for (int i = 0; i < lhs.value.size(); i++) {
		if (lhs.value[i] != rhs.value[i])return false;
	}
	return true;

}

bool operator!=(const BigNum& lhs, const BigNum& rhs) {
	return !(lhs == rhs);
}

bool operator>(const BigNum& lhs, const BigNum& rhs) {
	//check if equal
	if (lhs == rhs)return false;


	bool reverse = false;

	if (lhs.sign&&rhs.sign);//do_nothing.
	else if (lhs.sign && !rhs.sign) return true;
	else if (!lhs.sign&&rhs.sign) return false;
	else reverse = true;


	//lhs俱计计ゑ耕ぶ
	if (lhs.value.size() - lhs.floatPosition < rhs.value.size() - rhs.floatPosition) {
		return false^reverse;
	}
	//lhs俱计计ゑ耕
	else if (lhs.value.size() - lhs.floatPosition > rhs.value.size() - rhs.floatPosition) {
		return true ^ reverse;
	}
	//俱计计妓
	else {
		int max = lhs.value.size() < rhs.value.size() ? lhs.value.size() : rhs.value.size();
		for (int i = 1; i <= max; i++) {
			if (*(lhs.value.end() - i) > *(rhs.value.end() - i))
				return true ^ reverse;
			else if (*(lhs.value.end() - i) < *(rhs.value.end() - i)) {
				return false ^ reverse;
			}
		}
		if (lhs.value.size() < rhs.value.size()) {
			//rhs计计ゑ耕
			return false ^ reverse;
		}
		else {
			return true ^ reverse;
		}
	}	
}

bool operator<(const BigNum& lhs, const BigNum& rhs) {
	//check if equal
	if (lhs == rhs)return false;


	bool reverse = false;

	if (lhs.sign&&rhs.sign);//do_nothing.
	else if (lhs.sign && !rhs.sign) return false;
	else if (!lhs.sign&&rhs.sign) return true;
	else reverse = true;


	//lhs俱计计ゑ耕ぶ
	if (lhs.value.size() - lhs.floatPosition < rhs.value.size() - rhs.floatPosition) {
		return true ^ reverse;
	}
	//lhs俱计计ゑ耕
	else if (lhs.value.size() - lhs.floatPosition > rhs.value.size() - rhs.floatPosition) {
		return false ^ reverse;
	}
	//俱计计妓
	else {
		int max = lhs.value.size() < rhs.value.size() ? lhs.value.size() : rhs.value.size();
		for (int i = 1; i <= max; i++) {
			if (*(lhs.value.end()-i) > *(rhs.value.end() - i))
				return false ^ reverse;
			else if (*(lhs.value.end() - i) < *(rhs.value.end() - i)) {
				return true ^ reverse;
			}
		}

		if (lhs.value.size() > rhs.value.size()) {
			//俱计计常妓τrhs计计ゑ耕	
			return false ^ reverse;
		}
		else {
			return true ^ reverse;
		}
	}
}

bool operator>=(const BigNum& lhs, const BigNum& rhs) {
	return lhs == rhs || lhs > rhs;
}

bool operator<=(const BigNum& lhs, const BigNum& rhs) {
	return lhs == rhs || lhs < rhs;
}




//==============USER CONVERSION==============//

BigNum::operator long long int() const {
	
	if (floatPosition != 0) {
#if DEBUG >=4
		cout << "BigNum:operator long long int(), floatPosition!=0" << endl;
#endif // DEBUG
		return BigNum("0");
	}
	else if (*this > BigNum("9223372036854775808")) {
#if DEBUG >= 4 
		cout << "BigNum:operator long long int(), *this >  9223372036854775808" << endl;
#endif // DEBUG
		return BigNum("0");
	}

	long long int scaler = 1;
	long long int result = 0;

	for (int i = 0; i < value.size(); i++) {
		result += value[i]*scaler;
		scaler *= 10;
	}
	int sign_int = 1;
	if (!sign) sign_int = -1;

	return result*sign_int;
}

BigNum::operator  int() const {

	if (floatPosition != 0) {
#if DEBUG >= 4
		cout << "BigNum:operator long long int(), floatPosition!=0" << endl;
#endif // DEBUG
		return BigNum("0");
	}
	else if (*this > BigNum("2147483648")) {
#if DEBUG >= 4
		cout << "BigNum:operator long long int(), *this >  2147483648" << endl;
#endif // DEBUG
		return BigNum("0");
	}

	
	int scaler = 1;
	int result = 0;

	for (int i = 0; i < value.size(); i++) {
		result += value[i] * scaler;
		scaler *= 10;
	}

	int sign_int = 1;
	if (!sign) sign_int = -1;

	return result*sign_int;
}

BigNum::operator double() const {
	string ret_str;

	if (!sign) ret_str.push_back('-');

	for (int i = value.size() - 1; i >= 0; i--) {
		if (i == floatPosition&&floatPosition!=0) {
			ret_str.push_back('.');
			continue;
		}
		ret_str.push_back('0' + value[i]);
	}

	double ret_double = strtold(ret_str.c_str(), nullptr);

#if DEBUG>=3
	cout << ret_str << " -D-O-U-B-L-E-> " << ret_double << endl;;
#endif
	return ret_double;
}



/* ========== Sign ========== */
BigNum BigNum::Sign_Pos() const {
	BigNum Result(*this);
	Result.sign = true;
	return  move(Result);
}

BigNum BigNum::Sign_Neg() const {
	BigNum Result(*this);
	Result.sign = false;
	return  move(Result);
}

BigNum BigNum::Sign_Switch() const {
	BigNum Result(*this);
	Result.sign ^= 1;
	return  move(Result);
}

void BigNum::Sign_Set(bool sign){ 
	this->sign = sign;
}
//=============             ==============//
void BigNum::Refine() {

	//崩絋玂秈
	value.push_back(0);
	for (int i = 0; i < value.size()-1; i++) {
		value[i+1] += (value[i] / 10);
		value[i] %= 10;
	}

	//浪琩Τ⊿Τ秈⊿Τ碞р穝糤0奔
	if (*(value.end()-1) == 0) {
		value.erase(value.end() - 1);
	}
}



void BigNum::Vec_Multiply(const vector<int>& source_1,const vector<int>& source_2) {
	value.clear();
	for (int i = 0; i < source_1.size(); i++) {
		for (int j = 0; j < source_2.size(); j++) {
			if (i + j + 1 > value.size() ) {
				value.push_back(0);
			}
			value[i + j] += source_1[i] * source_2[j];
		}
	}
	Refine();
}

//start = the starting element (skips start-1 elements);
//length = the length of the result.
vector<int> BigNum::Vec_Cut(int start, int length) {
	int rm_elements = (start - 1);
	if ((start-1)+length> value.size()|| start > value.size() || start <0 || length < 0) {
#if DEBUG >= 3
		cout << "Vec_Cut(): Error!" << endl;
		cout << "\t\"value.size()\":" << value.size() << endl;
		cout << "\t\"length\":" << length << endl;
		cout << "\t\"start\":" << start << endl;
#endif // DEBUG
		return move(vector<int>());
	}

	vector<int> Return = value;
	Return.erase(Return.begin() + rm_elements + length, Return.end());
	Return.erase(Return.begin(), Return.begin() + rm_elements);
	return move(Return);
}


//============= Number Authentications ==============//
void BigNum::Num_CheckFraction() {
	if (floatPosition > FLOAT_MAX) {
#if DEBUG >=4
		cout << "BigNum::Num_CheckFraction():Input Value has Fraction(" << floatPosition << ") larger than " << FLOAT_MAX << "!" << endl;
#endif // DEBUG


		//erase overflowing zeros;
		value.erase(value.begin(), value.begin() + (floatPosition - FLOAT_MAX));
		floatPosition = FLOAT_MAX;

#if (DEBUG >= 3)
		cout << "BigNum::Num_CheckFraction():" << endl;
		cout << "\t*this = " << *this << endl;
#endif // (DEBUG > 3)
	}
}

void BigNum::Num_CheckRedundant() {
	//erase leading zeros;

	for (int i = value.size() - 1; i > floatPosition - 1; i--) {
		if (value.back() != 0)break;
		else {
			value.pop_back();
		}
	}


	//prevent empty value;
	if (value.size() == floatPosition) {
		value.push_back(0);
	}

	//erase tailing zeros;
	int zerofloats = 0;
	for (int i = 0; i<floatPosition; i++) {
		if (value[i] == 0) {
			zerofloats += 1;
		}
		else break;
	}
	if (zerofloats)
	{
		value.erase(value.begin(), value.begin() + zerofloats);
		floatPosition -= zerofloats;
	}

#if (DEBUG >= 3)
	cout << "DEBUG:" << 3 << ",CheckRedundant:" << endl;
	cout << "\t*this = " << *this << endl;
#endif // (DEBUG > 3)
}

bool BigNum::Num_CheckClose() {
	return Num_CheckCloseTo1() || Num_CheckCloseTo0();
}

bool BigNum::Num_CheckCloseTo1()
{
	if (floatPosition >= FLOAT_EFFECTIVE_RANGE) {
		for (int i = floatPosition - FLOAT_EFFECTIVE_RANGE; i < floatPosition; i++) {
			if (value[i] != 9) {
#if DEBUG >= 5
				cout << "BigNum::CheckCloseTo1(): not all 9, not changes." << endl;
#endif // DEBUG
				goto End;
			}
		}
#if DEBUG >= 5
		cout << "BigNum::CheckCloseTo1(): all 9, remove fractions, +1.0 ." << endl;
#endif // DEBUG

		value.erase(value.begin(), value.begin() + floatPosition);
		floatPosition = 0;
		value[0] += 1;
		return true;
	}

End:
#if (DEBUG >= 3)
	cout << "BigNum::CheckCloseTo1():" << endl;
	cout << "\t*this = " << *this << endl;
#endif // (DEBUG > 3)
	return false;
}

bool BigNum::Num_CheckCloseTo0() {
	if (floatPosition >= FLOAT_EFFECTIVE_RANGE) {
		for (int i = floatPosition - FLOAT_EFFECTIVE_RANGE; i < floatPosition; i++) {
			if (value[i] != 0) {
#if DEBUG >= 5
				cout << "BigNum::CheckCloseTo0(): not all 0, not changes." << endl;
#endif // DEBUG
				goto End;
			}
		}

#if DEBUG >= 5
		cout << "BigNum::CheckCloseTo0(): all 0, remove fractions." << endl;
#endif // DEBUG

		value.erase(value.begin(), value.begin() + floatPosition);
		floatPosition = 0;
		return true;
	}

End:
#if (DEBUG >= 3)
	cout << "BigNum::CheckCloseTo0():" << endl;
	cout << "\t*this = " << *this << endl;
#endif // (DEBUG > 3)
	return false;
}
//=============                       ==============//

BigNum BigNum::FloatPoint_Swap(int adjFloatPosition) const{

	if (adjFloatPosition == floatPosition) {
		return *this;
	}

	BigNum Result = *this;

	if (adjFloatPosition > floatPosition) {
		if (adjFloatPosition > Result.value.size()) {
			for (int i = 0; i < adjFloatPosition - floatPosition + 1; i++) {
				Result.value.push_back(0);
			}
		}
		Result.floatPosition = adjFloatPosition;
	}
	else if (adjFloatPosition < 0) {
		for (int i = adjFloatPosition; i < 0; i++) {
			Result.value.insert(Result.value.begin(), 0);
		}
		Result.floatPosition = 0;
	}
	else {
		Result.floatPosition = adjFloatPosition;
	}

	Result.Num_CheckRedundant(); 
	return move(Result);
}

BigNum BigNum::FloatPoint_Cut(int length)const {
	if (length > floatPosition) {
#if  DEBUG >= 7
		cout << "BigNum::FloatPoint_Cut(),length(" << length << ") >" << "floatPosition(" << floatPosition << ")." << endl;
#endif // DEBUG
		return *this;
	}
	BigNum Result = *this;
	Result.value.erase(Result.value.begin(),Result.value.begin()+Result.floatPosition-length);
	Result.floatPosition = length;
	return move(Result);
}

BigNum BigNum::GCD(const BigNum& rhs) const{
	if (rhs.floatPosition || this->floatPosition) {
		return move(BigNum("1"));
	}

	//礚阶锣Θタ计
	BigNum Me(*this); Me.sign = true;
	BigNum cp_RHS = rhs; cp_RHS.sign = true;


	BigNum Zero("0");

	while ((Me != Zero) && (cp_RHS != Zero)) {
		if (Me > cp_RHS)
		{
			Me = Me % cp_RHS;
		}
		else {
			cp_RHS = cp_RHS % Me;
		}
	}

	if (Me != Zero) {
		Me.sign = !(sign^rhs.sign);
		return move(Me);
	}
	else {
		return move(cp_RHS);
	}
}