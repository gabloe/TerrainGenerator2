#pragma once

#include <string>
#include <iostream>
#include <limits.h>

#if defined(__APPLE__) || defined(__MACH__)
#include <mach-o/dyld.h>
#elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h> 
#endif

namespace asset {

const std::string getExecutablePath();

class Asset {
    public:
        static const std::string RESOURCE_DIR;
        static const std::string SHADERS_DIR;
        static const std::string MODELS_DIR;
        static const std::string CONFIG_PATH;
};
} // namespace asset