/************************************************************************/
/* AthrunArthur 2011.7.25                                               */
/************************************************************************/
#ifndef UTILS_ALLOCATOR_H
#define UTILS_ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>


namespace utl
{

  template<size_t Align = 2>
  class allocator{
  public:
    allocator()
      : m_iAlign(Align){}
    void *        allocate(size_t iBytes)
    {
      if(iBytes == 0)
        iBytes = 1;
      void *res;
#ifdef WIN32
      res = _aligned_malloc(iBytes, m_iAlign);
#else
      posix_memalign(&res, m_iAlign, iBytes);
#endif
      return res;
    }

    void         deallocate(void *pAddr)
    {
      free(pAddr);
    }
  protected:
    const size_t    m_iAlign;
  };
};//end namespace utl

template<size_t Align>
inline void *     operator new(size_t iBytes, utl::allocator<Align> & a)
{
  return a.allocate(iBytes);
}

template<size_t Align>
inline void      operator delete(void *pAddr, utl::allocator<Align> & a)
{
  return a.deallocate(pAddr);
}

#endif

