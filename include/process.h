#ifndef PROCESS_H
#define PROCESS_H

#include <memory>
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
   public:
    virtual int Pid() const = 0;
    virtual std::string User() const = 0;
    virtual std::string Command() const = 0;
    virtual long RamInMb() const = 0;
    virtual long UpTime() const = 0;
    virtual float CpuUtilization() const = 0;
    bool operator<(const Process& a) const;

    static std::shared_ptr<Process> makeProcess(int pid);
};

#endif