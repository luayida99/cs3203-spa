#pragma once

#include <string>
#include <vector>
#include <ctype.h>
#include "./Token.h"

using namespace std;

string KEYWORD_LIST[];
char SYMBOL_LIST[];
char WHITESPACE_LIST[];
string OPERATOR_LIST[]; // rel & cond op not included yet
char opChar[];

enum TokenType {
	WHITESPACE, // act as default token type
	NAME,
	CONSTANT,
	KEYWORD, // word that decide stmt type 
	OPERATOR, // cond, rel, arithmetic symbols
	SYMBOL // parenthesis and ';'
};

class Tokenizer {
public:
	Tokenizer(const string& sourceProg);
	vector<Token> tokenize();
private:
	string& input;
	Token createToken(TokenType type, string value);
};
