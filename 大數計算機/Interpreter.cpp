#include "Interpreter.h"
//#define Show
map<string, Decimal> Interpreter::GLOBAL_VAR_MAP;

vector<BaseCalcObj*>* Interpreter::Converter(string command) {
	vector<BaseCalcObj*> * out = new vector<BaseCalcObj*>;
	string variableName = "";//變數名稱(被派值 or 創造)
	string expression = "";//運算式
	int mode = checkMode(command, variableName, expression);//0為正常運算 1為dec  2為int 3派值
	bool pass = checkright(expression);//確認算式正確
	if (!pass) {
		cout << "Error\n";
		return new vector<BaseCalcObj*>;
	}

	//正負號轉換 & 簡化
	changeSign(expression);
	//模式
	switch (mode)
	{
	case 0://一般運算
#ifdef Show
		cout << "運算" << expression << "___\n";
#endif 
		break;
	case 1://Set Decimal

	case 2://Set Interger
	
	case 3://派值
#ifdef Show		
		cout << variableName << " be set to " << expression << "___\n";
#endif		
		//加入變數 和 ' = '
		
		if (mode == 2)
		{
			GLOBAL_VAR_MAP[variableName] = Decimal();
#ifdef Show
			cout << "Create Decimal " << variableName << " = " << expression << "___\n";

#endif
			GLOBAL_VAR_MAP[variableName].setPureInt(true);
		}
		else if (mode == 1) {
			GLOBAL_VAR_MAP[variableName] = Decimal();
#ifdef Show
			cout << "Create Interger " << variableName << " = " << expression << "___\n";
#endif	
			GLOBAL_VAR_MAP[variableName].setPureInt(false);
		}

		{BaseCalcObj*x = new CalcObj_variable(variableName);
		x->setPrior(0);
		out->push_back(x);
		BaseCalcObj*y = new CalcObj_assign("=");
		y->setPrior(-1);
		out->push_back(y); }
		break;
	default:
		return new vector<BaseCalcObj*>;
		break;
	}
	//將數字轉為object
	toUnit(expression, out);
	return out;
}

Decimal& Interpreter::getGVM(string id) {
	if (!GLOBAL_VAR_MAP.count(id)) {
		cout << "Error! Variable " << id << " Has Not Been Set." << endl;
		return GLOBAL_VAR_MAP["default"];
	}
	return GLOBAL_VAR_MAP[id];
}

Interpreter::Interpreter()
{
} 

Interpreter::~Interpreter()
{
	for (int i = 0; i < obj_list->size(); i++) {
		delete (*obj_list)[i];
	}
}

vector<BaseCalcObj*> Interpreter::Converter_Save(string command) {
	obj_list =  Interpreter::Converter(command);
	return *obj_list;
}


/* ========== TOOLS ========== */

int Interpreter::checkMode(string in, string &var, string &expression) {//0為正常運算 1為dec  2為int 3為派值 4為輸入格式錯誤
	int variablePlace = 0, mode = 0;
	int atInt = in.find("Set Integer "), atDec = in.find("Set Decimal ");
	if (atInt == 0) {
		variablePlace = 12;
	}
	else if (atDec == 0) {
		variablePlace = 12;
	}
	while (ischar(in[variablePlace])) {
		var += in[variablePlace];
		variablePlace++;
	}
	while (in[variablePlace] == ' ') {
		variablePlace++;
	}
	if (in[variablePlace] == '=') {
		expression.assign(in.begin() + variablePlace + 1, in.end());
		if (atDec == 0) {
			mode = 1;
#ifdef Show
			cout << "Mode :" << mode << " = Set dec\n";
#endif		
		}
		else if (atInt == 0) {
			mode = 2;
#ifdef Show			
			cout << "Mode :" << mode << " = Set int\n";
#endif		
		}
		else if (mode == 0) {
			mode = 3;
#ifdef Show			
			cout << "Mode :" << mode << " = Give num\n";
#endif
		}
	}
	else {
		expression.assign(in.begin(), in.end());
#ifdef Show
		cout << "Normal calculous\n";
#endif	
	}
	return mode;
}

