#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "DBQuery.hpp"
#include "Statement.hpp"

namespace ECE141 {
class SQLStatement : public Statement {
   public:
    SQLStatement(Keywords aStatementType);
    ~SQLStatement(){};

    void        setTableName(std::string &aName);
    std::string getTableName();
    bool        createTableStatement(Tokenizer &aTokenizer);
    bool        showTableStatement(Tokenizer &aTokenizer);
    bool        describeTableStatement(Tokenizer &aTokenizer);
    bool        dropTableStatement(Tokenizer &aTokenizer);
    bool        insertTableStatement(Tokenizer &aTokenizer);
    bool        parseValueList(StringList &aList, Tokenizer &aTokenizer);
    bool        parseIdentifierList(StringList &aList, Tokenizer &aTokenizer);
    void        setDBQueryCommand(Tokenizer &aTokenizer);
    DBQuery    &getDBQuery() { return theDBQuery; }
    bool        vectorPush(Attribute anAttr) {
        theTableAttributes.push_back(anAttr);
        return true;
    }
    std::vector<Attribute> &getAttributevector() { return theTableAttributes; }

   protected:
    std::string            theTableName;
    std::vector<Attribute> theTableAttributes;
    DBQuery                theDBQuery;
};

}  // namespace ECE141