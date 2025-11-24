#include <boost_ipc_util/util/misc.h>

boost::posix_time::ptime BoostIPCUtil::util::time::getUniversalTime() {
    return boost::posix_time::microsec_clock::universal_time();
}

int64_t BoostIPCUtil::util::time::getTimestamp() {
    static const boost::posix_time::ptime epoch = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
    return (getUniversalTime() - epoch).total_milliseconds();
}

boost::process::pid_type BoostIPCUtil::util::system::getPid() {
    return boost::process::current_pid();
}
