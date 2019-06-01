cmake_minimum_required(VERSION 3.10)
project(gui LANGUAGES C CXX VERSION 1.0)

option(BUILD_TEST "Build the test application." ON)

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

	"pugixml/*.h"
	"pugixml/*.hpp"
	"pugixml/*.cpp"
)

find_package(SDL2 CONFIG REQUIRED SDL2)

set(Boost_USE_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS locale)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/nanovg)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/osdialog)

if (BUILD_TEST)
	add_executable(${PROJECT_NAME}_test "test/main.cpp")
	target_link_libraries(${PROJECT_NAME}_test
		PRIVATE
		${PROJECT_NAME}
		osdialog
		Boost::locale
	)
endif()

if (MINGW OR APPLE)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		${SDL2_LIBRARIES}
		nanovg
		osdialog
		Boost::locale
	)
else()
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		SDL2::SDL2 SDL2::SDL2main
		nanovg
		osdialog
		Boost::locale
	)
endif()

if (CMAKE_DL_LIBS)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		${CMAKE_DL_LIBS}
	)
endif()
