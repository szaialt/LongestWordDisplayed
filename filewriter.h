#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>

//It works only with ANSI-encoded files.
//It uses a temporarly file named tmp.txt for owerwriting.

class FileWriter {
      public:
              void writeFile(const char* in, const char* out, const char* offset); 
              void overWriteFile(const char* in, const char* offset);
      private:
              std::string addAsTime(std::string time1, std::string time2);
      }; 
#endif //FILEWRITER_H
