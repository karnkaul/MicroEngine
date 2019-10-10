cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

include(FetchContent)

## Build Configurations
set(CMAKE_CONFIGURATION_TYPES
	Debug
	RelWithDebInfo
	Release
	CACHE STRING "" FORCE
)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")

project(MicroEngine)
## Enforce x64
set(CMAKE_VS_PLATFORM_NAME "x64" CACHE STRING "" FORCE)
if(NOT CMAKE_VS_PLATFORM_NAME STREQUAL "x64")
	message(FATAL_ERROR "Only x64 builds are supported!")
endif()

## Globals
set(TP_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty")
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Build/Lib")
set(INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Include")
set(EXE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIB_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXE_PATH}")

## Flags
set(W_MSBUILD 0)
set(W_CLANG 0)
set(W_VCXX 0)
set(W_GCC 0)
set(LX_CLANG 0)
set(LX_GCC 0)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(PLATFORM "Win64")
	if(CMAKE_GENERATOR MATCHES "^(Visual Studio)")
		set(W_MSBUILD 1)
	endif()
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(W_CLANG 1)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(W_VCXX 1)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(W_GCC 1)
	else()
		message("\tWARNING: Unsupported compiler [${CMAKE_CXX_COMPILER_ID}], expect build warnings/errors!")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PLATFORM "Linux")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(LX_CLANG 1)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(LX_GCC 1)
	else()
		message("\tWARNING: Unsupported compiler [${CMAKE_CXX_COMPILER_ID}], expect build warnings/errors!")
	endif()
else()
	message(FATAL_ERROR "Unsupported system [${CMAKE_SYSTEM_NAME}]!")
endif()
message(STATUS "Environment: [${PLATFORM}] [${CMAKE_GENERATOR}] [${CMAKE_CXX_COMPILER_ID}]")
include(.CMake/Platform.cmake)

## SFML
set(SFML_VERSION 2.5.1)
set(SFML_SOURCE_DIR "${TP_SRC_PATH}/SFML")
download_extract_archive(
	"SFML-${SFML_VERSION}-sources.zip"
	"https://www.sfml-dev.org/files/SFML-${SFML_VERSION}-sources.zip"
	"${TP_SRC_PATH}"
	"SFML-${SFML_VERSION}"
	"SFML"
)
if(NOT EXISTS "${SFML_SOURCE_DIR}")
	message(FATAL_ERROR "Unknown error downloading SFML!")
endif()

pre_target()
file(COPY "${SFML_SOURCE_DIR}/include/SFML" DESTINATION "${INC_PATH}")
add_subdirectory(${SFML_SOURCE_DIR})

## Sources
set(SOURCES Source/Main.cpp)
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
include("Source/Source.cmake")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PUBLIC SFML_STATIC $<$<CONFIG:Debug>:DEBUGGING>)
target_include_directories(${PROJECT_NAME} PUBLIC "${INC_PATH}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game")
target_link_directories(${PROJECT_NAME} PUBLIC "${LIB_PATH}")
target_link_libraries(${PROJECT_NAME} PUBLIC
	sfml-graphics-s$<$<CONFIG:Debug>:-d>
	sfml-window-s$<$<CONFIG:Debug>:-d>
	sfml-system-s$<$<CONFIG:Debug>:-d>
	${SFML_DEPS}
)
add_dependencies(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
)
set(EXE_NAME Untitled$<$<NOT:$<CONFIG:Release>>:-$<CONFIG>>${EXE_SUFFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXE_NAME})
post_target()
