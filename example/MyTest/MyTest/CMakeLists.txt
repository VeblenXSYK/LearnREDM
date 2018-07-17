project(MyTest) 

file(GLOB ROOT_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB ROOT_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp;src/*.c)

source_group("inc" 				FILES 	${ROOT_HEADER})
source_group("src"				FILES 	${ROOT_SOURCE})

use_precompiled_header(MyTest
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/StdAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/StdAfx.cpp"
  )
  
#-------------------------------------------------------------------------------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc
					${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${PROJDIR}/DmMain/inc
					${PROJDIR}/DmMain/inc/Common;
					${PROJDIR}/DmMain/inc/Common/Template;
					${PROJDIR}/DmMain/inc/Core;
					${PROJDIR}/DmMain/inc/Core/Msg;
					${PROJDIR}/DmMain/inc/Core/Dui;
					${PROJDIR}/DmMain/inc/Core/Event;
					${PROJDIR}/DmMain/inc/IDmMain;
					${PROJDIR}/DmMain/inc/Modules;
					${PROJDIR}/DmMain/inc/Widgets;)
		
#-------------------------------------------------------------------------------------------------------------------		
if(USE_ALLMT_)
set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
add_definitions(-DUNICODE -D_UNICODE)  # unicode
add_executable(MyTest  WIN32 ${ROOT};${ROOT_HEADER};${ROOT_SOURCE};)
set_target_properties(MyTest PROPERTIES OUTPUT_NAME "MyTest")

#-------------------------------------------------------------------------------------------------------------------
add_dependencies(MyTest  DmMain)

 