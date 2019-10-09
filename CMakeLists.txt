cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

include(FetchContent)

# Build Configurations
set(CMAKE_CONFIGURATION_TYPES
	"Debug"
	"RelWithDebInfo"
	CACHE STRING "" FORCE
)
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "")

project(MicroEngine)

string(REPLACE "ZI" "Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "Zi" "Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "ZI" "Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "Zi" "Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(PLATFORM "Win64")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PLATFORM "Linux")
else()
	message(FATAL_ERROR "Unsupported system [${CMAKE_SYSTEM_NAME}]!")
endif()

set(LIB_PATH "${CMAKE_SOURCE_DIR}/Lib")
set(INC_PATH "${CMAKE_SOURCE_DIR}/Include")
set(EXE_PATH "${CMAKE_SOURCE_DIR}/Exe")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIB_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXE_PATH}")

## Sources
set(SOURCES
	${GAME_SOURCES}
	Source/Main.cpp
)
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
include("Source/Source.cmake")
include("Source/Game/Asserts/Asserts.cmake")
include("Source/Game/Delegate/Delegate.cmake")
include("Source/Game/GameTypes/GameTypes.cmake")
include("Source/Game/Logger/Logger.cmake")

# SFML
FetchContent_Declare(
	sfml
	GIT_REPOSITORY	https://github.com/SFML/SFML.git
	GIT_TAG			2.5.1
)
FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
	message(STATUS "== Fetching SFML...")
	FetchContent_Populate(sfml)
	set(SFML_EXTLIBS_PATH "${sfml_SOURCE_DIR}/extlibs")
	set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
	if(PLATFORM STREQUAL "Win64")
		# Force SFML static libraries
		set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			file(GLOB MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/libs-mingw/x64/*.a")
		else()
			if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
				# extlibs path is not auto-detected for clang-cl
				list(APPEND CMAKE_LIBRARY_PATH "${SFML_EXTLIBS_PATH}/libs-msvc-universal/x64")
				# CRT flags are not set for clang-cl
				set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Z7")
				set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD /Z7")
			endif()
			file(GLOB MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/libs-msvc-universal/x64/*.lib")
		endif()
		list(APPEND MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/bin/x64/openal32.dll")
		file(COPY ${MSVC_EXT_LIBS} DESTINATION "${LIB_PATH}")
	elseif(PLATFORM STREQUAL "Linux")
		# Set SFML static libraries as default
		set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
	endif()
	list(APPEND CMAKE_LIBRARY_PATH "${FORCE_EXTLIBS_PATH}")
	file(COPY "${sfml_SOURCE_DIR}/include/SFML" DESTINATION "${INC_PATH}")
	add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PUBLIC SFML_STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC "${INC_PATH}")
target_link_directories(${PROJECT_NAME} PUBLIC "${LIB_PATH}")
target_link_libraries(${PROJECT_NAME} PUBLIC
	sfml-graphics-s$<$<CONFIG:Debug>:-d>
	sfml-window-s$<$<CONFIG:Debug>:-d>
	sfml-system-s$<$<CONFIG:Debug>:-d>
	
	freetype
	gdi32
	winmm
	opengl32
)
add_dependencies(${PROJECT_NAME}
	sfml-graphics
	sfml-window
	sfml-system
)
