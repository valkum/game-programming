#include "Helper.hh"
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

namespace Helper {
std::string getExePath() {
  char exe_path[PATH_MAX] = { 0 };
  // uint32_t bufsize        = sizeof(exe_path);

    #ifdef __APPLE__
  _NSGetExecutablePath(exe_path, &bufsize);
    #else // ifdef __APPLE__
  // if(readlink("/proc/self/exe", exe_path, bufsize) == -1) 
  	return "";
    #endif // ifdef __APPLE__
  return dirname(exe_path);
}
}
