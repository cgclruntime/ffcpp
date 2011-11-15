#include "threadpool/function_worker.h"
#include <vector>
#include "parallel/parallel_base.h"
#include "common/log.h"
#include "utils/os_utils.h"
#include "threadpool/function_thread_pool.h"

namespace ff{
namespace details{

FFFunctionWorker::FFFunctionWorker()
: m_oTasks(64)
, m_pPool(0)
, m_oPort()
,m_bIsTerminated(false)
{
}

FFFunctionWorker::~FFFunctionWorker()
{
}

void FFFunctionWorker::run()
{
	ParallelBase *pTask = NULL;

REDO:
	while(!m_bIsTerminated)
	{
		//Work-stealing policy.
		pTask = m_oTasks.popBottom();
#ifdef _DEBUG
		log_worker("FFFunctionWorker", "run(), worker loop, with task:"+utl::str(pTask));
#endif
		if(pTask == NULL)
		{
			//Make room for other workers;
			utl::sleep<1>();
			pTask = stealTask();
			if(pTask == NULL)
			{
				utl::sleep<1>();
				goto REDO;
			}
			else{
				executeTask(pTask);
			}
		}
		else{
			executeTask(pTask);
		}
	}//end while
#ifdef _DEBUG
	log_worker("FFFunctionWorker", "run(), quit! ");
#endif
};

void FFFunctionWorker::setPool(FFFunctionThreadPool *pPool) {
	m_pPool = pPool;
	m_oPort.setMsgSwitch(m_pPool->m_oSwitch);
}


ParallelBase * FFFunctionWorker::stealTask()
{
	FFFunctionWorker * pWorker = m_pPool->getMostRelexedWorker();
#ifdef _DEBUG
	log_worker("FFFunctionWorker", "stealTask(), try to steal a work!");
#endif
	return pWorker->m_oTasks.getBottom();
}//end stealTask;

void FFFunctionWorker::executeTask(ParallelBase *pFunction)
{
#ifdef _DEBUG
	log_worker("FFFunctionWorker", "executeTask(), processing pfid: " + pFunction->toString());
#endif
	pFunction->run();
	FFMsgPtr pMsg(new FFMsg(FFMsg::mt_pf_finish, pFunction->identifier()));
	m_oPort.sendMsg(pMsg->getType(), pMsg);
}

};//end namespace detail
};//end namespace ff
