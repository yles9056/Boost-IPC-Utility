#pragma once

/*
 * BOOST_INTERPROCESS_SHARED_DIR_FUNC
 * BOOST_INTERPROCESS_SHARED_DIR_IN_USER_APPDATA
 * BOOST_INTERPROCESS_SHARED_DIR_NAME
 */

 /*
  * Missing template argument list after template keyword in interprocess/segment_manager.hpp
  * https://github.com/boostorg/interprocess/issues/256
  */

  /*
   * Custom Shared Directory Provider Usage:
   *
   * To set a custom directory provider for std::string:
   *   boost::interprocess::ipcdetail::set_shared_dir_provider<char>([]() -> std::string {
   *       return "C:\\my_custom_path";
   *   });
   *
   * To set a custom directory provider for std::wstring:
   *   boost::interprocess::ipcdetail::set_shared_dir_provider<wchar_t>([]() -> std::wstring {
   *  return L"C:\\my_custom_path";
   *   });
   *
   * To reset to default behavior (no custom provider):
   *   boost::interprocess::ipcdetail::reset_shared_dir_provider<char>();
   *   boost::interprocess::ipcdetail::reset_shared_dir_provider<wchar_t>();
   *
   * Example:
   *   #include <boost_ipc_utility/boost/interprocess.h>
   *
   *   // Set custom provider
   *   boost::interprocess::ipcdetail::set_shared_dir_provider<char>([]() {
   *       return std::string("/tmp/my_shared_memory");
   *   });
   *
   *   // Now get_shared_dir will use the custom provider
   *   std::string dir;
   *   boost::interprocess::ipcdetail::get_shared_dir(dir);
   */

#ifdef BOOST_INTERPROCESS_WINDOWS
#include <boost/interprocess/detail/win32_api.hpp>
#endif  // BOOST_INTERPROCESS_WINDOWS

#include <boost/interprocess/detail/shared_dir_helpers.hpp>
#include <string>
#include <functional>
#include <mutex>
#include <boost_ipc_util/util/singleton.h>

   // The code below is modified from Boost source code

namespace boost {
    namespace interprocess {
#ifdef BOOST_INTERPROCESS_WINDOWS
        namespace winapi {
            static const hkey hkey_current_user = (hkey)(unsigned long*)(long)(0x80000001);

            template <class CharT>
            inline void get_user_appdata_folder(std::basic_string<CharT>& s) {
                get_registry_value_string(hkey_current_user, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", "AppData", s);
            }

            inline void get_user_appdata_folder(std::wstring& s) {
                get_registry_value_string(hkey_current_user, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", L"AppData", s);
            }
        }  // namespace winapi
#endif  // BOOST_INTERPROCESS_WINDOWS

        namespace ipcdetail {
#ifdef BOOST_INTERPROCESS_SHARED_DIR_FUNC
            // Shared directory provider singleton
            template <typename CharT>
            class shared_dir_provider {
                SINGLETON(shared_dir_provider)
            public:
                using string_type = std::basic_string<CharT>;
                using provider_func = std::function<string_type()>;

                // Set custom directory provider function
                void set_provider(provider_func func) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_provider = std::move(func);
                }

                // Get directory using custom provider if set
                bool get_directory(string_type& dir) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    if (m_provider) {
                        dir = m_provider();
                        return true;
                    }
                    
                    // Fallback to default behavior
                    get_shared_dir_root(dir);
#if defined(BOOST_INTERPROCESS_HAS_KERNEL_BOOTTIME)
                    dir += shared_dir_constants<CharT>::dir_separator();
                    get_bootstamp(dir, true);
#endif

                    return false;
                }

                // Reset provider to default
                void reset_provider() {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_provider = nullptr;
                }

            private:
                provider_func m_provider = nullptr;
                std::mutex m_mutex;

                shared_dir_provider() = default;
                ~shared_dir_provider() = default;
            };

            inline void get_shared_dir(std::string& shared_dir) {
                shared_dir_provider<char>::getInstance().get_directory(shared_dir);
            }

            inline void get_shared_dir(std::wstring& shared_dir) {
                shared_dir_provider<wchar_t>::getInstance().get_directory(shared_dir);
            }

            // Public API to set custom shared directory provider
            template <typename CharT>
            inline void set_shared_dir_provider(std::function<std::basic_string<CharT>()> func) {
                shared_dir_provider<CharT>::getInstance().set_provider(std::move(func));
            }

            // Public API to reset shared directory provider
            template <typename CharT>
            inline void reset_shared_dir_provider() {
                shared_dir_provider<CharT>::getInstance().reset_provider();
            }
#endif  // BOOST_INTERPROCESS_SHARED_DIR_FUNC
        }  // namespace ipcdetail
    }  // namespace interprocess
}  // namespace boost
