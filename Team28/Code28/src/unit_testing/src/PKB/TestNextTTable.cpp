#include "catch.hpp"

#include "PKB/Algorithms/ControlFlowGraph.h"
#include "PKB/Storage/Storage.h"

/*
 * Tested resetCache by using a macro to make private fields public to check
 * that the private matrix is empty. However, since that macro is dangerous and
 * we don't want to change the internal implementation (protected field) to
 * allow for stubbing, we choose not to push the test to GitHub since the macro
 * fails the CI.
 */

struct InitNextTTable {
public:
    static std::pair<NextTTable *, StorageView *> initSimple();
};

std::pair<NextTTable *, StorageView *> InitNextTTable::initSimple() {
    Storage *storage = new Storage();
    NextTable *nextTable = storage->getTable<NextTable>();
    NextTTable *nextTTable = storage->getTable<NextTTable>();
    FollowsTable *followsTable = storage->getTable<FollowsTable>();
    ProceduresTable *procTable = storage->getTable<ProceduresTable>();
    StatementsTable *statementsTable = storage->getTable<StatementsTable>();

    Statement *s1 = new Statement(1, StatementType::ASSIGN);
    Statement *s2 = new Statement(2, StatementType::ASSIGN);
    Statement *s3 = new Statement(3, StatementType::ASSIGN);
    std::vector<Statement *> stmts = {s1, s2, s3};
    for (Statement *stmt : stmts) {
        statementsTable->store(stmt);
    }

    // Procedure(s)
    Procedure main = Procedure("main", 1);
    procTable->store(&main);

    // Follows
    Relationship<int, int> relation =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 2, 3);
    followsTable->store(&relation);

    ControlFlowGraph cfg =
        ControlFlowGraph(nextTable, storage->getStorageView());
    cfg.populateNext();
    nextTTable->initNextT(storage->getStorageView());

    return std::make_pair(nextTTable, storage->getStorageView());
}

TEST_CASE("CFG Traverses Correctly (NextT) - 1 procedure, infinite loop") {
    Storage *storage = new Storage();
    NextTable *nextTable = storage->getTable<NextTable>();
    NextTTable *nextTTable = storage->getTable<NextTTable>();
    BranchInTable *branchIn = storage->getTable<BranchInTable>();
    BranchOutTable *branchOut = storage->getTable<BranchOutTable>();
    ProceduresTable *procTable = storage->getTable<ProceduresTable>();
    StatementsTable *statementsTable = storage->getTable<StatementsTable>();

    Statement s1 = Statement(1, StatementType::WHILE);
    Statement s2 = Statement(2, StatementType::ASSIGN);
    statementsTable->store(&s1);
    statementsTable->store(&s2);

    // BranchIn
    Relationship<int, int> relation =
        Relationship(RelationshipReference::NEXT, 1, 2);
    branchIn->store(&relation);

    // BranchOut
    relation = Relationship(RelationshipReference::NEXT, 2, 1);
    branchOut->store(&relation);

    // Procedure(s)
    Procedure main = Procedure("main", 1);
    procTable->store(&main);

    StorageView *storageView = storage->getStorageView();
    ControlFlowGraph cfg = ControlFlowGraph(nextTable, storageView);

    cfg.populateNext();
    nextTTable->initNextT(storageView);

    REQUIRE(nextTTable->validate(Reference("1"), Reference("2")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("1")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("1")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("2")));
}

