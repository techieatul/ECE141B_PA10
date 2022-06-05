#pragma once
#include "InsertTableStatement.hpp"

#include "Database.hpp"
#include "Helpers.hpp"

namespace ECE141 {

StatusResult InsertTableStatement::parse(Tokenizer &aTokenizer) {
    if (2 < aTokenizer.remaining()) {  // now token: table name
        aTokenizer.next();
        StringList theFields;
        if (aTokenizer.skipIf('(')) {
            parseIdentifierList(theFields, aTokenizer);
            if (0 == theFields.size()) return StatusResult{syntaxError};
        }

        if (aTokenizer.skipIf(Keywords::values_kw)) {
            if (!makeRowsFromValueLists(aTokenizer, theFields)) {
                StatusResult{syntaxError};
            }
            return StatusResult{};
        } else
            StatusResult{syntaxError};
    }
    return StatusResult{syntaxError};
}

bool InsertTableStatement::createRow(InsertTableStatement               &aStatement,
                                     std::map<std::string, std::string> &aKVList) {
    KeyValues            theData;

    const AttributeList &theAttributes = entity->getAttributes();
    for (auto &theAttr : theAttributes) {
        const std::string &theName = theAttr.getName();

        Value              theValue("NULL");
        if (aKVList.count(theName)) {
            if (theAttr.getType() == DataTypes::int_type) {
                theValue = Helpers::convertStrToInt(aKVList[theName]);
            } else if (theAttr.getType() == DataTypes::bool_type) {
                theValue = Helpers::convertStrToBool(aKVList[theName]);
            } else if (theAttr.getType() == DataTypes::float_type) {
                theValue = Helpers::convertStrToDouble(aKVList[theName]);
            } else {
                theValue = aKVList[theName];
            }
        }

        theData[theName] = theValue;
        // std::cout << "size " << theData.size() << "\n";
    }

    Row theRow(theData);
    aStatement.rows->push_back(theRow);
    // std::cout << "size " << aStatement.rows[0].getData().size() << "\n";

    return true;
}
bool InsertTableStatement::makeRowsFromValueLists(Tokenizer  &aTokenizer,
                                                  StringList &aFields) {
    while (aTokenizer.current().data == "(") {
        aTokenizer.next();  // into value
        StringList theValues;
        parseValueList(theValues, aTokenizer);

        aTokenizer.skipIf(',');  // ignore comma...

        // let's combine the keys+values, so we can build a row...
        size_t theSize = aFields.size();
        if (theValues.size() == theSize) {
            std::map<std::string, std::string> theKVs;
            for (size_t i = 0; i < theSize; i++) {
                theKVs[aFields[i]] = theValues[i];
            }
            if (!createRow(*this, theKVs)) {
                return false;
            }
        } else
            return false;
    }

    return true;
}

bool InsertTableStatement::checkInsertTable(Tokenizer aTokenizer) {
    Token              theInsertToken{TokenType::keyword, Keywords::insert_kw,
                         Operators::unknown_op, "insert"};
    Token              theIntoToken{TokenType::keyword, Keywords::into_kw,
                       Operators::unknown_op, "into"};
    Token              theTableNameToken{TokenType::identifier, Keywords::unknown_kw,
                            Operators::unknown_op, "table_name"};
    Token              theOpenParanToken{TokenType::punctuation, Keywords::unknown_kw,
                            Operators::unknown_op, "("};
    Token              theValuesToken{TokenType::keyword, Keywords::values_kw,
                         Operators::unknown_op, "values"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theInsertToken);
    SQLVector.push_back(theIntoToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theOpenParanToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
            (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }
    // jump to value keyword
    if (!aTokenizer.skipTo(Keywords::values_kw)) {
        return false;
    }
    // ckeck ')' vlaues '('
    if ((aTokenizer.peek(-1).data != ")") || aTokenizer.peek(1).data != "(") {
        return false;
    }
    aTokenizer.skipTo(';');

    if (aTokenizer.tokenAt(aTokenizer.getIndex() - 1).data[0] != ')' ||
        aTokenizer.current().data[0] != ';') {
        return false;
    }

    return true;
}
// Function to check if tokenized tokens represent SELECT...;
Statement *InsertTableStatement::insertTableStatement(SQLProcessor *aProc, Tokenizer &aTokenizer) {
    aProc->resetRowData();
    aTokenizer.skipTo(TokenType::identifier);
    Block     theDescribeBlock;
    Database *theCurrentActiveDb = aProc->getDatabaseInUse();
    uint32_t  theBlockNum = (theCurrentActiveDb)->getEntityFromMap(aTokenizer.current().data);

    (theCurrentActiveDb)->getStorage().readBlock(theBlockNum, theDescribeBlock);
    Entity *theEntity;

    if (theDescribeBlock.header.theTitle == aTokenizer.current().data) {
        theEntity = new Entity(aTokenizer.current().data);
        theEntity->decodeBlock(theDescribeBlock);
    }
    InsertTableStatement *theInsertTable = new InsertTableStatement(aProc, Keywords::insert_kw, aProc->getTheRowData(), theEntity);

    theInsertTable->setTableName(aTokenizer.current().data);
    theInsertTable->parse(aTokenizer);
    delete theEntity;
    return theInsertTable;
}
StatusResult InsertTableStatement::run(std::ostream &aStream) const {
    Database             *theDatabase = theSQLProcessorPtr->getDatabaseInUse();
    InsertTableStatement *theInsertTableStmt = const_cast<InsertTableStatement *>(this);
    return theDatabase->insertTable(theInsertTableStmt, aStream);
};
}  // namespace ECE141