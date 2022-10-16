#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Parser/SolvableQuery.h"
#include "../Utils.h"
#include "PatternEvaluator.h"
#include "QueryResult.h"
#include "SuchThatEvaluator.h"
#include <string>

// comment header for all public api

class QueryEvaluator {
private:
    QueryFacade *queryFacade;
    SuchThatEvaluator suchThatEvaluator;
    PatternEvaluator patternEvaluator;

public:
    explicit QueryEvaluator(QueryFacade *queryFacade)
        : queryFacade(queryFacade),
          suchThatEvaluator(SuchThatEvaluator(queryFacade)),
          patternEvaluator(PatternEvaluator(queryFacade)){};
    QueryResult evaluate(SolvableQuery *solvableQ);
    std::vector<std::string> interpretQueryResult(QueryResult *queryResult);

private:
    void checkAllClauseResult(std::vector<ClauseResult> clauseResultList,
                              bool *isAnyTableEmpty, bool *haveTableToJoin);
    std::vector<std::string> extractReferenceFromTable(Reference selectedRef,
                                                       ClauseTable result);
    std::vector<std::string>
    extractTuplesFromTable(std::vector<Reference> selectRefs,
                           ClauseTable result);
    std::vector<std::string> getAll(Reference select);
    std::vector<std::string> handleNoTables(QueryResult *queryResult);
    ClauseTable joinAllClauseTables(std::vector<ClauseResult> clauseResultList);
    std::string getAttributeValue(Reference ref, std::string synonymValue);
    bool isAlternativeAttribute(Reference ref);
};
