#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    Processor processor;
    setcpu_(processor);
}

void System::setcpu_(Processor const& cpu){
    cpu_ = cpu;
}

void System::setprocesses_(std::vector<Process> processes){
    processes_ = processes;
}
Processor& System::Cpu() { 
    return cpu_; }

vector<Process>& System::Processes() { 
    vector<int> pid_vector = LinuxParser::Pids();
    vector<Process> process_vector;
    for (int pid : pid_vector){
        Process process(pid);
        process_vector.push_back(process);
    }

    setprocesses_(process_vector);
    std::sort(processes_.begin(), processes_.end());
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