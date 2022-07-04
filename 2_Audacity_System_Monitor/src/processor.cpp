#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <vector>

long previousActiveTime{};
long previousTotalTime{};

float Processor::Utilization() {
  long activeTime = LinuxParser::ActiveJiffies();
  long idleTime = LinuxParser::IdleJiffies();
  long totalTime = activeTime + idleTime;

  long deltaActiveTime = activeTime - previousActiveTime;
  long deltaTotalTime = totalTime - previousTotalTime;
  float cpuUtilization = deltaTotalTime == 0 ? 0.0 : 1.0 * deltaActiveTime / deltaTotalTime;

  previousActiveTime = activeTime;
  previousTotalTime = totalTime;

  return cpuUtilization;
}