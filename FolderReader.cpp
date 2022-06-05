#include "FolderReader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
namespace ECE141 {

std::string path;
FolderReader::FolderReader(const char *aPath) : path(aPath){};
FolderReader::~FolderReader(){};
// Function that goes through each of the file with the given extension in a directory and
// calls aVisitor function
void FolderReader::each(const std::string &anExt, const FileVisitor &aVisitor) const {
    for (auto &fileName : fs::directory_iterator(path)) {
        if (fileName.path().extension() == anExt)

            aVisitor(fileName.path().stem().string());
    }
};
}  // namespace ECE141
