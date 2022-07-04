#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int id) : _id {id} {}

// TODO: Return this process's ID
int Process::Pid() { return _id; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long totaltime = LinuxParser::ActiveJiffies(_id);
  long seconds = LinuxParser::UpTime() - LinuxParser::UpTime(_id);
  _cpuUtilization = 1.0 * totaltime / seconds;
  return _cpuUtilization;
}

// TODO: Return the command that generated this process
    string Process::Command() { return LinuxParser::Command(_id); }

    // TODO: Return this process's memory utilization
    string Process::Ram() { return LinuxParser::Ram(_id); }

    // TODO: Return the user (name) that generated this process
    string Process::User() { return LinuxParser::User(_id); }

    // TODO: Return the age of this process (in seconds)
    long int Process::UpTime() { return LinuxParser::UpTime(_id); }

    // TODO: Overload the "less than" comparison operator for Process objects
    // REMOVE: [[maybe_unused]] once you define the function
    bool Process::operator>(Process const& a) const {
      return this->_cpuUtilization > a._cpuUtilization; }
  
