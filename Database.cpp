//
//  Database.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "Database.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include "Config.hpp"
#include "ShowsTablesView.hpp"

namespace ECE141 {

Database::Database(const std::string aName, CreateDB)
    : name(aName), changed(true), storage(stream), entity_id(0) {
    theRowCache.setsize(Config::getCacheSize(CacheType::row));
    theViewCache.setsize(Config::getCacheSize(CacheType::view));
    std::string theDbPath = Config::getDBPath(aName);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
    stream.close();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::binary | std::fstream::in | std::fstream::out);

    Block *theMetaBlock = new Block(BlockType::meta_block);
    theMetaBlock->header.theBlockId = 0;
    theMetaBlock->header.type = 'M';
    theMetaBlock->header.theEntityId = this->entity_id;
    theMetaBlock->header.theBlockNum = 0;
    strcpy(theMetaBlock->header.theTitle, this->name.c_str());
    // Make the entity_id 1
    uint32_t theNewId = 1;
    this->setEntityId(theNewId);

    StatusResult theCreateResult = storage.writeBlock(0, *theMetaBlock);
    delete theMetaBlock;
    // Increase the block count
    Database::blockCount++;
}

Database::Database(const std::string aName, OpenDB)
    : name(aName), changed(false), storage(stream) {
    // decode the meta block and populate the below variables
    // idx_map, entity_id
    // open for read/write
    // Populate index related data as well
    theRowCache.setsize(Config::getCacheSize(CacheType::row));
    theViewCache.setsize(Config::getCacheSize(CacheType::view));
    std::string theDbPath = Config::getDBPath(name);
    stream.clear();
    stream.open(theDbPath.c_str(), std::fstream::binary | std::fstream::in | std::fstream::out);
    stream.seekg(0, std::ios::end);
    size_t theFileSize = stream.tellg();
    size_t theNumBlocks = theFileSize / ECE141::kBlockSize;
    this->blockCount = theNumBlocks;
    stream.seekg(0, std::ios::beg);
    StatusResult theStatus = storage.decodeMetaBlock(this->idx_map, this->entity_id);
    //  theStatus = populateIndexData();
    storage.populateFreeBlockQ();
}

Database::~Database() {
    if (changed) {
        // encode the below variables into Block 0
        // idx_map, entity_id
        std::map<std::string, uint32_t> theMap = this->idx_map;
        uint32_t                        theEntityId = this->entity_id;
        Block                           theBlock = storage.encodeMetaBlock(theMap, theEntityId);
        StatusResult                    theStatus = storage.writeBlock(0, theBlock);
    }
    stream.close();
}

// Use: populate index related data
StatusResult Database::populateIndexData() {
    // Decode info related to indexes
    std::map<std::string, uint32_t> theIdxMap = this->getIdxMap();
    for (auto &theEntity : theIdxMap) {
        uint32_t    theBlockNum = theEntity.second;
        std::string theEntityName = theEntity.first;
        Block      *theBlock = new Block(BlockType::entity_block);
        storage.readBlock(theBlockNum, *theBlock);
        Entity *theNewEntity = new Entity(theEntityName);
        theNewEntity->decodeBlock(*theBlock);
        uint32_t theIndexBlockNum = theNewEntity->getIndexBlockNum();
        this->setEntityIndexMap(theEntityName, theIndexBlockNum);
        this->setIndexBlockNum(theIndexBlockNum);
        Index *theIndex = new Index(storage);
        Block *theIndexBlock = new Block(BlockType::index_block);
        storage.readBlock(theIndexBlockNum, *theIndexBlock);
        std::stringstream ss;
        ss.write(theIndexBlock->payload, kPayloadSize);
        theIndex->decode(ss);
        this->setIndexVec(*theIndex);
        delete theBlock;
        delete theNewEntity;
        delete theIndex;
        delete theIndexBlock;
    }
}

uint32_t Database::getEntityFromMap(std::string aKey) {
    if (idx_map.find(aKey) != idx_map.end()) {
        return idx_map[aKey];
    }
    return -1;
}

// Check if our Queue that holds FreeBlocks is empty
bool Database::freeBlockQEmpty() {
    return storage.getQueue().empty();
}
// To get a Free BlockNumber
uint32_t Database::getFreeBlock() {
    if (storage.getQueue().empty()) {
        // this->setBlockCount(blockCount+1);
        return blockCount;
    }
    uint32_t thefreeBlock = storage.getQueue().front();
    storage.getQueue().pop();
    return thefreeBlock;
}

