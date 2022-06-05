//
//  Storage.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "Storage.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <sstream>
#include <vector>

#include "Config.hpp"

namespace ECE141 {

// storage class?
Storage::Storage(std::iostream &aStream) : BlockIO(aStream) {}

StatusResult Storage::freeBlocks(uint32_t &aBlockNum){
    Block theBlock;
    this->readBlock(aBlockNum,theBlock);
    theBlock.header.type = 'F';
    this->setQueue(aBlockNum);
    return this->writeBlock(aBlockNum,theBlock);
    
}

void Storage::populateFreeBlockQ(){
    Block theBlock;
    stream.seekg(0,std::ios::end);
    size_t theFileSize = stream.tellg();
    size_t theNumBlocks = theFileSize/ECE141::kBlockSize;
    stream.seekg(0,std::ios::beg);
    for(size_t i = 0; i<theNumBlocks;i++){
        BlockIO::readBlock(i,theBlock);
        if(theBlock.header.type == 'F'){
            thefreeBlockQ.push(i);
        }
    }
}

StatusResult Storage::getTables(uint32_t &aBlockCount,std::vector<std::string>& TableVec){
    Block theBlock;
    for(size_t i = 0; i<aBlockCount;i++){
        BlockIO::readBlock(i,theBlock);
        if(theBlock.header.type=='E'){
            TableVec.push_back(theBlock.header.theTitle);
        }
        
    }

    return StatusResult(Errors::noError);
}

StatusResult Storage::getTableByName(uint32_t &aBlockCount,const std::string &aName, Block &aBlock){
    for(size_t i = 0; i<aBlockCount;i++){
        BlockIO::readBlock(i,aBlock);
        if(aBlock.header.theTitle==aName && aBlock.header.type=='E'){
            return StatusResult(Errors::noError);
        }
    }

    return StatusResult(Errors::unknownTable);
}


Block Storage::encodeMetaBlock(std::map<std::string,uint32_t> &anIdxMap, uint32_t &anEntityId){
    // Encode meta block
    Block aBlock;
    stream.seekg(0,std::ios::beg);
    stream.read(reinterpret_cast<char*>(&aBlock), kBlockSize);
    aBlock.header.theEntityId = anEntityId;
    std::stringstream ss;

    ss<<"EntityMap:" <<" ";
    
    // Store key-val
    for (auto const& [key, val] : anIdxMap){
        ss<<"#"<<" "<<key<<" "<<" "<<val<<" "<<"#"<<"\n";
    }
    ss<<"END"<<"\n";
    ss.read(aBlock.payload,kPayloadSize);

    return aBlock;

}

// USE: Decode the metablock to populate Entity Map and Entityid
StatusResult Storage::decodeMetaBlock(std::map<std::string,uint32_t>& anIdxMap, uint32_t &anEntityId){

    
    Block theBlock;
    BlockIO::readBlock(0,theBlock);
    anEntityId = theBlock.header.theEntityId;
    std::stringstream theStream;
    theStream.write(theBlock.payload,kPayloadSize); // string stream -> read -> write 
    std::string theInfo; // first one will be EntityMap:
   // theStream>>theInfo>>theInfo;
    theStream>>theInfo;
    while(theStream>>theInfo){
        if(theInfo=="END" || theInfo == "\0"){
            break;
        }
        uint32_t val;
        std::string key;
        theStream>>key>>val>>theInfo;  // Encoding format is # key val #
        anIdxMap[key]=val;

    }
    
    // Entity_Map: # Users 20 # #Payments 30# #Books 1 # END
    return StatusResult(Errors::noError);

}

}  // namespace ECE141
