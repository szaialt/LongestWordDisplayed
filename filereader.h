#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>
#include <map>

//It works only with ANSI-encoded files.

class FileReader {
       public:
              void readFile(const char* fileName);
              std::pair<char*, int> findOftestWord();
              int findWord(const char* word);
       private:
              std::map<std::string, long long> wordCalculator;
              std::map<std::string, int> wordTimesCalculator;
              std::string selectMaximum(std::map<std::string, long long> hash);
              long long calculateDifferenceAsTime(std::string time1, std::string time2);
       };

#endif //FILEREADER_H
