#include "PKB/Facades/QueryFacade.h"
#include "QPS/Evaluator/ClauseResult.h"
#include "QPS/Evaluator/PatternEvaluator.h"
#include "QPS/Evaluator/QueryEvaluator.h"
#include "QPS/Evaluator/QueryResult.h"
#include "QPS/Evaluator/SuchThatEvaluator.h"
#include "QPS/Parser/QueryParser.h"
#include "QPS/Parser/SolvableQuery.h"

#include "catch.hpp"

TEST_CASE(
    "Query evaluator can evaluate query with only select statement clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements =
        storage->getTable<StatementsTable>(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);

    statements->store(&test1);
    statements->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("stmt s; Select s");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select variable clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    VariablesTable *variables =
        storage->getTable<VariablesTable>(TableName::VARIABLES);
    Variable test1 = Variable("test1");
    Variable test2 = Variable("test2");

    variables->store(&test1);
    variables->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("variable v; Select v");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select constant clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ConstantsTable *constants =
        storage->getTable<ConstantsTable>(TableName::CONSTANTS);
    Constant test1 = Constant("test1");
    Constant test2 = Constant("test2");

    constants->store(&test1);
    constants->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("constant c; Select c");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE(
    "Query evaluator can evaluate query with only select procedure clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    ProceduresTable *procedures =
        storage->getTable<ProceduresTable>(TableName::PROCEDURES);
    Procedure test1 = Procedure("test1");
    Procedure test2 = Procedure("test2");

    procedures->store(&test1);
    procedures->store(&test2);

    SolvableQuery solvableQ = QueryParser::parse("procedure p; Select p");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Evaluator can retrieve specific statement types") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements =
        storage->getTable<StatementsTable>(TableName::STATEMENTS);
    Statement test1 = Statement(1, StatementType::ASSIGN);
    Statement test2 = Statement(2, StatementType::ASSIGN);
    Statement test3 = Statement(3, StatementType::WHILE);
    Statement test4 = Statement(3, StatementType::IF);
    Statement test5 = Statement(3, StatementType::WHILE);

    statements->store(&test1);
    statements->store(&test2);
    statements->store(&test3);
    statements->store(&test4);
    statements->store(&test5);

    std::string input;
    std::vector<std::string> result;
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    input = "stmt s; Select s";
    SolvableQuery solvableQ_stmt = QueryParser::parse(input);
    QueryResult queryResult_stmt = queryEvaluator.evaluate(&solvableQ_stmt);
    result = queryEvaluator.interpretQueryResult(&queryResult_stmt);

    REQUIRE(result.size() == 5);

    input = "if s; Select s";
    SolvableQuery solvableQ_if = QueryParser::parse(input);
    QueryResult queryResult_if = queryEvaluator.evaluate(&solvableQ_if);
    result = queryEvaluator.interpretQueryResult(&queryResult_if);

    REQUIRE(result.size() == 1);

    input = "assign s; Select s";
    SolvableQuery solvableQ_assign = QueryParser::parse(input);
    QueryResult queryResult_assign = queryEvaluator.evaluate(&solvableQ_assign);
    result = queryEvaluator.interpretQueryResult(&queryResult_assign);

    REQUIRE(result.size() == 2);

    input = "while s; Select s";
    SolvableQuery solvableQ_while = QueryParser::parse(input);
    QueryResult queryResult_while = queryEvaluator.evaluate(&solvableQ_while);
    result = queryEvaluator.interpretQueryResult(&queryResult_while);

    REQUIRE(result.size() == 2);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements =
        storage->getTable<StatementsTable>(TableName::STATEMENTS);
    FollowsTable *follows = storage->getTable<FollowsTable>(TableName::FOLLOWS);

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    SolvableQuery solvableQ =
        QueryParser::parse("stmt s; Select s such that Follows(1,2)");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 2);
}
TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "that is false") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements =
        storage->getTable<StatementsTable>(TableName::STATEMENTS);
    FollowsTable *follows = storage->getTable<FollowsTable>(TableName::FOLLOWS);

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    SolvableQuery solvableQ =
        QueryParser::parse("stmt s; Select s such that Follows(2,1)");
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);
    QueryResult queryResult = queryEvaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        queryEvaluator.interpretQueryResult(&queryResult);

    REQUIRE(result.size() == 0);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "with synonym") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    StatementsTable *statements =
        storage->getTable<StatementsTable>(TableName::STATEMENTS);
    FollowsTable *follows = storage->getTable<FollowsTable>(TableName::FOLLOWS);

    Statement line1 = Statement(1, StatementType::ASSIGN);
    Statement line2 = Statement(2, StatementType::ASSIGN);
    Relationship<int, int> rs =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Reference leftRef = Reference("1");
    Reference rightRef = Reference("2");

    statements->store(&line1);
    statements->store(&line2);
    follows->store(&rs);

    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    SolvableQuery solvableQ_right =
        QueryParser::parse("stmt s; Select s such that Follows(1,s)");
    QueryResult queryResult_right = queryEvaluator.evaluate(&solvableQ_right);
    std::vector<std::string> result_right =
        queryEvaluator.interpretQueryResult(&queryResult_right);

    REQUIRE(result_right.size() == 1);

    SolvableQuery solvableQ_left =
        QueryParser::parse("stmt s; Select s such that Follows(s,2)");
    QueryResult queryResult_left = queryEvaluator.evaluate(&solvableQ_left);
    std::vector<std::string> result_left =
        queryEvaluator.interpretQueryResult(&queryResult_left);

    REQUIRE(result_left.size() == 1);

    SolvableQuery solvableQ_wildcard =
        QueryParser::parse("stmt s; Select s such that Follows(_, s)");
    QueryResult queryResult_wildcard =
        queryEvaluator.evaluate(&solvableQ_wildcard);
    std::vector<std::string> result_wildcard =
        queryEvaluator.interpretQueryResult(&queryResult_wildcard);

    REQUIRE(result_wildcard.size() == 1);

    SolvableQuery solvableQ_wildcard_right =
        QueryParser::parse("stmt s; Select s such that Follows(s, _)");
    QueryResult queryResult_wildcard_right =
        queryEvaluator.evaluate(&solvableQ_wildcard_right);
    std::vector<std::string> result_wildcard_right =
        queryEvaluator.interpretQueryResult(&queryResult_wildcard_right);

    REQUIRE(result_wildcard_right.size() == 1);

    SolvableQuery solvableQ_zero =
        QueryParser::parse("stmt s; Select s such that Follows(s,1)");
    QueryResult queryResult_zero = queryEvaluator.evaluate(&solvableQ_zero);
    std::vector<std::string> result_zero =
        queryEvaluator.interpretQueryResult(&queryResult_zero);

    REQUIRE(result_zero.size() == 0);
}

