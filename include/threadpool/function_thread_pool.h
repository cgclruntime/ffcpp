#ifndef FFCPP_THREAD_POOL_FUNCTION_THREAD_POOL_H
#define FFCPP_THREAD_POOL_FUNCTION_THREAD_POOL_H

#include <boost/tr1/memory.hpp>
#include <boost/noncopyable.hpp>
#include "threadpool/function_scheduler.h"
#include "threadpool/function_worker.h"
#include "threadpool/threadpool.hpp"
#include "utils/atomic.h"

namespace ff{
namespace details{

class FFFunctionThreadPool: public boost::noncopyable
{
public:
	FFFunctionThreadPool();
	virtual ~FFFunctionThreadPool();

	//! Initialize the thread pool, with the specified (iPoolSize + 1).
	/// In the threadpool, one is for scheduler, and iPoolSize is for workers.
	/// After the initialization, run the threadpool
	void initialize(int iPoolSize);

	//! Put the pFunction into one of the m_pWorkers.
	void scheduleTask(ff::details::ParallelBase * pFunction);

	//! Set the pFunction invalid!
	/// This will let the worker don't execute the pFunction.
	void invalidTask(ff::details::ParallelBase * pFunction);

	inline FFFunctionScheduler & scheduler()
	{
		return m_oScheduler;
	}

	inline void incrementParaFuncNum()
	{
		utl::Atomic<utl::uint32_t>::inc(&m_iParaFuncNum);
	}
	inline void decrementParaFuncNum()
	{
		utl::Atomic<utl::uint32_t>::dec(&m_iParaFuncNum);
	}

	//! The number of workers.
	int poolSize() const
	{
		return m_iPoolSize;
	}
protected:
	friend class FFFunctionWorker;

	//! Get a worker which has the most minimal size();
	FFFunctionWorker * getMostRelexedWorker();

	//! Get a worker which has the most max size();
	FFFunctionWorker * getMostBusyWorker();

protected:
	FFFunctionScheduler m_oScheduler;
	FFFunctionWorker * m_pWorkers; ///< Array of workers
	boost::threadpool::pool m_oTP;
	utl::MsgSwitch<FFMsg> m_oSwitch;
	int m_iPoolSize;

	volatile utl::uint32_t m_iParaFuncNum;
};//end class FFFunctionThreadPool

};//end namespace detail
};//end namespace ff;

#endif
