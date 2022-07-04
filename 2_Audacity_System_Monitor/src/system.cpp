#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/
System::System() {}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {

  set<int> distinctIds;
  for (int& _pid : LinuxParser::Pids()) {
    distinctIds.insert(_pid);
  }

  vector<int> actualpids;
  for (auto& process : processes_) {
    actualpids.push_back(process.Pid());
  }

  for (auto& _distinctId : distinctIds) {
    Process process(_distinctId);
    if (std::find(actualpids.begin(), actualpids.end(), _distinctId) == actualpids.end()) {
      processes_.push_back(process);
    }
  }

  for (Process& process : processes_) {
    process.CpuUtilization();
  }

  std::sort(processes_.begin(), processes_.end(),
            [](Process& p1, Process& p2) { return (p1 > p2); });
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
