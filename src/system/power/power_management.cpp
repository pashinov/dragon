// system includes
#include <unistd.h>
#include <sys/reboot.h>
#ifdef __linux__
#include <linux/reboot.h>
#endif

// project includes
#include <system/power/power_management.hpp>

namespace sys::power
{
    void reboot_system()
    {
        sync();
        reboot(RB_AUTOBOOT);
    }

    void shutdown_system()
    {
        sync();
#ifdef __APPLE__
        // TODO: MacOS shutdown system implementation
#elif __linux__
        reboot(LINUX_REBOOT_CMD_POWER_OFF);
#endif
    }
}
