#include "stack.h"
#include <cstdio>
#include <iostream>

using namespace std;

void convert(stack<char> &s, __int64 n, int base) { //十进制数n到base进制的转换（迭代版）
	static char digit[] 
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n > 0) {
		int remainder = (int)(n % base);
		s.push(digit[remainder]);
		n /= base;
	}
}

bool paren(const char exp[], int lo, int hi) { //符号匹配
	stack<char> myS;
	for (int i = lo; exp[i] ; i++) {
		switch (exp[i])
		{
		case '(':
		case '[':
		case'{':
			myS.push(exp[i]);
			break;
		case  ')': if (myS.empty() || '(' != myS.pop()) return false; break;
		case  ']': if (myS.empty() || '[' != myS.pop()) return false; break;
		case  '}': if (myS.empty() || '{' != myS.pop()) return false; break;
		}
	}
	return myS.empty();
}
int main() {
	int num[] = { 5,1,3,2,6,4,8,9,7,0 };
	cout << "++++++++++stack test++++++++++" << endl;
	stack<int> s;
	for (int i = 0; i < 10; i++) {
		s.push(num[i]);
	}
	for (int i = 0; i < 10; i++) {
		cout << s.pop();
	}
	cout << endl;
	cout << "++++++++++convert test++++++++++" << endl;
	stack<char> s1;
	convert(s1, 102400, 8);
	for (int i = 0; i < 10; i++) {
		cout << s1.pop();
	}
	cout << endl;
	cout << "++++++++++paren test++++++++++" << endl;
	char exp[] = "[[(a+b)]]+()+([c+d]*a())";
	if (paren(exp, 0, strlen(exp))) cout << "true" << endl;
	else  cout << "false" << endl;

	char exp1[] = "[[(a+b)]]+()+([c+d]*a())()[[[][]]]]]]";
	if (paren(exp1, 0, strlen(exp1))) cout << "true" << endl;
	else  cout << "false" << endl;
}