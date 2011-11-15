#ifndef FFCPP_THREAD_POOL_WAITS_DEFENDER_H_
#define FFCPP_THREAD_POOL_WAITS_DEFENDER_H_

#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include "utils/types.h"

namespace ff{
namespace details{

class ParallelBase;
class WaitTree;
class FFFunctionThreadPool;

typedef utl::uint32_t	pfid_t;


class WaitsDefender : public boost::noncopyable
{
public:
	WaitsDefender(FFFunctionThreadPool *pPool);
	~WaitsDefender(){};

	//! add <iPFID, pWait> into m_oWaits.
	/// Acquire a unique lock.
	void				addWait(pfid_t iPFID, WaitTree *pWait);

	//! add <pWait, pPF> into m_oWait2PFs.
	/// Acquire a unique lock.
	void				addWaitAndPF(WaitTree *pWait, ParallelBase *pPF);

	//! Process when a parallel function with iPFID has finished.
	void				handleFinishedPF(pfid_t iPFID);

	//! Erase corresponding infomation of iPFID.
	void				eraseAllInfoOfPF(pfid_t iPFID);

	WaitTree *			findWaitTree(pfid_t iPFID) const;

protected:
	typedef 	std::map<pfid_t, WaitTree *>   PFID2Waits_t;
	typedef 	std::map<WaitTree *, ParallelBase *>	Waits2PF_t;
	typedef		std::vector<WaitTree *>					Waits_t;
	typedef		std::map<pfid_t, Waits_t *>				ReversedWaits_t;

	typedef		boost::unique_lock<boost::shared_mutex> unique_lock;
	typedef 	boost::shared_lock<boost::shared_mutex>	shared_lock;


	void				addReversedWait(WaitTree *pWait);

	bool				findReversedWaits(pfid_t iPFID, std::vector<WaitTree *> & oWaits);

	void				createReversedWaitAndPFID(pfid_t iPFID, WaitTree *pWait);

	void				addReversedWaitsAtPos(ReversedWaits_t::iterator & it, WaitTree *pWait);

protected:

	PFID2Waits_t	m_oWaits;
	Waits2PF_t		m_oWait2PFs;
	ReversedWaits_t	m_oReversedWaits;
	boost::shared_mutex		m_oMutex;
	FFFunctionThreadPool *		m_pPool;
};

}//end namespace details
}//end namespace ff

#endif

