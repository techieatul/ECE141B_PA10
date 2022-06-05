//
//  DBProcessor.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "DBProcessor.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Application.hpp"
#include "Config.hpp"
#include "DBProcessor.hpp"
#include "Database.hpp"
#include "FolderReader.hpp"
#include "FolderView.hpp"
#include "Helpers.hpp"
#include "ShowsTablesView.hpp"
#include "DBStatement.hpp"

namespace ECE141 {

// like a typedef!!!
using StmtFactory = Statement *(*)(DBProcessor *aProc,
                                   Tokenizer   &aTokenizer);

DBProcessor::DBProcessor(std::ostream &anOutput, Database **aCurrentActiveDbPtr)
    : CmdProcessor(anOutput), DBMessageHandler(), currentActiveDbPtr(aCurrentActiveDbPtr) {
}

DBProcessor::~DBProcessor() {
}

// To set a new active database connection
void DBProcessor::setDatabaseInUse(Database *database){
    (*currentActiveDbPtr) = database;
}

// To remove a database connection
void DBProcessor::releaseDatabase(){
    delete (*currentActiveDbPtr);
}

// To dispact respective recognize function
std::map<Keywords,recognizeVisitor> theRecognizeMap{
    {Keywords::create_kw,CreateDBStatement::checkCreateDB},
    {Keywords::show_kw,ShowDBStatement::checkShowDB},
    {Keywords::use_kw,UseDBStatement::checkUseDB},
    {Keywords::drop_kw,DropDBStatement::checkDropDB},
    {Keywords::dump_kw,DumpDBStatement::checkDumpDB},
  };

// To dispatch respective makeStatement
std::map<Keywords,StmtFactory> theStatementMap{

    {Keywords::create_kw,CreateDBStatement::createDBStatement},
    {Keywords::show_kw,ShowDBStatement::showDBStatement},
    {Keywords::use_kw,UseDBStatement::useDBStatement},
    {Keywords::drop_kw,DropDBStatement::dropDBStatement},
    {Keywords::dump_kw,DumpDBStatement::dumpDBStatement},
    
};

// CmdProcessor interface ...
CmdProcessor *DBProcessor::recognizes(Tokenizer &aTokenizer) {
    // Code to recognise DB commands
    Keywords theKw = aTokenizer.current().keyword;
    if(theRecognizeMap.find(theKw)!=theRecognizeMap.end()){
        if(theRecognizeMap[theKw](aTokenizer)){
            DBProcessor::keywordStatement = theKw;
            return this;
        }
    }
    return nullptr;
}

// // Function to dispatch the run logic of each command
StatusResult DBProcessor::run(Statement *aStmt) {
    return aStmt->run(output);
}

// USE: retrieve a statement based on given text input...
Statement *DBProcessor::makeStatement(Tokenizer    &aTokenizer,
                                      StatusResult &aResult) {
    Statement *theStatement = nullptr;
    if(theStatementMap.find(DBProcessor::keywordStatement) != theStatementMap.end()){
        theStatement = theStatementMap[DBProcessor::keywordStatement](this,aTokenizer);
    }
    DBProcessor::keywordStatement = Keywords::unknown_kw;
    return theStatement;

}

// To return current active Database pointer
Database *DBProcessor::getDatabaseInUse() {
    return *currentActiveDbPtr;
}
// Check if DB already exists
bool DBProcessor::dbExists(const std::string &aDBName) {
    // Atul added
    std::string theDBPath = Config::getDBPath(aDBName);
    return std::filesystem::exists(theDBPath);
}

// Check if the current active DB matches aDBName
bool DBProcessor::checkActiveDBName(const std::string &aDBName) {
    if ((*currentActiveDbPtr) != nullptr && (*currentActiveDbPtr)->getDbName() == aDBName) {
            return true;
    }
    return false;
}

}  // namespace ECE141
