#pragma once

#include "Storage.h"
#include "StatementPredicateMap.h"

/*
* A Facade class for QPS->PKB interactions.
*/
class QueryFacade {
public:
	/*
	* Explicit constructor for a QueryFacade.
	*/
	explicit QueryFacade(Storage* storage);

	/*
	* Returns all statements inside the StatementsTable.
	*/
	std::vector<Statement*> getAllStatements();

	/*
	* Returns all statements inside the StatementsTable.
	*/
	std::vector<Statement*> getAllStatementsByType(StatementType type);

	/*
	* Returns statement with the given line number.
	*/
	Statement* getStatementByLineNo(const int& lineNo);

	/*
	* Returns all variable names inside the VariablesTable.
	*/
	std::vector<std::string> getAllVariables();

	/*
	* Returns variable that matches a given name.
	*/
	Variable* getVariableByName(const std::string& name);

	/*
	* Returns all constant names inside the ConstantsTable.
	*/
	std::vector<std::string> getAllConstants();

	/*
	* Returns constant that matches a given name.
	*/
	Constant* getConstantByName(const std::string& name);

	/*
	* Returns all procedure names inside the ProceduresTable.
	*/
	std::vector<std::string> getAllProcedures();

	/*
	* Returns procedure that matches a given name.
	*/
	Procedure* getProcedureByName(const std::string& name);

private:
	Storage* storage;
};
