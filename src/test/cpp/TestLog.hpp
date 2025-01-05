#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <sstream>

#ifndef TEST_LOG_F
#define TEST_LOG_F(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::FATAL)
#endif

#ifndef TEST_LOG_E
#define TEST_LOG_E(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::ERROR)
#endif

#ifndef TEST_LOG_W
#define TEST_LOG_W(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::WARNING)
#endif

#ifndef TEST_LOG_I
#define TEST_LOG_I(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::INFO)
#endif

#ifndef TEST_LOG_D
#define TEST_LOG_D(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::DEBUG)
#endif

#ifndef TEST_LOG_T
#define TEST_LOG_T(id) TestLog::Writer(__FILE__, __LINE__, __FUNCTION__, id, (unsigned short) TestLog::Level::TRACE)
#endif

class TestLog {

    public:

        enum class Level : unsigned short {
            FATAL = 1,
            ERROR = 2,
            WARNING = 3,
            INFO = 4,
            DEBUG = 5,
            TRACE = 6
        };

        class Writer {

            private:

                std::string file;
                size_t line = 0;
                std::string function;
                std::string id;
                unsigned short level = 0;
                std::ostringstream stream;

            public:

                Writer(
                    std::string file,
                    size_t line,
                    std::string function,
                    std::string id,
                    unsigned short level
                );

                Writer() = delete;
                Writer(const Writer&) = delete;
                Writer& operator=(const Writer&) = delete;

                template <typename T>
                inline Writer& operator<<(const T& value) {
                    stream << value;
                    return *this;
                }

                ~Writer();

        };

    private:

        static void log(
                const std::string& file,
                const size_t& line,
                const std::string& function,
                const std::string& id,
                const unsigned short& level,
                const std::string& message
        );

};
