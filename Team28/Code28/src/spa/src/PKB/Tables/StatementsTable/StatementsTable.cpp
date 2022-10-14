#include "StatementsTable.h"

#include "../../../commons/StringUtil.h"
#include <algorithm>

StatementsTable::StatementsTable() = default;

void StatementsTable::store(TableValue *statement) {
    Statement *stmt = static_cast<Statement *>(statement);
    StatementType type = stmt->getStatementType();
    int index = stmt->getLineNumber();

    if (type == StatementType::NONE) {
        // TODO error handling
    }

    this->statements.push_back(stmt);
    this->statementTypeIndexes[type].push_back(index);
}

Statement *StatementsTable::retrieve(const int &lineNum) {
    // currently assumes that lineNum is unique
    for (Statement *statement : this->statements) {
        if (statement->getLineNumber() == lineNum) {
            return statement;
        }
    }

    return nullptr;
}

StatementType StatementsTable::getStatementType(const int &lineNum) {
    // currently assumes that lineNum is unique
    for (Statement *statement : this->statements) {
        if (statement->getLineNumber() == lineNum) {
            return statement->getStatementType();
        }
    }

    return StatementType::NONE;
}

std::vector<Statement *> StatementsTable::getAll() { return this->statements; }

std::vector<int> StatementsTable::getAllLineNumbers() {
    std::vector<Statement *> statements = this->statements;
    std::vector<int> lineNumbers;
    std::transform(statements.begin(), statements.end(),
                   std::back_inserter(lineNumbers),
                   Statement::getLineNumberFromStatement);
    return lineNumbers;
}

std::vector<int> StatementsTable::getStatementsByType(StatementType type) {
    return this->statementTypeIndexes[type];
}

bool StatementsTable::isAttributableStatement(const int &lineNum) {
    StatementType stmtType = this->getStatementType(lineNum);
    if (stmtType == StatementType::CALL || stmtType == StatementType::READ ||
        stmtType == StatementType::PRINT) {
        return true;
    }
    return false;
}

std::vector<Value> StatementsTable::getValue(std::string value,
                                             EntityName entity) {
    int v = std::stoi(value);
    std::vector<int> values;
    std::vector<Value> result = {};
    if (entity == EntityName::STMT) {
        values = this->getAllLineNumbers();
    } else {
        StatementType stmtType = Statement::entityToStatementMap[entity];
        values = this->getStatementsByType(stmtType);
    }
    for (int i : values) {
        if (i == v) {
            result.push_back(Value(ValueType::STMT_NUM, toString(i)));
        }
    }
    return result;
}