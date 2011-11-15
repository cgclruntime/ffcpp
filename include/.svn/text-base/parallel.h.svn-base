/* AthrunArthur 2011.8.23
 * */
#ifndef FFCPP_PARALLEL_H_
#define FFCPP_PARALLEL_H_

#include "parallel/parallel.h"
#include "threadpool/function_thread_pool.h"
#include "parallel/wait_conds_expr.h"
#include "common/log.h"


extern ff::details::FFFunctionThreadPool g_oThreadPool;

#ifdef _DEBUG
#define FF_INITIALIZE(pool_size)  \
		utl::Logger<0> *__pLog = utl::Logger<0>::initial("log.txt");  \
		__pLog->logModule("finishes"); \
		__pLog->logModule("scheduler"); \
		__pLog->logModule("parallel"); \
		__pLog->logModule("waits"); \
		__pLog->logModule("taskq"); \
		g_oThreadPool.initialize(pool_size);
#else
#define FF_INITIALIZE(pool_size) g_oThreadPool.initialize(pool_size);
#endif


#endif


