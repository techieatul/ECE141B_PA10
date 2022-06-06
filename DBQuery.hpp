#ifndef DBQuery_hpp
#define DBQuery_hpp

#include <stdio.h>

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Attribute.hpp"
#include "Entity.hpp"
#include "Filters.hpp"
#include "Row.hpp"
#include "Tokenizer.hpp"
#include "keywords.hpp"
using FilterKeyword = std::vector<ECE141::Keywords>;
using FilterStruct = std::vector<ECE141::selectField>;
namespace ECE141 {

struct GroupField{
    std::string theAggrFunc;
    std::string theFieldName;

};
struct JoinField {
    JoinField() {}
    JoinField(Keywords aJoinKey, selectField &aLeft, selectField &aRight, std::string &aTableName) : theJoinKey(aJoinKey), theLeftField(aLeft), theRightField(aRight), theDefaultTable(aTableName) {}
    ~JoinField(){};
    JoinField(JoinField &aCopy) : theJoinKey(aCopy.theJoinKey), theLeftField(aCopy.theLeftField), theRightField(aCopy.theRightField), theDefaultTable(aCopy.theDefaultTable) {
        *this = aCopy;
    }
    JoinField &operator=(JoinField &aCopy) {
        theJoinKey = aCopy.theJoinKey;
        theLeftField = aCopy.theLeftField;
        theRightField = aCopy.theRightField;
        theDefaultTable = aCopy.theDefaultTable;
        return *this;
    }
    Keywords    theJoinKey;
    selectField theLeftField;
    selectField theRightField;
    std::string theDefaultTable;
};
class DBQuery {
   public:
    DBQuery();
    DBQuery(const DBQuery &aCopy);
    ~DBQuery();
    DBQuery      &operator=(const DBQuery &aCopy);

    bool          getAllField() { return theAllField; };
    StringList    getAttr() { return theAttr; };
    std::string   getEntityName() { return theEntityName; };
    Entity       *getEntity() { return theEntity; };
    std::string  &getOrderBy() { return theOrderBy; }
    bool          getIsAscending() { return isAscending; }
    Filters      &getFilter() { return theFilter; }
    int           getLimit() { return theLimit; }
    FilterKeyword getFilterKey() { return theFilterKey; }
    FilterStruct &getFilterKeyStruct() { return theFilterStructKey; }
    Value        &getKeyValue(const std::string &aKey);
    void          getUpdateKeys(StringList &aKeyToUpdate);
    bool          getJoin() { return theJoin; }
    std::string   getCommandString() { return theCommandString; }
    void          setCommandString(std::string aCommand) { theCommandString = aCommand; };
    JoinField    &getJoinField() { return theJoinField; }
    DBQuery      &setAllField(bool aValue);
    DBQuery      &setAttr(const std::string &aField);
    DBQuery      &setAttrPrimary(const std::string &aField);
    DBQuery      &setEntityName(const std::string &aName);
    DBQuery      &setEntity(Entity *anEntity);
    DBQuery      &setOrderBy(std::string &anOrderBy);
    DBQuery      &setIsAcending(bool &anAsc);
    DBQuery      &setLimit(int &aLimit);
    DBQuery      &setFilterKey(const Keywords &aKey);
    DBQuery      &setFilterStruct(const selectField &aStruct);
    DBQuery      &setKeyValue(const std::string &aKey, Value &aVal);
    DBQuery      &setJoin(bool aJoin, JoinField &aJoinKey);
    DBQuery      &setGroupBy(bool &aGroupBy,std::string &aggrField, std::string &aFieldName);

   protected:
    std::string   theEntityName;
    Entity       *theEntity;
    StringList    theAttr;
    FilterKeyword theFilterKey;
    bool          theAllField;
    std::string   theOrderBy;
    bool          isAscending;
    Filters       theFilter;
    int           theLimit;
    KeyValues     theKeyValue;
    FilterStruct  theFilterStructKey;
    bool          theJoin;
    JoinField     theJoinField;
    std::string   theCommandString;
    GroupField    theGroupField;
    bool          theGroupBy;
};
}  // namespace ECE141

#endif /* DBQuery_hpp */