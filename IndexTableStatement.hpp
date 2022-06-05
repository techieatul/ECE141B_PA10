#pragma once
#include <iostream>
#include "Statement.hpp"
#include "SQLStatement.hpp"
//#include "SQLProcessor.hpp"
#include "keywords.hpp"
#include <string>
namespace ECE141{
    class SQLProcessor;
    class IndexTableStatement:public SQLStatement{
        protected:
        SQLProcessor* theSQLProcessorPtr;
        public:
        IndexTableStatement(SQLProcessor* aSQLProc,Keywords aStatementType=Keywords::unknown_kw):SQLStatement::SQLStatement(aStatementType),theSQLProcessorPtr(aSQLProc){}
        ~IndexTableStatement(){};

        SQLProcessor* getSQLProcessor() const {return theSQLProcessorPtr;}


        // Function to check if tokenized tokens represent SHOW TABLES;
        static bool checkIndexTable(Tokenizer aTokenizer);

        static Statement* indexTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer);
        std::string getTableName() { return theTableName; }
        std::vector<std::string>& getFieldsName() { return theFieldsName; }
        StatusResult run(std::ostream &aStream) const override;
		StatusResult parse(Tokenizer& aTokenizer);

		protected:
      		std::string theTableName;
			std::vector<std::string> theFieldsName;
    };

}