//Check
bool Interpreter::checkright(string in) {

	/*if (in == "")return false;
	//確認非法字元
	for (auto i:in) {
		if (!(isnum(i) || ischar(i) || checkoperator(i) || i==' ' || i=='.'||i=='=')) {
			return false;
		}
	}
	//確認變數相鄰
	int variablecount = 0;
	for (int i = 0; i < in.length();i++) {
		bool findVariable=false;

		while (ischar(in[i]) )
		{
			if (!findVariable) {
				if ( i != 0 && (  (in[i - 1] == ')') || (in[i - 1] == '!')  || (in[i - 1] == '.') || (in[i - 1] == '!')   )) return false;//ex:  (1+1)apple
					findVariable = true;
			}
			i++;//找到變數後一位
	}
	if (findVariable) {
		while(in[i]==' ') {
			i++;
		}
	if ( ischar(in[i]) || isnum(in[i]) || in[i]=='('  )return false;//變數後接 "變數" "數字" '(' 都不行
	}
	}
	//運算子不能相鄰(正負號前可遇到運算子)
	for (int i = 0; i < in.length(); i++) {
	}*/
	return true;
}

bool Interpreter::checkoperator(char c) {
	string ope = "()!^+-*/";
	for (int i = 0; i < ope.length(); i++) {
		if (ope[i] == c)return true;
	}
	return false;
}

bool Interpreter::ischar(char c) {
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
		return true;
	}
	return false;
}

//數字
bool Interpreter::isnum(char c) {
	if ('0' <= c  && c <= '9')return true;
	return false;
}

//加減正負判斷  +-#_
bool Interpreter::is_plus_minus(char c) {
	if (c == '-' || c == '+' || c == '#' || c == '_')return true;
	return false;
}

//operator
bool Interpreter::is_operator(char c) {
	string ope = "()!^+-*/#_";
	for (int i = 0; i < ope.length(); i++) {
		if (ope[i] == c)return true;
	}
	return false;
}

//把正負號改變 並且簡化
void Interpreter::changeSign(string &in) {
	for (int i = 0; i < in.length() - 1; i++) {
		if (in[i] == ' ') {//消除空白
			in.erase(in.begin() + i);
			i--;
			continue;
		}
		if (is_plus_minus(in[i])) {
			if (i == 0) {//運算式中 最左邊必為負號
				if (in[i] == '-')in[i] = '_';
				else in[i] = '#';
				continue;
			}
			else if (in[i - 1] == '(') {//左邊有 '('  為正負號
				if (in[i] == '-')in[i] = '_';
				else in.erase(i, 1);
			}
			else if (in[i - 1] == '*' || in[i - 1] == '/' || in[i - 1] == '^') {
				if (in[i] == '-')in[i] = '_';
				else in[i] = '#';
			}
			else if (in[i + 1] == '(') {//右邊有'(' 維持原樣

			}else if ((is_operator(in[i - 1]) && is_operator(in[i + 1]))) {//左右兩邊都是operator 歸類為正負號
				if (in[i] == '-')in[i] = '_';
				else in[i] = '#';
			}
		}
	}

	int at = 0;
	while (in[at]) {
		int signFlag = 1;//1為+ -1為-
		bool hasChange = false;
		//最初遇到的+-為起始 遇到非+-#_停止  用 '+','-','#','_'替代
		while ((at + 1 < in.length()) && is_plus_minus(in[at]) && is_plus_minus(in[at + 1]))
		{
			hasChange = true;
			if (in[at + 1] == '#' || in[at + 1] == '+') {
				in.erase(at + 1, 1);
				continue;
			}
			else if (in[at + 1] == '-' || in[at + 1] == '_')
			{
				signFlag *= -1;//紀錄遇到負號
				in.erase(at + 1, 1);
				continue;
			}

		}
		if (hasChange) {//有削去符號
			if (in[at] == '-' || in[at] == '_')signFlag *= -1;//起始符號
			in.erase(at, 1);
			if (at == 0) {
				if (signFlag == -1)in.insert(in.begin() + at, '_');//式子最左邊 負號'_' 正號不處理
			}
			else {
				if (in[at - 1] == '*' || in[at - 1] == '/' || in[at - 1] == '^' || in[at - 1] == '(') {//左邊有*/^  負號 '_' 正號不處理  
					if (signFlag != 1)in.insert(in.begin() + at, '_');
				}
				else {
					if (signFlag == 1)in.insert(in.begin() + at, '+');
					else in.insert(in.begin() + at, '-');
				}
			}
		}
		at++;
	}
}

