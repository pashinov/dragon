#include <sysinfo/cpuinfo.hpp>

CPUInfo::CPUInfo()
{
    // Get vendor name EAX=0
    CPUID cpuID0(0, 0);
    vendor_id_ += std::string((const char *)&cpuID0.EBX(), 4);
    vendor_id_ += std::string((const char *)&cpuID0.EDX(), 4);
    vendor_id_ += std::string((const char *)&cpuID0.ECX(), 4);

    // Get processor brand string
    // This seems to be working for both Intel & AMD vendors
    for(unsigned int i=0x80000002; i<0x80000005; ++i) {
        CPUID cpuID(i, 0);
        model_name_ += std::string((const char*)&cpuID.EAX(), 4);
        model_name_ += std::string((const char*)&cpuID.EBX(), 4);
        model_name_ += std::string((const char*)&cpuID.ECX(), 4);
        model_name_ += std::string((const char*)&cpuID.EDX(), 4);
    }
}
