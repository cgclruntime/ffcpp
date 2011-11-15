#include "parallel/parallel_base.h"
#include "utils/atomic.h"
#include "utils/singleton.h"
#include "threadpool/function_thread_pool.h"
#include "threadpool/eager_scheduler.h"
#include "common/log.h"

#ifdef _DEBUG
#include <sstream>
#endif

ff::details::FFFunctionThreadPool g_oThreadPool;


namespace ff{
namespace details{

volatile utl::uint32_t ParallelBase::s_iIdentifier = 1;

ParallelBase::ParallelBase()
: m_iIdentifier(s_iIdentifier){
	setConstructed();
	while(!utl::Atomic<utl::uint32_t>::bool_cas(&s_iIdentifier,m_iIdentifier + 1, m_iIdentifier))
	{
		m_iIdentifier = s_iIdentifier;
	}
}

ParallelBase::~ParallelBase() {
	setDeconstructed();
#ifdef _DEBUG
	log_parallel("ParallelBase", "~ParallelBase(), "+toString()+". End!");
#endif
}

void ParallelBase::waitFinish()
{
	if(isConstructed())
		return ;
	setWaitSemaphore();
#ifdef _DEBUG
	log_parallel("ParallelBase", "waitFinish(), "+toString());
#endif
	FFFunctionScheduler &S = g_oThreadPool.scheduler();
	WaitsDefender & WD = S.waits();
	FinishedPFIDs & FP = S.finishes();
	EagerScheduler &ES = S.eager();

	bool flag_finished = false;
	while(!flag_finished){
		if(!FP.wait(m_iIdentifier)){
			switch(ES.getState()){
			case EagerScheduler::state_null:
				break;
			case EagerScheduler::state_dead_lock:
				if(isCalledParen() && !isRunning() && !isExeOver())
					ES.pushUnrunnedPF(this);
				break;
			case EagerScheduler::state_scheduled:
				ES.setStateNull();
				ES.executeAllPF();
				break;
			}//end switch
		}//end if
		else{
			WD.eraseAllInfoOfPF(m_iIdentifier);
			g_oThreadPool.decrementParaFuncNum();
			flag_finished = true;
		}//end else
	}//end while
}



void ParallelBase::run() {
	setRunning();
	g_oThreadPool.incrementParaFuncNum();
}

void ParallelBase::schedule()
{
	setCalledParen();

	FFFunctionScheduler &S = g_oThreadPool.scheduler();
	WaitsDefender & WD = S.waits();
	FinishedPFIDs & FP = S.finishes();

	WaitTree * pTree = WD.findWaitTree(m_iIdentifier);
	if(pTree == NULL)
	{
		g_oThreadPool.scheduleTask(this);
#ifdef _DEBUG
		log_parallel("ParallelBase", "schedule(), to worker "+toString());
#endif
		return ;
	}

	//Check the "finished" waited pFunctions.
	//This operation shouldn't be disturbed by FFFunctionScheduler's handleFinishMsg()
	//So we try to achieve atomic. SharedAtomic provide concurrents between multiple schedule();
	S.acquireSharedAtomic();

	std::vector<pfid_t> oWaits1, oWaits2;
	pTree->getWaitPFIDs(oWaits1);
	for(size_t i = 0; i < oWaits1.size(); ++i)
	{
		if(FP.has(oWaits1[i]))
			pTree->notifyFinish(oWaits1[i]);
	}
	if(pTree->isFinished())
	{
#ifdef _DEBUG
		log_parallel("ParallelBase", "schedule(), wait tree is finished, to worker " + toString());
#endif
		g_oThreadPool.scheduleTask(this);
		S.releaseSharedAtomic();
		return ;
	}
#ifdef _DEBUG
	log_parallel("ParallelBase", "schedule(), wait tree is not finished, to scheduler " + toString());
#endif
	WD.addWaitAndPF(pTree, this);
	S.releaseSharedAtomic();
}

void ParallelBase::wait(WaitConds w)
{
	WaitsDefender & WD = g_oThreadPool.scheduler().waits();
	WD.addWait(m_iIdentifier, w.m_pWaitTree);
}
#ifdef _DEBUG
std::string ParallelBase::toString() const {
	std::ostringstream ss;
	ss<<"id:"<<m_iIdentifier<<"; state: ";
	if(isConstructed())
		ss<<"constructed, ";
	if(isCalledParen())
		ss<<"called_paren_operator, ";
	if(isDeconstructed())
		ss<<"deconstructing, ";
	if(isExeOver())
		ss<<"exe_over, ";
	if(isRunning())
		ss<<"running, ";
	if(isWaitSemaphore())
		ss<<"wait, ";
	return ss.str();
};
#endif

};//end namespace detail;
};//end namespace ff;
