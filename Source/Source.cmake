list(APPEND SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/Source.cmake"

	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/GameContext.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/GameContext.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/GameObjectBase.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/GameObjectBase.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/GameObject.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/GameObject.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/Tutorials/Bubble.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/Tutorials/Bubble.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/Tutorials/Controller.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Objects/Tutorials/Controller.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/GameWorld.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/GameWorld.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/GameWorld.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/GameWorld.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial0.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial0.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/TutorialKing.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/TutorialKing.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial1.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial1.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial2.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial2.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial3.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial3.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial4.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Tutorial4.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Temp.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameContext/Worlds/Tutorials/Temp.cpp"

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
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Primitive.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Primitive.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/PrimitiveData.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Renderer.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Rendering/Renderer.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resource.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resource.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resources.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Resources/Resources.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/Viewport.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/Viewport.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/ViewportData.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Viewport/ViewportData.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/Handles.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/SFTypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GameServices.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GameServices.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GData.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GData.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GFX.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/Engine/GFX.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Game/Factory/Factory.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/App.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/App.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameLoop/GameLoop.cpp"
	
	"${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes.h"
	"${CMAKE_CURRENT_LIST_DIR}/Game/NoCopy.h"
)

include("${CMAKE_CURRENT_LIST_DIR}/Game/Asserts/Asserts.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Delegate/Delegate.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/GameTypes/GameTypes.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Game/Logger/Logger.cmake")
