file(GLOB_RECURSE OpenIW4 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.hpp)

add_executable(OpenIW4
	${OpenIW4}
	stdafx.cpp
	stdafx.hpp
)

target_link_libraries(OpenIW4
	Zynamic
	Winmm
)

target_include_directories(${PROJECT_NAME}
PRIVATE
	$<INSTALL_INTERFACE:utils>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/utils>
)

target_link_options(OpenIW4
PRIVATE
	/IGNORE:4254 /DYNAMICBASE:NO /NXCOMPAT:NO /SAFESEH:NO /INCREMENTAL:NO /LARGEADDRESSAWARE /LAST:.text
)

target_precompile_headers(OpenIW4
PRIVATE
	stdafx.hpp
)

target_compile_options(OpenIW4
PRIVATE
	/MP
	/Zm200
	/utf-8
	/std:c++17
	/bigobj
)

set_target_properties(OpenIW4
PROPERTIES
  PDB_OUTPUT_DIRECTORY          "${ZYNAMIC_OUT}$<0:>"
  ARCHIVE_OUTPUT_DIRECTORY      "${ZYNAMIC_OUT}$<0:>"
  LIBRARY_OUTPUT_DIRECTORY      "${ZYNAMIC_OUT}$<0:>"
  RUNTIME_OUTPUT_DIRECTORY      "${ZYNAMIC_OUT}$<0:>"
  VS_DEBUGGER_WORKING_DIRECTORY "${ZYNAMIC_OUT}"
)

target_compile_definitions(OpenIW4
PRIVATE
  -DFAKE_PDB
  "NOMINMAX"
  "WIN32_LEAN_AND_MEAN"
  "_CRT_SECURE_NO_WARNINGS"
  "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
)

target_vcxproj_filters(OpenIW4)