#ifndef FFCPP_THREAD_POOL_FUNCTION_SCHEDULER_H
#define FFCPP_THREAD_POOL_FUNCTION_SCHEDULER_H
#include <set>
#include <vector>
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/tr1/memory.hpp>
#include "utils/types.h"
#include "utils/msg_port.h"
#include "threadpool/finished_pfids.h"
#include "message/message.h"
#include "threadpool/waits_defender.h"
#include "threadpool/eager_scheduler.h"

namespace ff{
  namespace details {

class ParallelBase;
class FFFunctionThreadPool;
typedef utl::uint32_t pfid_t;

class FFFunctionScheduler: public boost::noncopyable {
public:
	//! Constructor
	FFFunctionScheduler(FFFunctionThreadPool *pPool);
	virtual ~FFFunctionScheduler();

	//!
	virtual void run();

	void	initialSwitch(utl::MsgSwitch<FFMsg> & msgSwitch);

	FinishedPFIDs &			finishes(){return m_oFinishes;}
	WaitsDefender &			waits(){return m_oWaits;}
	EagerScheduler &		eager() {return m_oEagerScheduler; }

protected:

	void					handlePFFinishMsg(const FFMsg & msg);

	inline void				acquireSharedAtomic() { m_oAtomicMutex.lock_shared();}
	inline void				acquireUniqueAtomic() { m_oAtomicMutex.lock();}
	inline void				releaseSharedAtomic() { m_oAtomicMutex.unlock_shared();}
	inline void				releaseUniqueAtomic() { m_oAtomicMutex.unlock();}

protected:
	friend class FFFunctionThreadPool;
	friend class ParallelBase;

	boost::shared_mutex		m_oAtomicMutex;//This is used to ensure some atomic operations.

	FinishedPFIDs			m_oFinishes;
	WaitsDefender			m_oWaits;
	utl::MsgPort<FFMsg>		m_oPort;
	EagerScheduler			m_oEagerScheduler;
	FFFunctionThreadPool *	m_pPool;
};//end class FFFunctionScheduler

};//end namespace detail;
};//end namespace ff;
#endif

