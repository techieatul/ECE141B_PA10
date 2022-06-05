#include "DBStatement.hpp"
#include <string>
#include <vector>
#include "MessageViewer.hpp"
#include "FolderReader.hpp"
#include "ShowsTablesView.hpp"
#include "BlockIO.hpp"

namespace ECE141 {
    DBStatement::DBStatement(Keywords aStatementType)
    : Statement(aStatementType) {}
    void DBStatement::setDBProcessor(DBProcessor* aDBProcessorPtr){theDBProcessorPtr=aDBProcessorPtr;}
    
    CreateDBStatement::CreateDBStatement(Keywords aStatementType):DBStatement(aStatementType){}
    ShowDBStatement::ShowDBStatement(Keywords aStatementType):DBStatement(aStatementType){}
    UseDBStatement::UseDBStatement(Keywords aStatementType):DBStatement(aStatementType){}
    DropDBStatement::DropDBStatement(Keywords aStatementType):DBStatement(aStatementType){}
    DumpDBStatement::DumpDBStatement(Keywords aStatementType):DBStatement(aStatementType){}
  // Function to check if tokenized tokens represent CREATE DATABASE DB_NAME;  
    bool CreateDBStatement::checkCreateDB(Tokenizer aTokenizer) {
        Token theCreateToken{TokenType::keyword, Keywords::create_kw, Operators::unknown_op, "create"};
        Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
        Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
        Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
        std::vector<Token> DBVector;
        DBVector.push_back(theCreateToken);
        DBVector.push_back(theDBToken);
        DBVector.push_back(theDBNameToken);
        DBVector.push_back(theSemiColonToken);
        size_t theVecSize = DBVector.size();

        size_t theCount = 0;
        // Lambda function to count tokens till ;
        std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
            size_t theCurrIdx = aTokenizer.getIndex();
            while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
                theCount++;
                theCurrIdx++;
            }
        };

        // Calling the function
        theCountTillSemi();

        // If size unequal then return false
        if (theVecSize != theCount + 1) {
            return false;
        }
        // bool theReturnVal = true;
        for (size_t i = 0; i < theVecSize; ++i) {
            if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
    }
  
   // Function to check if tokenized tokens represent SHOW DATABASE;
    bool ShowDBStatement::checkShowDB(Tokenizer aTokenizer) {
    Token              theShowToken{TokenType::keyword, Keywords::show_kw, Operators::unknown_op, "show"};
    Token              theDBToken{TokenType::keyword, Keywords::databases_kw, Operators::unknown_op, "databases"};
    std::vector<Token> theTokenVector;
    theTokenVector.push_back(theShowToken);
    theTokenVector.push_back(theDBToken);
    size_t theVecSize = theTokenVector.size();

    size_t theCount = 0;
    // Lambda function to count tokens till ;
    std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
        size_t theCurrIdx = aTokenizer.getIndex();
        while (theCurrIdx<aTokenizer.size() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
            theCount++;
            theCurrIdx++;
        }
    };

    // Calling the function
    theCountTillSemi();

    // If size unequal then return false
    if (theVecSize != theCount) {
        return false;
    }

    // bool theReturnVal = true;
    for (size_t i = 0; i < theVecSize; ++i) {
        if ((theTokenVector.at(i).keyword != aTokenizer.current().keyword) || (theTokenVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

// Function to check if tokenized tokens represent USE DB_NAME;
    bool UseDBStatement::checkUseDB(Tokenizer aTokenizer) {
        Token theUseToken{TokenType::keyword, Keywords::use_kw, Operators::unknown_op, "use"};
        Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
        Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
        std::vector<Token> DBVector;
        DBVector.push_back(theUseToken);
        DBVector.push_back(theDBNameToken);
        DBVector.push_back(theSemiColonToken);
        size_t theVecSize = DBVector.size();

        size_t theCount = 0;
        // Lambda function to count tokens till ;
        std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
            size_t theCurrIdx = aTokenizer.getIndex();
            while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
                theCount++;
                theCurrIdx++;
            }
        };

        // Calling the function
        theCountTillSemi();

        // If size unequal then return false
        if (theVecSize != theCount + 1) {
            return false;
        }

        // bool theReturnVal = true;
        for (size_t i = 0; i < theVecSize; ++i) {
            if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
}

// Function to check if tokenized tokens represent DROP DATABASE DB_NAME;
    bool DropDBStatement::checkDropDB(Tokenizer aTokenizer) {
        Token theDropToken{TokenType::keyword, Keywords::drop_kw, Operators::unknown_op, "drop"};
        Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
        Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
        Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
        std::vector<Token> DBVector;
        DBVector.push_back(theDropToken);
        DBVector.push_back(theDBToken);
        DBVector.push_back(theDBNameToken);
        DBVector.push_back(theSemiColonToken);
        size_t theVecSize = DBVector.size();

        size_t theCount = 0;
        // Lambda function to count tokens till ;
        std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
            size_t theCurrIdx = aTokenizer.getIndex();
            while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
                theCount++;
                theCurrIdx++;
            }
        };

        // Calling the function
        theCountTillSemi();

        // If size unequal then return false
        if (theVecSize != theCount + 1) {
            return false;
        }

        // bool theReturnVal = true;
        for (size_t i = 0; i < theVecSize; ++i) {
            if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
}

// Function to check if given input matches DUMp DATABASE <DBNAME>
    bool DumpDBStatement::checkDumpDB(Tokenizer aTokenizer) {
        Token theDumpToken{TokenType::keyword, Keywords::dump_kw, Operators::unknown_op, "dump"};
        Token theDBToken{TokenType::keyword, Keywords::database_kw, Operators::unknown_op, "database"};
        Token theDBNameToken{TokenType::identifier, Keywords::unknown_kw, Operators::unknown_op, "DB_NAME"};
        Token theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op, ";"};
        std::vector<Token> DBVector;
        DBVector.push_back(theDumpToken);
        DBVector.push_back(theDBToken);
        DBVector.push_back(theDBNameToken);
        DBVector.push_back(theSemiColonToken);
        size_t theVecSize = DBVector.size();

        size_t theCount = 0;
        // Lambda function to count tokens till ;
        std::function<void()> theCountTillSemi = [&theCount, &aTokenizer]() {
            size_t theCurrIdx = aTokenizer.getIndex();
            while (aTokenizer.more() && aTokenizer.tokenAt(theCurrIdx).data != ";") {
                theCount++;
                theCurrIdx++;
            }
        };

        // Calling the function
        theCountTillSemi();

        // If size unequal then return false
        if (theVecSize != theCount + 1) {
            return false;
        }
        // bool theReturnVal = true;
        for (size_t i = 0; i < theVecSize; ++i) {
            if ((DBVector.at(i).keyword != aTokenizer.current().keyword) || (DBVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
}
    
    // To create statement for create DB
    Statement* CreateDBStatement::createDBStatement(DBProcessor* aProc, Tokenizer& aTokenizer){
        CreateDBStatement* theCreateDBStatement = new CreateDBStatement(Keywords::create_kw);
        aTokenizer.skipTo(TokenType::identifier);
        theCreateDBStatement->setDBName(aTokenizer.current().data);
        theCreateDBStatement->setDBProcessor(aProc);
        aTokenizer.skipTo(TokenType::punctuation);
        aTokenizer.next();
        return theCreateDBStatement;
    }

    // To create statement for Use DB
    Statement* UseDBStatement::useDBStatement(DBProcessor* aProc, Tokenizer& aTokenizer){
        UseDBStatement* theUseDBStatement = new UseDBStatement(Keywords::use_kw);
        aTokenizer.skipTo(TokenType::identifier);
        theUseDBStatement->setDBName(aTokenizer.current().data);
        theUseDBStatement->setDBProcessor(aProc);
        aTokenizer.skipTo(TokenType::punctuation);
        aTokenizer.next();
        return theUseDBStatement;
    }
    
    // To create statement for show DB
    Statement* ShowDBStatement::showDBStatement(DBProcessor* aProc,Tokenizer& aTokenizer){
        ShowDBStatement* theShowDBStatement = new ShowDBStatement(Keywords::show_kw);
        theShowDBStatement->setDBProcessor(aProc);
        aTokenizer.skipTo(TokenType::punctuation);
        aTokenizer.next();
        return theShowDBStatement;
    }

   
    // To create statement for dump DB 
    Statement* DumpDBStatement::dumpDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer){
        DumpDBStatement* theDumpStatement = new DumpDBStatement(Keywords::dump_kw);
        aTokenizer.skipTo(TokenType::identifier);
        theDumpStatement->setDBName(aTokenizer.current().data);
        theDumpStatement->setDBProcessor(aProc);
        aTokenizer.skipTo(TokenType::punctuation);
        aTokenizer.next();
        return theDumpStatement;
    }

    // To create statement for drop DB
    Statement* DropDBStatement::dropDBStatement(DBProcessor* aProc,Tokenizer& aTokenizer){
        DropDBStatement* theDropStatement = new DropDBStatement(Keywords::drop_kw);
        aTokenizer.skipTo(TokenType::identifier);
        theDropStatement->setDBName(aTokenizer.current().data);
        theDropStatement->setDBProcessor(aProc);
        aTokenizer.skipTo(TokenType::punctuation);
        aTokenizer.next();
        return theDropStatement;
    }
    // Function to run the main logic of SHOW DATABASES
    StatusResult ShowDBStatement::run(std::ostream &aStream) const {
        std::vector<std::string> theDBVector;
        theDBVector.push_back("Database");
        size_t theLongestString = theDBVector.at(0).length();
        std::string thePath = Config::getStoragePath();
        FolderReader theFr(thePath.c_str());
        theFr.each(".db", [&theDBVector, &theLongestString](const std::string& aName) {
            theDBVector.push_back(aName);
            theLongestString = std::max(theLongestString, aName.length());
            return true;
        });
        theLongestString += 10;
        MessageViewer theDBMessageHandler;
        theDBMessageHandler.showTableView(aStream, theDBVector, theLongestString);

        std::stringstream theMessage;
        theMessage << theDBVector.size() - 1 << " rows in set";
        std::string theMsg = std::string(theMessage.str());
        theDBMessageHandler.setMessage(theMsg);
        theDBMessageHandler.show(aStream);
        return StatusResult(Errors::noError);
    }
    // Function to run the main logic of DROP DATABASE <DBNAME>
    StatusResult DropDBStatement::run(std::ostream &aStream) const {
        const std::string theDbName = getDBName();
        if (!theDBProcessorPtr->dbExists(theDbName)) {
            return StatusResult(Errors::unknownDatabase);
        }
        std::filesystem::remove(Config::getDBPath(theDbName));

        // Delete the activeDB if it is the one to be dropped
        if(theDBProcessorPtr->checkActiveDBName(theDbName)){
            theDBProcessorPtr->releaseDatabase();
            theDBProcessorPtr->setDatabaseInUse(nullptr);
        }
        // Atul added to take use of Views class
        MessageViewer theDBMessageHandler;
        std::string theMessage = "Query OK, 0 rows affected";
        theDBMessageHandler.setMessage(theMessage);
        theDBMessageHandler.show(aStream);
        return StatusResult(Errors::noError);
    }
    // Function to run the main logic of USE <DBNAME>
    StatusResult UseDBStatement::run(std::ostream &aStream) const {
        const std::string theDbName = getDBName();
        if (!theDBProcessorPtr->dbExists(theDbName)) {
            return StatusResult(Errors::unknownDatabase);
        }
        theDBProcessorPtr->releaseDatabase();

        Database* theNewDB = new Database(theDbName, OpenDB{});

        theDBProcessorPtr->setDatabaseInUse(theNewDB);

        aStream << "Database changed" << std::endl;
        return StatusResult{ECE141::noError};
    }
    // Function to run the main logic of DUMP DATABASE <DBNAME>
    StatusResult DumpDBStatement::run(std::ostream &aStream) const {
        const std::string theName = getDBName();
        std::fstream theDumpStream;
        std::string theDbPath = Config::getDBPath(theName);
        theDumpStream.open(theDbPath, std::ios::in);
        theDumpStream.seekg(0, std::ios::end);
        size_t theFileSize = theDumpStream.tellg();
        size_t theNumBlocks = theFileSize / ECE141::kBlockSize;
        theDumpStream.seekg(0, std::ios::beg);
        Block theHeader;
        // Table View class
        ShowsTablesView theTb(true);
        theTb.addToRow("Type");
        theTb.addToRow("id");
        theTb.addToRow("Extra");
        theTb.endOfRow();

        for (size_t i = 0; i < theNumBlocks; i++) {
            theDumpStream.read(reinterpret_cast<char*>(&theHeader), ECE141::kBlockSize);
            char c = theHeader.header.type;
            size_t id = theHeader.header.theBlockId;
            std::string theName = std::string(theHeader.header.theTitle);
            switch (c) {
                case 'M':
                    theTb.addToRow("Meta");
                    break;
                case 'E':
                    theTb.addToRow("Entity");
                    break;
                case 'D':
                    theTb.addToRow("Data");
                    break;
                case 'F':
                    theTb.addToRow("Free");
                    break;
            }
            theTb.addToRow(std::to_string(id));
            if (c == 'D' || c == 'E') {
                theTb.addToRow(theName);
            } else {
                theTb.addToRow("");
            }

            theTb.endOfRow();
            theDumpStream.seekg((i + 1) * ECE141::kBlockSize, std::ios::beg);
        }

        theDumpStream.close();
        theTb.show(aStream);
        aStream<< theNumBlocks << " rows in set (" << Config::getTimer().elapsed() << " sec)\n";

        return StatusResult(Errors::noError);
    }
    // Function to run the main logic of CREATE DATABASE <DBNAME>
    StatusResult CreateDBStatement::run(std::ostream &aStream) const {
        const std::string theDbName = getDBName();
        if (DBProcessor::dbExists(theDbName)) {
        return StatusResult(Errors::databaseExists);
        }
        theDBProcessorPtr->releaseDatabase();
        Database* theNewDb = new Database(theDbName, CreateDB{});
        theDBProcessorPtr->setDatabaseInUse(theNewDb);
        aStream<< "Query OK, 1 rows affected";
        aStream<< " (" << Config::getTimer().elapsed() << " secs)" << std::endl;
        return StatusResult(Errors::noError);
    }



}