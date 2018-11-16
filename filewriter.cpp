#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "filewriter.h"
#include "exceptions.h"
#include "utility.h"

using namespace std;

//It works only with ANSI-encoded files.

void FileWriter::writeFile(const char* in, const char* out, const char* offset){
  string line;
  long long difference = 0;
  ifstream inFile(in);
  ofstream outFile;
  outFile.open(out, ios::app);
  outFile.eof();
  if (inFile.is_open() && (outFile.good()))
  {
    while ( getline (inFile,line) )
    {
      if (line.empty()) {
          outFile << endl;
          continue;
      }
      else {
          bool hasPunctuation = false;
          bool hasLetter = false;
          for (int i = 0; i < line.size(); i++){
              if (ispunct(line.at(i))) hasPunctuation = true;
              if (isalpha(line.at(i))) hasLetter = true;
         }
         if (hasPunctuation && !hasLetter) {
              vector<string> tokens = Utility::split(line, ' ');
              string w = "";
              for (int j = 0; j < tokens.at(0).size(); j++){
                   if ((tokens.at(0).at(j) != '-') && (tokens.at(0).at(j) != '>')) {
                       w.append(tokens.at(0).substr(j, 1));
                   }
               }
               string begin = addAsTime(w, offset);
               string end = addAsTime(tokens.at(2), offset);
               outFile << begin;
               outFile << " --> ";
               outFile << end;
               outFile << endl;
        }
        else {
               outFile << line << endl;
        }
      }
    }
    outFile.flush();
    inFile.close();
    outFile.close();
    }
     else throw FileNotFoundException(); 
}

void FileWriter::overWriteFile(const char* in, const char* offset){
    const char* out = "tmp.txt";
    //Create the file
     ofstream output(out);
     output.close();
     writeFile(in, out, offset);
     remove(in);
     rename(out, in);
}

std::string FileWriter::addAsTime(std::string time1, std::string time2){
  int hours1, minutes1, seconds1, milliseconds1;
    int hours2, minutes2, seconds2, milliseconds2;

    sscanf(time1.c_str(), "%2d:%2d:%2d,%3d", &hours1, &minutes1, &seconds1, &milliseconds1);

    sscanf(time2.c_str(), "%2d:%2d:%2d,%3d", &hours2, &minutes2, &seconds2, &milliseconds2);
    
    int milliseconds = milliseconds1 + milliseconds2;
    int transfer = milliseconds / 1000;
    milliseconds = milliseconds % 1000;
    
    int seconds = transfer + seconds1 + seconds2;
    transfer = seconds / 60;
    seconds = seconds % 60;
    
    int minutes = transfer + minutes1 + minutes2;
    transfer = minutes / 60;
    minutes = minutes % 60;
    
    int hours = transfer + hours1 + hours2;
    
    std::stringstream ss;
    ss<<setfill('0')<<setw(2)<<hours<<":"<<minutes<<":"<<seconds<<","<<setw(3)<<milliseconds;
    return ss.str();
}
