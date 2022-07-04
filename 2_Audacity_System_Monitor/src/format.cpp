#include <string>

#include "format.h"
using std::string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::TimeToString(int& hours, int& minuts, int& seconds){
    short n= 10;
    std::string hoursStr, minutsStr, SecondsStr;
    (hours < n) ? hoursStr = "0" + std::to_string(hours) : hoursStr =  std::to_string(hours);
    (minuts < n) ? minutsStr = "0" + std::to_string(minuts) : minutsStr =  std::to_string(minuts);
    (seconds < n) ? SecondsStr = "0" + std::to_string(seconds) : SecondsStr =  std::to_string(seconds);

    return hoursStr + ":" + minutsStr + ":" +SecondsStr;
}

string Format::ElapsedTime(long seconds) {
  
  int elapsedHours = seconds / 3600;
  int elapsedMinutes = (seconds % 3600) / 60;
  int elapsedSeconds = seconds % 60;
  
  return Format::TimeToString(elapsedHours, elapsedMinutes, elapsedSeconds);
}
