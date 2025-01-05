#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "exqudens/log/easyloggingpp/Logging.hpp"
#include "TestUtils.hpp"
#include "TestConfig.hpp"
#include "TestLog.hpp"

namespace exqudens::log::easyloggingpp {

    class UnitTests: public testing::Test {

        protected:

            inline static const char* LOGGER_ID = "exqudens.log.easyloggingpp.UnitTests";

            std::string currentWorkingDir = "";
            std::string currentProjectDir = "";
            std::string currentProjectBinaryDir = "";
            std::string currentTestGroup = "";
            std::string currentTestCase = "";
            std::string currentTestResourcesDir = "";
            std::string currentTestOutputDir = "";

            void SetUp() override {
                currentWorkingDir = std::filesystem::current_path().generic_string();
                ASSERT_FALSE(currentWorkingDir.empty());

                currentProjectDir = std::filesystem::weakly_canonical(std::filesystem::path(__FILE__) / ".." / ".." / ".." / ".." / "..").generic_string();
                ASSERT_FALSE(currentProjectDir.empty());

                currentProjectBinaryDir = std::filesystem::weakly_canonical(std::filesystem::path(TestUtils::getEnvVar("PROJECT_BINARY_DIR").value())).generic_string();
                ASSERT_FALSE(currentProjectBinaryDir.empty());

                currentTestGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
                ASSERT_FALSE(currentTestGroup.empty());

                currentTestCase = testing::UnitTest::GetInstance()->current_test_info()->name();
                ASSERT_FALSE(currentTestCase.empty());

                currentTestResourcesDir = std::filesystem::weakly_canonical(std::filesystem::path(currentProjectDir) / "src" / "test" / "resources" / currentTestGroup / currentTestCase).generic_string();
                ASSERT_FALSE(currentTestResourcesDir.empty());

                currentTestOutputDir = std::filesystem::weakly_canonical(std::filesystem::path(currentProjectBinaryDir) / "test" / "output" / currentTestGroup / currentTestCase).generic_string();
                ASSERT_FALSE(currentTestOutputDir.empty());

                std::filesystem::remove_all(currentTestOutputDir);
                std::filesystem::create_directories(currentTestOutputDir);
                std::filesystem::copy(currentTestResourcesDir, currentTestOutputDir, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
                std::filesystem::current_path(std::filesystem::path(currentTestOutputDir));
            }

            void TearDown() override {
                std::filesystem::current_path(std::filesystem::path(currentWorkingDir));

                currentWorkingDir = "";
                ASSERT_TRUE(currentWorkingDir.empty());

                currentProjectDir = "";
                ASSERT_TRUE(currentProjectDir.empty());

                currentProjectBinaryDir = "";
                ASSERT_TRUE(currentProjectBinaryDir.empty());

                currentTestGroup = "";
                ASSERT_TRUE(currentTestGroup.empty());

                currentTestCase = "";
                ASSERT_TRUE(currentTestCase.empty());

                currentTestResourcesDir = "";
                ASSERT_TRUE(currentTestResourcesDir.empty());

                currentTestOutputDir = "";
                ASSERT_TRUE(currentTestOutputDir.empty());
            }

    };

    TEST_F(UnitTests, test1) {
        try {
            TEST_LOG_I(LOGGER_ID) << "bgn";

            TEST_LOG_I(LOGGER_ID) << "cwd: '" << std::filesystem::current_path().generic_string() << "'";

            exqudens::log::api::Logging::reset();
            std::string type = exqudens::log::api::Logging::configureCommandLine({
                (std::filesystem::current_path() / "app").generic_string(),
                exqudens::log::api::Logging::commandLineKey(),
                (std::filesystem::current_path() / "logging-config.txt").generic_string()
            });
            TEST_LOG_I(LOGGER_ID) << "type: '" << type << "'";

            ASSERT_EQ(std::string("full-path: '") + (std::filesystem::current_path() / "logging-config.txt").generic_string() + "'", type);

            TEST_LOG_I(LOGGER_ID) << "end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            TEST_LOG_E(LOGGER_ID) << errorMessage;
            FAIL() << errorMessage;
        }
    }

    TEST_F(UnitTests, test2) {
        try {
            TEST_LOG_I(LOGGER_ID) << "bgn";

            TEST_LOG_I(LOGGER_ID) << "cwd: '" << std::filesystem::current_path().generic_string() << "'";

            exqudens::log::api::Logging::reset();
            std::string type = exqudens::log::api::Logging::configureCommandLine({
                (std::filesystem::current_path() / "dir" / "app").generic_string(),
                exqudens::log::api::Logging::commandLineKey(),
                "logging-config.txt"
            });
            TEST_LOG_I(LOGGER_ID) << "type: '" << type << "'";

            ASSERT_EQ(std::string("executable-dir: '") + (std::filesystem::current_path() / "dir" / "logging-config.txt").generic_string() + "'", type);

            TEST_LOG_I(LOGGER_ID) << "end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            TEST_LOG_E(LOGGER_ID) << errorMessage;
            FAIL() << errorMessage;
        }
    }

    TEST_F(UnitTests, test3) {
        try {
            TEST_LOG_I(LOGGER_ID) << "bgn";

            TEST_LOG_I(LOGGER_ID) << "cwd: '" << std::filesystem::current_path().generic_string() << "'";

            exqudens::log::api::Logging::reset();
            std::string type = exqudens::log::api::Logging::configureCommandLine({
                (std::filesystem::current_path() / "dir" / "app").generic_string(),
                exqudens::log::api::Logging::commandLineKey(),
                "logging-config.txt"
            });
            TEST_LOG_I(LOGGER_ID) << "type: '" << type << "'";

            ASSERT_EQ(std::string("current-dir: '") + (std::filesystem::current_path() / "logging-config.txt").generic_string() + "'", type);

            TEST_LOG_I(LOGGER_ID) << "end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            TEST_LOG_E(LOGGER_ID) << errorMessage;
            FAIL() << errorMessage;
        }
    }

}
