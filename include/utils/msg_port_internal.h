/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_MSG_PORT_INTERNAL_H
#define FFCPP_UTILS_MSG_PORT_INTERNAL_H
#include <boost/noncopyable.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <vector>
#include <queue>

namespace utl
{

  template <class MT_>
  class MsgSwitch;

  template <class MT_>
  class MsgPort;

  template <class MT_>
  class MsgPortInternal : public boost::noncopyable
  {
  public:
    enum{
      invalid_msg_type = 0
    };
    typedef MT_                             MsgType_t;
    typedef MsgPortInternal< MsgType_t>     MsgPortInternal_t;

  public:
    MsgPortInternal(){};
    virtual ~MsgPortInternal(){};

    inline const std::vector<int> &           getInterestTypes() const{return m_oInterestTypes;};

    void                                      pushBackMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg);

    size_t                                    msgQueueSize() const;

    int                                       nonBlockGetMsg(std::tr1::shared_ptr<MsgType_t> &pMsg);

    int                                       getMsg(std::tr1::shared_ptr<MsgType_t> &pMsg);

  protected:
    friend class MsgSwitch<MsgType_t>;
    friend class MsgPort<MsgType_t>;

    void                                      addMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg);

  protected:
    typedef boost::condition_variable         ConditionVar_t;
    typedef boost::mutex                      Mutex_t;
    typedef boost::unique_lock<Mutex_t>       UniqueLock_t;
    typedef std::tr1::shared_ptr<MsgType_t>   MsgSharedPtr_t;
    typedef std::pair<int, MsgSharedPtr_t>    TypeAndMsg_t;
    typedef std::queue<TypeAndMsg_t>          MsgContainer_t;

    ConditionVar_t        m_oEmptyCond;
    mutable Mutex_t               m_oMutex;
    MsgContainer_t        m_oMsgs;
    std::vector<int>      m_oInterestTypes;
  };//end class MsgPortInternal;

#include "inl/msg_port_internal_inl.h"
};//end namespace utl;

#endif