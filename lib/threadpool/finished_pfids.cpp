#include "threadpool/finished_pfids.h"
#include "threadpool/function_thread_pool.h"
#include "threadpool/function_scheduler.h"
#include "threadpool/eager_scheduler.h"
#include "utils/atomic.h"
#include "common/log.h"


namespace ff{
namespace details{

FinishedPFIDs::FinishedPFIDs(FFFunctionThreadPool *pPool)
: m_oMutex()
, m_oConds()
, m_iTop(0)
, m_iBottom(0)
, m_oPFIDs()
, m_pPool(pPool)
, m_iWaitNumber(0)
{
}



FinishedPFIDs::~FinishedPFIDs()
{
}


void FinishedPFIDs::pushBack(pfid_t pfid)
{
	unique_lock _t(m_oMutex);
#ifdef _DEBUG
	log_finishes("FinishedPFIDs", "pushBack(), pfid: " + utl::str(pfid) );
#endif
	if(m_iTop-m_iBottom+2>=m_oPFIDs.getCapacity())
		m_oPFIDs.resizeAndCopy(64,m_iTop, m_iBottom);
	m_oPFIDs[m_iTop].id = pfid;
	m_oPFIDs[m_iTop].bErased = false;
	m_iTop ++;
}



bool FinishedPFIDs::has(pfid_t pfid) const
{
	shared_lock _t(m_oMutex);
	for(size_t i = m_iBottom; i < m_iTop; ++i)
	{
		if(!(m_oPFIDs[i].bErased) && m_oPFIDs[i].id == pfid)
			return true;
	}
	return false;
}

bool FinishedPFIDs::nonBlockedhas(pfid_t pfid) const
{
#ifdef _DEBUG
	log_finishes("FinishedPFIDs", "nonBlockedhas(), bottom:"+ utl::str(m_iBottom)+" top:"+utl::str(m_iTop));
	for(size_t i = m_iBottom; i < m_iTop; ++i)
	{
		log_finishes("FinishedPFIDs", "nonBlockedhas(), "+utl::str(i)+" : "+utl::str(m_oPFIDs[i].id));
	}
#endif
	for(size_t i = m_iBottom; i < m_iTop; ++i) {
		if ((!m_oPFIDs[i].bErased) && m_oPFIDs[i].id == pfid)
			return true;
	}
	return false;
}


void FinishedPFIDs::erase(pfid_t pfid)
{
	if(m_oPFIDs[m_iBottom].id == pfid)
	{
		unique_lock _t(m_oMutex);
		m_oPFIDs[m_iBottom].bErased = true;
		for(size_t i = m_iBottom; i < m_iTop && m_oPFIDs[i].bErased; ++i) m_iBottom = i;
		m_iBottom ++;
		return ;
	}
	shared_lock _t(m_oMutex);
	for(size_t i = m_iBottom; i < m_iTop; ++i)
	{
		if(m_oPFIDs[i].id == pfid)
		{
			m_oPFIDs[i].bErased = true;
		}
	}
}



bool FinishedPFIDs::wait(pfid_t pfid)
{
	shared_lock _t(m_oMutex);
	bool flag = nonBlockedhas(pfid);
#ifdef _DEBUG
	log_finishes("FinishedPFIDs", "wait(), checking: " + utl::str(pfid) + " ret:" + utl::str(flag));
#endif
	if(flag)
		return true;

	EagerScheduler & E = m_pPool->scheduler().eager();
	int iPoolSize = m_pPool->poolSize();

	while(!flag)
	{
#ifdef _DEBUG
		log_finishes("FinishedPFIDs", "wait(), waiting: " + utl::str(pfid));
#endif
		utl::Atomic<utl::uint16_t>::inc(&m_iWaitNumber);
		if(m_iWaitNumber > iPoolSize)	//dead-lock happenes!!
		{
#ifdef _DEBUG
			log_finishes("FinishedPFIDs", "wait(), dead-lock happens! pool size:" + utl::str(iPoolSize) + ", waits number:"+utl::str(m_iWaitNumber));
#endif
			if(E.getState() == EagerScheduler::state_null)
				E.setDeadLock();
			else if(E.getState() == EagerScheduler::state_dead_lock)
			{
				E.eagerSchedulePFs();
				E.setScheduled();
			}
			utl::Atomic<utl::uint16_t>::dec(&m_iWaitNumber);
			notify();//wait again.
			return false;
		}
		else {
			m_oConds.wait(_t);
			utl::Atomic<utl::uint16_t>::dec(&m_iWaitNumber);
			flag = nonBlockedhas(pfid);
			if(E.getState() != EagerScheduler::state_null)
				return flag;
		}//end else
	}//end while
}


};//end namespace details;
};//end namespace ff;
