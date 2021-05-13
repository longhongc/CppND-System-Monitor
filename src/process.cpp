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

int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return 0; }

string Process::Command() const {
    return LinuxParser::Command(pid_);  
}

string Process::Ram() const {
    return LinuxParser::Ram(pid_); 
}

string Process::User() const {
    return LinuxParser::User(pid_); 
}

long int Process::UpTime() const {
    return LinuxParser::UpTime(pid_); 
}

bool Process::operator<(Process const& a) const {
    return pid_ < a.Pid(); 
}
