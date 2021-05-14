#ifndef FORMAT_H
#define FORMAT_H

#include <map>
#include <string>
#include <vector>

namespace Format {
std::string ElapsedTime(long times);
std::map<std::string, double> CpuData(std::vector<std::string> &cpu_data);
std::map<std::string, double> ProcessData(
    std::vector<std::string> &process_data);
};  // namespace Format

#endif
