#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Modifies relationship.
 */
class Modifies : Relationship<Reference, Reference> {
public:
    Modifies(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        return this->getRight().type == ReferenceType::ENT_REF;
    }
};