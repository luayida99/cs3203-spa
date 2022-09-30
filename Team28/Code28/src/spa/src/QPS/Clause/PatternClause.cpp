#include "PatternClause.h"

PatternClause::PatternClause() { this->patternType = PatternType::EMPTY; }
PatternClause::PatternClause(Synonym syn, Reference entRef,
                             Expression expression, bool isExact) {
    this->patternType = PatternType::ASSIGN;
    this->syn = syn;
    this->entRef = entRef;
    this->expression = expression;
    this->isExact = isExact;
}
