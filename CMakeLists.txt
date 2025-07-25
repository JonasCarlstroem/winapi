cmake_minimum_required(VERSION 3.23)

project(winapi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

#add_library(winapi INTERFACE)
CREATE_LIB(winapi INTERFACE)

target_compile_features(winapi INTERFACE cxx_std_17)

target_include_directories(winapi INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/winapi>
)

INSTALL_LIB(winapi True winapi)