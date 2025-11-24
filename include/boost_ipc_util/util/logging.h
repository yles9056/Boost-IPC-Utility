#pragma once

#include <string>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/current_process_id.hpp>
#include <boost/log/attributes/current_thread_id.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>

#include <boost/date_time.hpp>

#include <boost_ipc_util/util/singleton.h>

#if defined(BIU_ENABLE_LOGGING)
#define BIU_LOG(lvl) BOOST_LOG_SEV(BoostIPCUtil::util::logging::getBIULogger(), boost::log::trivial::lvl)
#else
#define BIU_LOG(lvl) BoostIPCUtil::util::logging::NullLogger::getInstance()
#endif  // BIU_ENABLE_LOGGING

namespace BoostIPCUtil {
    namespace util {
        namespace logging {
#if !defined(BIU_ENABLE_LOGGING)
            class NullLogger {
                SINGLETON(NullLogger)

            protected:
                NullLogger() = default;
                ~NullLogger() = default;

            public:
                template <typename Val>
                NullLogger& operator<< (const Val&) { return *this; };
            };
#endif  // !BIU_ENABLE_LOGGING

            void configureCoreLogging(boost::log::trivial::severity_level level = boost::log::trivial::trace);

            template <typename T>
            void initSimpleLogging(boost::shared_ptr<boost::log::sinks::synchronous_sink<T>> sink) {
                boost::shared_ptr<boost::log::core> core = boost::log::core::get();
                sink->set_formatter(
                    boost::log::expressions::format("[%1%][%2%] [%3%][%4%]\t%5%\n")
                    % boost::log::expressions::attr<boost::log::attributes::current_process_id::value_type>("ProcessID")
                    % boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
                    % boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") // Need to include <boost/log/support/date_time.hpp>
                    % boost::log::trivial::severity
                    % boost::log::expressions::smessage
                );

                core->add_sink(sink);
            }

            void initSimpleDebugLogging();
            void initSimpleTextLogging();

            boost::log::sources::severity_logger<boost::log::trivial::severity_level>& getBIULogger();
        }  // namespace logging
    }  // namespace util
}  // namespace BoostIPCUtil