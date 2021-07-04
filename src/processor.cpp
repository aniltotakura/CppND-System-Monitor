#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long diff= LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
    float cpu_percentage = (float)diff/LinuxParser::Jiffies();
    return cpu_percentage; }