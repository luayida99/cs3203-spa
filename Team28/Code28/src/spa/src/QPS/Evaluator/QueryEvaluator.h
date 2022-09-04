#pragma once

#include "../../PKB/QueryFacade.h"
#include "../Parser/SolvableQuery.h"
#include "SuchThatEvaluator.h"
#include "PatternEvaluator.h"
#include "QueryResult.h"

class QueryEvaluator {
private:
    QueryFacade* queryFacade;
    SuchThatEvaluator suchThatEvaluator;
    PatternEvaluator patternEvaluator;

public:
    explicit QueryEvaluator(QueryFacade* queryFacade) :
        queryFacade(queryFacade),
        suchThatEvaluator(SuchThatEvaluator(queryFacade)),
        patternEvaluator(PatternEvaluator(queryFacade)) {};
    QueryResult evaluate(SolvableQuery* solvableQ);
    std::vector<std::string> interpretQueryResult(QueryResult* queryResult);
};