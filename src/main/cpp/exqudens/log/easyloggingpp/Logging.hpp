/*!
* @file Logging.hpp
*/

#pragma once

#include <string>

#include "exqudens/log/easyloggingpp/export.hpp"

namespace exqudens::log::easyloggingpp {

    class EXQUDENS_LOG_EASYLOGGINGPP_EXPORT Logging {

        public:

            static std::string getVersion();

    };

}
