/*!
* @file Logging.cpp
*/

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <filesystem>

#include <exqudens/log/api/Logging.hpp>

#define ELPP_NO_DEFAULT_LOG_FILE
//#define ELPP_THREAD_SAFE
#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "exqudens/log/easyloggingpp/Logging.hpp"
#include "exqudens/log/easyloggingpp/versions.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

namespace exqudens::log::easyloggingpp {

    std::string Logging::getVersion() {
        return std::to_string(PROJECT_VERSION_MAJOR) + "." + std::to_string(PROJECT_VERSION_MINOR) + "." + std::to_string(PROJECT_VERSION_PATCH);
    }

}

namespace exqudens::log::api {

    Logging::Writer::Writer(
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
        level(level) {
    }

    Logging::Writer::~Writer() {
        Logging::write(
            file,
            line,
            function,
            id,
            level,
            stream.str()
        );
    }

    std::string Logging::getKey() {
        return "logging-config";
    }

    std::string Logging::configure(const std::vector<std::string>& arguments) {
        try {
            if (configured) {
                return "configured";
            }

            std::optional<size_t> indexOfKey = {};

            if (arguments.size() < 3) {
                throw std::runtime_error(CALL_INFO + ": size of arguments less than 3!");
            }

            for (size_t i = 0; i < arguments.size(); i++) {
                if (arguments.at(i) == "--" + getKey()) {
                    indexOfKey = i;
                    break;
                }
            }

            if (!indexOfKey) {
                throw std::runtime_error(CALL_INFO + ": unable to find: '" + std::string("--") + getKey() + "' in arguments!");
            }

            if (arguments.size() < indexOfKey.value() + 2) {
                throw std::runtime_error(CALL_INFO + ": missing value for key: '" + std::string("--") + getKey() + "' arguments!");
            }

            std::string type = "not-found";
            std::string value = arguments.at(indexOfKey.value() + 1);
            std::optional<std::filesystem::path> file = {};
            std::filesystem::path executableDir = std::filesystem::path(arguments.at(0)).parent_path();
            std::filesystem::path currentDir = std::filesystem::current_path();

            // try full path
            if (!file && std::filesystem::path(value).filename().string() != value && std::filesystem::exists(std::filesystem::path(value))) {
                file = std::filesystem::path(value);
                type = "full-path";
            }

            // try executable dir
            if (!file && std::filesystem::exists(executableDir / value)) {
                file = executableDir / value;
                type = "executable-dir";
            }

            // try current dir
            if (!file && std::filesystem::exists(currentDir / value)) {
                file = currentDir / value;
                type = "current-dir";
            }

            if (!file) {
                throw std::runtime_error(CALL_INFO + ": unable to find: '" + value + "'!");
            }

            el::Loggers::configureFromGlobal(file.value().generic_string().c_str());
            el::Configurations* configurations = el::Loggers::getLogger("default")->configurations();
            el::Loggers::setDefaultConfigurations(*configurations, true);
            el::Loggers::configureFromGlobal(file.value().generic_string().c_str());

            configured = true;
            return type + ": '" + file.value().generic_string() + "'";
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    bool Logging::isConfigured() {
        return configured;
    }

    void Logging::reset() {
        try {
            //el::base::type::StoragePointer storage(new el::base::Storage(el::LogBuilderPtr(new el::base::DefaultLogBuilder())));
            //el::Helpers::setStorage(storage);
            configured = false;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Logging::write(
        const std::string& file,
        const size_t line,
        const std::string& function,
        const std::string& id,
        const unsigned short level,
        const std::string& message
    ) {
        try {
            el::Level internalLevel = el::Level::Unknown;
            std::string internalFile = std::filesystem::path(file).filename().string();
            if (level == 1) {
                internalLevel = el::Level::Fatal;
            } else if (level == 2) {
                internalLevel = el::Level::Error;
            } else if (level == 3) {
                internalLevel = el::Level::Warning;
            } else if (level == 4) {
                internalLevel = el::Level::Info;
            } else if (level == 5) {
                internalLevel = el::Level::Debug;
            } else if (level == 6) {
                internalLevel = el::Level::Verbose;
            }
            el::base::Writer(internalLevel, internalFile.c_str(), line, function.c_str(), el::base::DispatchAction::NormalLog).construct(1, id.c_str()) << message;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

}

#undef CALL_INFO
