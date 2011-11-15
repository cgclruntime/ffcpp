#include "threadpool/function_thread_pool.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "parallel/parallel_base.h"
#include "threadpool/eager_scheduler.h"
#include "utils/os_utils.h"
#include "utils/msg_port.h"
#ifdef _DEBUG
#include <iostream>
#include "common/log.h"
#endif

namespace ff {
namespace details {

FFFunctionThreadPool::FFFunctionThreadPool() :
		m_oScheduler(this), m_pWorkers(NULL), m_oTP(1), m_iPoolSize(0), m_iParaFuncNum(
				0)
{
	m_oScheduler.initialSwitch(m_oSwitch);
}

FFFunctionThreadPool::~FFFunctionThreadPool()
{
	while (m_iParaFuncNum)
	{
		utl::sleep<1>();
	}
	utl::MsgPort<FFMsg> port;
	port.setMsgSwitch(m_oSwitch);
	FFMsgPtr pMsg(new FFMsg(FFMsg::mt_terminate, 0));
	port.sendMsg(pMsg->getType(), pMsg);

	for (int i = 0; i < m_iPoolSize; ++i)
		m_pWorkers[i].terminate();

	m_oTP.wait();
#ifdef _DEBUG
	log_tpool("FFFunctionThreadPool", "~FFFunctionThreadPool(), workers terminated!");
#endif
	delete[] m_pWorkers;
}

void FFFunctionThreadPool::initialize(int iPoolSize)
{
	m_iPoolSize = iPoolSize;
	m_oTP.size_controller().resize(iPoolSize + 1);
	m_pWorkers = new FFFunctionWorker[iPoolSize];
	for (int i = 0; i < iPoolSize; i++)
		m_pWorkers[i].setPool(this);

	m_oTP.schedule(boost::bind(&FFFunctionScheduler::run, &m_oScheduler));
	for (int i = 0; i < iPoolSize; i++)
		m_oTP.schedule(boost::bind(&FFFunctionWorker::run, m_pWorkers + i));
}

//! Put the pFunction into one of the m_pWorkers.
void FFFunctionThreadPool::scheduleTask(ff::details::ParallelBase * pFunction)
{
	FFFunctionWorker * pWorker = getMostRelexedWorker();
	pWorker->scheduleTask(pFunction);
};

void FFFunctionThreadPool::invalidTask(ParallelBase *pFunction)
{
	for (int i = 0; i < m_iPoolSize; ++i)
	{
		m_pWorkers[i].invalidTask(pFunction);
	}
}

//! Get a worker which has the most minimal size();
FFFunctionWorker * FFFunctionThreadPool::getMostRelexedWorker()
{
	int suffix = 0;
	utl::uint32_t temp = m_pWorkers->queueSize();
	for (int i = 0; i < m_iPoolSize; i++)
	{
		if (temp > (m_pWorkers + i)->queueSize())
		{
			temp = (m_pWorkers + i)->queueSize();
			suffix = i;
		}
	}
	return (m_pWorkers + suffix);
};

//! Get a worker which has the most max size();
FFFunctionWorker * FFFunctionThreadPool::getMostBusyWorker()
{
	int suffix = 0;
	utl::uint32_t temp = m_pWorkers->queueSize();
	for (int i = 0; i < m_iPoolSize; i++)
	{
		if (temp < (m_pWorkers + i)->queueSize())
		{
			temp = (m_pWorkers + i)->queueSize();
			suffix = i;
		}
	}
	return (m_pWorkers + suffix);
};

};//end namespace detail;
};//end namespace ff;
