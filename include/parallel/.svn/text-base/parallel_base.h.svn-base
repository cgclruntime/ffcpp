/************************************************************************/
/* AthrunArthur 2011.7.3 
This class is for separating the template arguments from most members.
*/
/************************************************************************/
#ifndef FFCPP_PARALLEL_PARALLEL_BASE_H
#define FFCPP_PARALLEL_PARALLEL_BASE_H
#include <boost/noncopyable.hpp>
#include "utils/types.h"
#include "parallel/wait_conds.h"
#ifdef _DEBUG
#include <string>
#endif

namespace ff {
namespace details {
typedef utl::uint32_t pfid_t;
class ParallelBase: public boost::noncopyable {
protected:
	//! Constructor
	/// In this constructor, m_iIdentifier must be initialized using FFIDGenerator,
	ParallelBase();
public:
	//! Deconstructor
	/// This should block until this functor is over.
	virtual ~ParallelBase();

	//! Call the binded functor.
	/// This function is used in worker thread.
	virtual void run();

	//! Put itself into threadpool;
	void schedule();

	//! Wait some specified conditions.
	void wait(WaitConds w);

#ifdef _DEBUG
	//! For log and debug!
	std::string toString() const;
#endif

	//! Wait for the finish of its execution.
	/// This is called at the deconstruct time.
	void						waitFinish();

	// Get and set methods.
	inline pfid_t &				identifier(){return m_iIdentifier;}
	inline const pfid_t &		identifier() const {return m_iIdentifier;}

	inline void					setConstructed()	{m_iState.D.s3 = state_constructed;};
	inline void					setCalledParen()	{m_iState.D.s3 = state_called_paren_operator;};
	inline void					setRunning()	{m_iState.D.s1 = state_running;};
	inline void					setExeOver()	{m_iState.D.s1 = state_exe_over;};
	inline void					setWaitSemaphore(){m_iState.D.s2 = state_wait_semaphore;};
	inline void					setDeconstructed(){m_iState.D.s3 = state_deconstructing;};

	inline bool					isConstructed() const {return m_iState.D.s3 == state_constructed;};
	inline bool					isCalledParen() const {return m_iState.D.s3 == state_called_paren_operator;};
	inline bool					isRunning()	const {return m_iState.D.s1 == state_running;};
	inline bool					isExeOver() const {return m_iState.D.s1 == state_exe_over;};
	inline bool					isWaitSemaphore() const	{return m_iState.D.s2 == state_wait_semaphore;};
	inline bool					isDeconstructed() const	{return m_iState.D.s3 == state_deconstructing;};

protected:
	friend class FFFunctionScheduler;
	friend class FFFunctionWorker;
	friend class FFFunctionThreadPool;
	enum {
		state_constructed = 1,
		state_called_paren_operator = 0x2,
		state_deconstructing = 0x3
	};
	enum {
		state_running = 0x1,
		state_exe_over = 0x2,
		state_wait_semaphore = 0x3,
	};

	struct State{
		utl::uint8_t 	state;	// no use.
		struct {
			utl::uint8_t s1 : 4; //can be state_running, state_exe_over
			utl::uint8_t s2 : 2; //can be state_wait_semaphoer
			utl::uint8_t s3 : 2; //can be state_constructed, state_deconstructing and state_called_paren_operator
		}D;
	};
	///< The identifier of the parallel function
	pfid_t 		m_iIdentifier;
	State		m_iState;	///< The state of the parallel function.

	static volatile utl::uint32_t s_iIdentifier; //To generate the identifier;
};//end class ParallelBase;


}//end namespace details;
}//end namespace ff;

#endif

