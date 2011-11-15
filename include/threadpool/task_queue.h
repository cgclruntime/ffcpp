/************************************************************************/
/* AthrunArthur 2011.8.17
 * This file declares class TaskQueue, a key component for work stealing.
 * In our work stealing, each thread has a TaskQueue. Each thread calls
 * popBottom() to get a task to execute. If popBottom() retruns NULL, it
 * try to "steal" another task from other TaskQueue, by calling getBottom().
 * The method pushTop() is used to add a task in the task queue.
 *
 * As the method pushTop(), getBottom() can be called by
 * multiple threads, they must be multi-thread safe. And popBottom() is only
 * called by one thread
 *
 * It's not smart to use locks or mutexs to achieve threads safe, as the
 * TaskQueue is a key performance awaring component. So we use atomic access
 * to avoid performance bottleneck.                                               */
/************************************************************************/


#ifndef FFCPP_THREAD_POOL_TASK_QUEUE_H_
#define FFCPP_THREAD_POOL_TASK_QUEUE_H_

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

#include "utils/atomic_stamped_reference.h"
#include "threadpool/circle_array.h"
#include "common/log.h"


namespace ff{
namespace details{

template <typename Task_t>
class TaskQueue : public boost::noncopyable
{
public:
	explicit TaskQueue(utl::uint32_t iCapacity)
	: m_oTop(0)
	, m_oBottom(0)
	, m_oArray(iCapacity)
	, m_oInvalidFlags(iCapacity)
	{};

	//! Get the size of the TaskQueue.
	/// Although the TaskQueue is accessed by multiple threads,
	/// we still not involve atomic access in this function.
	/// Because we don't need accuracy size of the queue!!!
	inline size_t				size() const
	{
		return (m_oTop.get() - m_oBottom.get());
	};

	void 				pushTop(Task_t *pTask)
	{
		if(isTopCloseToBottom())
		{
			m_oMutex.lock();
			if(isTopCloseToBottom())
			{
				utl::Atomic<utl::mem>::sync();
				m_oArray.resizeAndCopy(64, m_oTop.get(), m_oBottom.get());
				m_oInvalidFlags.resizeAndCopy(64, m_oTop.get(), m_oBottom.get());
#ifdef _DEBUG
				log_taskq("TaskQueue",
						"pushTop(), resize, top:" + utl::str(m_oTop.get())+
						" bottom:"+utl::str(m_oBottom.get()));
#endif
			}
			m_oMutex.unlock();
		}
		utl::int32_t oldTop, newTop;
		utl::uint32_t oldStamp, newStamp;
		if(isBottomCloseToTop())
		{
			m_oMutex.lock();
			oldTop = m_oTop.get(oldStamp);
			m_oTop.set(oldTop+1, oldStamp+1);
			m_oArray[oldTop] = pTask;
			m_oInvalidFlags[oldTop] = false;
#ifdef _DEBUG
			log_taskq("TaskQueue", "pushTop(), mutex push location:"+ utl::str(oldTop));
#endif
			m_oMutex.unlock();
			return ;
		}

		bool flag = false;
		while(!flag)
		{
			if(isTopCloseToBottom())
			{
				m_oMutex.lock();
				if(isTopCloseToBottom())
				{
					m_oArray.resizeAndCopy(64, m_oTop.get(), m_oBottom.get());
					m_oInvalidFlags.resizeAndCopy(64, m_oTop.get(), m_oBottom.get());
				}
				m_oMutex.unlock();
			}
			oldTop = m_oTop.get(oldStamp);
			newTop = oldTop +1, newStamp = oldStamp+1;
			flag = m_oTop.compareAndSet(oldTop, newTop, oldStamp, newStamp);
		}
#ifdef _DEBUG
		log_taskq("TaskQueue", "pushTop(), shared push location:"+utl::str(oldTop));
#endif
		m_oArray[oldTop] = pTask;
		m_oInvalidFlags[oldTop] = false;
	};

