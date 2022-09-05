#include "Token.h"
#include <string>
#include <iostream>

using namespace std;

Token::Token() {}

Token::Token(string s) {
	this->value = s;
}

string Token::getValue() {
	return this->value;
}

bool Token::equals(string other) {
	cout << "[DEBUG] Call token equals" << endl;
	if (value.compare(other) == 0) {
		return true;
	}
	else {
		return false;
	}
}

Keyword::Keyword(string s) {
	this->value = s;
}

bool Keyword::isKeyword() {
	cout << "[DEBUG] Call Keyword::isKeyword" << endl;
	return true;
}

bool Keyword::isName() {
	return false;
}

bool Keyword::isConstant() {
	return false;
}

Operator::Operator(string s) {
	this->value = s;
}

bool Operator::isKeyword() {
	cout << "[DEBUG] Call Operator::isKeyword" << endl;
	return false;
}

bool Operator::isName() {
	return false;
}

bool Operator::isConstant() {
	return false;
}

Symbol::Symbol(string s) {
	this->value = s;
}

bool Symbol::isKeyword() {
	cout << "[DEBUG] Call Symbol::isKeyword" << endl;
	return false;
}

bool Symbol::isName() {
	return false;
}

bool Symbol::isConstant() {
	return false;
}
