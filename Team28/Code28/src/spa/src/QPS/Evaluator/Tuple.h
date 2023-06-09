#pragma once
#include "../../commons/Value.h"
#include <vector>

/*
 * Encapsulates a single row in a table.
 */
class Tuple {
public:
    Tuple(std::vector<Value> values) { this->values = values; }

    size_t size();
    /*
     * Return a pair of tuples, first tuple contains the values corresponding to
     * the indices given in argument, second tuple contains the remaining
     * values.
     */
    std::vector<Tuple> splitTuple(std::vector<int> indices);

    static Tuple combineSubTuples(std::vector<Tuple> subTuples);

    bool equal(Tuple rhs);

    Value getValue(int index);

    std::vector<Value> getValues();

    std::string to_string();

private:
    std::vector<Value> values;
};
