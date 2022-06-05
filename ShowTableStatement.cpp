#include "Database.hpp"
#include "Attribute.hpp"
#include "ShowTableStatement.hpp"
#include <iostream>
#include <string>

namespace ECE141{

    bool ShowTableStatement::checkShowTable(Tokenizer aTokenizer){
        Token theShowToken{TokenType::keyword, Keywords::show_kw,
                           Operators::unknown_op, "show"};
        Token theTablesToken{TokenType::keyword, Keywords::tables_kw,
                             Operators::unknown_op, "tables"};

        std::vector<Token> SQLVector;
        SQLVector.push_back(theShowToken);
        //SQLVector.push_back(theTablesToken);

        for (size_t i = 0; i < SQLVector.size(); ++i) {
            if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
                (SQLVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
    }

    Statement* ShowTableStatement::showTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer){
        ShowTableStatement* theShowTable = new ShowTableStatement(aProc, Keywords::show_kw);
        return theShowTable;
    }

    StatusResult ShowTableStatement::run(std::ostream &aStream) const{
        SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
        Database* theDatabase = theSQLProcessorPtr->getDatabaseInUse();
        ShowTableStatement* theShowTableStmt = const_cast<ShowTableStatement*>(this);
        return theDatabase->showTable(theShowTableStmt, aStream);
    }
};