
cmake_minimum_required(VERSION 2.8)

project(MyTest)  
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
set(PROJDIR ${CMAKE_CURRENT_SOURCE_DIR})

#-------------------------------------------------------------------------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${MyTest_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${MyTest_BINARY_DIR}/bin)

set(LIBS ${LIBRARY_OUTPUT_PATH})
link_directories(${LIBS})

#-------------------------------------------------------------------------------------------------------------------
# support MT
option(USE_ALLMT_ "Compile all projects as mt"   ON)
option(USE_DMDLL_ "Compile DMMain as dll"   ON) 
if(USE_DMDLL_)
	add_definitions(-DDLL_DMMAIN)
endif()
option(USE_DMLOG_  "DM open log"    ON)
if(USE_DMLOG_)
	add_definitions(-DDMLOG_ON)
endif()

#-------------------------------------------------------------------------------------------------------------------
# support precompile
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include(PrecompiledHeader)

#-------------------------------------------------------------------------------------------------------------------
# add project
add_subdirectory(${PROJDIR}/DMMain)
add_subdirectory(${PROJDIR}/MyTest)
