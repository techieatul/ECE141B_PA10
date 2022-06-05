#pragma once
#include <iostream>
#include "Statement.hpp"
#include "SQLStatement.hpp"
#include "SQLProcessor.hpp"
#include "keywords.hpp"
#include <string>
namespace ECE141{
    class SQLProcessor;
    class ShowTableStatement:public SQLStatement{
        protected:
        SQLProcessor* theSQLProcessorPtr;
        public:
        ShowTableStatement(SQLProcessor* aSQLProc,Keywords aStatementType=Keywords::unknown_kw):SQLStatement::SQLStatement(aStatementType),theSQLProcessorPtr(aSQLProc){}
        ~ShowTableStatement(){};

        SQLProcessor* getSQLProcessor() const {return theSQLProcessorPtr;}


        // Function to check if tokenized tokens represent SHOW TABLES;
        static bool checkShowTable(Tokenizer aTokenizer);

        static Statement* showTableStatement(SQLProcessor* aProc, Tokenizer &aTokenizer);

        StatusResult run(std::ostream &aStream) const override;
    };

}