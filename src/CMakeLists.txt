add_library(TaskManager main.cpp TaskQueue.cpp Scheduler.cpp Holder.cpp)

target_include_directories(TaskManager 
	PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/TaskManager>
		$<INSTALL_INTERFACE:include/TaskManager>
	INTERFACE
		$<INSTALL_INTERFACE:include>
)

install(
	TARGETS TaskManager EXPORT TaskManagerTarget
	ARCHIVE	
)

install(
	EXPORT TaskManagerTarget
	DESTINATION lib/TaskManager/cmake
	NAMESPACE TM::	
)

install (
	DIRECTORY include/TaskManager
	DESTINATION include	
)
