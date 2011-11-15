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

#ifndef UTL_THREADS_DETAILS_LINUX_SEMAPHORE_H_
#define UTL_THREADS_DETAILS_LINUX_SEMAPHORE_H_

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <cassert>
#include <boost/noncopyable.hpp>

#define EXCEPTION( bVal, reason ) \
	{ \
		if(bVal) \
		{	\
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
				int res = sem_init(&m_sem,0,iInitVal);
				EXCEPTION(res,"sem_init failed!");
			};
			inline ~semaphore()
			{
				int res = sem_destroy(&m_sem);
				EXCEPTION(res,"sem_destroy failed!");
			};

			inline void 	P( )
			{
				int res = sem_wait(&m_sem);
				EXCEPTION(res,"sem_wait failed!");
			};
			inline void	V( )
			{
				int res = sem_post(&m_sem);
				EXCEPTION(res,"sem_post failed!");
			};

		protected:
			sem_t			m_sem;
	};//end class semaphore
};//end namespace utl

#endif
