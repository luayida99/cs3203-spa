#include "EntityNode.h"
#include "ExtractUtils.h"
#include <vector>

EntityNode::EntityNode(){}
StatementNode::StatementNode() { this->line = 0; }

// Program
ProgramNode::ProgramNode(std::vector<ProcedureNode*> procList) {
	this->procList = procList;
}

ProgramNode::ProgramNode() {}

std::vector<ProcedureNode*> ProgramNode::getProcList() {
	return this->procList;
}

bool ProgramNode::equals(ProgramNode* other) {
	std::vector<ProcedureNode*> procedures = this->getProcList();
	std::vector<ProcedureNode*> others = other->getProcList();
	return ExtractUtils::compareProcList(procedures, others);
}

// Procedure
ProcedureNode::ProcedureNode(std::string procName, std::vector<StatementNode*> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
}

bool ProcedureNode::equals(ProcedureNode* other) {
	if (this->getName() != other->getName()) {
		return false;
	}
	std::vector<StatementNode*> statements = this->getStmtList();
	std::vector<StatementNode*> others = other->getStmtList();
	return ExtractUtils::compareStmtList(statements, others);
}

std::string ProcedureNode::getName() {
	return this->procName;
}

std::vector<StatementNode*> ProcedureNode::getStmtList() {
	return this->stmtList;
}

int ProcedureNode::getEndline() {
	return stmtList.back()->getEndLine();
}


// Statement - equals
bool ReadStatementNode::equals(StatementNode* other) {
	return other->isRead() && this->getVariable() == other->getVariable();
}

bool PrintStatementNode::equals(StatementNode* other) {
	return other->isPrint() && this->getVariable() == other->getVariable();
}

bool CallStatementNode::equals(StatementNode* other) {
	return other->isCall() && this->getVariable() == other->getVariable();
}

bool AssignStatementNode::equals(StatementNode* other) {
	// Expressions not checked
	return other->isAssign() && this->getVariable() == other->getVariable();
}

bool WhileStatementNode::equals(StatementNode* other) {
	if (other->isWhile()) {
		WhileStatementNode* temp = static_cast<WhileStatementNode*>(other);
		return this->cond->equals(temp->cond) && ExtractUtils::compareStmtList(this->getStmtList(), temp->getStmtList());
	}
	else {
		return false;
	}
}

// Read Statement
ReadStatementNode::ReadStatementNode(VariableNode* VariableNode, int line) {
	this->var = VariableNode;
	this->line = line;
}

std::string ReadStatementNode::getVariable() {
	return this->var->getValue();
}

void ReadStatementNode::getVariablesInto(std::vector<std::string>& result) {
	result.push_back(this->getVariable());
}

void ReadStatementNode::getStatementsInto(std::vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::READ)); 
}

// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode* VariableNode, int line ) {
	this->var = VariableNode;
	this->line = line;
}

std::string PrintStatementNode::getVariable() {
	return this->var->getValue();
}

void PrintStatementNode::getVariablesInto(std::vector<std::string>& result) {
	result.push_back(this->getVariable());
}

void PrintStatementNode::getStatementsInto(std::vector<Statement*>& result) {
	result.push_back(new Statement(line, StatementType::PRINT)); 
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode* VariableNode, int line ) {
	this->var = VariableNode;
	this->line = line;
}

std::string CallStatementNode::getVariable() {
	return this->var->getValue();
}

void CallStatementNode::getStatementsInto(std::vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::CALL)); 
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode* VariableNode , ExpressionNode* expression, int line) {
	var = VariableNode;
	expr = expression;
	this->line = line;
}

std::string AssignStatementNode::getVariable() {
	return this->var->getValue();
}

void AssignStatementNode::getVariablesInto(std::vector<std::string>& result) {
	result.push_back(this->getVariable());
	this->expr->getVariablesInto(result);
}

void AssignStatementNode::getConstantsInto(std::vector<std::string>& result) {
	this->expr->getConstantsInto(result);
}

void AssignStatementNode::getStatementsInto(std::vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::ASSIGN)); 
}

// While Statement
WhileStatementNode::WhileStatementNode(const std::vector<StatementNode*>& stmtList, ExpressionNode* cond, int line)
{
	this->stmtList = stmtList;
	this->cond = cond;
	this->line = line;
}

int WhileStatementNode::getEndLine() {
	return this->stmtList.back()->getEndLine();
}

void WhileStatementNode::getVariablesInto(std::vector<std::string>& result)
{
	cond->getVariablesInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getVariablesInto(result);
	}
}

void WhileStatementNode::getConstantsInto(std::vector<std::string>& result)
{
	cond->getConstantsInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getConstantsInto(result);
	}
}

