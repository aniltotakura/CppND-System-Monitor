#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template<typename T>
T LinuxParser::retrievewithKey(string const &name, string const &filename) {
  string line, key;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == name) return value;
      }
    }
  }
  return value;
}

template <typename T>
T LinuxParser::retrieve(string const &filename) {
  string line;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

template <typename T>
T LinuxParser::retrievebyPosition(int pos, string const& line){
  T value;
  std::istringstream linestream(line);
  int i = 0;
  while(i < pos){
    linestream>> value;
    i++;
  }
  return value;
}


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
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
  std::array<float,2> mem;
  mem[0]= (retrievewithKey<float>("MemTotal:", kProcDirectory+kMeminfoFilename));
  mem[1]= (retrievewithKey<float>("MemFree:", kProcDirectory+kMeminfoFilename));
  return  (mem[0]-mem[1])/mem[0]; 
}


long LinuxParser::UpTime() { 
  return retrieve<long>(kProcDirectory +kUptimeFilename); 
  }


long LinuxParser::Jiffies() { 
  return ActiveJiffies()+IdleJiffies(); }



long LinuxParser::ActiveJiffies() { 
  auto stream = std::ifstream(kProcDirectory+kStatFilename);
	string line;
	std::getline(stream, line);
  long activejiffies =  stol(retrievebyPosition<string>(2,line))+
                        stol(retrievebyPosition<string>(3,line))+
                        stol(retrievebyPosition<string>(4,line))+
                        stol(retrievebyPosition<string>(7,line))+
                        stol(retrievebyPosition<string>(8,line))+
                        stol(retrievebyPosition<string>(9,line));

  return activejiffies; }

long LinuxParser::IdleJiffies() { 
  auto stream = std::ifstream(kProcDirectory+kStatFilename);
	string line;
	std::getline(stream, line);
  long idlejiffies =  stol(retrievebyPosition<string>(5,line))+
                      stol(retrievebyPosition<string>(6,line));
  return idlejiffies; 
}


vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() { 
  return retrievewithKey<int>("processes",kProcDirectory+kStatFilename);
}

int LinuxParser::RunningProcesses() { 
  return retrievewithKey<int>("procs_running",kProcDirectory+kStatFilename);
}


string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + "/"+ std::to_string(pid)+ kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + "/"+ std::to_string(pid)+ kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>> key>>value;
      if (key == "VmSize:"){
        auto kb= stof(value);
        return to_string(kb/1024);
      }
    } 
  }
  return string();
}

string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + "/"+ std::to_string(pid)+ kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>> key>>value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) { 
  string line, key, value1, value2;
  std::ifstream stream(kPasswordPath);
  while (std::getline(stream, line)){
    std::replace(line.begin(), line.end() , ':', ' ');  
    std::istringstream linestream(line);
    linestream >> key >> value1>> value2;
    if (value2 == Uid(pid)){
      return key;
    }
  }
return string(); 
}

long LinuxParser::UpTime(int pid) {
  auto stream = std::ifstream(kProcDirectory+ "/"+ std::to_string(pid)+ kStatFilename);
	string line;
	std::getline(stream, line);
  long uptime = stol(retrievebyPosition<string>(22, line));
  return uptime/sysconf(_SC_CLK_TCK); 
}

float LinuxParser::pidCpu(int pid){
    long sysuptime = UpTime();
    auto stream = std::ifstream(kProcDirectory+ "/"+ std::to_string(pid)+ kStatFilename);
	  string line;
	  getline(stream, line);
    long total_time=  stol(retrievebyPosition<string>(14,line))+
                      stol(retrievebyPosition<string>(15,line))+
                      stol(retrievebyPosition<string>(16,line))+
                      stol(retrievebyPosition<string>(17,line));
    long start_time= UpTime(pid);
    float seconds = (float) sysuptime - (float) start_time;
    float cpu_usage = (float) (total_time/sysconf(_SC_CLK_TCK))/seconds;
    
    if (cpu_usage < 0 || isnan(cpu_usage)){
      cpu_usage =0;
    }
  return cpu_usage;
}
