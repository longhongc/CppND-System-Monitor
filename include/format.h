#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <vector>
#include <map>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::map<std::string, double> CpuData(std::vector<std::string> &cpu_data); 
};                                    // namespace Format

#endif
