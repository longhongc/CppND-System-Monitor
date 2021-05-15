#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

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

bool IsGreater(Process *a, Process *b){
    return a->cpu_percentage_> b->cpu_percentage_; 
}

vector<Process *>& System::Processes() {
    processes_.clear(); 
    auto pids = LinuxParser::Pids(); 
    for(const auto &pid: pids){
        if(process_map_.find(pid) == process_map_.end()){
            Process *process_ptr = new Process(pid);  
            process_map_[pid] = process_ptr; 
        }
    }
    for(auto it = process_map_.begin(); it != process_map_.end();){
        auto p = std::find(pids.begin(), pids.end(), it->first); 
        if(p==pids.end()){
            delete it->second; 
            it = process_map_.erase(it); 
        }else{
            processes_.push_back(it->second);
            ++it; 
        }
    }
    std::sort(processes_.begin(), processes_.end(),IsGreater); 
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

