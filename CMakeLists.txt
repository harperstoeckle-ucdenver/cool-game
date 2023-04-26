cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# The platform to build for. Platform-specific source files are in src/${COOL_GAME_PLATFORM}/. The
# only supported platforms are "arduino" and "pc".
set(COOL_GAME_PLATFORM "pc" CACHE STRING "Platform to build for. Either \"arduino\" or \"pc\".")

if (COOL_GAME_PLATFORM STREQUAL "arduino")
	set(ARDUINO_ENABLE_PACKAGE_MANAGER YES CACHE BOOL "Disable annoying errors")
	set(ARDUINO_BOARD avr.uno CACHE STRING "Board name")
	set(ARDUINO_INSTALL_PATH "$ENV{HOME}/Library/Arduino15/"
		CACHE PATH "Arduino SDK installation path")
	option(NO_SYSTEM_INCLUDE "" ON)
	set(CMAKE_TOOLCHAIN_FILE cmake/Arduino-CMake-Toolchain/Arduino-toolchain.cmake)
endif()

project(cool-game CXX)

# Ensure all dependencies have been pulled properly.
foreach (d "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Arduino-CMake-Toolchain/"
		"${CMAKE_CURRENT_SOURCE_DIR}/external/etl/")
	if (NOT EXISTS "${d}/.git")
		message(FATAL_ERROR "Could not find ${d}. "
			"Please pull all dependencies using `git submodule init && git submodule update.")
	endif()
endforeach()

# Platform-specific source path and config file.
set(platform_path "${CMAKE_CURRENT_SOURCE_DIR}/src/${COOL_GAME_PLATFORM}")
if (NOT EXISTS "${platform_path}")
	message(FATAL_ERROR "Invalid platform \"${COOL_GAME_PLATFORM}\"")
endif()

file(GLOB common_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB common_headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB platform_sources CONFIGURE_DEPENDS "${platform_path}/*.cpp")
file(GLOB platform_headers CONFIGURE_DEPENDS "${platform_path}/*.hpp")

add_executable(cool-game
	${common_sources} ${common_headers} ${platform_sources} ${platform_headers})

target_include_directories(cool-game PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_compile_features(cool-game PRIVATE cxx_std_17)
target_compile_options(cool-game PRIVATE -nostdinc++ -Wall -Wextra -pedantic -Wconversion)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/etl/")
target_link_libraries(cool-game PRIVATE etl::etl)
target_compile_definitions(cool-game PRIVATE ETL_NO_STL)

if (COOL_GAME_PLATFORM STREQUAL "arduino")
	target_link_arduino_libraries(cool-game PRIVATE core EEPROM)
	target_enable_arduino_upload(cool-game)
else()
	find_package(Curses REQUIRED)
	target_link_libraries(cool-game PRIVATE ${CURSES_LIBRARIES})
	target_include_directories(cool-game PRIVATE ${CURSES_INCLUDE_DIRS})
endif()
