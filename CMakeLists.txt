cmake_minimum_required(VERSION 3.14)
project(PacmanSFML)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(CMAKE_PREFIX_PATH C:/)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

set(SOURCE_FILES src/main.cpp
        src/Game.cpp src/Game.h
        src/ResourceHandler.cpp src/ResourceHandler.h
        src/GameStates/GameStateBase.cpp src/GameStates/GameStateBase.h
        src/GameStates/GameMenu.cpp src/GameStates/GameMenu.h
        src/GameStates/GamePlay.cpp src/GameStates/GamePlay.h
        src/GameStates/GameOver.cpp src/GameStates/GameOver.h
        src/GameObjects/GameObject.cpp src/GameObjects/GameObject.h
        src/Map/Level.cpp src/Map/Level.h 
        src/Map/Tile.cpp src/Map/Tile.h
        src/GameObjects/Player.cpp src/GameObjects/Player.h
        src/Other/Pathfinding.cpp src/Other/Pathfinding.h
        src/GameObjects/Entity.cpp src/GameObjects/Entity.h
        src/Other/TextSetup.cpp src/Other/TextSetup.h
        src/Map/PillTile.cpp src/Map/PillTile.h src/GameObjects/Ghost.cpp src/GameObjects/Ghost.h src/BehaviourTrees/BehaviourTree.h src/BehaviourTrees/TreeNodes.h src/BehaviourTrees/BlackBoard.h src/BehaviourTrees/GhostNodes.h)
set(TINYXML tinyxml2/tinyxml2.h tinyxml2/tinyxml2.cpp)
add_executable(PacmanSFML ${SOURCE_FILES} ${TINYXML})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

target_link_libraries (PacmanSFML sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)