// USE: Call this to dump the db for debug purposes...
StatusResult Database::dump(std::ostream &anOutput) {
    for (auto i = 0; i < blockCount; i++) {
        Block theTempBlock(BlockType::meta_block);
        storage.readBlock(i, theTempBlock);
        anOutput << theTempBlock.header.type << "|" << i << "|"
                 << "ExtraInfo";
    }

    return StatusResult{noError};
}

// To populate the Map <Table_name,BlockNum>
void Database::setEntityMap(std::string &aName, uint32_t &aBlocNum) {
    this->idx_map[aName] = aBlocNum;
}

StatusResult Database::openDB() {
    this->stream.open(Config::getDBPath(this->getDbName()).c_str(), std::fstream::out | std::fstream::in | std::fstream::app);
    return StatusResult(Errors::noError);
}

StatusResult Database::closeDB() {
    this->stream.close();
    return StatusResult(Errors::noError);
}

StatusResult Database::updateTable(UpdateTableStatement *aSqlStmt, std::ostream &anOutput) {
    DBQuery   &theDBQuery = aSqlStmt->getDBQuery();
    Entity    *theEntity = theDBQuery.getEntity();
    StringList theFieldsToUpdate;
    theDBQuery.getUpdateKeys(theFieldsToUpdate);
    uint32_t         theEntityHash = theEntity->getEntityHashString();
    RawRowCollection theRows;
    each(theEntityHash, theRows);
    RawRowCollection theFilteredRows;
    filterRows(theDBQuery, theRows, theFilteredRows);
    for (auto &r : theFilteredRows) {
        for (auto &s : theFieldsToUpdate) {
            Value theNewVal = theDBQuery.getKeyValue(s);
            r.set(s, theNewVal);
        }
        Block theNewBlock;
        r.getBlock(theNewBlock);
        storage.writeBlock(r.getStorageBlockNumber(), theNewBlock);
    }
    anOutput << "Query Ok. " << theFilteredRows.size() << " rows affected (" << Config::getTimer().elapsed() << " sec)"
             << "\n";
    return StatusResult(Errors::noError);
}
StatusResult Database::deleteRow(DeleteRowStatement *aSqlStmt, std::ostream &anOutput) {
    DBQuery         &theDBQuery = aSqlStmt->getDBQuery();
    Entity          *theEntity = theDBQuery.getEntity();
    uint32_t         theEntityHash = theEntity->getEntityHashString();
    uint32_t         theEntityBlockNum = theEntity->getBlockNum();
    RawRowCollection theRows;
    each(theEntityHash, theRows);
    RawRowCollection theFilteredRows;
    filterRows(theDBQuery, theRows, theFilteredRows);
    StatusResult theStatus(Errors::noError);
    for (auto &r : theFilteredRows) {
        uint32_t theRowBlockNum = r.getStorageBlockNumber();
        theStatus = storage.freeBlocks(theRowBlockNum);
        if (!theStatus) {
            return theStatus;
        }
        theEntity->eraseDataRows(theRowBlockNum);
    }
    // Need to write back the entity.
    Block theDescribeBlock = theEntity->getBlock();
    storage.writeBlock(theEntityBlockNum, theDescribeBlock);
    anOutput << "Query Ok. " << theFilteredRows.size() << " rows affected (" << Config::getTimer().elapsed() << " sec)"
             << "\n";
    delete theEntity;

    // if caching is enables and theFilteredRows.size() > 0, need to remove from map
    if (Config::useCache(CacheType::row)) {
        if (theFilteredRows.size() > 0 && theRowCache.contains(theEntityHash)) {
            theRowCache.remove(theEntityHash);
        }
    }

    return StatusResult(Errors::noError);
}

