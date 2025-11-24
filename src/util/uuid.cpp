#include <boost_ipc_util/util/uuid.h>

// UUID utility implementations

boost::uuids::uuid BoostIPCUtil::util::uuid::generateUuid() {
    static boost::uuids::random_generator generator;
    return generator();
}

std::string BoostIPCUtil::util::uuid::generateUuidString() {
    return boost::uuids::to_string(generateUuid());
}
