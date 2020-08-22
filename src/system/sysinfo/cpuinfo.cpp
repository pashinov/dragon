#include <system/sysinfo/cpuinfo.hpp>

namespace sys::sysinfo
{
    std::string cpu_vendor()
    {
        std::string vendor_id;
#ifdef __x86_64__
        CPUID cpuID0(0, 0);

        vendor_id += std::string((const char *)&cpuID0.EBX(), 4);
        vendor_id += std::string((const char *)&cpuID0.EDX(), 4);
        vendor_id += std::string((const char *)&cpuID0.ECX(), 4);
#elif __arm__
        vendor_id = "Unknown"; // TODO: need to implement
#endif
        return vendor_id;
    }

    std::string cpu_model()
    {
        std::string model_name;
#ifdef __x86_64__
        for(unsigned int i=0x80000002; i<0x80000005; ++i)
        {
            CPUID cpuID(i, 0);
            model_name += std::string((const char*)&cpuID.EAX(), 4);
            model_name += std::string((const char*)&cpuID.EBX(), 4);
            model_name += std::string((const char*)&cpuID.ECX(), 4);
            model_name += std::string((const char*)&cpuID.EDX(), 4);
        }
#elif __arm__
        model_name = "ARM"; // TODO: need to implement
#endif
        return model_name;
    }
} // namespace sysinfo
