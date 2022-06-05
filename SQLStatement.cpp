#include "SQLStatement.hpp"

#include <sstream>

#include "Database.hpp"
#include "Entity.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {
SQLStatement::SQLStatement(Keywords aStatementType)
    : Statement(aStatementType) {}

void SQLStatement::setTableName(std::string &aName) {
    SQLStatement::theTableName = aName;
}

std::string SQLStatement::getTableName() { return SQLStatement::theTableName; }

bool        SQLStatement::createTableStatement(Tokenizer &aTokenizer) {
    aTokenizer.skipTo(TokenType::identifier);  // Skip to table name
    this->setTableName(aTokenizer.current().data);
    aTokenizer.skipTo('(');
    Attribute theAttr;
    bool      theNot = false;
    while (aTokenizer.current().data != ";" && aTokenizer.more()) {
        while (aTokenizer.current().data != "," &&
               aTokenizer.current().data != ")" && aTokenizer.more()) {
            if (aTokenizer.current().type == TokenType::identifier) {
                theAttr.setName(aTokenizer.current().data);
            }
            switch (aTokenizer.current().keyword) {
                case Keywords::primary_kw:
                    theAttr.setPrimaryKey(true);
                    break;

                case Keywords::not_kw:
                    theNot = true;
                    break;

                case Keywords::null_kw:
                    theAttr.setNullable(true);
                    if (theNot) {
                        theAttr.setNullable(false);
                    }
                    theNot = false;
                    break;

                case Keywords::auto_increment_kw:
                    theAttr.setAutoIncrement(true);
                    break;

                case Keywords::integer_kw:
                    theAttr.setDataType(DataTypes::int_type);
                    break;

                case Keywords::boolean_kw:
                    theAttr.setDataType(DataTypes::bool_type);
                    break;

                case Keywords::float_kw:
                    theAttr.setDataType(DataTypes::float_type);
                    break;

                case Keywords::current_timestamp_kw:
                    theAttr.setDataType(DataTypes::datetime_type);
                    break;

                case Keywords::varchar_kw:
                    theAttr.setDataType(DataTypes::varchar_type);
                    aTokenizer.skipTo(TokenType::number);
                    theAttr.setSize(stoi(aTokenizer.current().data));
                    aTokenizer.next();
                    break;
            }

            aTokenizer.next();
        }
        this->vectorPush(theAttr);
        theNot = false;
        theAttr.reset();
        aTokenizer.next();
    }
    // aTokenizer.skipTo(';');

    return true;
}
bool SQLStatement::parseIdentifierList(StringList &aList,
                                       Tokenizer  &aTokenizer) {
    while (aTokenizer.more()) {
        Token &theToken = aTokenizer.current();
        if (TokenType::identifier == aTokenizer.current().type) {
            aList.push_back(theToken.data);
            aTokenizer.next();  // skip identifier...
            aTokenizer.skipIf(',');
        } else if (aTokenizer.skipIf(')')) {
            break;
        } else
            return false;
    }
    return true;
}
bool SQLStatement::parseValueList(StringList &aList, Tokenizer &aTokenizer) {
    while (aTokenizer.more()) {
        Token &theToken = aTokenizer.current();
        if (TokenType::identifier == theToken.type ||
            TokenType::number == theToken.type) {
            std::stringstream ss;
            std::string       theData = aTokenizer.current().data;
            for (int i = 0; i < theData.length(); i++) {
                if (theData.at(i) != ',') {
                    ss << theData.at(i);
                }
            }
            // Author C,
            // I,I,puch
            theData = ss.str();
            aList.push_back(theData);

            aTokenizer.next();  // skip identifier...
            aTokenizer.skipIf(',');
        } else if (aTokenizer.skipIf(')')) {
            break;
        } else
            return false;
    }
    return true;
}
void SQLStatement::setDBQueryCommand(Tokenizer &aTokenizer) {
    aTokenizer.restart();
    std::stringstream ss;
    while (aTokenizer.more()) {
        ss >> aTokenizer.current().data;
        aTokenizer.next();
    }
    this->theDBQuery.setCommandString(ss.str());
}

}  // namespace ECE141