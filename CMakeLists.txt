cmake_minimum_required(VERSION 3.25)

project("openshoedesigner" DESCRIPTION "Open Source Shoe Design" VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

include(CTest)

add_subdirectory(src)

install(
	TARGETS openshoedesigner 
	EXPORT openshoedesigner
	RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}
)
	
