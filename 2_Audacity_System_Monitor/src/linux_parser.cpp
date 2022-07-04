#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  vector<float> memValues;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if ((key == "MemTotal:") || (key == "MemFree:")) {
          memValues.push_back(value / 1024);
        }
      }
    }
  }
  // MemTotal - Memfree
  return ((memValues.at(0) - memValues.at(1)) / memValues.at(0));
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return (sysconf(_SC_CLK_TCK) * UpTime()); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string pidLink = "/" + to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidLink + kStatFilename);
  string line;
  string key;
  long activejiffies{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int n = 1; n <= 17; n++) {
      linestream >> key;
      if ((n >= 14)) {
        activejiffies += std::stol(key);
      }
    }
  }
  return activejiffies / sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line;
  string key;
  long activejiffies{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int n = 1; n <= 8; n++) {
      linestream >> key;
      if ((n > 1) && (n < 5)) activejiffies += std::stol(key);
      if (n > 6) activejiffies += std::stol(key);
    }
  }
  return activejiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line;
  string key;
  long idlejiffies{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int n = 1; n <= 6; n++) {
      linestream >> key;
      if ((n == 5) || (n == 6)) {
        idlejiffies += std::stol(key);
      }
    }
  }
  return idlejiffies;
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> cpuUtilizationVector;
  cpuUtilizationVector.push_back(ActiveJiffies());
  cpuUtilizationVector.push_back(IdleJiffies());
  return cpuUtilizationVector;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value{};
  int totalprocesses{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") totalprocesses = value;
      }
    }
  }
  return totalprocesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  int runningprocesses{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") runningprocesses = value;
      }
    }
  }
  return runningprocesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string pidLink = "/" + to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidLink + kCmdlineFilename);
  string line{};
  string command;
  if (filestream.is_open()) std::getline(filestream, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string pidLink = "/" + to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidLink + kStatusFilename);
  string line;
  string key;
  int value; 
  int ram;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:")  ram = value;
      }
    }
  }
  return to_string(ram / 1000);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string pidLink = "/" + to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidLink + kStatusFilename);
  string line;
  string key;
  int value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return to_string(value);
        }
      }
    }
  }
  return string();
}
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  if (uid == "0") {
    return "root";
  } else {
    std::ifstream filestream(kPasswordPath);
    string username;
    string line;
    string key;
    string user;
    int value;
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> username >> key >> value) {
          if (value == stoi(uid)) {
            user = username;
          }
        }
      }
    }
    return user;
  }
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::UpTime(int pid) {
  string pidLink = "/" + to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidLink + kStatFilename);
  string line;
  string key;
  long uptime;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 22; i++) {
      linestream >> key;
      if (i == 22) {
        uptime = std::stol(key);
      }
    }
  }
  return uptime / sysconf(_SC_CLK_TCK);
}
  
