#based off of OpenIW scripts
cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)
project(OpenIW4 LANGUAGES CXX)

option(SUBMODULES "Init repo deps" ON)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  if(CMAKE_SUBMODULES)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE RESULT)
    if(NOT RESULT EQUAL "0")
      message(FATAL_ERROR "Failed to install repos, try installing them manually.")
    endif()
  endif()
endif()

add_subdirectory(deps)
add_subdirectory(src)