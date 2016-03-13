#ifndef HELPER_H
#define HELPER_H

#include <string>
#if __APPLE__
# include <mach-o/dyld.h>
#endif // if __APPLE__

namespace Helper {
#ifndef WIN32
template<typename _CountofType, size_t _SizeOfArray>
char(*__countof_helper(_CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];
# define _countof(_Array) (sizeof(*__countof_helper(_Array)) + 0)

# define sprintf_s snprintf
# define _vsnprintf_s vsnprintf
#endif // ifndef WIN32


std::string getExePath();
std::string getFullPath(std::string,
                        std::string);
}
#endif // ifndef HELPER_H
