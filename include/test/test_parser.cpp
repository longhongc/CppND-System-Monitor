#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <iomanip>

#define debug(x) std::cout<< "Var " << (#x) << ": " << x << std::endl  

using std::stoi; 
using std::stol; 
using std::string;
using std::vector;

const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

string FindParameter(string filename, int row, int col, char separator = ' '){
    string line;
    string param; 
    std::ifstream filestream(filename);
    if(filestream.is_open()){
        for(int i=0; i<row; i++){
            std::getline(filestream, line); 
        }
        if(separator != ' '){
            std::replace(line.begin(), line.end(), ' ', '_'); 
            std::replace(line.begin(), line.end(), separator, ' '); 
        }
        std::istringstream linestream(line); 
        for(int i=0; i<col; i++){
            linestream >> param; 
        }
        if(separator != ' '){
            std::replace(param.begin(), param.end(), '_', ' '); 
        }
        return param; 
    }
    return {}; 
}

string OperatingSystem(){
    string os = FindParameter(kOSPath, 5, 2, '=');  
    return os.substr(1, os.size()-2); 
}

float MemoryUtilization(){
    string memtotal = FindParameter(kProcDirectory + kMeminfoFilename, 1, 2); 
    string memfree = FindParameter(kProcDirectory + kMeminfoFilename, 2, 2); 
    if(!memtotal.empty() && !memfree.empty()){
        return static_cast<float>(stoi(memtotal) - stoi(memfree)) / stoi(memtotal); 
    }
    return -1; 
}

long UpTime(){
    string uptime = FindParameter(kProcDirectory + kUptimeFilename, 1, 1);  
    if (!uptime.empty()){
        return stol(uptime); 
    }
    return -1; 
}

vector<string> CpuUtilization(){
    string line;
    string key;
    vector<string> cpu_data(11); 
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        std::getline(filestream, line); 
        std::istringstream linestream(line);
        for(auto &data: cpu_data){
            linestream >> data;  
        }
        return cpu_data; 
    }
    return {};
}

int TotalProcesses(){
    string running = FindParameter(kProcDirectory + kStatFilename, 9, 2); 
    if(!running.empty()){
        return stoi(running); 
    }
    return -1;
}

int RunningProcesses(){
    string running = FindParameter(kProcDirectory + kStatFilename, 10, 2); 
    if(!running.empty()){
        return stoi(running); 
    }
    return -1; 
}

string ChangeTime(long seconds){
    double days = double(seconds)/60/60/24; 
    double hours = (days - int(days))*24; 
    double minutes = (hours - int(hours))*60;
    seconds = (minutes - int(minutes))*60;
    std::ostringstream datetimestream; 
    datetimestream << int(days) << "d "
      << std::setfill('0') << std::setw(2) << int(hours) << ":"
      << std::setfill('0') << std::setw(2) << int(minutes) << ":"  
      << std::setfill('0') << std::setw(2) << seconds; 
    return datetimestream.str(); 
}

long UpTime(int pid) {
    string starttime = FindParameter(kProcDirectory + std::to_string(pid) + kStatFilename, 1, 22); 
    if(!starttime.empty()){
        return stoi(starttime); 
    }
    return -1; 
}

string User(int pid) {
    string line;
    string user;
    string pw; 
    string uid; 
    std::map<string, string> uid_user; 
    // parse /etc/passwd to find user: uid
    std::ifstream userstream(kPasswordPath); 
    if(userstream.is_open()){
        while(std::getline(userstream, line)){
            std::replace(line.begin(), line.end(), ':', ' '); 
            std::istringstream linestream(line); 
            linestream >> user >> pw >> uid; 
            uid_user[uid] = user; 
        }
    }
    string key;
    string value; 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
    if(filestream.is_open()){
        while(std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' '); 
            std::istringstream linestream(line); 
            if(linestream >> key >> value){
                if(key == "Uid"){
                    return uid_user[value]; 
                }
            }
        }

    }
    return ""; 
}
string Ram(int pid) {
    string ram = FindParameter(kProcDirectory + std::to_string(pid) + kStatusFilename, 16, 2); 
    return ram; 
}
string Command(int pid) {
    string command = FindParameter(kProcDirectory + std::to_string(pid) + kCmdlineFilename, 9, 2); 
    if(command.size()>30){
        return command.substr(0, 30) + "..."; 
    }
    return command; 
}
int main(){
    //std::cout << ChangeTime(26170) << std::endl; 
    std::cout << OperatingSystem() << std::endl; 
    std::cout << MemoryUtilization() << std::endl; 
    std::cout << UpTime(916) << std::endl; 
    std::cout << Ram(916) << std::endl; 
    std::cout << Command(916) << std::endl; 
    //auto test_cpu_data = CpuUtilization(); 
    //std::cout << test_cpu_data[0] << std::endl; 
    //std::cout << test_cpu_data[1] << std::endl; 
    //std::cout << test_cpu_data[10] << std::endl; 
    std::cout << TotalProcesses() << std::endl; 
    std::cout << RunningProcesses() << std::endl; 

    return 0; 
}

