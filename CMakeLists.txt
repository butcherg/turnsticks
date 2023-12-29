cmake_minimum_required(VERSION 3.15)

project(turnsticks LANGUAGES CXX)

add_executable(turnsticks turnsticks.cpp)

if(WX_CONFIG)
	execute_process(COMMAND bash -c "${WX_CONFIG}/wx-config --cxxflags" OUTPUT_VARIABLE WX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
	execute_process(COMMAND bash -c "${WX_CONFIG}/wx-config --libs" OUTPUT_VARIABLE WX_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE )
	list(APPEND CMAKE_CXX_FLAGS "${WX_FLAGS}")
	target_link_libraries(turnsticks ${WX_LIBS})
else()
	message(FATAL_ERROR "WX_CONFIG not defined.")
	#target_include_directories(turnsticks PUBLIC ${WX_FLAGS})
	#target_link_libraries(turnsticks ${WX_LIBS})
endif(WX_CONFIG)



