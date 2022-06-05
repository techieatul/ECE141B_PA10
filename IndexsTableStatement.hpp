#pragma once
#include <iostream>
#include "Statement.hpp"
#include "SQLStatement.hpp"
//#include "SQLProcessor.hpp"
#include "keywords.hpp"
#include <string>
namespace ECE141{
    class SQLProcessor;
    class IndexsTableStatement:public SQLStatement{
        protected:
        SQLProcessor* theSQLProcessorPtr;
        public:
        IndexsTableStatement(SQLProcessor* aSQLProc,Keywords aStatementType=Keywords::unknown_kw):SQLStatement::SQLStatement(aStatementType),theSQLProcessorPtr(aSQLProc){}
        ~IndexsTableStatement(){};

        SQLProcessor* getSQLProcessor() const {return theSQLProcessorPtr;}


        // Function to check if tokenized tokens represent SHOW Indexs;
        static bool parse(Tokenizer aTokenizer);
        static Statement* indexsTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer);
        StatusResult run(std::ostream &aStream) const override;
    };

}