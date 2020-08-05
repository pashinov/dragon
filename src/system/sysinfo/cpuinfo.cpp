#include <system/sysinfo/cpuinfo.hpp>

namespace sys::sysinfo
{
    std::string cpu_vendor()
    {
        CPUID cpuID0(0, 0);

        std::string vendor_id;
        vendor_id += std::string((const char *)&cpuID0.EBX(), 4);
        vendor_id += std::string((const char *)&cpuID0.EDX(), 4);
        vendor_id += std::string((const char *)&cpuID0.ECX(), 4);

        return vendor_id;
    }

    std::string cpu_model()
    {
        std::string model_name;

        for(unsigned int i=0x80000002; i<0x80000005; ++i)
        {
            CPUID cpuID(i, 0);
            model_name += std::string((const char*)&cpuID.EAX(), 4);
            model_name += std::string((const char*)&cpuID.EBX(), 4);
            model_name += std::string((const char*)&cpuID.ECX(), 4);
            model_name += std::string((const char*)&cpuID.EDX(), 4);
        }

        return model_name;
    }
} // namespace sysinfo