void WhileStatementNode::getStatementsInto(std::vector<Statement*>& result)
{
	result.push_back(new Statement(line, StatementType::WHILE));
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getStatementsInto(result);
	}
}

void WhileStatementNode::getFollowsInto(std::vector<Relationship<int, int>*>& result) {
	ExtractUtils::follows(this->getStmtList(), result);
}

void WhileStatementNode::getFollowsTInto(std::vector<Relationship<int, int>*>& result) {
	ExtractUtils::followsT(this->getStmtList(), result);
}


// Expression
ExpressionNode::ExpressionNode(Token* token) {
	this->token = token;
	this->left = NULL;
	this->right = NULL;
}

ExpressionNode::ExpressionNode() {};

void ExpressionNode::getVariablesInto(std::vector<std::string>& result) {
	if (this->token->isName()) {
		result.push_back(this->token->getValue());
	}
	else if (this->token->isConstant()) {}
	else {
		this->left->getVariablesInto(result);
		this->right->getVariablesInto(result);
	}
}

void ExpressionNode::getConstantsInto(std::vector<std::string>& result) {
	if (this->token->isConstant()) {
		result.push_back(this->token->getValue());
	}
	else if (this->token->isName()) {}
	else {
		this->left->getConstantsInto(result);
		this->right->getConstantsInto(result);
	}
}

bool ExpressionNode::equals(ExpressionNode* other)
{
	if (this->left == NULL && this->right == NULL) {
		return other->left == NULL && other->right == NULL && this->token->equals(other->token);
	}
	else if (this->right == NULL) {
		if (other->left == NULL) return false;
		return this->token->equals(other->token) && this->left->equals(other->left);
	}
	else {
		if (other->left == NULL || other->right == NULL) { return false; }
		return this->token->equals(other->token) && this->left->equals(other->left) && this->right->equals(other->right);
	}

	
}


// Constant
ConstantNode ::ConstantNode (std::string s) {
	this->value = s;
}

// Variable
VariableNode ::VariableNode (std::string s) {
	this->value = s;
}

VariableNode ::VariableNode () {}


// If Statement
IfStatementNode::IfStatementNode(std::vector<StatementNode*>& ifBlock, std::vector<StatementNode*>& elseBlock, ExpressionNode* cond, int line)
{
	this->ifBlock = ifBlock;
	this->elseBlock = elseBlock;
	this->cond = cond;
	this->line = line;
}

int IfStatementNode::getEndLine()
{
	if (elseBlock.size() > 0) {
		return elseBlock.back()->getEndLine();
	}
	else {
		return ifBlock.back()->getEndLine();
	}
}

std::vector<StatementNode*> IfStatementNode::getStmtList()
{
	std::vector<StatementNode*> stmtList;
	for (size_t i = 0; i < ifBlock.size(); i++) {
		stmtList.push_back(ifBlock.at(i));
	}

	for (size_t i = 0; i < elseBlock.size(); i++) {
		stmtList.push_back(elseBlock.at(i));
	}

	return stmtList;
}

void IfStatementNode::getVariablesInto(std::vector<std::string>& result)
{
	std::vector<StatementNode*> stmtList = this->getStmtList();

	cond->getVariablesInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getVariablesInto(result);
	}
}

void IfStatementNode::getConstantsInto(std::vector<std::string>& result)
{
	std::vector<StatementNode*> stmtList = this->getStmtList();

	cond->getConstantsInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getConstantsInto(result);
	}
}

void IfStatementNode::getStatementsInto(std::vector<Statement*>& result)
{
	std::vector<StatementNode*> stmtList = this->getStmtList();
	result.push_back(new Statement(line, StatementType::IF));
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getStatementsInto(result);
	}
}

bool IfStatementNode::equals(StatementNode* other) {
	if (other->isIf()) {
		IfStatementNode* temp = static_cast<IfStatementNode*>(other);
		if (this->cond->equals(temp->cond) && this->ifBlock.size() == temp->ifBlock.size() && this->elseBlock.size() == temp->elseBlock.size()) {
			std::vector<StatementNode*> stmtList1 = this->getStmtList();
			std::vector<StatementNode*> stmtList2 = this->getStmtList();

			return ExtractUtils::compareStmtList(stmtList1, stmtList2);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void IfStatementNode::getFollowsInto(std::vector<Relationship<int, int>*>& result) {
	ExtractUtils::follows(this->ifBlock, result);
	ExtractUtils::follows(this->elseBlock, result);
}

void IfStatementNode::getFollowsTInto(std::vector<Relationship<int, int>*>& result) {
	ExtractUtils::followsT(this->ifBlock, result);
	ExtractUtils::followsT(this->elseBlock, result);
}

