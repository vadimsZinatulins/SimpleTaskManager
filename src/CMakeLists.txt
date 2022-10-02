add_library(SimpleTaskManager TaskQueue.cpp Scheduler.cpp Holder.cpp)

target_include_directories(SimpleTaskManager 
	PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleTaskManager>
		$<INSTALL_INTERFACE:include/SimpleTaskManager>
	INTERFACE
		$<INSTALL_INTERFACE:include>
)

install(
	TARGETS SimpleTaskManager EXPORT SimpleTaskManagerTarget
	ARCHIVE	
)

install(
	EXPORT SimpleTaskManagerTarget
	DESTINATION lib/SimpleTaskManager/cmake
	NAMESPACE STM::	
)

install (
	DIRECTORY include/SimpleTaskManager
	DESTINATION include	
)
