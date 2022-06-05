#pragma once
#include <iostream>
#include <string>

#include "Attribute.hpp"
#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "DBProcessor.hpp"
#include "Database.hpp"

namespace ECE141 {
    class DBStatement : public Statement {
        public:
        DBStatement(Keywords aStatementType);
        ~DBStatement(){};
        void setDBProcessor(DBProcessor* aDBProcessorPtr);

        protected:
        DBProcessor* theDBProcessorPtr;
    };

    class CreateDBStatement:public DBStatement{
        public:
        CreateDBStatement(Keywords aStatementType);
        ~CreateDBStatement(){};
        static bool checkCreateDB(Tokenizer aTokenizer);
        static Statement* createDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer);
        const char* getStatementName() const override {return "createdbstatement";}
        StatusResult  run(std::ostream &aStream) const override;


    };

    class ShowDBStatement:public DBStatement{
        public:
        ShowDBStatement(Keywords aStatementType);
        ~ShowDBStatement(){}
        static bool checkShowDB(Tokenizer aTokenizer);
        static Statement* showDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer);
        const char* getStatementName() const override {return "showdbstatement";}
        StatusResult  run(std::ostream &aStream) const override;

    };

    class DropDBStatement:public DBStatement{
        public:
        DropDBStatement(Keywords aStatementType);
        ~DropDBStatement(){}
        static bool checkDropDB(Tokenizer aTokenizer);
        static Statement* dropDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer);
        const char* getStatementName() const override {return "dropdbstatement";}
        StatusResult  run(std::ostream &aStream) const override;

    };

    class DumpDBStatement:public DBStatement{
        public:
        DumpDBStatement(Keywords aStatementType);
        ~DumpDBStatement(){}
        static bool checkDumpDB(Tokenizer aTokenizer);
        static Statement* dumpDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer);
        const char* getStatementName() const override {return "dumpdbstatement";}
        StatusResult  run(std::ostream &aStream) const override;

    };

    class UseDBStatement:public DBStatement{
        public:
        UseDBStatement(Keywords aStatementType);
        ~UseDBStatement(){}
        static bool checkUseDB(Tokenizer aTokenizer);
        static Statement* useDBStatement(DBProcessor *aProc,Tokenizer &aTokenizer);
        const char* getStatementName() const override {return "usedbstatement";}
        StatusResult  run(std::ostream &aStream) const override;

    };
}