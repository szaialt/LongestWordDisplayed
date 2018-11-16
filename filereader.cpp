#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iterator>
#include "filereader.h"
#include "exceptions.h"
#include "utility.h"

using namespace std;

//It works only with ANSI-encoded files.

void FileReader::readFile(const char* fileName){
  string line;
  long long difference = 0;
  ifstream myfile(fileName);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      if (line.empty()) continue;
      bool hasPunctuation = false;
      bool hasLetter = false;
      for (int i = 0; i < line.size(); i++){
          if (ispunct(line.at(i))) hasPunctuation = true;
          if (isalpha(line.at(i))) hasLetter = true;
      }
      if (hasPunctuation && !hasLetter){
           vector<string> tokens = Utility::split(line, ' ');
           difference = calculateDifferenceAsTime(tokens.at(0), tokens.at(2));
      }
      else if (hasLetter){
          vector<string> tokens = Utility::split(line, ' ');
          for (int i = 0; i < tokens.size(); i++){
              string word = tokens.at(i);
            if (wordCalculator.find(word) != wordCalculator.end()){
                long long time = wordCalculator.find(word)->second;
                time = time + difference;
                wordCalculator.erase(word);
                wordCalculator.insert(make_pair(word, time));
                
                int times = wordTimesCalculator.find(word)->second;
                times = times + 1;
                wordTimesCalculator.erase(word);
                wordTimesCalculator.insert(make_pair(word, times));
                
            }
            else {
                 wordCalculator.insert(make_pair(word, difference));
                 wordTimesCalculator.insert(make_pair(word, 1));
             }
              
          }
      }
      else {
           continue;
      }
    }
    myfile.close();
  }
  
  else throw FileNotFoundException(); 

  
}

std::pair<char*, int> FileReader::findOftestWord(){
  string oftestWord = selectMaximum(wordCalculator); 
  char * oftest = new char [oftestWord.length()+1];
  std::strcpy(oftest, oftestWord.c_str());
  int times = wordTimesCalculator.find(oftestWord)->second;
  return make_pair(oftest, times);
}

int FileReader::findWord(const char* word){
    
      if (wordTimesCalculator.find(word) != wordTimesCalculator.end()){
        return wordTimesCalculator.find(word)->second;
      }
      return 0;
}

std::string FileReader::selectMaximum(std::map<string, long long> hash){
    map<string, long long>::iterator it;
    long long max = 0;
    string word = "";
    for (it = hash.begin(); it != hash.end(); it++)
       {
            if ((it->first.size() >= 4) && (it->second > max)){
                max = it->second;
                word = it->first;
            }
       }
    return word;
}

long long FileReader::calculateDifferenceAsTime(std::string time1, std::string time2){
    
    int hours1, minutes1, seconds1, milliseconds1;
    int hours2, minutes2, seconds2, milliseconds2;

    sscanf(time1.c_str(), "%2d:%2d:%2d,%3d", &hours1, &minutes1, &seconds1, &milliseconds1);

    sscanf(time2.c_str(), "%2d:%2d:%2d,%3d", &hours2, &minutes2, &seconds2, &milliseconds2);

    int hours = hours2 - hours1;
    int minutes = minutes2 - minutes1;
    int seconds = seconds2 - seconds1;
    int milliseconds = milliseconds2 - milliseconds1;
    
    long long inMilliseconds;
    
    inMilliseconds = (hours * 60 + minutes) * 60 + seconds;
    inMilliseconds = milliseconds * 1000 + milliseconds;

    return inMilliseconds;
    
    }
