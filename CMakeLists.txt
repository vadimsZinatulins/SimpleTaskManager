cmake_minimum_required(VERSION 3.20)

project(SimpleTaskManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_subdirectory(src)

include(CMakePackageConfigHelpers)

set(LIB_CMAKE_DIR SimpleTaskManager/cmake)
set(CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/build/SimpleTaskManagerConfig.cmake")

configure_package_config_file(
	SimpleTaskManagerConfig.cmake.in ${CONFIG_FILE}
	INSTALL_DESTINATION lib/SimpleTaskManager
	PATH_VARS LIB_CMAKE_DIR	
)

install(
	FILES ${CONFIG_FILE}
	DESTINATION lib/${LIB_CMAKE_DIR}
)
