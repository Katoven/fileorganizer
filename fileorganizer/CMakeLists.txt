project(fileorganizer)

add_executable(${PROJECT_NAME} WIN32
	fileorganizer.cpp
	filemanager.cpp filemanager.h
	logger.h
	stringhelperfunctions.h
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
