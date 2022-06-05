//
//  BlockIO.cpp
//  RGAssignment2
//
//  Created by rick gessner on 2/27/21.
//

#include "BlockIO.hpp"

#include <cstring>
#include <iostream>

#include "Config.hpp"
#include "Errors.hpp"

namespace ECE141 {

Block::Block(BlockType aType) : header(aType) {
    memset(this->payload, 0, sizeof(this->payload));
}

Block::Block(const Block &aCopy) {
    // std::cout << "I am called" << std::endl;
    *this = aCopy;
}

Block &Block::operator=(const Block &aCopy) {
    std::memcpy(payload, aCopy.payload, kPayloadSize);
    header = aCopy.header;

    return *this;
}

StatusResult Block::write(std::ostream &aStream) {
    // aStream.write(payload, kBlockSize);

    return StatusResult(Errors::noError);
}

StatusResult Block::read(std::istream &aStream) {
    aStream.read(payload, kBlockSize);
    return StatusResult{Errors::noError};
}

//---------------------------------------------------

BlockIO::BlockIO(std::iostream &aStream) : stream(aStream) {
    theBlockLRUCache.setsize(Config::getCacheSize(CacheType::block));
}

// USE: write data a given block (after seek) ---------------------------------------
StatusResult BlockIO::writeBlock(uint32_t aBlockNum, Block &aBlock) {
    stream.seekp(aBlockNum * kBlockSize, std::ios::beg);
    stream.write(reinterpret_cast<char *>(&aBlock), kBlockSize);
    stream.flush();
    if (Config::useCache(CacheType::block)) {
        if (theBlockLRUCache.contains(aBlockNum)) {
            theBlockLRUCache.remove(aBlockNum);
        }
    }
    return StatusResult{Errors::noError};
}

// USE: read data a given block (after seek) ---------------------------------------
StatusResult BlockIO::readBlock(uint32_t aBlockNum, Block &aBlock) {
    if (Config::useCache(CacheType::block)) {
        if (theBlockLRUCache.contains(aBlockNum)) {
            aBlock = theBlockLRUCache.get(aBlockNum);
        } else {
            size_t theCurrentPos = stream.tellg();
            stream.seekg(aBlockNum * kBlockSize, std::ios::beg);
            stream.read(reinterpret_cast<char *>(&aBlock), kPayloadSize);
            stream.seekg(theCurrentPos, stream.beg);
            theBlockLRUCache.put(aBlockNum, aBlock);
        }

    } else {
        size_t theCurrentPos = stream.tellg();
        stream.seekg(aBlockNum * kBlockSize, std::ios::beg);
        stream.read(reinterpret_cast<char *>(&aBlock), kPayloadSize);
        stream.seekg(theCurrentPos, stream.beg);
    }
    return StatusResult(Errors::noError);

    //####################################################
    // size_t theCurrentPos = stream.tellg();
    // stream.seekg(aBlockNum * kBlockSize, std::ios::beg);
    // stream.read(reinterpret_cast<char *>(&aBlock), kPayloadSize);
    // stream.seekg(theCurrentPos, stream.beg);
    // return StatusResult(Errors::noError);
}

StatusResult BlockIO::createMetaBlock(Block &aBlock) {
    return StatusResult{writeError};
}

// StatusResult createMetaBlock() {
//     Block *theMetaBlock = new Block(BlockType::meta_block);
//     //StatusResult theCreateResult = writeBlock(0, *theMetaBlock);
//     //stream.close();
// 	return StatusResult{writeError};
// }

// USE: count blocks in file ---------------------------------------
uint32_t BlockIO::getBlockCount() {
    return blockCount;  // What should this be?
}

}  // namespace ECE141