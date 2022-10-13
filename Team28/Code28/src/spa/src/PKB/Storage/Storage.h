#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include "../../commons/Constant.h"
#include "../../commons/Entity.h"
#include "../../commons/Procedure.h"
#include "../../commons/Relationship.h"
#include "../../commons/TableValue.h"
#include "../../commons/Variable.h"

#include "../Tables/AssignmentsTable/AssignmentsTable.h"
#include "../Tables/NamesTable/NamesTable.h"
#include "../Tables/RelationshipsTable/CallProcTable.h"
#include "../Tables/RelationshipsTable/ProcToProcRelationshipsTable.h"
#include "../Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "../Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"
#include "../Tables/RelationshipsTable/UsesControlVarTable.h"
#include "../Tables/StatementsTable/StatementsTable.h"
#include "../Tables/Table.h"

#include "StorageView.h"

enum class Populate {
    STMT,
    ASSIGN,
    VAR,
    CONST,
    PROC,
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MOD_S,
    MOD_P,
    USE_S,
    USE_P,
    CALL,
    CALL_T,
    B_IN,
    B_OUT,
    IF_C,
    WHILE_C,
    PROC_NAME
};

/*
 * Encapsulates a Storage class which is responsible for storing information to
 * tables in PKB.
 */
class Storage {
public:
    /*
     * Explicit constructor for Storage.
     */
    Storage();

    /*
     * Retrieve a table by the templated class given
     */
    template <typename Subclass> Subclass *getTable() {
        Table *table = this->tables.at(typeid(Subclass));
        return dynamic_cast<Subclass *>(table);
    };

    /*
     * Retrieve a table by the RelationshipReference
     */
    Solvable *getRsTable(RelationshipReference rsRef, ReferenceType leftType);

    /*
     * Retrieve a table by Populate.
     */
    Table *getStoreTable(Populate popType);

    /*
     * Retrieves Modifies Tables
     */
    std::vector<Solvable *> getModifiesTables();

    /*
     * Retrieves Uses Tables
     */
    std::vector<Solvable *> getUsesTables();

    /*
     * Returns the StorageView
     */
    StorageView *getStorageView();

private:
    std::map<std::type_index, Table *> tables;
    /*
     * Mapping of RelationshipReference to Solvable tables.
     * Note: Modifies and Uses are complex and won't be mapped here.
     */
    std::map<RelationshipReference, Solvable *> rsTables;
    /*
     * Mapping of Populate to Table for PopulateFacade use.
     */
    std::map<Populate, Table *> popTables;
    StorageView *storageView;

    /*
     * Template method for getting tables used for Uses and Modifies.
     */
    template <typename STable, typename PTable>
    Solvable *getOnType(ReferenceType leftType) {
        if (leftType == ReferenceType::STMT_REF) {
            return this->getTable<STable>();
        }
        if (leftType == ReferenceType::ENT_REF) {
            return this->getTable<PTable>();
        }
        return nullptr;
    }
};
