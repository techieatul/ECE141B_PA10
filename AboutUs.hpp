//
//  AboutUs.hpp
//  About your team members...
//
//  Created by rick gessner on 3/23/22.
//

#ifndef AboutMe_hpp
#define AboutMe_hpp

#include <stdio.h>

#include <optional>
#include <string>

namespace ECE141 {

using StringOpt = std::optional<std::string>;

class AboutUs {
   private:
    std::vector<std::string> theStudentNameList{"YaChen Tsai", "Atul Acharya"};
    std::vector<std::string> theStudentGithubList{"b05611003", "techieatul"};

   public:
    AboutUs() = default;
    ~AboutUs() = default;

    size_t    getTeamSize() { return theStudentNameList.size(); }  // STUDENT: update this...

    StringOpt getName(size_t anIndex) const {
        // return name of student anIndex N (or nullopt)
        bool outOfBound = (anIndex < 0 || anIndex > theStudentNameList.size());
        if (outOfBound) return std::nullopt;
        return theStudentNameList[anIndex];
    }
    StringOpt getGithubUsername(size_t anIndex) const {
        // return github username of student anIndex N (or nullopt)
        bool outOfBound = (anIndex < 0 || anIndex > theStudentGithubList.size());
        if (outOfBound) return std::nullopt;
        return theStudentGithubList[anIndex];
    }
};

}  // namespace ECE141

#endif /* about_me */
