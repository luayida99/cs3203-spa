#include "catch.hpp"

#include "PKB/Tables/RelationshipsTable/ProcToProcRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/ProcToVarRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/RelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/StmtToStmtRelationshipsTable.h"
#include "PKB/Tables/RelationshipsTable/StmtToVarRelationshipsTable.h"

TEST_CASE("RelationshipsTable can be initialised correctly") {
    RelationshipsTable<int, int> table;

    REQUIRE(table.getTableSize() == -1);
}

TEST_CASE("RelationshipsTable can store and retrieve correctly") {
    RelationshipsTable<int, int> table;
    Relationship<int, int> test =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);

    table.store(&test);

    // successfully stored 1 item to left map
    REQUIRE(table.retrieveLeft(1).size() == 1);
    // successfully stored 1 item to right map
    REQUIRE(table.retrieveRight(2).size() == 1);
}

TEST_CASE("RelationshipsTable can storeLeft and retrieve correctly") {
    RelationshipsTable<int, int> table;

    table.storeRightToLeftMap(1, 2);

    // successfully stored 1 item to left map
    REQUIRE(table.retrieveLeft(1).size() == 1);
    // nothing stored to right map
    REQUIRE(table.retrieveRight(2).size() == 0);
}

TEST_CASE("RelationshipsTable can storeRight and retrieve correctly") {
    RelationshipsTable<int, int> table;

    table.storeLeftToRightMap(1, 2);

    // successfully stored 1 item to right map
    REQUIRE(table.retrieveRight(1).size() == 1);
    // nothing stored to left map
    REQUIRE(table.retrieveLeft(2).size() == 0);
}

TEST_CASE("ParentTable can store multiple Rights to the same Left and retrieve "
          "correctly") {
    ParentTable parentTable;

    parentTable.storeRightToLeftMap(1, 2);
    parentTable.storeRightToLeftMap(1, 3);
    parentTable.storeRightToLeftMap(1, 4);

    // successfully stored 3 items to right map
    REQUIRE(parentTable.retrieveLeft(1).size() == 3);
    // all items are stored
    REQUIRE(parentTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(parentTable.retrieveLeft(1).count(3) == 1);
    REQUIRE(parentTable.retrieveLeft(1).count(4) == 1);
    // no incorrect items are stored
    REQUIRE(parentTable.retrieveLeft(1).count(1) == 0);
    // nothing stored to left map
    REQUIRE(parentTable.retrieveRight(2).size() == 0);
}

TEST_CASE("ParentTTable can store multiple Lefts to the same Right and "
          "retrieve correctly") {
    ParentTTable parentTTable;

    parentTTable.storeLeftToRightMap(10, 1);
    parentTTable.storeLeftToRightMap(10, 2);
    parentTTable.storeLeftToRightMap(10, 5);

    // successfully stored 3 items to left map
    REQUIRE(parentTTable.retrieveRight(10).size() == 3);
    // all items are stored
    REQUIRE(parentTTable.retrieveRight(10).count(1) == 1);
    REQUIRE(parentTTable.retrieveRight(10).count(2) == 1);
    REQUIRE(parentTTable.retrieveRight(10).count(5) == 1);
    // no incorrect items are stored
    REQUIRE(parentTTable.retrieveRight(10).count(10) == 0);
    // nothing stored to right map
    REQUIRE(parentTTable.retrieveLeft(1).size() == 0);
}

TEST_CASE("FollowsTable can initialise, store and retrieve correctly") {
    FollowsTable followsTable;

    // Follows (1, 2)
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable.store(&test1);
    // Follows (2, 5)
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 5);
    followsTable.store(&test2);

    // successfully stored Follows (1, 2)
    REQUIRE(followsTable.retrieveLeft(1).size() == 1);
    REQUIRE(followsTable.retrieveRight(2).size() == 1);
    REQUIRE(followsTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(followsTable.retrieveRight(2).count(1) == 1);

    // successfully stored Follows (2, 5)
    REQUIRE(followsTable.retrieveLeft(2).size() == 1);
    REQUIRE(followsTable.retrieveRight(5).size() == 1);
    REQUIRE(followsTable.retrieveLeft(2).count(5) == 1);
    REQUIRE(followsTable.retrieveRight(5).count(2) == 1);

    // 1 and 5 aren't stored on wrong map
    REQUIRE(followsTable.retrieveRight(1).size() == 0);
    REQUIRE(followsTable.retrieveLeft(5).size() == 0);
}

TEST_CASE("FollowsTTable can initialise, store and retrieve correctly") {
    FollowsTTable followsTTable;

    // Follows* (1, 2)
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 2);
    followsTTable.store(&test1);
    // Follows* (1, 3), implicitly Follows (2, 3) but not testing latter
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS_T, 1, 3);
    followsTTable.store(&test2);

    // successfully stored Follows* relations
    REQUIRE(followsTTable.retrieveLeft(1).size() == 2);
    REQUIRE(followsTTable.retrieveRight(2).size() == 1);
    REQUIRE(followsTTable.retrieveRight(3).size() == 1);

    // values in maps are correct
    REQUIRE(followsTTable.retrieveLeft(1).count(2) == 1);
    REQUIRE(followsTTable.retrieveLeft(1).count(3) == 1);
    REQUIRE(followsTTable.retrieveRight(2).count(1) == 1);
    REQUIRE(followsTTable.retrieveRight(3).count(1) == 1);

    // 1 and 3 aren't stored on wrong map
    REQUIRE(followsTTable.retrieveRight(1).size() == 0);
    REQUIRE(followsTTable.retrieveLeft(3).size() == 0);
}

