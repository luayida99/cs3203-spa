#include "DesignExtractor.h"
#include "EntityNode.h"
#include <vector>
#include <algorithm>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode* program) {
	this->program = program;
}

DesignExtractor::DesignExtractor()
{
}

StatementExtractor::StatementExtractor(ProgramNode* program)
{
	this->program = program;
}

VariableExtractor::VariableExtractor(ProgramNode* program)
{
	this->program = program;
}

// DRY needs improvement

ProcedureExtractor::ProcedureExtractor(ProgramNode* program)
{
	this->program = program;
}

vector<Procedure*> ProcedureExtractor::extract(PopulateFacade storage) {
	vector<Procedure*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(new Procedure(procList.at(i)->getName()));
	}

	storage.storeProcedures(&result);
	return result;
}

vector<Statement*> StatementExtractor::extract(PopulateFacade storage) {
	vector<Statement*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		cout << stmtList.size() << endl;
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementType type;
			//cout << stmtList.at(i) << endl;
			if (stmtList.at(j)->isRead()) {
				type = StatementType::READ;
			}
			else if (stmtList.at(j)->isPrint()) {
				type = StatementType::PRINT;
			}
			else if (stmtList.at(j)->isAssign()) {
				type = StatementType::ASSIGN;
			}
			else if (stmtList.at(j)->isCall()) {
				type = StatementType::CALL;
			}
			else {
				type = StatementType::NONE;
			}
			//cout << stmtList.at(i).getLineNumber() << endl;
			//cout << static_cast<int>(type) << endl;
			Statement* statement = new Statement(stmtList.at(j)->getLineNumber(), type);
			result.push_back(statement); //implement string Statement::getName(); ie. getLineNumber()
		}
	}

	storage.storeStatements(&result);
	return result;
}


vector<Variable*> VariableExtractor::extract(PopulateFacade storage) {
	vector<string> preresult;
	vector<Variable*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getVariablesInto(preresult);
		}
	}

	sort(preresult.begin(), preresult.end());
	preresult.erase(unique(preresult.begin(), preresult.end()), preresult.end());

	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(new Variable(preresult[i]));
	}

	storage.storeVariables(&result);
	return result;
}

ConstantExtractor::ConstantExtractor(ProgramNode* program)
{
	this->program = program;
}

vector<Constant*> ConstantExtractor::extract(PopulateFacade storage) {
	vector<string> preresult;
	vector<Constant*> result;

	vector<ProcedureNode*> procList = program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			if (currStmt->isAssign()) {
				currStmt->getConstantsInto(preresult);
			}
		}
	}
	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(new Constant(preresult[i]));
	}
	storage.storeConstants(&result);
	return result;
}
