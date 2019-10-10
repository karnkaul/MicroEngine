function(pre_target)
	## Don't need network / audio
	set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
	set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
	if(PLATFORM STREQUAL "Win64")
		string(REPLACE "ZI" "Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "Zi" "Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "ZI" "Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
		string(REPLACE "Zi" "Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
		string(REPLACE "ZI" "Z7" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "Zi" "Z7" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		# Force SFML static libraries
		set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
		# Copy compiler's extlibs to LIB_PATH
		set(SFML_EXTLIBS_PATH "${SFML_SOURCE_DIR}/extlibs")
		if(W_GCC)
			# MinGW
			file(GLOB MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/libs-mingw/x64/*.a")
		else()
			if(W_CLANG)
				# extlibs path is not auto-detected for clang-cl
				list(APPEND CMAKE_LIBRARY_PATH "${SFML_EXTLIBS_PATH}/libs-msvc-universal/x64")
				# CRT flags are not set for clang-cl
				set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
				set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
				set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
			endif()
			file(GLOB MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/libs-msvc-universal/x64/*.lib")
		endif()
		list(APPEND MSVC_EXT_LIBS "${SFML_EXTLIBS_PATH}/bin/x64/openal32.dll")
		file(COPY ${MSVC_EXT_LIBS} DESTINATION "${LIB_PATH}")
		# Dependencies
		set(SFML_DEPS
			freetype
			gdi32
			winmm
			opengl32
			PARENT_SCOPE
		)
	elseif(PLATFORM STREQUAL "Linux")
		# Set RPATH
		set(CMAKE_INSTALL_RPATH_USE_ORIGIN ON)
		set(CMAKE_INSTALL_RPATH "$ORIGIN/Lib")
		SET(CMAKE_SKIP_BUILD_RPATH FALSE)
		SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
		SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
		# Set SFML static libraries as default
		set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
		# Dependencies
		set(SFML_DEPS
			m
			pthread
			freetype
			FLAC
			GL
			udev
			X11
			Xrandr
			PARENT_SCOPE
		)
	endif()
endfunction()

function(post_target)
	if(W_CLANG OR LX_CLANG)
		#set(COMPILE_FLAGS "-Werror=return-type -Wextra -Wconversion -Wunreachable-code -Wdeprecated-declarations -Wtype-limits")
		target_compile_options(${PROJECT_NAME} PRIVATE
			-Werror=return-type
			-Wextra
			-Wconversion
			-Wunreachable-code
			-Wdeprecated-declarations
			-Wtype-limits
			$<${LX_CLANG}:-fexceptions>
		)
	endif()
	if(PLATFORM STREQUAL "Linux" OR W_GCC)
		target_compile_options(${PROJECT_NAME} PRIVATE
			$<$<CONFIG:Debug>:
				-O0
			>
			$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Release>>:
				-O2
			>
			-g
			-Wextra
			-Werror=return-type
			-fexceptions
			$<${LX_GCC}:-utf-8>
			$<${W_GCC}:-utf-8>
		)
		if(PLATFORM STREQUAL "Linux")
			target_link_options(${PROJECT_NAME} PRIVATE
				-no-pie         # Build as application
				-Wl,-z,origin   # Allow $ORIGIN in RUNPATH
			)
		endif()
	endif()
	if(PLATFORM STREQUAL "Win64")
		if(W_MSBUILD)
			set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
			target_compile_options(${PROJECT_NAME} PRIVATE /MP)
		endif()
		if(NOT W_GCC)
			target_link_options(${PROJECT_NAME} PRIVATE
				$<$<CONFIG:Debug>:
					/SUBSYSTEM:CONSOLE
					/OPT:NOREF
				>
				$<$<OR:$<CONFIG:RelWithDebInfo>,$<CONFIG:Release>>:
					/ENTRY:mainCRTStartup
					/SUBSYSTEM:WINDOWS
					/OPT:REF
					/OPT:ICF
					/INCREMENTAL:NO
				>
				/DEBUG:FULL
			)
		endif()
	endif()
endfunction()

function(download_extract_archive ARCHIVE_NAME SRC_URL SUBDIR ORG_NAME DEST_NAME)
	if(EXISTS "${SUBDIR}/${ORG_NAME}")
		file(RENAME "${SUBDIR}/${ORG_NAME}" "${SUBDIR}/${DEST_NAME}")
	endif()
	if(NOT EXISTS "${SUBDIR}/${DEST_NAME}")
		set(DELETE_ZIP OFF)
		if(NOT EXISTS "${SUBDIR}/${ARCHIVE_NAME}")
			message(STATUS "Downloading ${SRC_URL}...")
			file(DOWNLOAD "${SRC_URL}" "${SUBDIR}/${ARCHIVE_NAME}")
			set(DELETE_ZIP ON)
		endif()
		message(STATUS "Extracting ${ARCHIVE_NAME} to ${DEST_NAME}...")
		execute_process(COMMAND 
			${CMAKE_COMMAND} -E tar -xf "${SUBDIR}/${ARCHIVE_NAME}"
			WORKING_DIRECTORY "${SUBDIR}" OUTPUT_QUIET ERROR_QUIET
		)
		file(RENAME "${SUBDIR}/${ORG_NAME}" "${SUBDIR}/${DEST_NAME}")
		if(DELETE_ZIP)
			file(REMOVE "${SUBDIR}/${ARCHIVE_NAME}")
		endif()
	endif()
endfunction()