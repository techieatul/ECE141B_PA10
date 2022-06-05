#include "DropTableStatement.hpp"

#include "Database.hpp"
namespace ECE141 {

bool DropTableStatement::checkDropTable(Tokenizer aTokenizer) {
    Token              theDropToken{TokenType::keyword, Keywords::drop_kw,
                       Operators::unknown_op, "drop"};
    Token              theTableToken{TokenType::keyword, Keywords::table_kw,
                        Operators::unknown_op, "table"};
    Token              theTableNameToken{TokenType::identifier, Keywords::unknown_kw,
                            Operators::unknown_op, "table_name"};
    Token              theSemiColonToken{TokenType::punctuation, Keywords::unknown_kw,
                            Operators::unknown_op, ";"};

    std::vector<Token> SQLVector;
    SQLVector.push_back(theDropToken);
    SQLVector.push_back(theTableToken);
    SQLVector.push_back(theTableNameToken);
    SQLVector.push_back(theSemiColonToken);

    for (size_t i = 0; i < SQLVector.size(); ++i) {
        if ((SQLVector.at(i).keyword != aTokenizer.current().keyword) ||
            (SQLVector.at(i).type != aTokenizer.current().type)) {
            return false;
        }
        aTokenizer.next();
    }

    return true;
}

Statement* DropTableStatement::dropTableStatement(SQLProcessor* aProc, Tokenizer& aTokenizer) {
    DropTableStatement* theDropTable = new DropTableStatement(aProc, Keywords::drop_kw);
    if (theDropTable->parse(aTokenizer)) {
        return theDropTable;
    }
    return nullptr;
}

StatusResult DropTableStatement::parse(Tokenizer& aTokenizer) {
    aTokenizer.skipTo(TokenType::identifier);
    this->setTableName(aTokenizer.current().data);
    return StatusResult(Errors::noError);
}

StatusResult DropTableStatement::run(std::ostream& aStream) const {
    SQLProcessor* theSQLProcessorPtr = getSQLProcessor();
    Database*     theDatabase = theSQLProcessorPtr->getDatabaseInUse();
    DropTableStatement* theDropTableStatement = const_cast<DropTableStatement*>(this);
    return theDatabase->dropTable(theDropTableStatement, aStream);
}

};  // namespace ECE141