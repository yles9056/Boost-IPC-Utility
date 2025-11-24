#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/process/pid.hpp>

namespace BoostIPCUtil {
    namespace util {
        namespace time {
            /**
             * @brief Get current UTC time
             * @return Current time as boost::posix_time::ptime
             */
            boost::posix_time::ptime getUniversalTime();

            /**
             * @brief Get Unix timestamp in milliseconds
             * @return Milliseconds since Unix epoch (1970-01-01 00:00:00 UTC)
             */
            int64_t getTimestamp();
        }  // namespace time

        namespace system {
            boost::process::pid_type getPid();
        }  // namespace system
    }  // namespace util
}  // namespace BoostIPCUtil