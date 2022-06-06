#include "AlterTableStatement.hpp"



namespace ECE141 {
    bool AlterTableStatement::checkAlterTable(Tokenizer aTokenizer){
        if (aTokenizer.current().keyword == Keywords::alter_kw) {
            aTokenizer.next();
            if (aTokenizer.current().keyword == Keywords::table_kw) {
                aTokenizer.next();
                if (aTokenizer.current().type == TokenType::identifier) {
                    return true;
                }
            }
        }
        return false;
    }
    Statement* AlterTableStatement::alterTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer){
        AlterTableStatement *theAlterTable = new AlterTableStatement(aProc,Keywords::alter_kw);
        StatusResult theStatus = theAlterTable->parse(aTokenizer);
        if(theStatus){
            return theAlterTable;
        }

        return nullptr;

    }
    StatusResult AlterTableStatement::run(std::ostream &aStream) const{
        SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
        Database* theDatabase = theSQLProcessorPtr->getDatabaseInUse();
        if(theDatabase==nullptr) {
            return StatusResult(Errors::unknownDatabase);
        }
        AlterTableStatement* theAlterTableStatement = const_cast<AlterTableStatement*>(this);
        return theDatabase->alterTable(theAlterTableStatement,aStream);

    }
    StatusResult AlterTableStatement::parse(Tokenizer& aTokenizer){
        aTokenizer.skipTo(TokenType::identifier);
        this->setTableName(aTokenizer.current().data);
        aTokenizer.next();
        this->setAlterFieldType(aTokenizer.current().data);
        aTokenizer.next();
        if(aTokenizer.current().keyword == Keywords::column_kw){
            aTokenizer.next();
        }
        Attribute theAttr;
        bool theNot = false;
        while (aTokenizer.current().data != ";" && aTokenizer.more()) {
            while (aTokenizer.current().data != "," &&
                   aTokenizer.current().data != ")" && aTokenizer.more()) {
                if (aTokenizer.current().type == TokenType::identifier) {
                    theAttr.setName(aTokenizer.current().data);
                }
                switch (aTokenizer.current().keyword) {
                    case Keywords::primary_kw:
                        theAttr.setPrimaryKey(true);
                        break;

                    case Keywords::not_kw:
                        theNot = true;
                        break;

                    case Keywords::null_kw:
                        theAttr.setNullable(true);
                        if (theNot) {
                            theAttr.setNullable(false);
                        }
                        theNot = false;
                        break;

                    case Keywords::auto_increment_kw:
                        theAttr.setAutoIncrement(true);
                        break;

                    case Keywords::integer_kw:
                        theAttr.setDataType(DataTypes::int_type);
                        break;

                    case Keywords::boolean_kw:
                        theAttr.setDataType(DataTypes::bool_type);
                        break;

                    case Keywords::float_kw:
                        theAttr.setDataType(DataTypes::float_type);
                        break;

                    case Keywords::current_timestamp_kw:
                        theAttr.setDataType(DataTypes::datetime_type);
                        break;

                    case Keywords::varchar_kw:
                        theAttr.setDataType(DataTypes::varchar_type);
                        aTokenizer.skipTo(TokenType::number);
                        theAttr.setSize(stoi(aTokenizer.current().data));
                        aTokenizer.next();
                        break;
                }

                aTokenizer.next();
            }
            this->vectorPush(theAttr);
            theNot = false;
            theAttr.reset();
            aTokenizer.next();
        }

        return StatusResult(Errors::noError);


    }

    SQLProcessor* AlterTableStatement::getSQLProcessor() const{
        return this->theSQLProcessorPtr;
    }

};