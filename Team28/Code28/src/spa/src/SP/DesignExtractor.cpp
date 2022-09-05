#include "DesignExtractor.h"
#include "EntityNode.h"
#include <vector>
#include <algorithm>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

DesignExtractor::DesignExtractor() {}

ProcedureExtractor::ProcedureExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

StatementExtractor::StatementExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

VariableExtractor::VariableExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

ConstantExtractor::ConstantExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

vector<Procedure*> ProcedureExtractor::extract() {
	vector<Procedure*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(new Procedure(procList.at(i)->getName()));
	}

	return result;
}

vector<Statement*> StatementExtractor::extract() {
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
			result.push_back(statement);
		}
	}

	return result;
}

vector<Variable*> VariableExtractor::extract() {
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

	return result;
}

vector<Constant*> ConstantExtractor::extract() {
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

	return result;
}


void DesignExtractor::extractAll() {
	ProcedureExtractor(this->program, this->storage).populate();
	StatementExtractor(this->program, this->storage).populate();
	VariableExtractor(this->program, this->storage).populate();
	ConstantExtractor(this->program, this->storage).populate();
}

void ProcedureExtractor::populate() {
	vector<Procedure*> procedures = this->extract();
	this->storage->storeProcedures(&procedures);
}

void StatementExtractor::populate() {
	vector<Statement*> statements = this->extract();
	this->storage->storeStatements(&statements);
}

void VariableExtractor::populate() {
	vector<Variable*> variables = this->extract();
	this->storage->storeVariables(&variables);
}

void ConstantExtractor::populate() {
	vector<Constant*> constants = this->extract();
	this->storage->storeConstants(&constants);
}