// To get RowCollections based on DBQuery
bool Database::selectRows(DBQuery &aDB, Entity &anEntity, std::ostream &anOutput) {
    std::string theEntityName = aDB.getEntityName();
    uint32_t    theBlockNum = this->getEntityFromMap(theEntityName);
    Block      *theEntityBlock = new Block(BlockType::entity_block);
    storage.readBlock(theBlockNum, *theEntityBlock);
    anEntity.decodeBlock(*theEntityBlock);
    uint32_t theEntityHash = anEntity.getEntityHashString();
    // Set the DBQuery stringList//

    // Push the primary key first if it exists
    const Attribute *thePrimaryAttr = anEntity.getPrimaryKey();
    if (thePrimaryAttr != nullptr) {
        aDB.setAttrPrimary(thePrimaryAttr->getName());
    }
    if (aDB.getAllField()) {
        AttributeList theAttr = anEntity.getAttributes();
        for (int i = 0; i < theAttr.size(); i++) {
            if (!theAttr.at(i).isPrimaryKey()) {
                aDB.setAttr(theAttr.at(i).getName());
            }
        }
    }

    RawRowCollection theRows;
    this->each(theEntityHash, theRows);
    aDB.setEntity(&anEntity);
    RawRowCollection theFilterRows;
    filterRows(aDB, theRows, theFilterRows);
    delete theEntityBlock;
    ShowsTablesView *theShow = new ShowsTablesView(false);
    theShow->showQuery(theFilterRows, aDB, anOutput);
    return true;
}
// To filter the RowCollection based on WHERE,ORDER BY and LIMIT
void Database::filterRows(DBQuery &aDB, RawRowCollection &theRow, RawRowCollection &theFilteredRow) {
    theFilteredRow.assign(theRow.begin(), theRow.end());
    FilterRow    *theFilter = new FilterRow();
    FilterKeyword theFilterKey = aDB.getFilterKey();
    theFilter->filterWhere(aDB, theFilteredRow).filterOrderBy(aDB, theFilteredRow).filterLimit(aDB, theFilteredRow);
    delete theFilter;
}

// To get all Rows of a given EntityHash
void Database::each(uint32_t &anEntityHash, RawRowCollection &theRows) {
    // To get from cache
    if (Config::useCache(CacheType::row)) {
        if (theRowCache.contains(anEntityHash)) {
            theRows = theRowCache.get(anEntityHash);
            return;
        }
    }
    int theCurrPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    size_t theFileSize = stream.tellg();
    size_t theNumBlocks = theFileSize / ECE141::kBlockSize;
    stream.seekg(0, stream.beg);
    for (int i = 0; i < theNumBlocks; i++) {
        Block *aBlock = new Block();
        storage.readBlock(i, *aBlock);
        if (aBlock->header.theTableNameHash == anEntityHash && aBlock->header.type == 'D') {
            // It's a data block, so decode the rowBlock
            // Create a Data and Row object
            KeyValues theData;
            Row      *theRow = new Row();
            theRow->decode(*aBlock);
            theRows.push_back(*theRow);
            delete theRow;
        }

        delete aBlock;
    }
    stream.seekg(theCurrPos, stream.beg);
    // If reached here then update cache
    if (Config::useCache(CacheType::row)) {
        theRowCache.put(anEntityHash, theRows);
    }
    return;
}

