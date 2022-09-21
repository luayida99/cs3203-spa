#pragma once

#include "RelationshipsTable.h"

class ProcToVarRelationshipsTable
    : public RelationshipsTable<std::string, std::string> {
public:
    /*
     * Returns true if the relationship holds between leftReference and
     * rightReference.
     */
    bool validate(Reference leftRef, Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be.
     */
    std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym,
                                  VariablesTable *variables);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 ProceduresTable *procedures);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   ProceduresTable *procedures,
                                                   VariablesTable *variables);
};

typedef ProcToVarRelationshipsTable ModifiesPTable;
typedef ProcToVarRelationshipsTable UsesPTable;