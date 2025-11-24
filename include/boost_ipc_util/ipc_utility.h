#ifndef BOOST_IPC_UTILITY_H
#define BOOST_IPC_UTILITY_H

#include <string>

namespace boost_ipc_util {

    /**
     * @brief A utility class for IPC operations
     */
    class IPCUtility {
    public:
        /**
         * @brief Constructor
         */
        IPCUtility();

        /**
         * @brief Destructor
         */
        ~IPCUtility();

        /**
         * @brief Get the library version
         * @return Version string
         */
        static std::string getVersion();

        /**
         * @brief Initialize the utility
         * @return true if successful, false otherwise
         */
        bool initialize();

        /**
         * @brief Cleanup resources
         */
        void cleanup();

    private:
        bool m_initialized;
    };

} // namespace boost_ipc_utility

#endif // BOOST_IPC_UTILITY_H