TEST_CASE("Query evaluator can evaluate query with single such that clause "
          "with synonyms that are the same") {
    Storage *storage = new Storage();
    QueryFacade facade = QueryFacade(storage);
    QueryEvaluator queryEvaluator = QueryEvaluator(&facade);

    SolvableQuery solvableQ_follow =
        QueryParser::parse("stmt s; Select s such that Follows(s, s)");
    QueryResult queryResult_follow = queryEvaluator.evaluate(&solvableQ_follow);
    std::vector<std::string> result_follow =
        queryEvaluator.interpretQueryResult(&queryResult_follow);

    REQUIRE(result_follow.size() == 0);

    SolvableQuery solvableQ_followT =
        QueryParser::parse("stmt s; Select s such that Follows*(s, s)");
    QueryResult queryResult_followT =
        queryEvaluator.evaluate(&solvableQ_followT);
    std::vector<std::string> result_followT =
        queryEvaluator.interpretQueryResult(&queryResult_followT);

    REQUIRE(result_followT.size() == 0);

    SolvableQuery solvableQ_parent =
        QueryParser::parse("stmt s; Select s such that Parent*(s, s)");
    QueryResult queryResult_parent = queryEvaluator.evaluate(&solvableQ_parent);
    std::vector<std::string> result_parent =
        queryEvaluator.interpretQueryResult(&queryResult_parent);

    REQUIRE(result_parent.size() == 0);

    SolvableQuery solvableQ_parentT =
        QueryParser::parse("stmt s; Select s such that Parent*(s, s)");
    QueryResult queryResult_parentT =
        queryEvaluator.evaluate(&solvableQ_parentT);
    std::vector<std::string> result_parentT =
        queryEvaluator.interpretQueryResult(&queryResult_parentT);

    REQUIRE(result_parentT.size() == 0);
}
