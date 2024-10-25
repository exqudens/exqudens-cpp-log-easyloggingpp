#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestUtils.hpp"
#include "TestConfig.hpp"
#include "TestLog.hpp"
#include <exqudens/log/api/Logging.hpp>

namespace exqudens::log::easyloggingpp {

    class UnitTests: public testing::Test {

        protected:

            inline static const char* LOGGER_ID = "exqudens.log.easyloggingpp.UnitTests";

    };

    TEST_F(UnitTests, test1) {
        try {
            std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
            std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
            PLOGI << "'" << testGroup << "." << testCase << "' start";

            if (std::filesystem::exists(std::filesystem::path(TestConfig::getExecutableDir()) / "log.txt")) {
                std::filesystem::remove(std::filesystem::path(TestConfig::getExecutableDir()) / "log.txt");
            }

            // command line bgn ----------------------------------------------------------------------------------------------------

            std::vector<std::string> arguments = {
                TestConfig::getExecutableFile(),

                "--" + exqudens::log::api::Logging::getKey(),

                (std::filesystem::path(__FILE__) / ".." / ".." / ".." / "resources" / testGroup / testCase / "command-line.txt").generic_string()
            };
            PLOGI << "arguments: " << TestUtils::join(arguments, ", ", "[", "]");

            bool expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            bool actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            std::string expectedConfiguredType = std::string("full-path: '") + arguments.at(2) + "'";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            std::string actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            expectedConfigured = true;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            expectedConfiguredType = "configured";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            exqudens::log::api::Logging::reset();

            expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            // command line end ----------------------------------------------------------------------------------------------------

            // executable dir bgn ----------------------------------------------------------------------------------------------------

            arguments = {
                (std::filesystem::path(__FILE__) / ".." / ".." / ".." / "resources" / testGroup / testCase / "test-app").generic_string(),

                "--" + exqudens::log::api::Logging::getKey(),

                "executable-dir.txt"
            };
            PLOGI << "arguments: " << TestUtils::join(arguments, ", ", "[", "]");

            expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            expectedConfiguredType = std::string("executable-dir: '");
            expectedConfiguredType += (std::filesystem::path(__FILE__) / ".." / ".." / ".." / "resources" / testGroup / testCase / arguments.at(2)).generic_string();
            expectedConfiguredType += "'";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            expectedConfigured = true;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            expectedConfiguredType = "configured";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            exqudens::log::api::Logging::reset();

            expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            // executable dir end ----------------------------------------------------------------------------------------------------

            // current dir bgn ----------------------------------------------------------------------------------------------------

            arguments = {
                (std::filesystem::path(__FILE__) / ".." / ".." / ".." / "resources" / testGroup / "test-app").generic_string(),

                "--" + exqudens::log::api::Logging::getKey(),

                "current-dir.txt"
            };
            PLOGI << "arguments: " << TestUtils::join(arguments, ", ", "[", "]");

            std::filesystem::copy(
                std::filesystem::path(__FILE__) / ".." / ".." / ".." / "resources" / testGroup / testCase / arguments.at(2),
                std::filesystem::path(TestConfig::getExecutableDir()),
                std::filesystem::copy_options::overwrite_existing
            );

            expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            expectedConfiguredType = std::string("current-dir: '");
            expectedConfiguredType += (std::filesystem::current_path() / arguments.at(2)).generic_string();
            expectedConfiguredType += "'";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            expectedConfigured = true;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            ASSERT_EQ(expectedConfigured, actualConfigured);

            expectedConfiguredType = "configured";
            PLOGI << "expectedConfiguredType: '" << expectedConfiguredType << "'";
            actualConfiguredType = exqudens::log::api::Logging::configure(arguments);
            PLOGI << "actualConfiguredType: '" << actualConfiguredType << "'";

            ASSERT_EQ(expectedConfiguredType, actualConfiguredType);

            exqudens::log::api::Logging::reset();

            expectedConfigured = false;
            PLOGI << "expectedConfigured: " << expectedConfigured;
            actualConfigured = exqudens::log::api::Logging::isConfigured();
            PLOGI << "actualConfigured: " << actualConfigured;

            // current dir end ----------------------------------------------------------------------------------------------------

            EXQUDENS_LOG_DEBUG("command.line.Test") << "aaa";

            std::string content = TestUtils::readFileString((std::filesystem::path(TestConfig::getExecutableDir()) / "log.txt").generic_string());
            content = TestUtils::trim(content);
            PLOGI << "content: '" << content << "'";

            ASSERT_TRUE(content.ends_with("DEBUG [command.line.Test] aaa"));

            PLOGI << "'" << testGroup << "." << testCase << "' end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            PLOGE << errorMessage;
            FAIL() << errorMessage;
        }
    }

}
