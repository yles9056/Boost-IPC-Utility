#include "boost_ipc_util/ipc_utility.h"
#include "AsyncStateMachine.h"

#include <boost_ipc_util/boost/interprocess.h>
#include <boost_ipc_util/util/logging.h>

namespace boost_ipc_util {

    IPCUtility::IPCUtility() : m_initialized(false) {}

    IPCUtility::~IPCUtility() {
        cleanup();
    }

    std::string IPCUtility::getVersion() {
        return "1.0.0";
    }

    bool IPCUtility::initialize() {
        if (m_initialized) {
            return true;
        }

        BoostIPCUtil::util::logging::configureCoreLogging(boost::log::trivial::trace);
        BoostIPCUtil::util::logging::initSimpleDebugLogging();
        BIU_LOG(info) << "Boost IPC Utility version " << getVersion() << " initialized.";

        // Add initialization logic here
        m_initialized = true;

        return true;
    }

    void IPCUtility::cleanup() {
        if (!m_initialized) {
            return;
        }

        // Add cleanup logic here
        m_initialized = false;
    }

} // namespace boost_ipc_utility
