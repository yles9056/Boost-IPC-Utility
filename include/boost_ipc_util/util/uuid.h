#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// UUID utility functions
namespace BoostIPCUtil {
    namespace util {
        namespace uuid {
            /**
             * @brief Generate a new random UUID (Version 4)
             * @return A newly generated boost::uuids::uuid
             * @note Thread-safe. Uses a static random generator for efficiency.
             */
            boost::uuids::uuid generateUuid();

            /**
             * @brief Generate a new random UUID as a string
             * @return UUID string in format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
             * @note Thread-safe.
             */
            std::string generateUuidString();
        }  // namespace uuid
    }  // namespace util
}
