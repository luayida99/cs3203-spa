#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "TableValue.h"

enum class EntityName {
    PROCEDURE,
    STMT,
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    VARIABLE,
    CONSTANT,
};

enum class EntityAttribute { PROC_NAME, VAR_NAME, VALUE, STMT_NO };

typedef std::unordered_set<EntityName> EntityTypeSet;
const EntityTypeSet STMT_REF_SET = {
    EntityName::STMT,  EntityName::READ, EntityName::PRINT, EntityName::CALL,
    EntityName::WHILE, EntityName::IF,   EntityName::ASSIGN};

const EntityTypeSet ENT_REF_SET = {EntityName::VARIABLE, EntityName::PROCEDURE,
                                   EntityName::CONSTANT};

typedef std::map<EntityName, EntityAttribute> InferredEntityAttributeMap;

const InferredEntityAttributeMap SECONDARY_ATTRIBUTE_MAP = {
    {EntityName::CALL, EntityAttribute::PROC_NAME},
    {EntityName::PRINT, EntityAttribute::VAR_NAME},
    {EntityName::READ, EntityAttribute::VAR_NAME},
};

typedef std::unordered_map<EntityName, std::unordered_set<EntityAttribute>>
    EntityAtttributeMap;

const EntityAtttributeMap ENTITY_ATTRIBUTE_MAP = {
    {EntityName::PROCEDURE, {EntityAttribute::PROC_NAME}},
    {EntityName::STMT, {EntityAttribute::STMT_NO}},
    {EntityName::READ, {EntityAttribute::STMT_NO, EntityAttribute::VAR_NAME}},
    {EntityName::PRINT, {EntityAttribute::STMT_NO, EntityAttribute::VAR_NAME}},
    {EntityName::ASSIGN, {EntityAttribute::STMT_NO}},
    {EntityName::CALL, {EntityAttribute::STMT_NO, EntityAttribute::PROC_NAME}},
    {EntityName::WHILE, {EntityAttribute::STMT_NO}},
    {EntityName::IF, {EntityAttribute::STMT_NO}},
    {EntityName::VARIABLE, {EntityAttribute::VAR_NAME}},
    {EntityName::CONSTANT, {EntityAttribute::VALUE}},
};

/*
 * Class encapsulating 1 of the following design entities: Procedure, Constant,
 * Variable.
 */
class Entity : public TableValue {
public:
    explicit Entity(std::string name) { this->name = name; };

    std::string getName() { return this->name; };

    bool operator==(const Entity &other) const {
        return this->name == other.name;
    };

private:
    std::string name;
};
