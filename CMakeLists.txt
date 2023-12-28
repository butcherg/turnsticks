cmake_minimum_required(VERSION 3.15)

project(turnstick LANGUAGES CXX)

#set(wxWidgets_ROOT_DIR, "/dhome/glenn/rawproc10/build_support/wxWidgets-3.1.4")
#set(wxWidgets_LIB_DIR , "/home/glenn/rawproc10/build_support/wxWidgets-3.1.4/build-w64")
#find_package(wxWidgets)

if(WX_CONFIG)
	execute_process(COMMAND bash -c "${WX_CONFIG}/wx-config --cxxflags" OUTPUT_VARIABLE wxWidgets_USE_FILE OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND bash -c "${WX_CONFIG}/wx-config --libs" OUTPUT_VARIABLE wxWidgets_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE )
endif(WX_CONFIG)


add_executable(turnsticks turnsticks.cpp)
include_directories(${wxWidgets_USE_FILE})
target_link_libraries(turnsticks ${wxWidgets_LIBRARIES})
