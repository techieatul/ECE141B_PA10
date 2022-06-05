#include "Database.hpp"
#include "Attribute.hpp"
#include "IndexTableStatement.hpp"
#include <iostream>
#include <string>

namespace ECE141{

    bool IndexTableStatement::checkIndexTable(Tokenizer aTokenizer){
        Token theShowToken{TokenType::keyword, Keywords::show_kw,
                           Operators::unknown_op, "show"};
        Token theTablesToken{TokenType::keyword, Keywords::tables_kw,
                             Operators::unknown_op, "tables"};

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

    Statement* IndexTableStatement::indexTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer){
        IndexTableStatement* theIndexTable = new IndexTableStatement(aProc, Keywords::show_kw);
        theIndexTable->parse(aTokenizer);
        return theIndexTable;
    }
	StatusResult IndexTableStatement::parse(Tokenizer& aTokenizer){
		if (aTokenizer.skipIf(Keywords::show_kw))
			if (aTokenizer.skipIf(Keywords::index_kw))
				while(aTokenizer.current().type == TokenType::identifier){
					theFieldsName.push_back(aTokenizer.current().data);
					aTokenizer.next();
					aTokenizer.skipIf(',');
				}
				if (aTokenizer.skipIf(Keywords::from_kw)) {
					if (aTokenizer.current().type == TokenType::identifier) {
						theTableName = aTokenizer.current().data;
						aTokenizer.next();
						return StatusResult{Errors::noError};
					}
				}
				
		return StatusResult{Errors::unknownCommand};        
	}

    StatusResult IndexTableStatement::run(std::ostream &aStream) const{
        SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
        Database* theDatabase = theSQLProcessorPtr->getDatabaseInUse();
        IndexTableStatement* theIndexTableStmt = const_cast<IndexTableStatement*>(this);
        return theDatabase->indexTable(theIndexTableStmt, aStream);
    }
};