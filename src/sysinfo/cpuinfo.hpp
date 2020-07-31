//
// Created by apashinov on 31/07/20
//

#ifndef SYSINFO_CPUINFO_HPP
#define SYSINFO_CPUINFO_HPP

// system includes
#include <cstdint>
#include <string>

class CPUID
{
public:
    explicit CPUID(unsigned funcId, unsigned subFuncId)
    {
        asm volatile
        ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
        : "a" (funcId), "c" (subFuncId));
    }

    [[nodiscard]] const uint32_t &EAX() const {return regs[0];}
    [[nodiscard]] const uint32_t &EBX() const {return regs[1];}
    [[nodiscard]] const uint32_t &ECX() const {return regs[2];}
    [[nodiscard]] const uint32_t &EDX() const {return regs[3];}
private:
    uint32_t regs[4]{};
};

class CPUInfo
{
public:
    CPUInfo();
    [[nodiscard]] std::string  vendor() const { return vendor_id_;  }
    [[nodiscard]] std::string  model()  const { return model_name_; }

private:
    std::string vendor_id_;
    std::string model_name_;
};

#endif // SYSINFO_CPUINFO_HPP
