/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 2010 Athrun Arthur	                                 */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

#ifndef UTL_THREADS_DETAILS_WINDOWS_SEMAPHORE_H_
#define UTL_THREADS_DETAILS_WINDOWS_SEMAPHORE_H_

#define _WINDOWS_MUTEX_DEATILS_MAX_SEMAPHORE_COUNT 1024

#include <windows.h>
#include <cassert>
#include <boost/noncopyable.hpp>

#define EXCEPTION( bVal, reason ) \
{ \
	if(bVal) {	\
		assert(0 && reason);	\
		exit(0);	\
	} \
}

namespace utl
{
  //! Semaphore
  /// This class is forbidden to copy or assign
class semaphore : public boost::noncopyable
{
public:
  inline semaphore(int iInitVal = 0)
  {
    m_hSem = CreateSemaphore(NULL,iInitVal,_WINDOWS_MUTEX_DEATILS_MAX_SEMAPHORE_COUNT, NULL);
    EXCEPTION(m_hSem==NULL,"CreateSemaphore failed!")
  };
  inline ~semaphore()
  {
    CloseHandle(m_hSem);
  };

  inline void	P()
  {
    WaitForSingleObject(m_hSem,INFINITE);
  };
  inline void	V()
  {
    ReleaseSemaphore(m_hSem,1,NULL);
  };

protected:
  HANDLE	 m_hSem;
};//end class Semaphore;

};//end namespace utl

#undef _WINDOWS_MUTEX_DEATILS_MAX_SEMAPHORE_COUNT

#endif

