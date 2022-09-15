#include <Asset.hpp>
#include <filesystem>
#include <iostream>

using namespace asset;
namespace fs = std::filesystem;

const std::string asset::getExecutablePath() {

    // buffer the path data
    uint32_t path_length = PATH_MAX;
    std::string path_buf(path_length, std::string::value_type());

    // construct a std::filesystem::path to get the parent
    fs::path executable_path;

    #if defined(__APPLE__) || defined(__MACH__)
    if (_NSGetExecutablePath(&path_buf[0], &path_length) == 0) {
        executable_path = path_buf;
    }
    #elif defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
    if (GetModuleFileName(NULL, &path_buf[0], path_length) > 0) {
        executable_path = path_buf;
    }
    #elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", &path_buf[0], path_length);
    if (count > 0) {
        executable_path = path_buf;
    }
    #else
    throw new std::runtime_error{"Invalid operating system"};
    #endif

    if (!executable_path.empty()) {
        return std::string(executable_path.parent_path());
    } else {
        throw new std::runtime_error{"Could not determine current executable path"};
    }
}

const std::string Asset::RESOURCE_DIR = getExecutablePath() +  "/resources";
const std::string Asset::SHADERS_DIR = Asset::RESOURCE_DIR + "/shaders";
const std::string Asset::MODELS_DIR = Asset::RESOURCE_DIR + "/models";
const std::string Asset::CONFIG_PATH = Asset::RESOURCE_DIR + "/config/config.json";
