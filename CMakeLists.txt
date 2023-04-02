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
