#include "helper.h"
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

namespace helper {
    std::string getExePath() {
        char exe_path[PATH_MAX] = {0};
        uint32_t bufsize = sizeof(exe_path);
        #ifdef __APPLE__
            _NSGetExecutablePath(exe_path, &bufsize);
        #else
            readlink("/proc/self/exe", exe_path, bufsize);
        #endif
        return dirname(exe_path);
    }
}
