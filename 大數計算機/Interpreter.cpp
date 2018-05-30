#include "Interpreter.h"
#include <string>
using namespace std;
//#define Show
map<string, Decimal> Interpreter::GLOBAL_VAR_MAP;


Decimal& Interpreter::getGVM(string id) {
	if (!GLOBAL_VAR_MAP.count(id)) {
		cout << "Error! Variable " << id << " Has Not Been Set." << endl;
		return GLOBAL_VAR_MAP["0"];
	}
	return GLOBAL_VAR_MAP[id];
}

vector<BaseCalcObj*>* Interpreter::Converter(string command) {
	int rmSpace = 0;
	while (command[rmSpace] == ' ') {//去除前面空白
		command.erase(0, 1);
	}
	vector<BaseCalcObj*> * out = new vector<BaseCalcObj*>;
	string variableName = "";//變數名稱(被派值 or 創造)
	string expression = "";//運算式
	int mode = checkMode(command, variableName, expression);//0為正常運算 1為dec  2為int 3派值
	if (mode == 4) {
		cout << "格式錯誤 請重新輸入\n";
		return out;
	}
	bool pass = checkright(expression);//確認算式正確
	if (!pass) {
		cout << "格式錯誤 請重新輸入\n";
		return out;
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
		if (mode == 1) {
#ifdef Show
			cout << "Create Decimal " << variableName << " = " << expression << "___\n";

#endif
			GLOBAL_VAR_MAP[variableName] = Decimal();
		}
		else if (mode == 2)
		{
#ifdef Show
			cout << "Create Interger " << variableName << " = " << expression << "___\n";
#endif	
			cout << "Integer Not Supported, Creating Decimal" << endl;
			GLOBAL_VAR_MAP[variableName] = Decimal();
		}

		//加入變數 和 ' = '
		{
			BaseCalcObj*x = new CalcObj_variable(variableName);
			x->setPrior(0);
			out->push_back(x);

			BaseCalcObj*y = new CalcObj_assign("=");
			y->setPrior(-1);
			out->push_back(y);
		}
		break;

	default:
		return out;
		break;
	}
	//將數字轉為object
	toUnit(expression, out);
	return out;
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

//判斷模式
int Interpreter::checkMode(string in, string &var, string &expression) {//0為正常運算 1為dec  2為int 3為派值 4為輸入格式錯誤
	int equalPlace = in.find('=');
	if (equalPlace == -1) {
		expression.assign(in.begin(),in.end());
		return 0;
	}
	string varCp="";
	bool getVar = false;
	varCp.assign(in.begin(),in.begin()+equalPlace);
	expression.assign(in.begin()+equalPlace+1, in.end());
	if (varCp.find("Set")==0) {
		int varInt = varCp.find("Integer");
		int varDec = varCp.find("Decimal");
		if (varInt != -1 && varDec != -1) {
#ifdef Show
			cout << "設定指令錯誤(同時設定 INT & DEC )!\n";
#endif	
			return 4;
		}if ( varInt!=-1 && (varInt-0)>=4) {
			for (int i = 4; i < varInt; i++) {
				if (varCp[i] != ' ') {
					#ifdef Show
					cout << "Integer前面錯誤! \n";
					#endif	
					return 4;
				}
			}
			for (int i = varInt + 8; i < varCp.length();i++) {
				if (varCp[i] == ' ')continue;
				else if (getVar) {
					#ifdef Show					
						cout << "變數名稱錯誤(set int var var)\n";
					#endif					
				}
				if (ischar(varCp[i])) {
					while ( (ischar(varCp[i]) || isnum(varCp[i])) && i<varCp.length() )
					{
						var.push_back(varCp[i]);
						i++;
					}
					getVar = true;
				}
				else if (isnum(varCp[i])) {
					#ifdef Show					
						cout << "變數名稱錯誤(數字在前)\n";
					#endif			
					return 4;
				}
			}
			if(var!="")return 2;
			else {
				#ifdef Show
				cout << "變數名稱空白\n";
				#endif
				return 4;
			}
		}else if (varDec!=-1 && (varDec - 0) >= 4) {
			for (int i = 4; i < varDec; i++) {
				if (varCp[i] != ' ') {
					#ifdef Show
					cout << "Dec前面錯誤! \n";
					#endif	
					return 4;
				}
			}
			for (int i = varDec + 8; i < varCp.length(); i++) {
				if (varCp[i] == ' ')continue;
				else if (getVar) {
					#ifdef Show					
						cout << "變數名稱錯誤(set int var var)\n";
					#endif					
				}
				if (ischar(varCp[i])) {
					while ((ischar(varCp[i]) || isnum(varCp[i])) && i<varCp.length())
					{
						var.push_back(varCp[i]);
						i++;
					}
					getVar = true;
				}else if (isnum(varCp[i])) {
					#ifdef Show					
						cout << "變數名稱錯誤(數字在前)\n";
					#endif			
					return 4;
				}
				
			}
			if (var != "")return 1;
			else {
				#ifdef Show
					cout << "變數名稱空白\n";
				#endif
				return 4;
			}
		}else {
			cout << "錯誤(沒有指定變數型態)\n";
			return 4;
		}
	}else {
		for (auto i:var) {
			if (!(isnum(i)||ischar(i))) {
#ifdef Show
				cout << "變數名稱錯誤\n";
#endif
				return 4;
			}
		}
		int at = 0;
		if (ischar(varCp[at])) {
			while (ischar(varCp[at])||isnum(var[at])) {
				var.push_back(varCp[at]);
				at++;
			}
			return 3;
		}
		else {
#ifdef Show			
			cout << "變數名稱錯誤\n";
#endif			
			return 4;
		}
	}
	return 0;
}

//Check
bool Interpreter::checkright(string in) {
	
	if (in == "")return false;

	//確認非法字元
 	for (auto i:in) {
		if (!(isnum(i) || ischar(i) || checkoperator(i) || i==' ' || i=='.')) {
			return false;
		}
	}
	//判斷括號正確(對稱)
	if (!checkBrackets(in)) return false;

	int i = 0,lastPosition,endPosition=in.length()-1;
	
	//忽略前面 後面 空白
	while (in[i]==' ') {
		i++;
	}
	while (in[endPosition]==' ') {
		endPosition--;
	}
	//首符號
	if (in[i] == '*' || in[i] == '!' || in[i] == '/' || in[i] == '^' || in[i] == '=' || in[i] == ')' || in[i]=='.') {
#ifdef Show
		cout << "First element Error!\n";
#endif 
		return false;
	}
	//尾符號
	if (  checkoperator(in[endPosition]) && in[endPosition]!=')'  && in[endPosition] != '!'  ) {
#ifdef Show
		cout << "Final element Error!\n";
#endif 
		return false;
	}

	bool firstFind=false;
	for (i,lastPosition = 0; i < in.length(); i++) {//開始
		if (in[i] == ' ')continue;
		if (ischar(in[i])) {
			if (!firstFind) {
				firstFind = true;
			}else {
				if ((in[lastPosition] == '!' || isnum(in[lastPosition]) || ischar(in[lastPosition]))) {//變數前不能有 ! num char
#ifdef Show
					cout << "variable before Error!";
#endif
					return false;
				}
			}
			while ( ischar(in[i]) || isnum(in[i])) {
				i++;
			}
			i--;
			lastPosition = i;//紀錄變數最後位置
		}
		//==========='('
		else if (in[i]=='(') {
			if (!firstFind) {
				firstFind = true;
			}
			else {
				if (in[lastPosition] == '!' || isnum(in[lastPosition]) || ischar(in[lastPosition]) || in[lastPosition]==')') {
#ifdef Show
					cout << "( before Error!";
#endif
					return false;
				}
			}
			lastPosition = i;
			while (in[i+1]==' ') {
				i++;
			}
			if ((checkoperator(in[i+1])&& (in[i+1]!='+' && in[i+1]!='-' && in[i + 1] != '(')  )&& i!=endPosition) {
#ifdef Show
				cout << "( after Error!";
#endif
				return false;
			}
		}
		//===========')'
		else if (in[i] == ')') {
			
			if (  checkoperator(in[lastPosition])&&in[lastPosition]!=')') {
#ifdef Show
				cout << " ) before Error!\n";
#endif
				return false;
			}
			lastPosition = i;
			while (in[i + 1] == ' ') {// ) 123a 
				i++;
			}
			if (!checkoperator(in[i + 1]) && i!=endPosition) {
#ifdef Show
				cout << ") after Error!\n";
#endif
				return false;
			}
		}
		else if ( isnum(in[i]) ) {
			int dotNums = 0,numNums=0;
			if (!firstFind) {
				firstFind = true;
			}
			else {
				if ((ischar(in[lastPosition]) || isnum(in[lastPosition]) || in[lastPosition] == '!')) {
#ifdef Show
					cout << "num before Error!\n";
#endif
					return false;
				}
			}
			while ( isnum(in[i]) || in[i]=='.' ) {
				if (numNums==0 && in[i]=='.') {
#ifdef Show
					cout << "before  '.' is empty !\n";
#endif
					return false;
				}else if (in[i] == '.')dotNums++;
				else numNums++;

				if (dotNums == 2) {
#ifdef Show
					cout << " '.' Error\n";
#endif
					return false;
				}
				i++;
			}
			i--;
			lastPosition = i;
		}
		else if (in[i]=='.') {
#ifdef Show
			cout << " '.' Error\n";
#endif
			return false;
		}
		else if (is_operator(in[i])) {
			switch (in[i])
			{
			case '*':
				if (in[i + 1] == '*' || in[i + 1] == '!' || in[i + 1] == '/' || in[i + 1] == '^') {
#ifdef Show
					cout << " '*' Error\n";
#endif
					return false;
				}
				break;
			case '/':
				if (in[i + 1] == '+' || in[i + 1] == '-' || in[i + 1] == '*' || in[i + 1] == '/' || in[i + 1] == '^' || in[i + 1] == '!') {
#ifdef Show
					cout << " '/' Error\n";
#endif
					return false;
				}
				break;
			case '^':
				if ( in[i + 1] == '*' || in[i + 1] == '/' || in[i + 1] == '^' || in[i + 1] == '!' ) {
#ifdef Show
					cout << " '^' Error\n";
#endif
					return false;
				}
				break;
			case '+':
				if (in[i + 1] == '*' || in[i + 1] == '/' || in[i + 1] == '^' || in[i + 1] == '!') {
#ifdef Show
					cout << " '+' Error\n";
#endif
					return false;
				}
				break;
			case '-':
				if (in[i + 1] == '*' || in[i + 1] == '/' || in[i + 1] == '^' || in[i + 1] == '!') {
#ifdef Show
					cout << " '-' Error\n";
#endif
					return false;
				}
				break;
			default:
				break;
			}


			

			lastPosition = i;
		}
		else {
			lastPosition = i;
		}
}
	return true;
}


//括號
bool Interpreter::checkBrackets(string in) {

	vector<char>stack;
	for (int i = 0; i < in.length(); i++) {
		if (in[i]=='(') {
			stack.push_back('(');
		}
		if (in[i] == ')') {
			if (stack.size() && stack[0] == '('){
				stack.pop_back();
				continue;
			}else{
				return false;
			}
		}
	}
	if (!stack.size()) {
		return true;
	}else {
#ifdef Show
		cout << "Brackets Error!\n";
#endif		
		return false;
	}
		
}
//operator   "不"包含正負 輸入檢測用
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

//operator 包含正負
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