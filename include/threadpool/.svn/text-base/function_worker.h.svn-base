#ifndef FFCPP_THREAD_POOL_FUNCTION_WORKER_H
#define FFCPP_THREAD_POOL_FUNCTION_WORKER_H
#include <boost/noncopyable.hpp>
#include "threadpool/task_queue.h"
#include "utils/msg_port.h"
#include "message/message.h"

namespace ff{
namespace details{

class ParallelBase;
class FFFunctionThreadPool;

class FFFunctionWorker: public boost::noncopyable
{
public:
	//! Constructor
	FFFunctionWorker();
	virtual ~FFFunctionWorker();

	//! Worker
	virtual void run();

	//! Put the pFunction into the TaskQueue, m_oTasks;
	inline void scheduleTask(ParallelBase *pFunction){m_oTasks.pushTop(pFunction);}

	inline void	invalidTask(ParallelBase *pFunction){ m_oTasks.invalidTask(pFunction);}

	//! Get the size of m_oTasks;
	inline utl::uint32_t queueSize() const{return m_oTasks.size();}

	void setPool(FFFunctionThreadPool *pPool);

	inline void terminate(){m_bIsTerminated = true;}

protected:
	//! Steal a task from other workers.
	/// This may return NULL.
	ParallelBase * 		stealTask();

	//! Execute a task.
	void 				executeTask(ParallelBase * pFunction);

protected:
	friend class FFFunctionThreadPool;
	TaskQueue<ff::details::ParallelBase> m_oTasks;
	FFFunctionThreadPool * 		m_pPool;
	utl::MsgPort<FFMsg> 		m_oPort;
	bool 						m_bIsTerminated;
};

};//end namespace detail
};//end namespace ff;
#endif

