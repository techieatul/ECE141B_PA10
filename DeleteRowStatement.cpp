#pragma once
#include "DeleteRowStatement.hpp"

#include "Database.hpp"
#include "Helpers.hpp"

namespace ECE141 {

StatusResult DeleteRowStatement::parse(Tokenizer& aTokenizer) {
    theDBQuery.setEntity(entity);
    aTokenizer.next();  // skip where
    aTokenizer.next();
    StatusResult theStatus = theDBQuery.getFilter().parse(aTokenizer, *entity);
    return theStatus;
}

bool DeleteRowStatement::checkDeleteRow(Tokenizer aTokenizer) {
    // DELETE FROM Users WHERE
    Token theUpdate = aTokenizer.current();
    Token theFrom = aTokenizer.peek(1);
    Token theTableName = aTokenizer.peek(2);
    Token theWhere = aTokenizer.peek(3);
    if (theUpdate.keyword != Keywords::delete_kw || theFrom.keyword != Keywords::from_kw || theTableName.type != TokenType::identifier || theWhere.keyword != Keywords::where_kw) {
        return false;
    }
    return true;
}

Statement* DeleteRowStatement::deleteRowStatement(SQLProcessor* aProc, Tokenizer& aTokenizer) {
    aTokenizer.skipTo(Keywords::from_kw);
    aTokenizer.next();  // Table Name
    std::string theTableName = aTokenizer.current().data;
    Block       theDescribeBlock;
    Database*   theDb = aProc->getDatabaseInUse();
    uint32_t    theBlockNum = theDb->getEntityFromMap(theTableName);
    if (theBlockNum == -1) {
        return nullptr;
    }
    theDb->getStorage().readBlock(theBlockNum, theDescribeBlock);
    Entity* theEntity;
    if (theDescribeBlock.header.theTitle == theTableName) {
        theEntity = new Entity(theTableName);
        theEntity->decodeBlock(theDescribeBlock);
    }

    DeleteRowStatement* theDeleteStmt = new DeleteRowStatement(aProc, Keywords::delete_kw, theEntity);
    StatusResult        theStatus = theDeleteStmt->parse(aTokenizer);
    if (!theStatus) {
        return nullptr;
    }
    //delete theEntity;
    return theDeleteStmt;
}

StatusResult DeleteRowStatement::run(std::ostream& aStream) const{
    SQLProcessor* theSQLProcessorPtr = this->getSQLProcessor();
    Database*     theDatabase = theSQLProcessorPtr->getDatabaseInUse();
    DeleteRowStatement* theDeleteStmt = const_cast<DeleteRowStatement*>(this);
    return theDatabase->deleteRow(theDeleteStmt, aStream);
}

}  // namespace ECE141