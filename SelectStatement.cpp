#pragma once
#include "SelectStatement.hpp"

#include "Database.hpp"
#include "Filters.hpp"
#include "Helpers.hpp"

namespace ECE141 {
SelectStatement::SelectStatement(SQLProcessor *aProc, Keywords aStatementType, Entity *anEntity) : SQLStatement::SQLStatement(aStatementType), entity(anEntity), theSQLProcessorPtr(aProc) {}

StatusResult SelectStatement::parseOperand(Tokenizer &aTokenizer, Operand &anOperand) {
    StatusResult theResult{noError};
    if (aTokenizer.current().type == TokenType::identifier) {
        anOperand.ttype = TokenType::identifier;
        try {
            int index = 0;

            while (theTableAttributes[index].getName() != aTokenizer.current().data) {
                index++;
            }
            anOperand.dtype = theTableAttributes[index].getType();
            anOperand.name = theTableAttributes[index].getName();

        } catch (...) {
            anOperand.ttype = TokenType::string;
            anOperand.value = aTokenizer.current().data;
        }
    } else if (aTokenizer.current().type == TokenType::number) {
        anOperand.ttype = TokenType::number;
        anOperand.value = aTokenizer.current().data;
    } else if (aTokenizer.current().type == TokenType::string) {
        anOperand.ttype = TokenType::string;
        anOperand.value = aTokenizer.current().data;
    } else {
        theResult.error = syntaxError;
    }
    return theResult;
}

StatusResult SelectStatement::parseJoin(Tokenizer &aTokenizer) {
    Keywords theJoinDir = aTokenizer.current().keyword;
    aTokenizer.next(2);
    std::string theTable = aTokenizer.current().data;
    aTokenizer.next(2);
    selectField theLeftField;
    selectField theRightField;
    if (aTokenizer.peek(1).data == ".") {
        std::string theTableName = aTokenizer.current().data;
        aTokenizer.next(2);
        std::string theField = aTokenizer.current().data;
        theLeftField.field = theField;
        theLeftField.table = theTableName;
    } else {
        std::string theTableName = this->getTableName();
        std::string theField = aTokenizer.current().data;
        theLeftField.field = theField;
        theLeftField.table = theTableName;
    }
    aTokenizer.next(2);  // = RightField

    if (aTokenizer.peek(1).data == ".") {
        std::string theTableName = aTokenizer.current().data;
        aTokenizer.next(2);
        std::string theField = aTokenizer.current().data;
        theRightField.field = theField;
        theRightField.table = theTableName;
    } else {
        std::string theTableName = this->getTableName();
        std::string theField = aTokenizer.current().data;
        theRightField.field = theField;
        theRightField.table = theTableName;
    }
    aTokenizer.next();

    JoinField theJoinField(theJoinDir, theLeftField, theRightField, theTable);
    bool      theJoin = true;
    this->theDBQuery.setJoin(theJoin, theJoinField);
    return StatusResult(Errors::noError);
}

StatusResult SelectStatement::parseStatement(Tokenizer &aTokenizer) {
    // First word is SELECT, so skip that
    aTokenizer.next();
    StatusResult theStatus = this->parseSelect(aTokenizer);
    if (!theStatus) {
        return StatusResult(Errors::unknownCommand);
    }

    aTokenizer.next();  // FROM
    aTokenizer.next();  // TableName
    this->theDBQuery.setEntityName(aTokenizer.current().data);
    aTokenizer.next();
    // Filters thefilters;
    while (aTokenizer.more() && aTokenizer.current().data != ";") {
        switch (aTokenizer.current().keyword) {
            case Keywords::left_kw:
                theStatus = parseJoin(aTokenizer);
                if (!theStatus) {
                    return StatusResult(theStatus.error);
                }
                break;
            case Keywords::right_kw:
                theStatus = parseJoin(aTokenizer);
                if (!theStatus) {
                    return StatusResult(theStatus.error);
                }
                break;
            case Keywords::where_kw:
                this->theDBQuery.setFilterKey(Keywords::where_kw);

                // Skip where
                aTokenizer.next();
                theStatus = this->theDBQuery.getFilter().parse(aTokenizer, *entity);
                if (!theStatus) {
                    return StatusResult(theStatus.error);
                }
                break;

            case Keywords::order_kw:
                this->theDBQuery.setFilterKey(Keywords::order_kw);
                aTokenizer.next();  // by
                if (aTokenizer.current().type == TokenType::keyword && aTokenizer.current().keyword == Keywords::by_kw) {
                    aTokenizer.next();  // should be fields
                    this->theDBQuery.setOrderBy(aTokenizer.current().data);
                    aTokenizer.next();
                } else {
                    return StatusResult(Errors::syntaxError);
                }
                break;
            case Keywords::limit_kw:
                this->theDBQuery.setFilterKey(Keywords::limit_kw);
                aTokenizer.next();
                if (aTokenizer.current().type == TokenType::number) {
                    int thelimit = std::stoi(aTokenizer.current().data);
                    this->theDBQuery.setLimit(thelimit);
                    aTokenizer.next();
                } else {
                    return StatusResult(Errors::syntaxError);
                }
                break;
            default:
                break;
        }
    }
    return StatusResult(Errors::noError);
}

StatusResult SelectStatement::parseSelect(Tokenizer &aTokenizer) {
    // Means we have *

    aTokenizer.skipTo(Keywords::from_kw);
    aTokenizer.next();
    std::string theDefaultTableName = aTokenizer.current().data;
    std::string theOtherTableName = "";
    Entity     *theLeftEntity = nullptr;
    Entity     *theRightEntity = nullptr;
    if (aTokenizer.skipTo(Keywords::join_kw)) {
        theOtherTableName = aTokenizer.peek(1).data;
        theLeftEntity = getEntity(theDefaultTableName);
        theRightEntity = getEntity(theOtherTableName);
        if (theLeftEntity == nullptr || theRightEntity == nullptr) {
            return StatusResult(Errors::unknownError);
        }
    }

    aTokenizer.restart();
    aTokenizer.next();
    if (aTokenizer.current().data == "*") {
        std::string theAll = "*";
        this->theDBQuery.setAllField(true);
        selectField theField(theDefaultTableName, theAll);
        this->theDBQuery.setFilterStruct(theField);
        return StatusResult(Errors::noError);
    }

    // Means we have atleast one field
    while (aTokenizer.peek(1).keyword != Keywords::from_kw || (aTokenizer.peek(1).data == "." && aTokenizer.peek(3).keyword != Keywords::from_kw)) {
        // this->theDBQuery.setAttr(aTokenizer.current().data);
        if (aTokenizer.peek(1).data == ".") {
            std::string thetableName = aTokenizer.current().data;
            aTokenizer.next();  // .
            aTokenizer.next();  // field name
            std::string thefieldName = aTokenizer.current().data;
            this->theDBQuery.setAttr(thefieldName);
            selectField theField(theTableName, thefieldName);
            this->theDBQuery.setFilterStruct(theField);
        } else {
            std::string thefieldName = aTokenizer.current().data;
            std::string theTbName = theDefaultTableName;
            bool        left = false;
            bool        right = false;
            if (theLeftEntity) {
                const Attribute *theLeftAttr = theLeftEntity->getAttribute(thefieldName);
                if (theLeftAttr) {
                    theTbName = theLeftEntity->getName();
                    left = true;
                }
            }
            if (theRightEntity) {
                const Attribute *theRightAttr = theRightEntity->getAttribute(thefieldName);
                if (theRightAttr) {
                    theTbName = theRightEntity->getName();
                    right = true;
                }
            }

            if (left == true && right == true) {
                return StatusResult(Errors::unknownAttribute);
            }

            this->theDBQuery.setAttr(thefieldName);
            selectField theField(theTbName, thefieldName);
            this->theDBQuery.setFilterStruct(theField);
        }
        aTokenizer.next(2);
    }
    // To push the last field
    if (aTokenizer.peek(1).data == ".") {
        std::string thetableName = aTokenizer.current().data;
        aTokenizer.next();  // .
        aTokenizer.next();  // field name
        std::string thefieldName = aTokenizer.current().data;
        selectField theField(theTableName, thefieldName);
        this->theDBQuery.setFilterStruct(theField);
    } else {
        std::string thefieldName = aTokenizer.current().data;
        std::string theTbName = theDefaultTableName;
        bool        left = false;
        bool        right = false;
        if (theLeftEntity) {
            const Attribute *theLeftAttr = theLeftEntity->getAttribute(thefieldName);
            if (theLeftAttr) {
                theTbName = theLeftEntity->getName();
                left = true;
            }
        }
        if (theRightEntity) {
            const Attribute *theRightAttr = theRightEntity->getAttribute(thefieldName);
            if (theRightAttr) {
                theTbName = theRightEntity->getName();
                right = true;
            }
        }
        if (left == true && right == true) {
            return StatusResult(Errors::unknownAttribute);
        }
        this->theDBQuery.setAttr(aTokenizer.current().data);
        selectField theField(theTbName, aTokenizer.current().data);
        this->theDBQuery.setFilterStruct(theField);
    }

    return StatusResult(Errors::noError);
}

// Function to check if tokenized tokens represent SELECT...;
bool SelectStatement::checkSelectTable(Tokenizer aTokenizer) {
    Token              theSelectToken{TokenType::keyword, Keywords::select_kw,
                         Operators::unknown_op, "select"};
    Token              theFromToken{TokenType::keyword, Keywords::from_kw,
                       Operators::unknown_op, "from"};
    Token              theTableToken{TokenType::identifier, Keywords::unknown_kw,
                        Operators::unknown_op, "table_name"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theSelectToken);
    SQLVector.push_back(theFromToken);
    SQLVector.push_back(theTableToken);
    // It has to be atleast SELECT * FROM Table_name
    if (aTokenizer.size() < 4) {
        return false;
    }
    bool checkSelect = false;

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
            (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }

        if (!checkSelect && aTokenizer.peek(1).data == "*") {
            checkSelect = true;
            aTokenizer.next();
        } else {
            // check if we have like this identifier_kw,identifier_kw,identifier_kw
            if (!checkSelect) {
                while (!checkSelect && aTokenizer.more() && aTokenizer.peek(2).data != "from") {
                    aTokenizer.next();
                    if (aTokenizer.current().type != TokenType::identifier) {
                        return false;
                    }
                    aTokenizer.skipTo(',');
                }
                checkSelect = true;
                aTokenizer.next();
            }
        }

        aTokenizer.next();
    }

