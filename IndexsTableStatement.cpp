#include "Database.hpp"
#include "Attribute.hpp"
#include "IndexsTableStatement.hpp"
#include <iostream>
#include <string>

namespace ECE141{

    bool IndexsTableStatement::parse(Tokenizer aTokenizer){
        Token theShowToken{TokenType::keyword, Keywords::show_kw,
                           Operators::unknown_op, "show"};
        Token theTablesToken{TokenType::keyword, Keywords::indexes_kw,
                             Operators::unknown_op, "indexs"};

        std::vector<Token> SQLVector;
        SQLVector.push_back(theShowToken);
        SQLVector.push_back(theTablesToken);

        for (size_t i = 0; i < SQLVector.size(); ++i) {
            if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
                (SQLVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        return true;
    }

    Statement* IndexsTableStatement::indexsTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer){
        IndexsTableStatement* theIndexsTable = new IndexsTableStatement(aProc, Keywords::show_kw);
        return theIndexsTable;
    }

    StatusResult IndexsTableStatement::run(std::ostream &aStream) const{
        SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
        Database* theDatabase = theSQLProcessorPtr->getDatabaseInUse();
        IndexsTableStatement* theIndexsTableStmt = const_cast<IndexsTableStatement*>(this);
        return theDatabase->indexsTable(theIndexsTableStmt, aStream);
    }
};