	Task_t *			popBottom()
	{
		if(m_oBottom.equalReference(m_oTop))
			return NULL;

		utl::int32_t oldBottom, newBottom;
		utl::uint32_t oldStamp, newStamp;
		if(isBottomCloseToTop())
		{
			m_oMutex.lock();
			bool flag_get = false;
			while(!flag_get)
			{
				if(m_oBottom.get() == (m_oTop.get()))
				{
					m_oMutex.unlock();
					return NULL;
				}
				oldBottom = m_oBottom.get(oldStamp);
				m_oBottom.set(oldBottom+1, oldStamp+1);
				if(m_oInvalidFlags[oldBottom] == false)
					flag_get = true;
			}
			m_oMutex.unlock();
#ifdef _DEBUG
			log_taskq("TaskQueue",
					"popBottom(), mutex pop location:"+utl::str(oldBottom)+
					" new bottom:"+utl::str(m_oBottom.get()));
#endif
			return m_oArray[oldBottom];
		}

		bool flag = false;
		while(!flag)
		{
			if(m_oBottom.equalReference(m_oTop))
				return NULL;
			oldBottom = m_oBottom.get(oldStamp);
			newBottom = oldBottom+1, newStamp = oldStamp +1;
			flag = m_oBottom.compareAndSet(oldBottom, newBottom, oldStamp, newStamp);
			flag = flag && (!m_oInvalidFlags[oldBottom]);
		}
		utl::Atomic<utl::mem>::sync();
#ifdef _DEBUG
		log_taskq("TaskQueue",
				"popBottom(), shared pop location:"+utl::str(oldBottom)+
				" new bottom:"+utl::str(m_oBottom.get()));
#endif
		return m_oArray[oldBottom];
	}

	Task_t *			getBottom(){
		if(isBottomCloseToTop())
			return NULL;

		utl::int32_t oldBottom, newBottom;
		utl::uint32_t oldStamp, newStamp;

		bool flag = false;

		while(!flag)
		{
			if(isBottomCloseToTop())
				return NULL;
			oldBottom = m_oBottom.get(oldStamp);
			newBottom = oldBottom+1, newStamp = oldStamp +1;
			flag = m_oBottom.compareAndSet(oldBottom, newBottom, oldStamp, newStamp);
			flag = flag && (!m_oInvalidFlags[oldBottom]);
		}
		return m_oArray[oldBottom];
	};

	//! Set the specified task invalid
	void				invalidTask(Task_t *pTask)
	{
		for(utl::int32_t i = m_oBottom.get(); i < m_oTop.get(); ++i)
		{
			if(m_oArray[i] == pTask)
			{
				m_oInvalidFlags[i] = true;
#ifdef _DEBUG
				log_taskq("TaskQueue", "invalidTask(), invalid pos:"+utl::str(i));
#endif
			}
		}
	}

protected:
	//! To see if the top is reaching to the bottom.
	/// If they are close, return true. Or return false.
	inline bool				isTopCloseToBottom()
	{
		utl::Atomic<utl::mem>::sync();
		return (static_cast<utl::uint32_t>(m_oTop.get()-m_oBottom.get() + 2) >= m_oArray.getCapacity());
	}

	//! To see if the bottom is reaching to the top.
	/// If they are close, return true, or return false.
	inline bool				isBottomCloseToTop()
	{
		utl::Atomic<utl::mem>::sync();
		return (m_oTop.get()-m_oBottom.get()<=2);
	}

protected:
	mutable utl::AtomicStampedReference<utl::int32_t>		m_oTop;
	mutable utl::AtomicStampedReference<utl::int32_t>		m_oBottom;
	ff::details::CircleArray<Task_t *>				m_oArray;
	ff::details::CircleArray<bool>					m_oInvalidFlags;//If the flag is true, then the corresponding Task_t * is invalid;
	boost::mutex				m_oMutex;
};//end class TaskQueue

};//end namespace details;
};//end namespace ff;

#endif

