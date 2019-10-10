list(APPEND SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputDataFrame.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputDataFrame.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputHandler.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputHandler.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputMappings.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputStateMachine.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/InputStateMachine.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/KeyboardInput.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/KeyboardInput.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/Input.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Input/Input.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Colour.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Colour.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resources.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resources.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/Viewport.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/Viewport.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/ViewportData.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/ViewportData.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/SFTypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GData.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GData.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Game/Factory/Factory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/NoCopy.h"
)

include("${CMAKE_CURRENT_LIST_DIR}/Game/Asserts/Asserts.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Delegate/Delegate.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes/GameTypes.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Logger/Logger.cmake")
