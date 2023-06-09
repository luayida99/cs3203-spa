#include "Storage.h"

Storage::Storage() {
    initDesignEntitiesTable();
    initRelationshipsTable();
    initRsTablesMap();
    initStorageView();
    initAttributesTableMap();
    initReflexiveTablesMap();
};

void Storage::initDesignEntitiesTable() {
    initTable<StatementsTable>(Designation::STMT);
    initTable<AssignmentsTable>(Designation::ASSIGN);
    initTable<ProceduresTable>(Designation::PROC);
    initTable<VariablesTable>(Designation::VAR);
    initTable<ConstantsTable>(Designation::CONST);
};

void Storage::initRelationshipsTable() {
    initTable<ParentTable>(Designation::PARENT);
    initTable<ParentTTable>(Designation::PARENT_T);
    initTable<FollowsTable>(Designation::FOLLOWS);
    initTable<FollowsTTable>(Designation::FOLLOWS_T);
    initTable<ModifiesSTable>(Designation::MOD_S);
    initTable<ModifiesPTable>(Designation::MOD_P);
    initTable<UsesSTable>(Designation::USE_S);
    initTable<UsesPTable>(Designation::USE_P);
    initTable<CallsTable>(Designation::CALL);
    initTable<CallsTTable>(Designation::CALL_T);
    initTable<BranchInTable>(Designation::B_IN);
    initTable<BranchOutTable>(Designation::B_OUT);
    initTable<IfControlVarTable>(Designation::IF_C);
    initTable<WhileControlVarTable>(Designation::WHILE_C);
    initTable<CallProcTable>(Designation::PROC_NAME);
    initTable<NextTable>(Designation::NEXT);
    initTable<NextTTable>(Designation::NEXT_T);
    initTable<AffectsTable>(Designation::AFFECTS);
    initTable<AffectsTTable>(Designation::AFFECTS_T);
}

void Storage::initRsTablesMap() {
    this->rsTables[RelationshipReference::FOLLOWS] =
        this->getTable<FollowsTable>();
    this->rsTables[RelationshipReference::FOLLOWS_T] =
        this->getTable<FollowsTTable>();
    this->rsTables[RelationshipReference::PARENT] =
        this->getTable<ParentTable>();
    this->rsTables[RelationshipReference::PARENT_T] =
        this->getTable<ParentTTable>();
    this->rsTables[RelationshipReference::CALLS] = this->getTable<CallsTable>();
    this->rsTables[RelationshipReference::CALLS_T] =
        this->getTable<CallsTTable>();
    this->rsTables[RelationshipReference::NEXT] = this->getTable<NextTable>();
    this->rsTables[RelationshipReference::NEXT_T] =
        this->getTable<NextTTable>();
    this->rsTables[RelationshipReference::AFFECTS] =
        this->getTable<AffectsTable>();
    this->rsTables[RelationshipReference::AFFECTS_T] =
        this->getTable<AffectsTTable>();
};

void Storage::initStorageView() {
    this->storageView = new StorageView();
    this->storageView->setTable<StatementsTable>(
        this->getTable<StatementsTable>());
    this->storageView->setTable<ProceduresTable>(
        this->getTable<ProceduresTable>());
    this->storageView->setTable<VariablesTable>(
        this->getTable<VariablesTable>());
    this->storageView->setTable<FollowsTable>(this->getTable<FollowsTable>());
    this->storageView->setTable<BranchInTable>(this->getTable<BranchInTable>());
    this->storageView->setTable<BranchOutTable>(
        this->getTable<BranchOutTable>());
    this->storageView->setTable<NextTable>(this->getTable<NextTable>());
    this->storageView->setTable<ModifiesSTable>(
        this->getTable<ModifiesSTable>());
    this->storageView->setTable<UsesSTable>(this->getTable<UsesSTable>());
    this->storageView->setTable<AffectsTable>(this->getTable<AffectsTable>());
};

void Storage::initReflexiveTablesMap() {
    this->reflexiveTables[RelationshipReference::NEXT_T] =
        this->getTable<NextTTable>();
    this->reflexiveTables[RelationshipReference::AFFECTS] =
        this->getTable<AffectsTable>();
    this->reflexiveTables[RelationshipReference::AFFECTS_T] =
        this->getTable<AffectsTTable>();
}

Solvable *Storage::getRsTable(RelationshipReference rsRef,
                              ReferenceType leftType) {
    if (rsRef == RelationshipReference::MODIFIES) {
        return this->getOnType<ModifiesSTable, ModifiesPTable>(leftType);
    }
    if (rsRef == RelationshipReference::USES) {
        return this->getOnType<UsesSTable, UsesPTable>(leftType);
    }
    return this->rsTables.at(rsRef);
};

Reflexive *Storage::getReflexiveTable(RelationshipReference rsRef) {
    return this->reflexiveTables.at(rsRef);
}

UsesControlVarTable *Storage::getControlVarTable(Designation designType) {
    if (designType == Designation::IF_C) {
        return this->getTable<IfControlVarTable>();
    }
    if (designType == Designation::WHILE_C) {
        return this->getTable<WhileControlVarTable>();
    }
    return nullptr;
};

Table *Storage::getDesignationTable(Designation designType) {
    return this->designTables.at(designType);
};

std::vector<Solvable *> Storage::getModifiesTables() {
    return std::vector<Solvable *>{this->getTable<ModifiesSTable>(),
                                   this->getTable<ModifiesPTable>()};
};

std::vector<Solvable *> Storage::getUsesTables() {
    return std::vector<Solvable *>{this->getTable<UsesSTable>(),
                                   this->getTable<UsesPTable>()};
};

StorageView *Storage::getStorageView() { return this->storageView; };

Attributable *Storage::getAttributesTable(EntityName entity,
                                          EntityAttribute attribute) {
    return this->attributesTables[{entity, attribute}];
};

void Storage::initAttributesTableMap() {
    this->attributesTables = {
        {std::make_pair(EntityName::STMT, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::READ, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::PRINT, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::CALL, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::WHILE, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::IF, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::ASSIGN, EntityAttribute::STMT_NO),
         this->getTable<StatementsTable>()},
        {std::make_pair(EntityName::CONSTANT, EntityAttribute::VALUE),
         this->getTable<ConstantsTable>()},
        {std::make_pair(EntityName::PROCEDURE, EntityAttribute::PROC_NAME),
         this->getTable<ProceduresTable>()},
        {std::make_pair(EntityName::CALL, EntityAttribute::PROC_NAME),
         this->getTable<CallProcTable>()},
        {std::make_pair(EntityName::VARIABLE, EntityAttribute::VAR_NAME),
         this->getTable<VariablesTable>()},
        {std::make_pair(EntityName::READ, EntityAttribute::VAR_NAME),
         this->getTable<ModifiesSTable>()},
        {std::make_pair(EntityName::PRINT, EntityAttribute::VAR_NAME),
         this->getTable<UsesSTable>()}};
};
