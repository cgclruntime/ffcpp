/************************************************************************/
/* AthrunArthur 2011.12.19 
This file is for foreward type declaration.
*/
/************************************************************************/
#ifndef PARALLEL_COMMON_DECL_TYPES_H_
#define PARALLEL_COMMON_DECL_TYPES_H_

#include "utils/types.h"

namespace ff
{
  namespace details
  {
    typedef utl::uint32_t pfid_t;
    class ParallelBase;

typedef union
{
    pfid_t	pfid;
    ParallelBase *pFunc;
}p_elem_t;

  };//end namespace details
};//end namespace ff

#endif