//get pri
int Interpreter::getPriority(char c, int &basepri) {
	if (isnum(c))return 0;
	if (ischar(c))return 0;
	if (c == '+' || c == '-')return 1 + basepri;
	if (c == '*' || c == '/')return 2 + basepri;
	if (c == '#' || c == '_')return 3 + basepri;
	if (c == '^')return 4 + basepri;
	if (c == '!')return 5 + basepri;
	if (c == '(') {
		basepri += 6;
		return basepri;
	}
	if (c == ')') {
		basepri -= 6;
		return basepri;
	}
}

//to Unit
void Interpreter::toUnit(string in, vector<BaseCalcObj*>* out) {
	int basepri = 0;
	int factorialNum = 0;//  !連續疊加
	int powerNum = 0;//^連續疊加
	char previousElement = '`';
	for (int i = 0; i < in.length(); i++) {
		string varbuff = "";

		if (ischar(in[i])) {//var
			while (ischar(in[i]) || isnum(in[i]))
			{
				varbuff.push_back(in[i]);
				i++;
			}
			i--;
			BaseCalcObj*x = new CalcObj_variable(varbuff);
			x->setPrior(0);
			out->push_back(x);
#ifdef Show
			cout << "got Var : " << varbuff << " pri=0" << getPriority(varbuff[0], basepri) << "\n";
#endif
		}
		else if (isnum(in[i])) {//num
			string numbuff = "";
			while (isnum(in[i]) || in[i] == '.') {
				numbuff.push_back(in[i]);
				i++;
			}
			i--;
			BaseCalcObj*x = new CalcObj_num(numbuff);
			x->setPrior(0);
			out->push_back(x);
#ifdef Show
			cout << "got Num : " << numbuff << " pri=0" << "\n";
#endif
		}
		else if (is_operator(in[i])) {

			char sign = in[i];
			int pri = getPriority(sign, basepri);

			if (previousElement == sign &&  sign == '^')powerNum += 6;
			else powerNum = 0;

			if (previousElement == sign &&  sign == '!')factorialNum -= 6;
			else factorialNum = 0;

			if (sign == '!')
			{
				BaseCalcObj* x = new  CalcObj_factorial("!");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif			
			}
			else if (sign == '^')
			{
				BaseCalcObj* x = new CalcObj_power("^");
				x->setPrior(pri + powerNum);
				out->push_back(x);
#ifdef Show		
				cout << "got sign " << sign << " pri=" << pri + powerNum << "\n";
#endif			
			}
			else if (sign == '+')
			{
				BaseCalcObj* x = new CalcObj_plus("+");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show		
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif			
			}
			else if (sign == '-')
			{
				BaseCalcObj* x = new CalcObj_minus("-");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show		
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif			
			}
			else if (sign == '*')
			{
				BaseCalcObj* x = new CalcObj_multi("*");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show				
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif
			}
			else if (sign == '/')
			{
				BaseCalcObj* x = new CalcObj_div("/");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show		
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif			
			}
			else if (sign == '_')
			{
				BaseCalcObj* x = new CalcObj_neg("_");
				x->setPrior(pri);
				out->push_back(x);
#ifdef Show		
				cout << "got sign " << sign << " pri=" << pri << "\n";
#endif			
			}
			previousElement = sign;
		}
	}
}