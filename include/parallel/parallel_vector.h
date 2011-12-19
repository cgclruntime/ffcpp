/************************************************************************/
/* AthrunArthur 2011.12.19 
This class is used for the array of parallel.
This is designed for performance consideration.
*/
/************************************************************************/
#ifndef FFCPP_PARALLEL_PARALLEL_ARRAY_H
#define FFCPP_PARALLEL_PARALLEL_ARRAY_H
#include <vector>
#include "common/decl_types.h"
#include "parallel/parallel.h"
#include "threadpool/function_thread_pool.h"


namespace ff
{
    
  extern details::FFFunctionThreadPool g_oThreadPool;
  using details::p_elem_t;
  
  template<typename signature, signature *func_sig>
  class parallel_vector
  {
  public:
    parallel_vector()
    : m_oVector()
    {
    }
    
    parallel_vector(int iSize)
    : m_oVector(iSize)
    {
      for(int i = 0; i < iSize; ++i)
      {
	p_elem_t e;
	e.pFunc = new Parallel<signature, func_sig>();
	m_oVector[i] = e;
      }
    }
    
    ~parallel_vector()
    {
      for(int i = 0; i < size(); ++i)
      {
	details::pfid_t t;
	Parallel<signature, func_sig> *p ;
	p = m_oVector[i].pFunc;
	t = p->identifier();
	m_oVector[i].pfid = t;
	p->setInCollection();
	delete p;
      }
      g_oThreadPool.scheduler().finishes().erase(m_oVector);
    }
    
    Parallel<signature, func_sig> &	at(size_t index)
    {
      return *(Parallel<signature, func_sig> *)(m_oVector.at(index).pFunc);
    };
    
    Parallel<signature, func_sig> &	back()
    {
      return *(Parallel<signature, func_sig> *)(m_oVector.back().pFunc);
    };
    size_t 				capacity(){
      return m_oVector.capacity();
    };
    void 				clear(){
      m_oVector.clear();
    };
    bool				empty(){
      return m_oVector.empty();
    };
    Parallel<signature, func_sig> &	front(){
      return *(Parallel<signature, func_sig> *)(m_oVector.front().pFunc);
    };
    void				erase(int index){
      int i = 0;
      for(parallel_vector_t::iterator it = m_oVector.begin(); it != m_oVector.end();++it)
      {
	if(i == index)
	{
	  m_oVector.erase(it);
	  break;
	}
	++i;
      }
    };
    Parallel<signature, func_sig> &	operator [](int index){
      return at(index);
    };
    void				pop_back(){
      return m_oVector.pop_back();
    };
    void				push_back(Parallel<signature, func_sig> * pFunc){
      p_elem_t e;
      e.pFunc = pFunc;
      m_oVector.push_back(e);
    };
    
    size_t				size()
    {
      return m_oVector.size();
    };
  protected:

    typedef std::vector<p_elem_t>	parallel_vector_t;
    
    parallel_vector_t m_oVector;
  };
  
  
};//end namespace



#endif