    return true;
}

Entity *SelectStatement::getEntity(std::string &aName) {
    SQLProcessor *theSQLProcessorPtr = getSQLProcessor();
    Database     *theDb = theSQLProcessorPtr->getDatabaseInUse();
    uint32_t      theBlockNum = theDb->getEntityFromMap(aName);
    if (theBlockNum == -1) {
        return nullptr;
    }
    Block theDescribeBlock;
    theDb->getStorage().readBlock(theBlockNum, theDescribeBlock);
    Entity *theEntity = nullptr;
    if (theDescribeBlock.header.theTitle == aName) {
        theEntity = new Entity(aName);
        theEntity->decodeBlock(theDescribeBlock);
    }

    return theEntity;
}

Statement *SelectStatement::selectStatement(SQLProcessor *aProc, Tokenizer &aTokenizer) {
    aTokenizer.skipTo(Keywords::from_kw);
    aTokenizer.next();
    Block     theDescribeBlock;
    Database *theDb = aProc->getDatabaseInUse();
    uint32_t  theBlockNum = theDb->getEntityFromMap(aTokenizer.current().data);
    theDb->getStorage().readBlock(theBlockNum, theDescribeBlock);
    Entity *theEntity;

    if (theDescribeBlock.header.theTitle == aTokenizer.current().data) {
        theEntity = new Entity(aTokenizer.current().data);
        theEntity->decodeBlock(theDescribeBlock);
    }
    SelectStatement *theSelectStatememt = new SelectStatement(aProc, Keywords::select_kw, theEntity);
    // Restart the token Index
    aTokenizer.restart();
    StatusResult theStatus = theSelectStatememt->parse(aTokenizer);
    delete theEntity;
    return theSelectStatememt;
}

StatusResult SelectStatement::parse(Tokenizer &aTokenizer) {
    setDBQueryCommand(aTokenizer);
    aTokenizer.restart();
    return parseStatement(aTokenizer);
}

StatusResult SelectStatement::run(std::ostream &aStream) const {
    SQLProcessor    *theSQLProcessorPtr = this->getSQLProcessor();
    Database        *theDatabase = theSQLProcessorPtr->getDatabaseInUse();
    SelectStatement *theSelectStatememt = const_cast<SelectStatement *>(this);
    return theDatabase->showQuery(theSelectStatememt, aStream);
}

}  // namespace ECE141
