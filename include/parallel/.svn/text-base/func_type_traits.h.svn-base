/* This file defines class FuncTypeTraits.
 * FuncTypeTraits is used to achieve the type of function's template.
 * */
#ifndef FFCPP_COMMON_FUNC_TYPE_TRAITS_H_
#define FFCPP_COMMON_FUNC_TYPE_TRAITS_H_

#include "common/param_traits.h"



#define FF_EXTRACT_FUNC_TYPE_TRAITS(n) \
  template <typename R, FF_ENUM_PARAMS(n)>  \
class FuncTypeTraits <R ( FF_ENUM_TYPES(n) )> \
{ \
public: \
  typedef R (FunctionType_t)(FF_ENUM_FUNCTION_TYPES(n));  \
};  \


#define FF_ENUM_FUNC_TYPE_TRAITS(n) RR_REPEAT(n, FF_EXTRACT_FUNC_TYPE_TRAITS)


namespace ff{
  namespace details{
    ///! forward declaration
    template <typename signature>
    class FuncTypeTraits{};

    ///! For function R (void)
    template <typename R>  
    class FuncTypeTraits <R ( void )> 
    { 
    public: 
            typedef R (FunctionType_t)(void);  
    };  

    ///! For function R (...)
    FF_ENUM_FUNC_TYPE_TRAITS(30)

  };//end namespace details
};//end namespace ff

#endif


