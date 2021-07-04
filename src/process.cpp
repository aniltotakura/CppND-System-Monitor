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

Process::Process( int pid ){
    setPid(pid);
    setCommand(LinuxParser::Command(pid));
    setRam(LinuxParser::Ram(pid));
    setUser(LinuxParser::User(pid));
    setUpTime(LinuxParser::UpTime(pid));
    setCpuUtilization(LinuxParser::pidCpu(pid));
  
}

int Process::Pid() {return Pid_; }


float Process::CpuUtilization()const{ return CpuUtilization_; }


string Process::Command() { return Command_; }


string Process::Ram() { return Ram_; }


string Process::User() { return User_; }


long int Process::UpTime() { return UpTime_; }

bool Process::operator<(Process const& a) const { 
    bool truth = (CpuUtilization_ <= a.CpuUtilization());
    return !truth; }

void Process::setPid(int const& pid){
    Pid_ = pid;
};
void Process::setUser(std::string const& user){
    User_= user;
};
void Process::setCommand(std::string const& command){
    Command_= command;
};
void Process::setCpuUtilization(float const& cpuutilization){
    CpuUtilization_ = cpuutilization;
};
void Process::setRam(std::string const& ram){
    Ram_ = ram;
};
void Process::setUpTime(long int const& uptime){
    UpTime_ = uptime;
};