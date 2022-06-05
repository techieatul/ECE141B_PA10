#include "CreateTableStatement.hpp"
#include "Database.hpp"
#include "SQLProcessor.hpp"

namespace ECE141 {

    bool CreateTableStatement::checkCreateTable(Tokenizer aTokenizer) {
        Token theCreateToken{TokenType::keyword, Keywords::create_kw,
                             Operators::unknown_op, "create"};
        Token theTableToken{TokenType::keyword, Keywords::table_kw,
                            Operators::unknown_op, "table"};
        Token theTableNameToken{TokenType::identifier, Keywords::unknown_kw,
                                Operators::unknown_op, "table_name"};
        Token theOpenParanToken{TokenType::punctuation, Keywords::unknown_kw,
                                Operators::unknown_op, "("};

        std::vector<Token> SQLVector;
        SQLVector.push_back(theCreateToken);
        SQLVector.push_back(theTableToken);
        SQLVector.push_back(theTableNameToken);
        SQLVector.push_back(theOpenParanToken);

        for (size_t i = 0; i < SQLVector.size(); ++i) {
            if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
                (SQLVector.at(i).type != aTokenizer.current().type)) {
                return false;
            }
            aTokenizer.next();
        }

        // check if we have ) and ;
        aTokenizer.skipTo(';');

        if (aTokenizer.tokenAt(aTokenizer.getIndex() - 1).data[0] != ')' ||
            aTokenizer.current().data[0] != ';') {
            return false;
        }

        return true;
    }

    Statement* CreateTableStatement::createTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer){
        CreateTableStatement *theCreateTable = new CreateTableStatement(aProc,Keywords::create_kw);
        StatusResult theStatus = theCreateTable->parse(aTokenizer);
        if(theStatus){
            return theCreateTable;
        }

        return nullptr;

    }

    StatusResult CreateTableStatement::parse(Tokenizer& aTokenizer){
        aTokenizer.skipTo(TokenType::identifier);  // Skip to table name
        this->setTableName(aTokenizer.current().data);
        aTokenizer.skipTo('(');
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

    StatusResult CreateTableStatement::run(std::ostream &aStream) const {
        SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
        Database* theDatabase = theSQLProcessorPtr->getDatabaseInUse();
        if(theDatabase==nullptr) {
            return StatusResult(Errors::unknownDatabase);
        }
        CreateTableStatement* theCreateTableStatement = const_cast<CreateTableStatement*>(this);
        return theDatabase->createTable(theCreateTableStatement,aStream);

    }

    SQLProcessor* CreateTableStatement::getSQLProcessor() const {return theSQLProcessorPtr;}
};