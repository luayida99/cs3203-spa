#pragma once
#include "../../commons/Synonym.h"
#include "Tuple.h"
#include <vector>

/*
 * Encapsulate a table for storing synonyms and possible values the synonyms can
 * be
 */
class ClauseTable {
public:
    std::vector<Synonym> header;
    std::vector<Tuple> rows;
    ClauseTable(){};
    ClauseTable(std::vector<Synonym> header);

    /*
     * Given a list of synonyms, get the indices they correspond to
     */
    std::vector<int> getIndices(std::vector<Synonym> common_headers);

    /*
     * Inserts a tuple into this table
     */
    void insert(Tuple row);

    /*
     * Returns number of rows in this table
     */
    size_t size();

    /*
     * Get list of values of synonym header
     */
    std::vector<Value> getValues(Synonym select);

    /*
     * Given two tables, find the common headers
     */
    static std::vector<Synonym> getCommonHeaders(ClauseTable table1,
                                                 ClauseTable table2);

    /*
     * Given two tables, construct a new table with all the unique headers from
     * both tables
     */
    static ClauseTable ConstructTable(ClauseTable table1, ClauseTable table2);

    /*
     * Given two tables, Joins the tables
     */
    static ClauseTable joinTables(ClauseTable table1, ClauseTable table2);
};