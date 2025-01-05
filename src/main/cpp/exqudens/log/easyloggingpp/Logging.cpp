/*!
* @file Logging.cpp
*/

#include <cstddef>
#include <typeinfo>
#include <optional>
#include <stdexcept>
#include <filesystem>

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

    void Logging::write(
        const std::string& file,
        const size_t line,
        const std::string& function,
        const std::string& id,
        const unsigned short level,
        const std::string& message
    ) {
        try {
            std::map<unsigned short, std::string> levelName = levelNameMap();
            if (levelName.contains(level)) {
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
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::map<unsigned short, std::string> Logging::levelNameMap() {
        return {
            {1, "FATAL"},
            {2, "ERROR"},
            {3, "WARNING"},
            {4, "INFO"},
            {5, "DEBUG"},
            {6, "VERBOSE"}
        };
    }

    std::string Logging::configure(const std::any& input) {
        try {
            if (configured) {
                return "configured";
            }

            std::optional<std::filesystem::path> file = {};

            if (typeid(std::filesystem::path) == input.type()) {
                std::filesystem::path tmp = std::any_cast<std::filesystem::path>(input);
                file = tmp;
            }

            if (!file) {
                throw std::runtime_error(CALL_INFO + ": unsupported input type: '" + input.type().name() + "'");
            }

            if (!std::filesystem::exists(file.value())) {
                throw std::runtime_error(CALL_INFO + ": not exists: '" + file.value().generic_string() + "'!");
            }

            el::Loggers::configureFromGlobal(file.value().generic_string().c_str());
            el::Configurations* configurations = el::Loggers::getLogger("default")->configurations();
            el::Loggers::setDefaultConfigurations(*configurations, true);
            el::Loggers::configureFromGlobal(file.value().generic_string().c_str());

            configured = true;
            return file.value().generic_string();
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

    std::string Logging::commandLineKey() {
        return "--log-config-file";
    }

    std::string Logging::configureCommandLine(const std::vector<std::string>& arguments) {
        try {
            if (exqudens::log::api::Logging::isConfigured()) {
                return exqudens::log::api::Logging::configure({});
            }

            std::optional<size_t> indexOfKey = {};

            if (arguments.size() < 3) {
                throw std::runtime_error(CALL_INFO + ": size of arguments less than 3!");
            }

            for (size_t i = 0; i < arguments.size(); i++) {
                if (arguments.at(i) == commandLineKey()) {
                    indexOfKey = i;
                    break;
                }
            }

            if (!indexOfKey) {
                throw std::runtime_error(CALL_INFO + ": unable to find: '" + commandLineKey() + "' in arguments!");
            }

            if (arguments.size() < indexOfKey.value() + 2) {
                throw std::runtime_error(CALL_INFO + ": missing value for key: '" + commandLineKey() + "' arguments!");
            }

            std::string type = "not-found";
            std::string value = arguments.at(indexOfKey.value() + 1);
            std::optional<std::filesystem::path> file = {};
            std::filesystem::path executableDir = std::filesystem::path(arguments.at(0)).parent_path();
            std::filesystem::path currentDir = std::filesystem::current_path();

            // try full path
            if (!file && std::filesystem::path(value).is_absolute() && std::filesystem::exists(std::filesystem::path(value))) {
                file = std::filesystem::path(value);
                type = "full-path";
            }

            // try executable dir
            if (!file && !std::filesystem::path(value).is_absolute() && std::filesystem::exists(executableDir / std::filesystem::path(value))) {
                file = executableDir / std::filesystem::path(value);
                type = "executable-dir";
            }

            // try current dir
            if (!file && !std::filesystem::path(value).is_absolute() && std::filesystem::exists(currentDir / std::filesystem::path(value))) {
                file = currentDir / std::filesystem::path(value);
                type = "current-dir";
            }

            if (!file) {
                throw std::runtime_error(CALL_INFO + ": unable to find: '" + value + "'!");
            }

            std::any input = file.value();
            std::string configureByAnyResult = exqudens::log::api::Logging::configure(input);

            return type + ": '" + configureByAnyResult + "'";
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

}

#undef CALL_INFO
