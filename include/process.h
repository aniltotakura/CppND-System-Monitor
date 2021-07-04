#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  void setPid(int const&);
  void setUser(std::string const&);
  void setCommand(std::string const&);
  void setCpuUtilization(float const&);
  void setRam(std::string const&);
  void setUpTime(long int const&);
  

 private:
    int Pid_;
    std::string User_;
    std::string Command_;
    float CpuUtilization_;
    std::string Ram_;
    long int UpTime_;
};

#endif