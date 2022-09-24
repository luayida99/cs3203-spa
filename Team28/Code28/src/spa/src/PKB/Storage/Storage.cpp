#include "Storage.h"

Storage::Storage() {
    StatementsTable *statements = new StatementsTable();
    AssignmentsTable *assignments = new AssignmentsTable();
    ProceduresTable *procedures = new ProceduresTable();
    VariablesTable *variables = new VariablesTable();
    ConstantsTable *constants = new ConstantsTable();
    ParentTable *parents = new ParentTable();
    ParentTTable *parentsT = new ParentTTable();
    FollowsTable *follows = new FollowsTable();
    FollowsTTable *followsT = new FollowsTTable();
    ModifiesSTable *modifiesS = new ModifiesSTable();
    ModifiesPTable *modifiesP = new ModifiesPTable();
    UsesSTable *usesS = new UsesSTable();
    UsesPTable *usesP = new UsesPTable();

    /*
     * TODO relook typecasting
     */
    this->tables[TableName::STATEMENTS] = (Table<TableValue> *)statements;
    this->tables[TableName::ASSIGNMENTS] = (Table<TableValue> *)assignments;
    this->tables[TableName::PROCEDURES] = (Table<TableValue> *)procedures;
    this->tables[TableName::VARIABLES] = (Table<TableValue> *)variables;
    this->tables[TableName::CONSTANTS] = (Table<TableValue> *)constants;
    this->tables[TableName::PARENT] = (Table<TableValue> *)parents;
    this->tables[TableName::PARENT_T] = (Table<TableValue> *)parentsT;
    this->tables[TableName::FOLLOWS] = (Table<TableValue> *)follows;
    this->tables[TableName::FOLLOWS_T] = (Table<TableValue> *)followsT;
    this->tables[TableName::MODIFIES_S] = (Table<TableValue> *)modifiesS;
    this->tables[TableName::MODIFIES_P] = (Table<TableValue> *)modifiesP;
    this->tables[TableName::USES_S] = (Table<TableValue> *)usesS;
    this->tables[TableName::USES_P] = (Table<TableValue> *)usesP;
};

Table<TableValue> *Storage::getTable(TableName name) {
    return this->tables.at(name);
};