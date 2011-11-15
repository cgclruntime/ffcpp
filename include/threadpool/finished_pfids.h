/* AthrunArthur 2011.8.23
 * This class is used to maintain the IDs of finished parallel functions. */
#ifndef FFCPP_THREAD_POOL_FINISHED_PFIDS_H_
#define FFCPP_THREAD_POOL_FINISHED_PFIDS_H_

#include "utils/types.h"
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include "threadpool/circle_array.h"

namespace ff
{
namespace details{

typedef utl::uint32_t pfid_t;
class FFFunctionThreadPool;

class FinishedPFIDs{
public:
	FinishedPFIDs(FFFunctionThreadPool *pPool);
	~FinishedPFIDs();

	//! Add an id.
	void				pushBack(pfid_t id);

	//! To see if the id is in.
	bool				has(pfid_t id) const;

	//! Delete the id.
	void				erase(pfid_t id);

	//! If id is in, return true. If dead lock happenes, return false. Otherwise, block!
	bool				wait(pfid_t id);

	//! Notify the blocked thread to recheck.
	inline void				notify(){
		m_oConds.notify_all();
	}

private:
	bool				nonBlockedhas(pfid_t pfid) const;

protected:
	typedef boost::shared_mutex			shared_mutex;
	typedef boost::shared_lock<shared_mutex>	shared_lock;
	typedef boost::unique_lock<shared_mutex>	unique_lock;
	typedef boost::condition_variable_any		condition_variable_any;
	struct FlagedPFID_t{
		FlagedPFID_t() : id(0), bErased(true){};
		FlagedPFID_t & 		operator=(const FlagedPFID_t & pfid){
			id = pfid.id;
			bErased = pfid.bErased;
			return *this;
		}
		pfid_t			id;
		utl::uint8_t	bErased : 1;		///< Indicate the id has been erased;
	};
	mutable shared_mutex			m_oMutex;
	condition_variable_any			m_oConds;
	size_t							m_iTop;
	size_t							m_iBottom;
	CircleArray<FinishedPFIDs::FlagedPFID_t>		m_oPFIDs;
	FFFunctionThreadPool *			m_pPool;
	volatile utl::uint16_t			m_iWaitNumber;	//Indicate the number of waits, used to detect dead-lock
};//end class FinishedPFIDs;
};//end namespace details;
};//end namespace ff;
#endif

