PROJECT(TEST)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
MESSAGE(STATUS "This is BINARY dir" ${PROJECT_BINARY_DIR})
MESSAGE(STATUE "This is SOURCE dir" ${PROJECT_SOURCE_DIR})

SET(DEBUG_DEFS "")

SET(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include "/home/zylia/boost/boost_1_46_1/" "usr/local/include")

SET(LIBS_LINK_DIRS "/home/zylia/boost/boost_1_46_1/usr/local/lib" "/usr/local/lib")
SET(LIBS_LINK_LIBS boost_date_time boost_thread log4cplus)

SET(TOOLS_LINK_DIRS "/home/zylia/boost/boost_1_46_1/usr/local/lib" ${PROJECT_SOURCE_DIR}/build/lib)
SET(TOOLS_LINK_LIBS "libparallel.so" "libthreadpool.so")

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0  -g")


ADD_SUBDIRECTORY(lib ${PROJECT_SOURCE_DIR}/build/lib)
ADD_SUBDIRECTORY(tools ${PROJECT_SOURCE_DIR}/build/tools)






