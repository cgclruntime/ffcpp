#include "threadpool/eager_scheduler.h"
#include "threadpool/function_scheduler.h"
#include "threadpool/function_thread_pool.h"
#include "threadpool/waits_defender.h"
#include "parallel/parallel_base.h"


namespace ff{
namespace details{

EagerScheduler::EagerScheduler(FFFunctionThreadPool *pPool)
: m_pPool(pPool)
, m_iState(state_null)
, m_oUnrunnedPFs()
, m_oPort(){}

EagerScheduler::~EagerScheduler()
{
}

void EagerScheduler::pushUnrunnedPF(ParallelBase *pFunction)
{
	m_oMutex.lock();
#ifdef _DEBUG
	log_scheduler("EagerScheduler", "pushUnrunnedPF(), "+ pFunction->toString());
#endif
	m_oUnrunnedPFs.push_back(pFunction);
	m_oMutex.unlock();
}

void EagerScheduler::eagerSchedulePFs()
{
	m_oMutex.lock();
	m_oReadyPFs.clear();
	WaitsDefender & W = m_pPool->scheduler().waits();

	size_t pf_size = m_oUnrunnedPFs.size();
	for(size_t i = 0; i < pf_size; ++i)
	{
		pfid_t id = m_oUnrunnedPFs[i]->identifier();
		WaitTree * pTree = W.findWaitTree(id);
		if(pTree == NULL)
		{
			m_oReadyPFs.push_back(m_oUnrunnedPFs[i]);
			m_pPool->invalidTask(m_oUnrunnedPFs[i]);
#ifdef _DEBUG
			log_scheduler("EagerScheduler", "eagerSchedulePFs(), invalid task: "+ utl::str(m_oUnrunnedPFs[i]));
#endif
		}
		else
		{
			//TODO(athrunarthur@gmail.com)Waiting functions should be achieved,
		}
	}
#ifdef _DEBUG
	log_scheduler("EagerScheduler", "eagerSchedulePFs(), ready PFs: "+ utl::str(m_oReadyPFs.size()));
#endif

	m_oMutex.unlock();
}

void EagerScheduler::executeAllPF()
{
	ParallelBase *pFunction = NULL;
	m_oMutex.lock();
	m_iState = state_null;
	while(!m_oReadyPFs.empty()){
		pFunction = m_oReadyPFs.back();
		m_oReadyPFs.pop_back();
		m_oMutex.unlock();
		executeOnePF(pFunction);
		m_oMutex.lock();
	}
	m_oMutex.unlock();

}
void EagerScheduler::executeOnePF(ParallelBase *pFunction)
{

	if(pFunction)
	{
		if(pFunction->isCalledParen() && !pFunction->isRunning())
		{
			pFunction->run();
#ifdef _DEBUG
			log_scheduler("EagerScheduler", "executeOnePF(), execute: "+ pFunction->toString());
#endif
			FFMsgPtr pMsg(new FFMsg(FFMsg::mt_pf_finish, pFunction->identifier()));
			m_oPort.sendMsg(pMsg->getType(), pMsg);
		}
	}
}

}//end namespace details
}//end namespace ff
