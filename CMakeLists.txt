cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(cool-game CXX)

# Ensure all dependencies have been pulled properly.
foreach (d "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Arduino-CMake-Toolchain/"
		"${CMAKE_CURRENT_SOURCE_DIR}/external/etl/")
	if (NOT EXISTS "${d}/.git")
		message(FATAL_ERROR "Could not find ${d}. "
			"Please pull all dependencies using `git submodule init && git submodule update.")
	endif()
endforeach()

# The platform to build for. Platform-specific source files are in src/${COOL_GAME_PLATFORM}/. The
# only supported platforms are "arduino" and "pc".
set(COOL_GAME_PLATFORM "pc" CACHE STRING "Platform to build for. Either \"arduino\" or \"pc\".")

# Platform-specific source path and config file.
set(platform_path "${CMAKE_CURRENT_SOURCE_DIR}/src/${COOL_GAME_PLATFORM}")
if (NOT EXISTS "${platform_path}")
	message(FATAL_ERROR "Invalid platform \"${COOL_GAME_PLATFORM}\"")
endif()

# Execute a platform-specific cmake source if it exists.
set(platform_cmake_file "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${COOL_GAME_PLATFORM}.cmake")
if (EXISTS "${platform_cmake_file}")
	include("${platform_cmake_file}")
endif()
