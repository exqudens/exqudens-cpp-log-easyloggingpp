#include <optional>
#include <filesystem>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "TestLog.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

TestLog::Writer::Writer(
        std::string file,
        size_t line,
        std::string function,
        std::string id,
        unsigned short level
):
        file(std::move(file)),
        line(line),
        function(std::move(function)),
        id(std::move(id)),
        level(level)
{}

TestLog::Writer::~Writer() {
    TestLog::log(
            file,
            line,
            function,
            id,
            level,
            stream.str()
    );
}

void TestLog::log(
        const std::string& file,
        const size_t& line,
        const std::string& function,
        const std::string& id,
        const unsigned short& level,
        const std::string& message
) {
    try {
        std::string internalLevel = "NONE";
        std::string internalFile = std::filesystem::path(file).filename().string();
        if (level == 1) {
            internalLevel = "FATAL";
        } else if (level == 2) {
            internalLevel = "ERROR";
        } else if (level == 3) {
            internalLevel = "WARNING";
        } else if (level == 4) {
            internalLevel = "INFO";
        } else if (level == 5) {
            internalLevel = "DEBUG";
        } else if (level == 6) {
            internalLevel = "TRACE";
        }
        std::cout << internalFile << " " << id << " (" << internalFile << ":" << line << ") [" << function << "]: " << message << std::endl;
    } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO));
    }
}

#undef CALL_INFO
