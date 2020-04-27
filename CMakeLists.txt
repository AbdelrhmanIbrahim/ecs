cmake_minimum_required(VERSION 3.17)

#project name
project(ecs)

#c++17
set(CMAKE_CXX_STANDARD 17)


#headers
set(HEADERS
	ecs/Entity.h
	ecs/Meta.h
	ecs/Component_Storage.h
	ecs/World.h
	ecs/Script.h
	)

#src
set(SOURCES
	main.cpp
	)

#make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(${PROJECT_NAME}
				${HEADERS}
				${SOURCES})