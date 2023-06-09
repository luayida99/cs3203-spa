#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    this->affects = storage->getTable<AffectsTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->assignments =
        statements->getStatementsSetByType(StatementType::ASSIGN);
}

void AffectsTTable::resetCache() {
    if (!shouldTableReset()) {
        return;
    }
    this->matrix.clear();
    this->isComputed = false;
    markTableResetted();
}

std::map<std::pair<int, int>, bool> AffectsTTable::computeClosure() {
    if (isComputed) {
        return this->matrix;
    }
    for (int i : this->assignments) {
        for (int j : this->assignments) {
            std::pair<int, int> curr = std::make_pair(i, j);
            this->matrix[curr] = false;
        }
    }
    for (const auto &elem : this->affects->eagerGetMatrix()) {
        this->matrix[elem.first] = elem.second;
    }
    for (int k : this->assignments) {
        for (int i : this->assignments) {
            for (int j : this->assignments) {
                std::pair<int, int> curr = std::make_pair(i, j);
                std::pair<int, int> left = std::make_pair(i, k);
                std::pair<int, int> right = std::make_pair(k, j);
                matrix[curr] = matrix[curr] || (matrix[left] && matrix[right]);
            }
        }
    }
    this->isComputed = true;
    markForReset();
    return this->matrix;
}

bool AffectsTTable::validate(Reference leftRef, Reference rightRef) {
    if (!isComputed) {
        this->computeClosure();
    }
    return validateHelper<AffectsTTable>(
        &leftRef, &rightRef, this, &AffectsTTable::verifyDoubleWildcards,
        &AffectsTTable::verifySingleWildcard, &AffectsTTable::checkAffectsT);
};

std::vector<Value> AffectsTTable::solveRight(Reference leftRef,
                                             EntityName rightSynonym,
                                             StorageView *storage) {
    if (!isAssignmentEntity(rightSynonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    return solveRightAffectsHelper<AffectsTTable>(
        &leftRef, this, &AffectsTTable::solveSingleWildcard,
        &AffectsTTable::solveHelper);
}

std::vector<Value> AffectsTTable::solveLeft(Reference rightRef,
                                            EntityName leftSynonym,
                                            StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    return solveLeftAffectsHelper<AffectsTTable>(
        &rightRef, this, &AffectsTTable::solveSingleWildcard,
        &AffectsTTable::solveHelper);
};

std::vector<std::pair<Value, Value>>
AffectsTTable::solveBoth(EntityName leftSynonym, EntityName rightSynonym,
                         StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym) || !isAssignmentEntity(rightSynonym)) {
        return std::vector<std::pair<Value, Value>>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    return solveBothAffectsHelper<AffectsTTable>(this,
                                                 &AffectsTTable::checkAffectsT);
}

std::vector<Value> AffectsTTable::solveBothReflexive(EntityName synonym,
                                                     StorageView *storage) {
    if (!isAssignmentEntity(synonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    return solveBothReflexAffectsHelper<AffectsTTable>(
        this, &AffectsTTable::checkAffectsT);
}

bool AffectsTTable::checkAffectsT(int left, int right) {
    std::pair curr = std::make_pair(left, right);
    return this->matrix[curr];
}

bool AffectsTTable::verifyDoubleWildcards() {
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            if (checkAffectsT(left, right)) {
                return true;
            }
        }
    }
    return false;
};

bool AffectsTTable::verifyLeftWildcard(int right) {
    if (!isAssignment(right)) {
        return false;
    }

    for (int left : this->assignments) {
        if (checkAffectsT(left, right)) {
            return true;
        }
    }
    return false;
}

bool AffectsTTable::verifyRightWildcard(int left) {
    if (!isAssignment(left)) {
        return false;
    }
    for (int right : this->assignments) {
        if (checkAffectsT(left, right)) {
            return true;
        }
    }
    return false;
}

bool AffectsTTable::verifySingleWildcard(int stmt, Position stmtPos) {
    if (stmtPos == Position::LEFT) {
        return verifyRightWildcard(stmt);
    } else {
        return verifyLeftWildcard(stmt);
    }
};

void AffectsTTable::solveSingleWildcard(
    std::unordered_set<Value> *intermediateResult, Position stmtPos) {
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            if (checkAffectsT(left, right)) {
                intermediateResult->insert(
                    Value(ValueType::STMT_NUM,
                          toString(chooseStmt(left, right, stmtPos))));
            }
        }
    }
};

void AffectsTTable::solveHelper(int stmt,
                                std::unordered_set<Value> *intermediateResult,
                                Position stmtPos) {
    auto check = [&](const int &other) -> bool {
        std::pair<int, int> curr = stmtPos == Position::LEFT
                                       ? std::make_pair(stmt, other)
                                       : std::make_pair(other, stmt);
        return this->matrix[curr];
    };
    for (int other : this->assignments) {
        if (check(other)) {
            intermediateResult->insert(
                Value(ValueType::STMT_NUM, toString(other)));
        }
    }
};