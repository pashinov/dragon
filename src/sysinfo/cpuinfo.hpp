//
// Created by apashinov on 31/07/20
//

#ifndef SYSINFO_CPUINFO_HPP
#define SYSINFO_CPUINFO_HPP

// system includes
#include <cstdint>
#include <string>

namespace sysinfo
{
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

    std::string cpu_vendor();
    std::string cpu_model();

} // namespace sysinfo

#endif // SYSINFO_CPUINFO_HPP
