//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
namespace fs = std::filesystem;

namespace ECE141 {

using FileVisitor = std::function<bool(const std::string &)>;

class FolderReader {
   public:
    std::string path;
    FolderReader(const char *aPath);
    virtual ~FolderReader();

    virtual bool exists(const std::string &aFilename) {
        std::ifstream theStream(aFilename);
        return !theStream ? false : true;
    }

    virtual void each(const std::string &anExt,
                      const FileVisitor &aVisitor) const;
};

}  // namespace ECE141

#endif /* FolderReader_h */
