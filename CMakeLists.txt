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

# Initialize vcxproj.filters in the solution.
function(target_vcxproj_filters target)
    get_target_property(sources "${target}" SOURCES)
    foreach(filename IN LISTS sources)
      get_filename_component(path "${filename}" PATH)
      string(REPLACE "/" "\\" group "${path}")
      source_group("${group}" FILES "${filename}")
    endforeach()
endfunction()

#users may have to edit this as this is the default install dir
set(ZYNAMIC_OUT "C:/Program Files (x86)/Steam/steamapps/common/Call of Duty Modern Warfare 2" CACHE PATH "Call of Duty: Modern Warfare 2 directory.")
set(ZYNAMIC_BIN "iw4mp" CACHE STRING "Call of Duty: Modern Warfare 2 bin")
add_definitions(-DFAKE_PDB)

add_subdirectory(deps)
add_subdirectory(src)