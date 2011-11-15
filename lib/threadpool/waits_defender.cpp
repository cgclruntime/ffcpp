#include "threadpool/waits_defender.h"
#include "parallel/parallel_base.h"
#include "parallel/wait_tree.h"
#include "threadpool/function_thread_pool.h"
#include "common/log.h"

namespace ff{
namespace details{
WaitsDefender::WaitsDefender(FFFunctionThreadPool *pPool)
: m_pPool(pPool){
}

void WaitsDefender::addWait(pfid_t iPFID, WaitTree *pWait)
{
	unique_lock t(m_oMutex);
	m_oWaits.insert(std::make_pair(iPFID, pWait));
}

void WaitsDefender::addWaitAndPF(WaitTree *pWait, ParallelBase *pPF)
{
	unique_lock t(m_oMutex);
	m_oWait2PFs.insert(std::make_pair(pWait, pPF));
	addReversedWait(pWait);
#ifdef _DEBUG
	log_waits("WaitDefender", "addWaitAndPF(), "+pPF->toString()+" has wait tree: "+pWait->toString());
#endif
}

void WaitsDefender::handleFinishedPF(pfid_t iPFID)
{
	unique_lock t(m_oMutex);
	std::vector<WaitTree *> oWaits;
	if(!findReversedWaits(iPFID, oWaits))
		return ;
#ifdef _DEBUG
	log_waits("WaitsDefender", "handleFinishedPF(), "+utl::str(iPFID)+" get corresponding wait trees!");
#endif

	std::vector<WaitTree *> oFinishes;
	size_t waits_size = oWaits.size();

	//Get finished wait trees
	for(size_t i = 0; i < waits_size; ++i)
	{
#ifdef _DEBUG
		log_waits("WaitsDefender", "handleFinishedPF(), notify wait tree: "+oWaits[i]->toString());
#endif
		if(oWaits[i]->notifyFinish(iPFID))
			oFinishes.push_back(oWaits[i]);
	}

#ifdef _DEBUG
	for(size_t i = 0; i < oFinishes.size(); ++i)
	{
		log_waits("WaitsDefender", "handleFinishedPF() finished wait tree: "+oFinishes[i]->toString());
	}
#endif

	if(oFinishes.empty())
		return ;

	size_t finishes_size = oFinishes.size();
	for(size_t i = 0; i < finishes_size; ++i)
	{
		Waits2PF_t::iterator it = m_oWait2PFs.find(oFinishes[i]);
		if(it == m_oWait2PFs.end())
		{
#ifdef _DEBUG
			log_waits("WaitsDefender", "handleFinished(), not found pFunction with wait tree:" + oFinishes[i]->toString());
#endif
			continue;
		}
		m_pPool->scheduleTask(it->second);
	}
}

void WaitsDefender::eraseAllInfoOfPF(pfid_t iPFID)
{
	unique_lock t(m_oMutex);
	PFID2Waits_t::iterator it1 = m_oWaits.find(iPFID);
	if(it1 == m_oWaits.end())
		return ;

	WaitTree *pTree = it1->second;
	m_oWait2PFs.erase(pTree);
	m_oReversedWaits.erase(iPFID);
}

WaitTree * WaitsDefender::findWaitTree(pfid_t iPFID) const
{
	shared_lock(m_oMutexPFs);
	PFID2Waits_t::const_iterator it = m_oWaits.find(iPFID);
	if(m_oWaits.end() == it)
		return NULL;
	return it->second;
}


void WaitsDefender::addReversedWait(WaitTree *pWait)
{
	std::vector<pfid_t> oWaitPFIDs;
	pWait->getUnfinishedPFIDs(oWaitPFIDs);

	ReversedWaits_t::iterator it;
	for(size_t i = 0; i < oWaitPFIDs.size(); ++i)
	{
		it = m_oReversedWaits.find(oWaitPFIDs[i]);
		if(it == m_oReversedWaits.end())
			createReversedWaitAndPFID(oWaitPFIDs[i], pWait);
		else
			addReversedWaitsAtPos(it, pWait);
	}
}

bool WaitsDefender::findReversedWaits(pfid_t iPFID, std::vector<WaitTree *> & oWaits)
{
	ReversedWaits_t::iterator it = m_oReversedWaits.find(iPFID);
	if(it == m_oReversedWaits.end())
		return false;

	Waits_t * pTree = it->second;
	size_t s=pTree->size();

	for(size_t i = 0; i < s; ++i)
	{
		if(!pTree->at(i)->isFinished())
			oWaits.push_back(pTree->at(i));
	}
	return true;
}

void WaitsDefender::createReversedWaitAndPFID(pfid_t iPFID, WaitTree *pWait)
{
	Waits_t * pWaits = new Waits_t;
	pWaits->push_back(pWait);
	std::pair<ReversedWaits_t::iterator, bool>res = m_oReversedWaits.insert(std::make_pair(iPFID, pWaits));
	assert(res.second && "insert failed!");
}

void WaitsDefender::addReversedWaitsAtPos(ReversedWaits_t::iterator & it, WaitTree *pWait)
{
	Waits_t *p = it->second;
	p->push_back(pWait);
}

}//end namespace details
}//end namespace ff
