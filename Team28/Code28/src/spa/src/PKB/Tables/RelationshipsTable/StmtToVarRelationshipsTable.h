#pragma once

#include "../Solvable.h"
#include "StmtToNameRelationshipsTable.h"

class StmtToVarRelationshipsTable : public StmtToNameRelationshipsTable,
                                    public Solvable {
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
                                  StorageView *storage);

    /*
     * Returns list of possible values that the left synonym can be.
     */
    std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym,
                                 StorageView *storage);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be.
     */
    std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym,
                                                   EntityName rightSynonym,
                                                   StorageView *storage);
};

class ModifiesSTable : public StmtToVarRelationshipsTable {};
class UsesSTable : public StmtToVarRelationshipsTable {};
