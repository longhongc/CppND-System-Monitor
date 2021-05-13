#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

float System::CpuUtilization() {
   return cpu_.Utilization(); 
}
 
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    auto pids = LinuxParser::Pids(); 
    for(auto &pid: pids){
        Process process{pid}; 
        processes_.push_back(process); 
    }
    return processes_; 
}

std::string System::Kernel() {
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() {
    return LinuxParser::UpTime(); 
}

