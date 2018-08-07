PROJECT(PsDump)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 									inc/*.h)
FILE(GLOB LIBPSD_HEADER      							libpsd-0.9/src/*.h;libpsd-0.9/include/*.h)

FILE(GLOB ROOT_SOURCE 									src/*.cpp)
FILE(GLOB LIBPSD_SOURCE      							libpsd-0.9/src/*.c)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Libpsd" 						FILES 	${LIBPSD_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Libpsd" 						FILES 	${LIBPSD_SOURCE})

# 输出设置
add_definitions(-DUNICODE -D_UNICODE)  # unicode版本
set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
if(USE_PDDLL_)
ADD_LIBRARY(PsDump SHARED
			${ROOT_HEADER};
			${LIBPSD_HEADER};
			${ROOT_SOURCE};
			${LIBPSD_SOURCE};
			)
else()
ADD_LIBRARY(PsDump STATIC
			${ROOT_HEADER};
			${LIBPSD_HEADER};
			${ROOT_SOURCE};
			${LIBPSD_SOURCE};
			)
endif()

target_include_directories(PsDump
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libpsd-0.9/include
)
SET_TARGET_PROPERTIES(PsDump PROPERTIES OUTPUT_NAME "PsDump")

# # 增加预编译头
# use_precompiled_header(PsDump
   # "${CMAKE_CURRENT_SOURCE_DIR}/inc/DmMainAfx.h"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/DmMainAfx.cpp"
  # )
