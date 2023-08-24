# The SimpleTaskManager static library
add_library(SimpleTaskManager STATIC TaskQueue.cpp Scheduler.cpp Holder.cpp)

# Define include directories
target_include_directories(SimpleTaskManager 
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install library targets
install(
	TARGETS SimpleTaskManager EXPORT SimpleTaskManagerTarget
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}	
)

# Export targets
install(
	EXPORT SimpleTaskManagerTarget
	FILE SimpleTaskManagerTarget.cmake
	NAMESPACE STM::	
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SimpleTaskManager
)

# Install headers
install (
	DIRECTORY include/SimpleTaskManager
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}	
)