StatusResult Database::createTable(CreateTableStatement *aStmt, std::ostream &anOutput) {
    CreateTableStatement *theStatement = aStmt;
    std::string           theTableName = theStatement->getTableName();
    Entity               *theEntity = new Entity(theStatement->getTableName());
    uint32_t              theCurrentEntityId = this->getEntityId();
    theEntity->setBlockId(theCurrentEntityId);
    std::vector<Attribute> theAttr = theStatement->getAttributevector();

    for (size_t i = 0; i < theAttr.size(); i++) {
        theEntity->addAttribute(theAttr.at(i));
    }

    StatusResult theStatus;
    // Checking if the table already exists
    bool theEntityExists = this->checkEntityInMap(theEntity->getName());
    if (theEntityExists) {
        return StatusResult(Errors::tableExists);
    }

    // Check if duplicate attributes
    bool theDuplicateAttrCheck = theEntity->checkDuplicateAttr();
    if (theDuplicateAttrCheck) {
        return StatusResult(Errors::attributeExists);
    }

    // Add code for index
    // Creating the index

    // const Attribute* thePrimaryKeyAttr = theEntity->getPrimaryKey();
    // IndexType theIndexType = (thePrimaryKeyAttr->getType() == DataTypes::int_type)?IndexType::intKey:IndexType::strKey;

    // // Adding the index
    // bool theCheckIfQIsEmpty = this->freeBlockQEmpty();
    // uint32_t theIdxBlockNum = this->getFreeBlock();
    // // Needs testing -> We are storing the index first and then the entity
    // uint32_t theNewBlockCount = theCheckIfQIsEmpty ? theIdxBlockNum + 1 : this->getBlockCount();
    // this->setBlockCount(theNewBlockCount);
    // //////////////
    // std::string theFieldName = const_cast<std::string&>(thePrimaryKeyAttr->getName());
    // Index theIndex(storage,theTableName,theFieldName,theIndexType,theIdxBlockNum);
    // theIndexVec.push_back(theIndex);
    // theIndexBlockNum.insert(theIdxBlockNum);

    //  // Blockifying the Index and Storing the index field in the filesystem
    // Block* theIndexBlock = new Block(BlockType::index_block);
    // theIndex.getBlock(*theIndexBlock);

    // // Store the index block in the filesystem
    // this->getStorage().writeBlock(theIdxBlockNum, *theIndexBlock);

    // // creating the table
    // // Store the entity as a block

    // // This part of code handles the blockifying the entity
    // theEntity->setIndexBlockNum(theIdxBlockNum);
    Block theConvertedBlock = theEntity->getBlock();

    // Update the the block num where the index of this entity is stored in theEntityIndexMap
    // this->setEntityIndexMap(theTableName,theIdxBlockNum);

    // For entity theBlockId is the entity number in the database
    theConvertedBlock.header.theBlockId = this->getEntityId();
    theConvertedBlock.header.theTableNameHash = Helpers::hashString(theEntity->getName().c_str());
    theEntity->setEntityHashString(theConvertedBlock.header.theTableNameHash);
    // For entity theEntityId is the current value of the auto_incr field

    theConvertedBlock.header.theEntityId = theEntity->getAutoIncr();
    bool     theCheckIfQIsEmpty = this->freeBlockQEmpty();
    uint32_t theBlockNum = this->getFreeBlock();
    theConvertedBlock.header.theBlockNum = theBlockNum;
    theEntity->setBlockNum(theBlockNum);
    this->getStorage().writeBlock(theBlockNum, theConvertedBlock);

    // Block count only incremented if no free block in between start to end of file
    uint32_t theNewBlockCount = theCheckIfQIsEmpty ? theBlockNum + 1 : this->getBlockCount();

    this->setBlockCount(theNewBlockCount);
    this->setChange(true);

    theStatus = StatusResult(Errors::noError);

    if (theStatus) {
        this->setEntityMap(theTableName, theBlockNum);
        uint32_t theEntityId = this->getEntityId() + 1;
        this->setEntityId(theEntityId);
    }

    delete theEntity;
    anOutput << "Query OK, 1 row affected";
    anOutput << " (" << Config::getTimer().elapsed() << " sec)" << std::endl;
    return theStatus;
}
StatusResult Database::insertTable(InsertTableStatement *aStmt, std::ostream &anOutput) {
    InsertTableStatement *theStatement = aStmt;
    Entity               *theEntity = new Entity(theStatement->getTableName());
    Block                *theDescribeBlock = new Block(BlockType::entity_block);
    uint32_t              theBlockNum = this->getEntityFromMap(theStatement->getTableName());
    // bool                  theCheckIfQIsEmpty = this->freeBlockQEmpty();
    // uint32_t              theBlockCount = this->getFreeBlock();
    this->getStorage().readBlock(theBlockNum, *theDescribeBlock);
    theEntity->decodeBlock(*theDescribeBlock);
    const Attribute *theAttr = theEntity->getPrimaryKey();
    std::string      thePrimaryKey = "";
    if (theAttr != nullptr) {
        thePrimaryKey = theAttr->getName();
    }

    delete theDescribeBlock;
    RowVectors theRowData = *(theStatement->getSQLProcessor()->getTheRowData());
    for (size_t i = 0; i < theRowData.size(); i++) {
        // blockify each row. The getBlock function in Row.cpp

        Block   *theRowBlock = new Block(BlockType::data_block);
        uint32_t theRowId = theEntity->getAutoIncr();
        bool     theCheckIfQIsEmpty = this->freeBlockQEmpty();
        uint32_t theBlockCount = this->getFreeBlock();
        //////////////////////////////////////////
        // Seeting the Autoincr id for primary Key

        if (thePrimaryKey != "") {
            Value thePrimaryValue = (int)theRowId;
            theRowData.at(i).set(thePrimaryKey, thePrimaryValue);
        }

        ///////////////////////////////////////////

        theRowData.at(i).setBlockNumber(theRowId);
        theRowData.at(i).tableName = theStatement->getTableName();
        theRowData.at(i).entityId = Helpers::hashString(theStatement->getTableName().c_str());
        theRowData.at(i).setStorageBlockNumber(theBlockCount);
        theRowData.at(i).getBlock(*theRowBlock);
        this->getStorage().writeBlock(theBlockCount, *theRowBlock);

        theEntity->insertDataRow(theBlockCount);
        theRowId++;
        theEntity->setAutoIncr(theRowId);
        // update blockCount
        theBlockCount = theCheckIfQIsEmpty ? ++theBlockCount : this->getBlockCount();
        this->setBlockCount(theBlockCount);
    }
    // Encode the entity block back
    Block theEntityBlock = theEntity->getBlock();
    this->getStorage().writeBlock(theBlockNum, theEntityBlock);

    delete theEntity;
    anOutput << "Query Ok, " << theRowData.size() << " rows affected (" << Config::getTimer().elapsed() << " secs)" << std::endl;
    return StatusResult(Errors::noError);
}
// StatusResult Database::indexsTable(ShowTableStatement *aStmt, std::ostream &anOutput) {

