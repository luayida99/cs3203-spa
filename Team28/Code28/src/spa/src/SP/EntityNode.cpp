#include "EntityNode.h"
#include <vector>

using namespace std;

EntityNode::EntityNode(){}
StatementNode::StatementNode(){}

ProgramNode::ProgramNode(vector<ProcedureNode> procList) {
	this->procList = procList;
}

vector<ProcedureNode> ProgramNode::getProcList() {
	return this->procList;
}

ProcedureNode::ProcedureNode(string procName, vector<StatementNode> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
}

string ProcedureNode::getName() {
	return this->procName;
}

vector<StatementNode> ProcedureNode::getStmtList() {
	return this->stmtList;
}

ReadStatementNode::ReadStatementNode(VariableNode  VariableNode ) {
	this->var = VariableNode ;
}

PrintStatementNode::PrintStatementNode(VariableNode  VariableNode ) {
	this->var = VariableNode ;
}

CallStatementNode::CallStatementNode(VariableNode  VariableNode ) {
	this->var = VariableNode ;
}

AssignStatementNode::AssignStatementNode(VariableNode  VariableNode , ExpressionNode expression) {
	var = VariableNode ;
	expr = expression;
}

ExpressionNode::ExpressionNode(Token token)
{
	this->token = token;
	this->left = nullptr;
	this->right = nullptr;
}

ExpressionNode::ExpressionNode() {};

ConstantNode ::ConstantNode (string s) {
	this->value = s;
}

bool ConstantNode::isKeyword() {
	return false;
}

bool ConstantNode ::isName() {
	return false;
}

VariableNode ::VariableNode (string s) {
	this->value = s;
}

VariableNode ::VariableNode () {}

bool VariableNode ::isKeyword() {
	return false;
}

bool VariableNode ::isName() {
	return true;
}
