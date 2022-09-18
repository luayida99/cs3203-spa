#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "../commons/Assignment.h"
#include "../commons/Value.h"
#include "StatementPredicateMap.h"
#include "Table.h"

typedef std::pair<int, std::string> IntStringPair;

/*
 * Class encapsulating a Table used to store SIMPLE statements.
 */
class AssignmentsTable : public Table<Assignment> {
public:
    AssignmentsTable();

    /*
     * Stores an Assignment into the AssignmentsTable.
     * Mapping will be done from variable -> (lineNo, expression)
     * as well as expression -> (lineNo, variable)
     */
    void store(Assignment *assignment);

    int getTableSize() const;

    /*
     * Return boolean of whether the given varName and expression exists in an
     * assignment.
     */
    std::vector<Value> containsVarAndExpr(std::string varName,
                                          std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and expression
     */
    std::vector<Value> getAssign(std::string varName, std::string expression);

    /*
     * Return list of possible (Assignment, Variable) pairs which satisfy the
     * given expression
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVar(std::string expression);

private:
    int tableSize = 0;

    std::vector<Value> allLineNumbers;
    std::vector<Assignment> allAssignments;

    /*
     * Return list of possible values of assignments that satisfy the given
     * non-wildcarded varName and expression
     */
    std::vector<Value> getAssignFromVarAndExpr(std::string varName,
                                               std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * expression
     */
    std::vector<Value> getAssignFromExpr(std::string expression);
};