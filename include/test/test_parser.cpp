#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <iomanip>

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


string OperatingSystem(){
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "PRETTY_NAME"){
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }

        }
    }
    return value; 
}

float MemoryUtilization(){
    string line;
    string key;
    int value; 
    double memtotal;
    double memfree;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "MemTotal"){
                    memtotal = value; 
                }
                if (key == "MemFree"){
                    memfree = value; 
                    return (memtotal - memfree)/memtotal; 
                }
            }

        }
    }
    return -1; 
}

long UpTime(){
    string line;
    string uptime;
    string idletime; 
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()){
        std::getline(filestream, line); 
        std::istringstream linestream(line);
        linestream >> uptime >> idletime; 
    }
    
    return stol(uptime); 

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
    string line;
    string key;
    string value; 
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "processes"){
                    return stoi(value); 
                }
            }

        }
    }
    return -1; 
}

int RunningProcesses(){
    string line;
    string key;
    string value; 
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()){
        while (std::getline(filestream, line)){
            std::istringstream linestream(line);
            while (linestream >> key >> value){
                if (key == "procs_running"){
                    return stoi(value); 
                }
            }

        }
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
    string line; 
    string temp; 
    string uptime; 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename); 
    if (filestream.is_open()){
        std::getline(filestream, line); 
        std::istringstream linestream(line); 
        for(int i=0; i<21; i++){
            linestream >> temp; 
        }
        linestream >> uptime; 
        return stoi(uptime); 
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
    string line;
    string key;
    string value; 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
    if(filestream.is_open()){
        while(std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' '); 
            std::istringstream linestream(line); 
            if(linestream >> key >> value){
                if(key == "VmSize"){
                    return value; 
                }
            }
        }
    }
    return ""; 
}
string Command(int pid) {
    string line; 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename); 
    if(filestream.is_open()){
        std::getline(filestream, line); 
        return line; 
    }
    return " "; 
}
int main(){
    //std::cout << ChangeTime(26170) << std::endl; 
    //std::cout << OperatingSystem() << std::endl; 
    //std::cout << MemoryUtilization() << std::endl; 
    //std::cout << UpTime(278) << std::endl; 
    //std::cout << Ram(279) << std::endl; 
    std::cout << Command(279) << std::endl; 
    //auto test_cpu_data = CpuUtilization(); 
    //std::cout << test_cpu_data[0] << std::endl; 
    //std::cout << test_cpu_data[1] << std::endl; 
    //std::cout << test_cpu_data[10] << std::endl; 
    //std::cout << TotalProcesses() << std::endl; 
    //std::cout << RunningProcesses() << std::endl; 

    return 0; 
}