TEST_CASE("CFG Traverses Correctly (NextT) - 1 procedure") {
    Storage *storage = new Storage();
    NextTable *nextTable = storage->getTable<NextTable>();
    NextTTable *nextTTable = storage->getTable<NextTTable>();
    FollowsTable *followsTable = storage->getTable<FollowsTable>();
    BranchInTable *branchIn = storage->getTable<BranchInTable>();
    BranchOutTable *branchOut = storage->getTable<BranchOutTable>();
    ProceduresTable *procTable = storage->getTable<ProceduresTable>();
    StatementsTable *statementsTable = storage->getTable<StatementsTable>();

    // Statements
    Statement s1 = Statement(1, StatementType::ASSIGN);
    Statement s2 = Statement(2, StatementType::ASSIGN);
    Statement s3 = Statement(3, StatementType::ASSIGN);
    Statement s4 = Statement(4, StatementType::ASSIGN);
    Statement s5 = Statement(5, StatementType::ASSIGN);
    Statement s6 = Statement(6, StatementType::ASSIGN);
    Statement s7 = Statement(7, StatementType::ASSIGN);
    Statement s8 = Statement(8, StatementType::ASSIGN);
    Statement s9 = Statement(9, StatementType::ASSIGN);
    Statement s10 = Statement(10, StatementType::ASSIGN);
    Statement s11 = Statement(11, StatementType::ASSIGN);
    Statement s12 = Statement(12, StatementType::ASSIGN);
    statementsTable->store(&s1);
    statementsTable->store(&s2);
    statementsTable->store(&s3);
    statementsTable->store(&s4);
    statementsTable->store(&s5);
    statementsTable->store(&s6);
    statementsTable->store(&s7);
    statementsTable->store(&s8);
    statementsTable->store(&s9);
    statementsTable->store(&s10);
    statementsTable->store(&s11);
    statementsTable->store(&s12);

    // Follows
    Relationship<int, int> relation =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 2, 3);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 4, 5);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 5, 6);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 3, 7);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 10, 11);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 11, 12);
    followsTable->store(&relation);

    // BranchIn
    relation = Relationship(RelationshipReference::NEXT, 3, 4);
    branchIn->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 7, 8);
    branchIn->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 7, 9);
    branchIn->store(&relation);

    // BranchOut
    relation = Relationship(RelationshipReference::NEXT, 6, 3);
    branchOut->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 8, 10);
    branchOut->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 9, 10);
    branchOut->store(&relation);

    // Procedure(s)
    Procedure test = Procedure("main", 1);
    procTable->store(&test);

    StorageView *storageView = storage->getStorageView();
    ControlFlowGraph cfg = ControlFlowGraph(nextTable, storageView);

    cfg.populateNext();
    nextTTable->initNextT(storageView);

    REQUIRE(nextTTable->validate(Reference("1"), Reference("2")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("1"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("2"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("3"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("4"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("5"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("3")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("4")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("5")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("6")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("7")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("6"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("7"), Reference("8")));
    REQUIRE(nextTTable->validate(Reference("7"), Reference("9")));
    REQUIRE(nextTTable->validate(Reference("7"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("7"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("7"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("8"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("8"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("8"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("9"), Reference("10")));
    REQUIRE(nextTTable->validate(Reference("9"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("9"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("10"), Reference("11")));
    REQUIRE(nextTTable->validate(Reference("10"), Reference("12")));
    REQUIRE(nextTTable->validate(Reference("11"), Reference("12")));
}

TEST_CASE("NextTTable: getTableSize works correctly") {
    NextTTable nextTTable;

    REQUIRE(nextTTable.getTableSize() == INT_MAX);
}

TEST_CASE("NextT solveRight works correctly") {
    std::pair<NextTTable *, StorageView *> pair = InitNextTTable::initSimple();
    NextTTable *nextTTable = pair.first;
    StorageView *storage = pair.second;

    Reference leftRef = Reference("1");
    EntityName rightSynonym = EntityName::STMT;

    // Next*(1, s)
    std::vector<Value> output =
        nextTTable->solveRight(leftRef, rightSynonym, storage);
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "2"),
                                         Value(ValueType::STMT_NUM, "3")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(2, a)
    leftRef = Reference("2");
    rightSynonym = EntityName::ASSIGN;
    output = nextTTable->solveRight(leftRef, rightSynonym, storage);
    expectedResult = {Value(ValueType::STMT_NUM, "3")};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(3, s)
    leftRef = Reference("3");
    rightSynonym = EntityName::STMT;
    output = nextTTable->solveRight(leftRef, rightSynonym, storage);
    expectedResult = {};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(1, p)
    leftRef = Reference("1");
    rightSynonym = EntityName::PRINT;
    output = nextTTable->solveRight(leftRef, rightSynonym, storage);
    expectedResult = {};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("NextT solveLeft works correctly") {
    std::pair<NextTTable *, StorageView *> pair = InitNextTTable::initSimple();
    NextTTable *nextTTable = pair.first;
    StorageView *storage = pair.second;

    Reference rightRef = Reference("3");
    EntityName leftSynonym = EntityName::STMT;

    // Next*(s, 3)
    std::vector<Value> output =
        nextTTable->solveLeft(rightRef, leftSynonym, storage);
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "1"),
                                         Value(ValueType::STMT_NUM, "2")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(a, 2)
    rightRef = Reference("2");
    leftSynonym = EntityName::ASSIGN;
    output = nextTTable->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {Value(ValueType::STMT_NUM, "1")};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(s, 1)
    rightRef = Reference("1");
    leftSynonym = EntityName::STMT;
    output = nextTTable->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Next*(p, 3)
    rightRef = Reference("3");
    leftSynonym = EntityName::PRINT;
    output = nextTTable->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {};

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("NextT solveBoth works correctly") {
    std::pair<NextTTable *, StorageView *> pair = InitNextTTable::initSimple();
    NextTTable *nextTTable = pair.first;
    StorageView *storage = pair.second;

    EntityName leftSynonym = EntityName::STMT;
    EntityName rightSynonym = EntityName::STMT;

    // Next*(s, s)
    std::vector<std::pair<Value, Value>> output =
        nextTTable->solveBoth(leftSynonym, rightSynonym, storage);
    std::vector<std::pair<Value, Value>> expectedResult = {
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "2")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "3")),
        std::make_pair(Value(ValueType::STMT_NUM, "2"),
                       Value(ValueType::STMT_NUM, "3"))};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}