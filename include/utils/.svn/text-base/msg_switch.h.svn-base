/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_MSG_SWITCH_H
#define FFCPP_UTILS_MSG_SWITCH_H

#include <boost/noncopyable.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/once.hpp>
#include <vector>
#include <map>
#include <iostream>


namespace utl
{
  template <class MT_>
  class MsgPortInternal;

  template <class MT_>
  class MsgPort;

  //! Message switch.
  /// This class is to switch messages, and dispatch them to corresponding
  /// message ports. Generally, this class shouldn't be used explicitly. 
  /// Users can get some necessary information from this class. And this 
  /// class is singleton.
  /// \param MT_, the type of each message.
  template <class MT_>
  class MsgSwitch : public boost::noncopyable
  {
  public:
    typedef MT_                         MsgType_t;
    typedef MsgSwitch<MsgType_t>        MsgSwitch_t;
    typedef MsgPortInternal<MsgType_t>  MsgPortInternal_t;

  protected:
    typedef std::tr1::shared_ptr<MsgPortInternal_t>     MsgPortInternalSharedPtr_t;
    typedef std::tr1::weak_ptr<MsgPortInternal_t>       MsgPortInternalWeakPtr_t;
    typedef std::vector<MsgPortInternalWeakPtr_t>       MsgPortsContainer_t;
    typedef std::map<int, std::tr1::shared_ptr<MsgPortsContainer_t> >   Type2MsgPortsContainer_t;
    typedef boost::shared_mutex                         SharedMutex_t;
    typedef boost::shared_lock<SharedMutex_t>           SharedLock_t;
    typedef boost::unique_lock<SharedMutex_t>           UniqueLock_t;
    typedef std::tr1::shared_ptr<MsgType_t>             MsgTypeSharedPtr_t;
  public:
    MsgSwitch(){};
    virtual ~MsgSwitch();

    //! Get the number of total message ports.
    size_t              getMsgPortsNumber() const;

    //! Get the number of registered message types.
    size_t              getMsgTypesNumber() const;

  protected:
    friend class MsgPortInternal<MsgType_t>;
    friend class MsgPort<MsgType_t>;

    //! Send the msg to "only one" message port.
    /// 
    void                sendUniqMsg(int iMsgType, const MsgTypeSharedPtr_t & pMsg);

    //! Multicast the msg to all message ports.
    void                sendMsg(int iMsgType, const MsgTypeSharedPtr_t & pMsg);

    void                registerInterestTypes(const MsgPortInternalSharedPtr_t &pPort, const std::vector<int> &oMsgTypes);

    void                unregisterMsgPort(const MsgPortInternalSharedPtr_t &pPort);

  private:
    Type2MsgPortsContainer_t    m_oPorts;
    mutable SharedMutex_t       m_oMutex;
    int                         m_iPortsNumber;
  };//end class MsgSwitch

#include "inl/msg_switch_inl.h"

};//end namespace utl;
#endif
