//
//  SQLProcessor.cpp
//  PA3
//
//  Created by rick gessner on 4/2/22.
//

#include "SQLProcessor.hpp"

#include <sstream>
#include <unordered_set>

#include "Attribute.hpp"
#include "Config.hpp"
#include "Database.hpp"
#include "Entity.hpp"
#include "Helpers.hpp"
#include "Row.hpp"
#include "SQLStatement.hpp"
#include "Tokenizer.hpp"
namespace ECE141 {

using SQLStmtFactory = Statement *(*)(SQLProcessor *aProc,
                                      Tokenizer    &aTokenizer);

SQLProcessor::SQLProcessor(std::ostream &anOutput,
                           Database    **aCurrentActiveDbPtr)
    : CmdProcessor(anOutput), currentActiveDbPtr(aCurrentActiveDbPtr) {}

SQLProcessor::~SQLProcessor() {}

void SQLProcessor::setDatabaseInUse(Database *database) {
    (*currentActiveDbPtr) = database;
}

// To remove a database connection
void SQLProcessor::releaseDatabase() {
    delete (*currentActiveDbPtr);
}
// Check if DB exists
bool SQLProcessor::dbExists(const std::string &aDBName) {
    // Atul added
    std::string theDBPath = Config::getDBPath(aDBName);
    return std::filesystem::exists(theDBPath);
}

// Check if the current active DB matches aDBName
bool SQLProcessor::checkActiveDBName(const std::string &aDBName) {
    if ((*currentActiveDbPtr) != nullptr && (*currentActiveDbPtr)->getDbName() == aDBName) {
        return true;
    }
    return false;
}

// To return current active Database pointer
Database *SQLProcessor::getDatabaseInUse() {
    return *currentActiveDbPtr;
}

RowVectors *SQLProcessor::getTheRowData() {
    return &theRowData;
}

// To dispact respectivve recognize function
std::map<Keywords, recognizeVisitor> theRecognizemap{
    {Keywords::create_kw, CreateTableStatement::checkCreateTable},
    {Keywords::show_kw, ShowTableStatement::checkShowTable},
    {Keywords::describe_kw, DescribeTableStatement::checkDescribeTable},
    {Keywords::drop_kw, DropTableStatement::checkDropTable},
    {Keywords::insert_kw, InsertTableStatement::checkInsertTable},
    {Keywords::select_kw, SelectStatement::checkSelectTable},
    {Keywords::update_kw, UpdateTableStatement::checkUpdateTable},
    {Keywords::delete_kw, DeleteRowStatement::checkDeleteRow},
    {Keywords::alter_kw, AlterTableStatement::checkAlterTable},

};

// Dispatcher to create statement based on keyword
std::map<Keywords, SQLStmtFactory> theSQLStatementMap{
    {Keywords::create_kw, CreateTableStatement::createTableStatement},
    {Keywords::show_kw, ShowTableStatement::showTableStatement},
    {Keywords::describe_kw, DescribeTableStatement::describeTableStatement},
    {Keywords::insert_kw, InsertTableStatement::insertTableStatement},
    {Keywords::select_kw, SelectStatement::selectStatement},
    {Keywords::update_kw, UpdateTableStatement::updateTableStatement},
    {Keywords::delete_kw, DeleteRowStatement::deleteRowStatement},
    {Keywords::drop_kw, DropTableStatement::dropTableStatement},
    {Keywords::index_kw, IndexTableStatement::indexTableStatement},
    {Keywords::indexes_kw, IndexsTableStatement::indexsTableStatement},
    {Keywords::alter_kw, AlterTableStatement::alterTableStatement},

};

// Function to check if given command is a valid command
CmdProcessor *SQLProcessor::recognizes(Tokenizer &aTokenizer) {
    Keywords theKw = aTokenizer.current().keyword;
    if (theRecognizemap.find(theKw) != theRecognizemap.end()) {
        if (theRecognizemap[theKw](aTokenizer)) {
            SQLProcessor::keywordStatement = theKw;
            return this;
        }
    }
    return nullptr;
}

// To create a Statement based on the command
Statement *SQLProcessor::makeStatement(Tokenizer    &aTokenizer,
                                       StatusResult &aResult) {
    // Atul: Added separate class to create statements for
    // create_table,show_table,drop_table, and describe_table
    Statement *theSQLStatement = nullptr;

    if (theSQLStatementMap.find(aTokenizer.peek().keyword) != theSQLStatementMap.end() && aTokenizer.current().keyword == Keywords::show_kw) {
        theSQLStatement = theSQLStatementMap[aTokenizer.peek().keyword](this, aTokenizer);
    } else if (theSQLStatementMap.find(aTokenizer.current().keyword) != theSQLStatementMap.end()) {
        theSQLStatement = theSQLStatementMap[aTokenizer.current().keyword](this, aTokenizer);
    }

    SQLProcessor::keywordStatement = Keywords::unknown_kw;  // resetting for the next command
    // Go to end of the command
    aTokenizer.skipTo(';');
    aTokenizer.next();
    return theSQLStatement;
}

// // Dispatcher to run methods for commands
StatusResult SQLProcessor::run(Statement *aStmt) {
    // running which command to run
    if (aStmt != nullptr) {
        return aStmt->run(output);
    }
    return StatusResult(Errors::noError);
}

void SQLProcessor::resetRowData() {
    this->theRowData.clear();
}

}  // namespace ECE141
