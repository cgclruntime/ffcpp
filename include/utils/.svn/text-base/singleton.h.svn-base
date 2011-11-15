/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_SINGLETON_H
#define FFCPP_UTILS_SINGLETON_H

#include <boost/noncopyable.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/thread/once.hpp>

namespace utl
{
  //! Template singleton
  /// This singleton is multiple thread safe.
  template <class Ty_>
  class Singleton : public boost::noncopyable
  {
  public:
    //! Get the pointer.
    static std::tr1::shared_ptr<Ty_>          getInstance()
    {
      if(!s_pInstance)
        boost::call_once(s_oSingletonFlag, Singleton<Ty_>::initializeInstance);
      assert(s_pInstance.get() &&"unexpected null pointer!");
      return s_pInstance;
    };
    virtual ~Singleton(){};

    static void       initializeInstance()
    {
      s_pInstance.reset(new Ty_());
      assert(s_pInstance.get() && "singleton alloc failed!");
    };

  protected:
    Singleton(){};    

  private:
    static boost::once_flag     s_oSingletonFlag;
    static std::tr1::shared_ptr<Ty_>      s_pInstance;
  };

  template < class Ty_>
  boost::once_flag    Singleton<Ty_>::s_oSingletonFlag = BOOST_ONCE_INIT;

  template < class Ty_>
  std::tr1::shared_ptr<Ty_> Singleton<Ty_>::s_pInstance;
}; //end namespace utl;

#endif

