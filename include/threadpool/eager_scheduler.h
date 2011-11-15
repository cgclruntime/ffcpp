#ifndef FFCPP_THREAD_POOL_EAGER_SCHEDULER_H_
#define FFCPP_THREAD_POOL_EAGER_SCHEDULER_H_
#include <boost/thread/mutex.hpp>
#include <vector>
#include "utils/msg_port.h"
#include "utils/types.h"
#include "message/message.h"
#include "common/log.h"

namespace ff{
namespace details{

class ParallelBase;
class FFFunctionThreadPool;
//! This scheduler is used at the time dead-lock happened!
class EagerScheduler
{
public:
	EagerScheduler(FFFunctionThreadPool *pPool);
	~EagerScheduler();

	inline void			setDeadLock(){ m_iState = state_dead_lock; m_oUnrunnedPFs.clear();};
	inline void			setScheduled(){ m_iState = state_scheduled;};
	inline void			setStateNull(){ m_iState = state_null; m_oUnrunnedPFs.clear();};

	inline utl::uint8_t	getState() const {return m_iState;};

	void				pushUnrunnedPF(ParallelBase *pFunction);

	void				executeAllPF();

	void				executeOnePF(ParallelBase *pFunction);

	void				eagerSchedulePFs();

	void				setMsgSwitch(utl::MsgSwitch<FFMsg> & oSwitch)
	{
		m_oPort.setMsgSwitch(oSwitch);
	}
protected:

public:
	enum{
		state_null,
		state_dead_lock,
		state_scheduled
	};
protected:
	typedef std::vector<ParallelBase *> PFContainer_t;

	FFFunctionThreadPool *	m_pPool;
	utl::uint8_t			m_iState;
	PFContainer_t 			m_oUnrunnedPFs;
	PFContainer_t			m_oReadyPFs;
	utl::MsgPort<FFMsg>		m_oPort;
	boost::mutex			m_oMutex;
};
}//end namespace details
}//end namespace ff

#endif

