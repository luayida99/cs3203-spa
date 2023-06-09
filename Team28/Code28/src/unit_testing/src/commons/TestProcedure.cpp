#include "catch.hpp"

#include "commons/Procedure.h"

TEST_CASE("Procedure is initialised correctly and inherits all Entity methods "
          "correctly") {
    Procedure test = Procedure("test", 1);
    Procedure testCopy = Procedure("test", 1);
    Procedure notTestCopy = Procedure("not test", 2);

    // getName() gets the name correctly
    REQUIRE(test.getName() == "test");
    // Different instances of Procedure are equal if ProcedureName is the same
    REQUIRE(test == testCopy);
}
