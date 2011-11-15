MESSAGE(STATUE "This is include dir" ${INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${INCLUDE_DIRS})
SET(THRDPOOL_LIST eager_scheduler.cpp finished_pfids.cpp function_scheduler.cpp function_thread_pool.cpp function_worker.cpp waits_defender.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
ADD_DEFINITIONS(${DEBUG_DEFS})
LINK_DIRECTORIES(${LIBS_LINK_DIRS})
ADD_LIBRARY(threadpool SHARED ${THRDPOOL_LIST})
TARGET_LINK_LIBRARIES(threadpool ${LIBS_LINK_LIBS})


