/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/

#ifndef FFCPP_PARAM_TRAITS_H
#define FFCPP_PARAM_TRAITS_H

#include "common/comma_if.h"
#include "common/repeat.h"

//#define FF_COMMA_IF(cond)
#undef COMMA_IF_NOT_I1
#define COMMA_IF_NOT_I1
#define FF_EXTRACT_FUNCTION_TYPES(n) COMMA_IF_NOT(n) T ## n t ## n 
#define FF_ENUM_FUNCTION_TYPES(TEMPLATE_ARGS_NUM) FF_REPEAT(TEMPLATE_ARGS_NUM, FF_EXTRACT_FUNCTION_TYPES)


#define FF_EXTRACT_TYPE_ARG(n) COMMA_IF_NOT(n) typename T ## n 
#define FF_ENUM_PARAMS(TEMPLATE_ARGS_NUM) FF_REPEAT(TEMPLATE_ARGS_NUM, FF_EXTRACT_TYPE_ARG)

#undef COMMA_IF_NOT_I1
#define COMMA_IF_NOT_I1

#define FF_EXTRACT_TYPES(n)  COMMA_IF_NOT(n) T ## n 
#define FF_ENUM_TYPES(n)  FF_REPEAT(n, FF_EXTRACT_TYPES)

#define FF_EXTRACT_FUNC_PARAMS(n) COMMA_IF_NOT(n) t ## n 
#define FF_ENUM_FUNC_PARAMS(n) FF_REPEAT(n, FF_EXTRACT_FUNC_PARAMS)





#endif
