#pragma once

#include "Parser.h"
#include "EntityNode.h"

class ExprParser : public Parser {
	bool iscond;
public:
	ExprParser(int offset, std::vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class TermParser : public Parser {
	bool iscond;
public:
	TermParser(int offset, std::vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class FactorParser : public Parser {
	bool iscond;
public:
	FactorParser(int offset, std::vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class CondParser : public Parser {
public:
	CondParser(int offset, std::vector<Token*> tokens) : Parser(offset, tokens) {};
	ExpressionNode* parse();
};
