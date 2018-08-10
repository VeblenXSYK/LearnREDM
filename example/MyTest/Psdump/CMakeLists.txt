PROJECT(PsDump)

# �ļ����飨���ˣ���ͬʱ������뵽ADD_LIBRARY�в�����ʾ
FILE(GLOB ROOT_HEADER 									inc/*.h)
FILE(GLOB LIBPSD_HEADER      							libpsd-0.9/src/*.h;libpsd-0.9/include/*.h)

FILE(GLOB ROOT_SOURCE 									src/*.cpp)
FILE(GLOB LIBPSD_SOURCE      							libpsd-0.9/src/*.c)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Libpsd" 						FILES 	${LIBPSD_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Libpsd" 						FILES 	${LIBPSD_SOURCE})

# �������
add_definitions(-DUNICODE -D_UNICODE)  # unicode�汾
set(CMAKE_DEBUG_POSTFIX "_d")# Debug�������ļ����Ӻ�׺,��exe��Ч.
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# ֱ�ӳ�gui��Ĭ�ϣ�Ȼ�� �ĳ�mtd
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

# # ����Ԥ����ͷ
# use_precompiled_header(PsDump
   # "${CMAKE_CURRENT_SOURCE_DIR}/inc/DmMainAfx.h"
  # "${CMAKE_CURRENT_SOURCE_DIR}/src/DmMainAfx.cpp"
  # )