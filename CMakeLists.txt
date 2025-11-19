cmake_minimum_required(VERSION 3.15)

project(BoostIPCUtility
    VERSION 1.0.0
    DESCRIPTION "A C++ static library for Boost IPC utilities"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Boost
set(BOOST_MIN_VERSION "1.70.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system)

if(Boost_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
endif()

# Windows version definition (user configurable)
if(WIN32)
    set(_WIN32_WINNT "0x0A00" CACHE STRING "Windows version definition (e.g., 0x0A00 = Windows 10/11, leave empty to not define)")
    option(USE_WIN32_LEAN_AND_MEAN "Define WIN32_LEAN_AND_MEAN to exclude rarely-used Windows headers" ON)
endif()

# Add AsyncStateMachine dependencies
# Set DelegateMQ build options for AsyncStateMachine
set(DMQ_ALLOCATOR "OFF" CACHE BOOL "Enable DelegateMQ custom allocator")
set(DMQ_UTIL "ON" CACHE BOOL "Enable DelegateMQ utilities")
set(DMQ_THREAD "DMQ_THREAD_STDLIB" CACHE STRING "DelegateMQ threading library")
set(DMQ_SERIALIZE "DMQ_SERIALIZE_NONE" CACHE STRING "DelegateMQ serialization library")
set(DMQ_TRANSPORT "DMQ_TRANSPORT_NONE" CACHE STRING "DelegateMQ transport library")

# Include DelegateMQ from AsyncStateMachine
include(${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/DelegateMQ/DelegateMQ.cmake)

# Add AsyncStateMachine libraries
add_subdirectory(external/AsyncStateMachine/Port)
add_subdirectory(external/AsyncStateMachine/StateMachine)

# StateMachineLib needs Port and DelegateMQ headers
target_include_directories(StateMachineLib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/Port
    ${DMQ_INCLUDE_DIR}
)

# Fix library include directories to avoid export errors
set_target_properties(PortLib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/Port>"
)
set_target_properties(StateMachineLib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/StateMachine>"
)

# Library source files
set(LIBRARY_SOURCES
    src/ipc_utility.cpp
    ${DMQ_PREDEF_SOURCES}
)

# Library header files
set(LIBRARY_HEADERS
    include/boost_ipc_utility/ipc_utility.h
)

# Organize source files into Visual Studio filters
source_group("Source Files" FILES ${LIBRARY_SOURCES})
source_group("Header Files" FILES ${LIBRARY_HEADERS})
source_group("DelegateMQ" FILES ${DMQ_PREDEF_SOURCES})

# Create static library
add_library(${PROJECT_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

# Specify include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${Boost_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${DMQ_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/Port>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/AsyncStateMachine/StateMachine>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Boost libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Boost::system
        PortLib
        StateMachineLib
)

# Platform-specific compiler flags and definitions
if(WIN32)
    # Windows version definition (optional)
    if(_WIN32_WINNT)
        target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32_WINNT=${_WIN32_WINNT})
    endif()
    
    # WIN32_LEAN_AND_MEAN (optional)
    if(USE_WIN32_LEAN_AND_MEAN)
        target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32_LEAN_AND_MEAN)
    endif()
endif()

if(MSVC)
    # Windows-specific flags
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # Linux and macOS flags
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} PortLib StateMachineLib
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Generate and install package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
