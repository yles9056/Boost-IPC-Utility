#include <boost_ipc_util/util/logging.h>

void BoostIPCUtil::util::logging::configureCoreLogging(boost::log::trivial::severity_level level) {
    boost::log::add_common_attributes();
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    core->set_filter(boost::log::trivial::severity >= level);
}

void BoostIPCUtil::util::logging::initSimpleDebugLogging() {
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::debug_output_backend> debug_sink;

    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

    static boost::shared_ptr<debug_sink> sink = nullptr;
    if (sink) {
        core->remove_sink(sink);
        sink = nullptr;
    }

    sink = boost::make_shared<debug_sink>();
    initSimpleLogging(sink);
}

void BoostIPCUtil::util::logging::initSimpleTextLogging() {
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;

    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

    static boost::shared_ptr<text_sink> sink = nullptr;
    if (sink) {
        core->remove_sink(sink);
        sink = nullptr;
    }

    boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
    backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    backend->auto_flush(true);

    sink = boost::make_shared<text_sink>(backend);
    initSimpleLogging(sink);
}

boost::log::sources::severity_logger<boost::log::trivial::severity_level>& BoostIPCUtil::util::logging::getBIULogger() {
    static boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger;
    return logger;
}

