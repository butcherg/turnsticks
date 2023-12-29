cmake_minimum_required(VERSION 3.15)

project(turnsticks LANGUAGES CXX)

#include_directories("/usr/include/wx-3.0" "/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.0")
add_executable(turnsticks turnsticks.cpp)

if(WXDIR)
	message(STATUS "wxWidgets configured with wx-config at ${WXDIR}")
	execute_process(COMMAND bash -c "${WXDIR}/wx-config --cxxflags" OUTPUT_VARIABLE WX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND bash -c "${WXDIR}/wx-config --libs" OUTPUT_VARIABLE WX_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE )
	list(APPEND CMAKE_CXX_FLAGS "${WX_FLAGS}")
	target_link_libraries(turnsticks ${WX_LIBS})
else()
	message(STATUS "wxWidgets configured with OS-installed library")
	find_package(wxWidgets REQUIRED COMPONENTS core base)
	include(${wxWidgets_USE_FILE})
	target_link_libraries(turnsticks ${wxWidgets_LIBRARIES})
endif(WXDIR)



