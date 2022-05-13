file(GLOB_RECURSE OpenIW4 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.hpp)

add_executable(OpenIW4
	${OpenIW4}
	stdafx.cpp
	stdafx.hpp
)

target_link_libraries(OpenIW4
	Zynamic
)

target_link_options(OpenIW4
PRIVATE
	/IGNORE:4254 /DYNAMICBASE:NO /NXCOMPAT:NO /SAFESEH:NO /INCREMENTAL:NO /LARGEADDRESSAWARE /LAST:.main
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

target_vcxproj_filters(OpenIW4)