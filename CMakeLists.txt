cmake_minimum_required(VERSION 3.20)

project(SimpleTaskManager VERSION 1.0.0 LANGUAGE CXX)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Define install directories in a platform-agnostic way
include(GNUInstallDirs)

# Add SimpleTaskManager Library
add_subdirectory(src)

# Package config
include(CMakePackageConfigHelpers)

set(LIB_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/SimpleTaskManager")
set(CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/SimpleTaskManagerConfig.cmake")

configure_package_config_file(
	SimpleTaskManagerConfig.cmake.in ${CONFIG_FILE}
	INSTALL_DESTINATION ${LIB_CMAKE_DIR}
	PATH_VARS LIB_CMAKE_DIR	
)

install(
	FILES ${CONFIG_FILE}
	DESTINATION ${LIB_CMAKE_DIR}
)
