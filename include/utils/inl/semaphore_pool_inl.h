
template<int iSize>
SemaphorePool<iSize>::SemaphorePool()
{
  for(int i = 0; i < iSize; ++i)
  {
    m_oFreeSems.push_back(new utl::semaphore());
  }
}

template<int iSize>
SemaphorePool<iSize>::~SemaphorePool()
{
  boost::unique_lock<boost::shared_mutex> l(m_oMutex);

  for(SemsContainer::iterator i = m_oFreeSems.begin(); i != m_oFreeSems.end(); ++i)
  {
    delete (*i);
  }
  LOG_DEBUG("Semaphore","~SemaphorePool(), delete all sems");
  //we don't free the using semaphores
}

template<int iSize>
semaphore * SemaphorePool<iSize>::getSemaphore()
{
  utl::semaphore * pSem = NULL;
  boost::unique_lock<boost::shared_mutex> l(m_oMutex);

  if(m_oFreeSems.empty())
    pSem = new utl::semaphore();
  else{
    pSem = m_oFreeSems.front();
    m_oFreeSems.pop_front();
  }

  m_oUsedSems.push_back(pSem);
  return pSem;
}

template<int iSize>
void  SemaphorePool<iSize>::releaseSemaphore(const semaphore * pSema)
{
  boost::unique_lock<boost::shared_mutex> l(m_oMutex);

  for(SemsContainer::iterator i = m_oUsedSems.begin(); i < m_oUsedSems.end(); ++i)
  {
    if((*i) == pSema)
    {
      m_oFreeSems.push_back(*i);
      m_oUsedSems.erase(i);
      break;
    }
  }
}

template<int iSize>
size_t SemaphorePool<iSize>::getPoolSize() const{
  boost::unique_lock<boost::shared_mutex> l(m_oMutex);

  return m_oUsedSems.size() + m_oFreeSems.size();
}