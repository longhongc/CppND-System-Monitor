#include <dirent.h>
#include <unistd.h>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::FindParameter(string filename, int row, int col, char separator = ' '){
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

string LinuxParser::OperatingSystem() {
    string os = LinuxParser::FindParameter(kOSPath, 5, 2, '=');  
    return os.substr(1, os.size()-2); 
}

string LinuxParser::Kernel() {
  string kernel = LinuxParser::FindParameter(kProcDirectory + kVersionFilename, 1, 3);  
  return kernel; 
}

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

float LinuxParser::MemoryUtilization() {
    string memtotal = LinuxParser::FindParameter(kProcDirectory + kMeminfoFilename, 1, 2); 
    string memfree = LinuxParser::FindParameter(kProcDirectory + kMeminfoFilename, 2, 2); 
    if(!memtotal.empty() && !memfree.empty()){
        return static_cast<float>(stoi(memtotal) - stoi(memfree)) / stoi(memtotal); 
    }
    return -1; 
}
 
long LinuxParser::UpTime() {
    string uptime = LinuxParser::FindParameter(kProcDirectory + kUptimeFilename, 1, 1);  
    if (!uptime.empty()){
        return stol(uptime); 
    }
    return -1; 
}

vector<string> LinuxParser::ProcessUtilization(int pid) {
  string line;
  vector<string> process_data(22);
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto& data : process_data) {
      linestream >> data;
    }
    return process_data;
  }
  return {};
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  vector<string> cpu_data(11);
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (auto& data : cpu_data) {
      linestream >> data;
    }
    return cpu_data;
  }
  return {};
}

int LinuxParser::TotalProcesses() {
    string total = LinuxParser::FindParameter(kProcDirectory + kStatFilename, 9, 2); 
    if(!total.empty()){
        return stoi(total); 
    }
    return -1;
}

int LinuxParser::RunningProcesses() {
    string running = LinuxParser::FindParameter(kProcDirectory + kStatFilename, 10, 2); 
    if(!running.empty()){
        return stoi(running); 
    }
    return -1; 
}

string LinuxParser::Command(int pid) {
    string command = LinuxParser::FindParameter(kProcDirectory + std::to_string(pid) + kCmdlineFilename, 9, 2); 
    if(command.size()>30){
        return command.substr(0, 30) + "..."; 
    }
    return command; 
}

string LinuxParser::Ram(int pid) {
  //VmData
  string ram = LinuxParser::FindParameter(kProcDirectory + std::to_string(pid) + kStatusFilename, 16, 2); 
  if(!ram.empty()){
      return std::to_string(stoi(ram)/1024.0); 
  }
  return ram; 
}

string LinuxParser::Uid(int pid) {
  string uid = LinuxParser::FindParameter(kProcDirectory + std::to_string(pid) + kStatusFilename, 7, 2); 
  return uid; 
}

string LinuxParser::User(int pid) {
  string line;
  string user;
  string pw;
  string uid;
  std::map<string, string> uid_user;
  // parse /etc/passwd to find user: uid
  std::ifstream userstream(kPasswordPath);
  if (userstream.is_open()) {
    while (std::getline(userstream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> pw >> uid;
      uid_user[uid] = user;
    }
  }
  return uid_user[LinuxParser::Uid(pid)];
}

float LinuxParser::StartTime(int pid) {
    string starttime = LinuxParser::FindParameter(kProcDirectory + std::to_string(pid) + kStatFilename, 1, 22); 
    if(!starttime.empty()){
        return static_cast<float>(stoi(starttime))/sysconf(_SC_CLK_TCK); 
    }
    return -1; 
}
