list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Game/Factory/Factory.h
    ${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.h
    ${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.cpp
	${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes.h
)

include("${CMAKE_CURRENT_LIST_DIR}/Game/Asserts/Asserts.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Delegate/Delegate.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes/GameTypes.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Logger/Logger.cmake")