// }
StatusResult Database::showTable(ShowTableStatement *aStmt, std::ostream &anOutput) {
    std::vector<std::string> theTableVector;
    std::stringstream        ss;
    ss << "Tables_in_" << this->getDbName();
    theTableVector.push_back(ss.str());
    // uint32_t theBlockNum = (*currentActiveDbPtr)->getBlockCount();
    for (auto const &imap : this->getIdxMap()) {
        theTableVector.push_back(imap.first);
    }
    size_t theLongestString = 0;
    for (size_t i = 0; i < theTableVector.size(); i++) {
        theLongestString =
            std::max(theLongestString, theTableVector.at(i).length());
    }
    MessageViewer *theMessageHandler = new MessageViewer();
    theMessageHandler->showTableView(anOutput, theTableVector,
                                     theLongestString);

    anOutput << theTableVector.size() - 1 << " rows in set ("
             << Config::getTimer().elapsed() << " sec.)"
             << "\n";

    delete theMessageHandler;
    return StatusResult(Errors::noError);
}

StatusResult Database::describeTable(DescribeTableStatement *aStmt, std::ostream &anOutput) {
    Block       theDescribeBlock;
    std::string theTableName = aStmt->getTableName();
    uint32_t    theBlockNum = this->getEntityFromMap(theTableName);
    this->getStorage().readBlock(theBlockNum, theDescribeBlock);

    if (theDescribeBlock.header.theTitle == theTableName) {
        // decode the block
        Entity *theEntity = new Entity(theTableName);
        theEntity->decodeBlock(theDescribeBlock);
        MessageViewer *theMessageHandler = new MessageViewer();
        theMessageHandler->printAttrTable(anOutput, theEntity->getAttributes());
        anOutput << theEntity->getAttributes().size() << " rows in set ("
                 << Config::getTimer().elapsed() << " sec.)" << std::endl;
        delete theEntity;
        delete theMessageHandler;
    }
    return StatusResult(Errors::noError);
}

StatusResult Database::dropTable(DropTableStatement *aStmt, std::ostream &anOutput) {
    std::string theTableName = aStmt->getTableName();
    if (!this->checkEntityInMap(theTableName)) {
        return StatusResult(Errors::unknownTable);
    }
    uint32_t         theBlockNum = this->getEntityFromMap(theTableName);
    StatusResult     theStatus = this->getStorage().freeBlocks(theBlockNum);
    uint32_t         theEntityHash = Helpers::hashString(theTableName.c_str());
    RawRowCollection theRows;
    this->each(theEntityHash, theRows);
    for (auto &r : theRows) {
        uint32_t theRowBlockNum = r.getStorageBlockNumber();
        theStatus = this->getStorage().freeBlocks(theRowBlockNum);
    }

    if (theStatus) {
        this->setChange(true);
        this->removeEntityFromMap(theTableName);
        anOutput << "Query OK, " << theRows.size() + 1 << " rows affected (" << Config::getTimer().elapsed()
                 << " sec)"
                 << "\n";
    }

    return theStatus;
}

