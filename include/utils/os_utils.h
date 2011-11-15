/************************************************************************/
/* AthrunArthur 2011.6.30                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_OS_UTILS_H
#define FFCPP_UTILS_OS_UTILS_H

#if defined WIN32 || defined _WIN32
#include "details/windows_utils.h"
#else
#include "details/linux_utils.h"
#endif

#endif