TEST_CASE("ModifiesSTable can store multiple Rights to the same Left and "
          "retrieve correctly") {
    ModifiesSTable modifiesSTable;

    // Modifies (1, "x1"), Modifies (1, "read"), Modifies (1, "B99")
    modifiesSTable.storeRightToLeftMap(1, "x1");
    modifiesSTable.storeRightToLeftMap(1, "read");
    modifiesSTable.storeRightToLeftMap(1, "B99");

    // successfully stored 3 items to right map
    REQUIRE(modifiesSTable.retrieveLeft(1).size() == 3);
    // all items are stored
    REQUIRE(modifiesSTable.retrieveLeft(1).count("x1") == 1);
    REQUIRE(modifiesSTable.retrieveLeft(1).count("read") == 1);
    REQUIRE(modifiesSTable.retrieveLeft(1).count("B99") == 1);
    // nothing stored to left map
    REQUIRE(modifiesSTable.retrieveRight("x1").size() == 0);
}

TEST_CASE("ModifiesSTable can store multiple Lefts to the same Right and "
          "retrieve correctly") {
    ModifiesSTable modifiesSTable;

    // Modifies (1, "x1"), Modifies (20, "x1"), Modifies (1000, "x1")
    modifiesSTable.storeLeftToRightMap("x1", 1);
    modifiesSTable.storeLeftToRightMap("x1", 20);
    modifiesSTable.storeLeftToRightMap("x1", 1000);

    // successfully stored 3 items to left map
    REQUIRE(modifiesSTable.retrieveRight("x1").size() == 3);
    // all items are stored
    REQUIRE(modifiesSTable.retrieveRight("x1").count(1) == 1);
    REQUIRE(modifiesSTable.retrieveRight("x1").count(20) == 1);
    REQUIRE(modifiesSTable.retrieveRight("x1").count(1000) == 1);
    // nothing stored to right map
    REQUIRE(modifiesSTable.retrieveLeft(1).size() == 0);
}

TEST_CASE("ModifiesPTable can initialise, store and retrieve correctly") {
    ModifiesPTable modifiesPTable;

    // Modifies ("main", "v")
    Relationship<std::string, std::string> test1 = Relationship(
        RelationshipReference::MODIFIES, std::string("main"), std::string("v"));
    modifiesPTable.store(&test1);
    // Modifies ("foo", "v")
    Relationship<std::string, std::string> test2 = Relationship(
        RelationshipReference::MODIFIES, std::string("foo"), std::string("v"));
    modifiesPTable.store(&test2);

    // successfully stored Modifies relations
    REQUIRE(modifiesPTable.retrieveLeft("main").size() == 1);
    REQUIRE(modifiesPTable.retrieveLeft("foo").size() == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").size() == 2);

    // values in maps are correct
    REQUIRE(modifiesPTable.retrieveLeft("main").count("v") == 1);
    REQUIRE(modifiesPTable.retrieveLeft("foo").count("v") == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").count("main") == 1);
    REQUIRE(modifiesPTable.retrieveRight("v").count("foo") == 1);

    // "main" and "v" aren't stored on wrong map
    REQUIRE(modifiesPTable.retrieveRight("main").size() == 0);
    REQUIRE(modifiesPTable.retrieveLeft("v").size() == 0);
}

TEST_CASE("UsesSTable can initialise, store and retrieve correctly") {
    UsesSTable usesSTable;

    // Uses (3, "v")
    Relationship<int, std::string> test =
        Relationship(RelationshipReference::USES, 3, std::string("v"));
    usesSTable.store(&test);

    // successfully stored Uses (3, "v")
    REQUIRE(usesSTable.retrieveLeft(3).size() == 1);
    REQUIRE(usesSTable.retrieveRight("v").size() == 1);

    // values in maps are correct
    REQUIRE(usesSTable.retrieveLeft(3).count("v") == 1);
    REQUIRE(usesSTable.retrieveRight("v").count(3) == 1);
}

TEST_CASE("UsesPTable can initialise, store and retrieve correctly") {
    UsesPTable usesPTable;

    // Uses ("foo", "v")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::USES, std::string("foo"), std::string("v"));
    usesPTable.store(&test);

    // successfully stored Uses ("foo", "v")
    REQUIRE(usesPTable.retrieveLeft("foo").size() == 1);
    REQUIRE(usesPTable.retrieveRight("v").size() == 1);

    // values in maps are correct
    REQUIRE(usesPTable.retrieveLeft("foo").count("v") == 1);
    REQUIRE(usesPTable.retrieveRight("v").count("foo") == 1);
}

TEST_CASE("CallsTable can initialise, store and retrieve correctly") {
    CallsTable callsTable;

    // Calls ("foo", "bar")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS, std::string("foo"), std::string("bar"));
    callsTable.store(&test);

    // successfully stored Calls ("foo", "bar")
    REQUIRE(callsTable.retrieveLeft("foo").size() == 1);
    REQUIRE(callsTable.retrieveRight("bar").size() == 1);

    // values in maps are correct
    REQUIRE(callsTable.retrieveLeft("foo").count("bar") == 1);
    REQUIRE(callsTable.retrieveRight("bar").count("foo") == 1);
}

TEST_CASE("CallsTTable can initialise, store and retrieve correctly") {
    CallsTTable callsTTable;

    // Calls* ("foo", "bar")
    Relationship<std::string, std::string> test = Relationship(
        RelationshipReference::CALLS, std::string("foo"), std::string("bar"));
    callsTTable.store(&test);

    // successfully stored Calls* ("foo", "bar")
    REQUIRE(callsTTable.retrieveLeft("foo").size() == 1);
    REQUIRE(callsTTable.retrieveRight("bar").size() == 1);

    // values in maps are correct
    REQUIRE(callsTTable.retrieveLeft("foo").count("bar") == 1);
    REQUIRE(callsTTable.retrieveRight("bar").count("foo") == 1);
}
