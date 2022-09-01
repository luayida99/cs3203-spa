#include "StatementsTable.h"
#include "Statement.h"
#include "StatementPredicateMap.h"

#include <algorithm>

StatementsTable::StatementsTable() = default;

int StatementsTable::getTableSize() const {
	return this->tableSize;
}

void StatementsTable::store(Statement* statement) {
	StatementType type = statement->getStatementType();
	int index = statement->getIndex();

	if (type == StatementType::NONE) {
		//TODO error handling
	}

	this->statements.push_back(statement);
	this->statementTypeIndexes[type].push_back(index);
	this->tableSize++;
}

Statement *StatementsTable::retrieve(const int& lineNum) {
	// currently assumes that lineNum is unique
	for(Statement* statement : this->statements) {
		if (statement->getIndex() == lineNum) {
			return statement;
		}
	}

	return nullptr;
}

StatementType StatementsTable::getStatementType(const int& lineNum) {
	// currently assumes that lineNum is unique
	for(Statement* statement : this->statements) {
		if (statement->getIndex() == lineNum) {
			return statement->getStatementType();
		}
	}

	return StatementType::NONE;

}

StatementsTable *StatementsTable::filter(StatementPredicateMap *predicateMap) {
	if ((*predicateMap).isEmpty()) {
		return this;
	}

	StatementsTable *newTable = new StatementsTable();
	std::map<StatementHeader, Statement*> extractedMap = (*predicateMap).getPredicateMap();

	for(Statement* statement : this->statements) {
		bool isFilter = true;
		for (auto [key, val] : extractedMap) {
			if (!statement->isValueEqual(key, val)) {
				isFilter = false;
			}
		}
		if (isFilter) {
			newTable->store(statement);
		}
	}

	return newTable;
}