StatusResult Database::showQuery(SelectStatement *aStmt, std::ostream &anOutput) {
    DBQuery &theDBQuery = aStmt->getDBQuery();
    Entity  *theEntity = new Entity(theDBQuery.getEntityName());
    bool     theStatus = true;
    if (theDBQuery.getJoin()) {
        theStatus = this->showJoinQuery(theDBQuery, *theEntity, anOutput);
    } else {
        theStatus = this->selectRows(theDBQuery, *theEntity, anOutput);
    }
    delete theEntity;
    if (theStatus == true) {
        return StatusResult(Errors::noError);
    }
    return StatusResult(Errors::unknownError);
}

StatusResult Database::showJoinQuery(DBQuery &aDB, Entity &anEntity, std::ostream &anOutput) {
    // theLeftTable:
    // theRightEable: Joined one
    std::string      theLeftTable = anEntity.getName();
    uint32_t         theLeftEntityHash = Helpers::hashString(theLeftTable.c_str());
    JoinField       &theJoinField = aDB.getJoinField();
    std::string      theRightTable = theJoinField.theDefaultTable;
    uint32_t         theRightEntityHash = Helpers::hashString(theRightTable.c_str());
    RawRowCollection theLeftRows;
    RawRowCollection theRightRows;

    this->each(theLeftEntityHash, theLeftRows);
    this->each(theRightEntityHash, theRightRows);

    const Attribute *thePrimaryAttr = anEntity.getPrimaryKey();
    if (thePrimaryAttr != nullptr) {
        aDB.setAttrPrimary(thePrimaryAttr->getName());
    }
    if (aDB.getAllField()) {
        AttributeList theAttr = anEntity.getAttributes();
        for (int i = 0; i < theAttr.size(); i++) {
            if (!theAttr.at(i).isPrimaryKey()) {
                aDB.setAttr(theAttr.at(i).getName());
            }
        }
    }
    StringList theFields = aDB.getAttr();
    // select first_name, last_name, title from Users left join Books on Users.id=Books.user_id order by last_name;
    //  Filter the right table based on new DBQuery
    RawRowCollection theFilterRightRows;
    for (int i = 0; i < theLeftRows.size(); i++) {
        bool match = false;
        for (int j = 0; j < theRightRows.size(); j++) {
            if (theLeftRows[i].getValueData()[theJoinField.theLeftField.field] == theRightRows[j].getValueData()[theJoinField.theRightField.field]) {
                match = true;
                KeyValues theData;
                for (std::string field : theFields) {
                    // in left
                    KeyValues theLeftData = theLeftRows[i].getValueData();
                    if (theLeftData.find(field) != theLeftData.end()) {
                        theData[field] = theLeftRows[i].getValueData()[field];
                    } else {
                        theData[field] = theRightRows[j].getValueData()[field];
                    }
                }
                Row *theRow = new Row(theData);
                theFilterRightRows.push_back(*theRow);
            }
        }
        if (!match) {
            KeyValues theData;
            for (std::string field : theFields) {
                // in left
                if (theLeftRows[i].getValueData().find(field) != theLeftRows[i].getValueData().end()) {
                    theData[field] = theLeftRows[i].getValueData()[field];
                }
            }
            Row *theRow = new Row(theData);
            theFilterRightRows.push_back(*theRow);
        }
    }
    RawRowCollection theFinal;
    filterRows(aDB, theFilterRightRows, theFinal);
    ShowsTablesView *theShow = new ShowsTablesView(false);
    theShow->showQuery(theFinal, aDB, anOutput);
    return StatusResult{};
    // Select * from the right table where field in (List in left table field)
    // maintain set  select user_id from Books where user_id in
    // theLeftRows -> all the Rows of User
    // theRightRows -> all the Rows of Book
}

void Database::setEntityIndexMap(const std::string &aName, const uint32_t &aBlocNum) {
    this->theEntityIndexMap[aName] = aBlocNum;
}

