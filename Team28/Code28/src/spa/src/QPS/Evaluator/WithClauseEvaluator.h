#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Clause/WithClause.h"
#include "ClauseResult.h"

/*
 * Evaluator class for the with clause.
 */
class WithClauseEvaluator {
private:
    QueryFacade *queryFacade;
    ClauseResult handleBothSynonyms(Reference left, Reference right);
    ClauseResult handleOneSynonym(Reference left, Reference right);

public:
    explicit WithClauseEvaluator(QueryFacade *queryFacade)
        : queryFacade(queryFacade){};
    ClauseResult evaluate(WithClause *withCl);
};
