cmake_minimum_required(VERSION 3.10)
project(gui LANGUAGES C CXX VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(WIN32 OR MINGW)
	add_definitions(-DSDL_MAIN_HANDLED)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

file(GLOB SRC
	"*.h" "*.hpp" "*.cpp" "*.c"

	"widgets/*.h"
	"widgets/*.hpp"
	"widgets/*.cpp"
	"widgets/*.c"

	"glad/*.h"
	"glad/*.c"
)

find_package(SDL2 CONFIG REQUIRED)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC /)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nanovg)

if (MINGW OR UNIX)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		${SDL2_LIBRARIES}
		nanovg
		stdc++fs
	)
else()
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		SDL2::SDL2 SDL2::SDL2main
		nanovg
	)
endif()