StatusResult Database::indexTable(IndexTableStatement *aStmt, std::ostream &anOutput) {
    std::string theTableName = aStmt->getTableName();
    if (!this->checkEntityInMap(theTableName)) {
        return StatusResult(Errors::unknownTable);
    }
    // To display the table
    ShowsTablesView *theShow = new ShowsTablesView(false);
    theShow->addToRow("Key");
    theShow->addToRow("block");
    theShow->endOfRow();
    uint32_t theBlockNum = this->getEntityFromMap(theTableName);
    Entity  *theEntity = new Entity(theTableName);
    Block   *theDescribeBlock = new Block(BlockType::entity_block);
    storage.readBlock(theBlockNum, *theDescribeBlock);
    theEntity->decodeBlock(*theDescribeBlock);
    DataRows &theRows = theEntity->getDataRows();
    for (auto &theRowBlockNum : theRows) {
        Block *theRowBlock = new Block(BlockType::data_block);
        this->getStorage().readBlock(theRowBlockNum, *theRowBlock);
        Row *theRow = new Row();
        theRow->decode(*theRowBlock);
        std::string theField = aStmt->getFieldsName()[0];
        Value       theFieldVal = theRow->getValData(theField);
        if (theFieldVal.index() == 1) {
            theShow->addToRow(std::to_string(std::get<int>(theFieldVal)));
            std::string theRowBlockString = std::to_string(theRowBlockNum);
            theShow->addToRow(theRowBlockString);
            theShow->endOfRow();
        }
    }
    theShow->show(anOutput);
    anOutput << theRows.size() << " rows in set ("
             << Config::getTimer().elapsed() << " sec.)"
             << "\n";
    return StatusResult(Errors::noError);
}
StatusResult Database::indexsTable(IndexsTableStatement *aStmt, std::ostream &anOutput) {
    std::vector<std::string> theTableVector;
    std::stringstream        ss;

    size_t                   theLongestString = 0;
    for (size_t i = 0; i < theTableVector.size(); i++) {
        theLongestString =
            std::max(theLongestString, theTableVector.at(i).length());
    }
    ShowsTablesView *theShow = new ShowsTablesView(false);
    theShow->addToRow("table");
    theShow->addToRow("field(s) ");
    theShow->endOfRow();
    for (auto const &imap : this->getIdxMap()) {
        theTableVector.push_back(imap.first);
        theShow->addToRow(imap.first);
        theShow->addToRow("id");
        theShow->endOfRow();
    }
    theShow->show(anOutput);

    anOutput << theTableVector.size() << " rows in set ("
             << Config::getTimer().elapsed() << " sec.)"
             << "\n";

    // delete theMessageHandler;
    return StatusResult(Errors::noError);
}

StatusResult Database::alterTable(AlterTableStatement *aStmt, std::ostream &anOutput){
    std::string theTableName = aStmt->getTableName();
    if (!this->checkEntityInMap(theTableName)) {
        return StatusResult(Errors::unknownTable);
    }
    uint32_t theBlockNum = this->getEntityFromMap(theTableName);
    std::string theModifyType = aStmt->getAlterFieldName();
    Entity  *theEntity = new Entity(theTableName);
    Block   *theDescribeBlock = new Block(BlockType::entity_block);
    storage.readBlock(theBlockNum, *theDescribeBlock);
    theEntity->decodeBlock(*theDescribeBlock);
    std::vector<std::string> theUpdateNames;
    // Perform action on the new attribute
    if(theModifyType == "add"){
        for(auto& attr: aStmt->getAttributevector()){
            theEntity->addAttribute(attr);
            theUpdateNames.push_back(attr.getName());
        }
        // Handle add for each Row
        for(auto& row: theEntity->getDataRows()){
            Block theRowBlock{BlockType::data_block};
            storage.readBlock(row, theRowBlock);
            Row theRow;
            theRow.decode(theRowBlock);
            for(auto& theField: theUpdateNames){
                theRow.set(theField,std::string("NULL"));
            }
            Block theNewRowBlock{BlockType::data_block};
            theRow.getBlock(theNewRowBlock);
            storage.writeBlock(row, theNewRowBlock);
        }
    }else if(theModifyType == "drop"){

    }else if(theModifyType == "alter"){

    }

    // Write the entity back 
    Block theEnityBlock = theEntity->getBlock();
    storage.writeBlock(theBlockNum, theEnityBlock);
    anOutput << "Query Ok. 1 " << " rows affected (" << Config::getTimer().elapsed() << " sec)"
             << "\n";
    return StatusResult(Errors::noError);
}
}  // namespace ECE141