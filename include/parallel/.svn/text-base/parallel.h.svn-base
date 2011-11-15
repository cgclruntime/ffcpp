#ifndef FFCPP_PARALLEL_PARALLEL_H
#define FFCPP_PARALLEL_PARALLEL_H
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "parallel/func_type_traits.h"
#include "parallel/parallel_base.h"

#define WAIT_SINGLE \
	template <typename signature, signature * func_sig> \
	ParallelType_t &          wait(Parallel<signature, func_sig>& p){details::WaitNode n; \
	n.createWithPfid(p.identifier());	\
	details::WaitConds w;	\
	w.m_pWaitTree = details::WaitTree::createWithNode(n);	\
	ParallelBase::wait(w); return *this;}

// An instance of a normal class Parallel<>;
#define FF_EXTRACT_PARALLEL(n)       \
  template < typename R, FF_ENUM_PARAMS(n), typename ff::details::FuncTypeTraits<R ( FF_ENUM_TYPES(n) ) >::FunctionType_t   *  func_addr> \
class Parallel<R ( FF_ENUM_TYPES(n) ), func_addr> : public details::ParallelBase   \
{ \
public: \
  typedef   R     ReturnType_t; \
  typedef   R (FunctionType_t)(FF_ENUM_FUNCTION_TYPES(n));   \
  typedef   Parallel< R (FF_ENUM_TYPES(n) ), func_addr> ParallelType_t; \
  typedef   boost::function< R(void) >      ParallelFunctor_t; \
public: \
  Parallel():m_pFunc(func_addr), m_pReturn(NULL){};  \
  virtual ~Parallel(void){ waitFinish(); };  \
  virtual void              run(){  ParallelBase::run();    \
                                if(m_pReturn) (*m_pReturn) = m_oFunctor();  \
                                else m_oFunctor();};   \
                                \
  ParallelType_t &          ret(R & r) {m_pReturn = &r; return *this;}        \
  \
  void                      operator()( FF_ENUM_FUNCTION_TYPES(n) ){ m_oFunctor = boost::bind(m_pFunc, FF_ENUM_FUNC_PARAMS(n) ); schedule();};  \
  \
  ParallelType_t &          wait(details::WaitConds w){ParallelBase::wait(w); return *this;} \
  WAIT_SINGLE ; \
protected:                                          \
  FunctionType_t *          m_pFunc;                         \
  R *                       m_pReturn;              \
  ParallelFunctor_t         m_oFunctor;     \
};  \

//A set of normal Parallel<>;
#define FF_ENUM_PARALLEL(n) RR_REPEAT(n, FF_EXTRACT_PARALLEL)


//An instance of class Parallel<>, with void return type.
#define FF_EXTRACT_PARALLEL_VOID(n)       \
  template <FF_ENUM_PARAMS(n), typename ff::details::FuncTypeTraits<void ( FF_ENUM_TYPES(n) ) >::FunctionType_t   *  func_addr> \
class Parallel<void ( FF_ENUM_TYPES(n) ), func_addr> : public details::ParallelBase   \
{ \
public: \
  typedef   void     ReturnType_t; \
  typedef   void (FunctionType_t)(FF_ENUM_FUNCTION_TYPES(n));   \
  typedef   Parallel< void (FF_ENUM_TYPES(n) ), func_addr> ParallelType_t; \
  typedef   boost::function< void (void) >      ParallelFunctor_t; \
public: \
  Parallel():m_pFunc(func_addr){};  \
  virtual ~Parallel(void){ waitFinish(); };  \
  virtual void              run(){  ParallelBase::run();    \
  	  	  	  	  	  	  	  	  	  	  m_oFunctor();};   \
                                \
  \
  void                      operator()( FF_ENUM_FUNCTION_TYPES(n) ){ m_oFunctor = boost::bind(m_pFunc, FF_ENUM_FUNC_PARAMS(n) ); schedule();};  \
  \
  ParallelType_t &          wait(details::WaitConds w){ParallelBase::wait(w); return *this;} \
  WAIT_SINGLE ;\
protected:                                          \
  FunctionType_t *          m_pFunc;                         \
  ParallelFunctor_t         m_oFunctor;     \
};  \


#define FF_ENUM_PARALLEL_VOID(n) RR_REPEAT(n, FF_EXTRACT_PARALLEL_VOID)

namespace ff{
    ///!forward declaration
  template <typename signature, signature * func_addr >
  class Parallel{};

  ///!For function R (void)
  template < typename R, typename ff::details::FuncTypeTraits<R ( void ) >::FunctionType_t   *  func_addr>
  class Parallel<R ( void ), func_addr> : public details::ParallelBase
  {
  public:
          typedef   R     ReturnType_t;
          typedef   R (FunctionType_t)(void);
          typedef   Parallel< R (void ), func_addr> ParallelType_t;
          typedef   boost::function< R(void) >      ParallelFunctor_t;
  public:
          Parallel():m_pFunc(func_addr), m_pReturn(NULL){};
          virtual ~Parallel(void){ waitFinish();};
          virtual void              run(){
              ParallelBase::run();
              if(m_pReturn)
                (*m_pReturn) = m_oFunctor();
              else
                m_oFunctor();
              };

          ParallelType_t &          ret(R & r) {m_pReturn = &r; return *this;}

          void                      operator()( ){ m_oFunctor = boost::bind(m_pFunc ); schedule();};

          ParallelType_t &          wait(details::WaitConds w){ParallelBase::wait(w);return *this;}

          WAIT_SINGLE;
  protected:
          FunctionType_t *          m_pFunc;
          R *                       m_pReturn;
          ParallelFunctor_t         m_oFunctor;
  };

  ///!For function void (void)
    template <typename ff::details::FuncTypeTraits<void ( void ) >::FunctionType_t   *  func_addr>
    class Parallel<void ( void ), func_addr> : public details::ParallelBase
    {
    public:
            typedef   void (FunctionType_t)(void);
            typedef   Parallel< void (void ), func_addr> ParallelType_t;
            typedef   boost::function< void (void) >      ParallelFunctor_t;
    public:
            Parallel():m_pFunc(func_addr){};
            virtual ~Parallel(void){ waitFinish();};
            virtual void              run(){
                ParallelBase::run();
                  m_oFunctor();
                };


            void                      operator()( ){ m_oFunctor = boost::bind(m_pFunc ); schedule();};

            ParallelType_t &          wait(details::WaitConds w){ParallelBase::wait(w);return *this;}

            WAIT_SINGLE;
    protected:
            FunctionType_t *          m_pFunc;
            ParallelFunctor_t         m_oFunctor;
    };


  FF_ENUM_PARALLEL(30)

  FF_ENUM_PARALLEL_VOID(30)
};//end namespace ff;

#endif
