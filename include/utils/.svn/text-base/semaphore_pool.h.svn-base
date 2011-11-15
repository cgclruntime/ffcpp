/************************************************************************/
/* AthrunArthur 2011.7.3                                                                     */
/************************************************************************/
#ifndef FFCPP_UTILS_SEMAPHORE_POOL_H
#define FFCPP_UTILS_SEMAPHORE_POOL_H

//! Pool of semaphores, and this is a singleton.
#include <deque>
#include "semaphore.h"
#include "singleton.h"
#include "log.h"

namespace utl{

template<int size = 32>
class SemaphorePool : public Singleton<SemaphorePool<size> >
{
protected:
  friend class Singleton<SemaphorePool>;
  SemaphorePool();
public:

  virtual ~SemaphorePool();

  //! Get a semaphore.
  /// This should be multi-thread safe.
  /// \return never return null.
  semaphore *           getSemaphore();

  //! Recycle a semaphore.
  /// This should be multi-thread sate.
  /// Remember checking that pSema == NULL.
  void                  releaseSemaphore(const semaphore * pSema);

  //! Get the pool size.
  size_t                  getPoolSize() const;

protected:
  typedef std::deque<utl::semaphore *>      SemsContainer;

  SemsContainer                         m_oUsedSems;
  SemsContainer                         m_oFreeSems;
  mutable boost::shared_mutex                   m_oMutex;
};//end class SemaphorePool

#include "inl/semaphore_pool_inl.h"

};//end namespace utl;

#endif

