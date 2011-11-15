/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_MSG_PORT_H
#define FFCPP_UTILS_MSG_PORT_H

#include <boost/noncopyable.hpp>
#include <boost/tr1/memory.hpp>
#include "msg_port_internal.h"
#include "msg_switch.h"

#include <vector>
#include <cassert>

namespace utl
{
  template <class MT_>
  class MsgSwitch;
  
  template <class MT_>
  class MsgPort : public boost::noncopyable
  {
  public:
    typedef MT_         MsgType_t;
    typedef MsgPort<MsgType_t>      MsgPort_t;
    typedef MsgSwitch<MsgType_t>    MsgSwitch_t;

  public:
    MsgPort();
    virtual ~MsgPort();

    inline void						setMsgSwitch(MsgSwitch_t &msgSwitch){m_pMsgSwitch = &msgSwitch;}
    void                            registerInterestTypes( const std::vector<int> &oTypes);

    void                            unregister();

    inline  const std::vector<int> & getInterestTypes() const {return m_pMsgPort->getInterestTypes();}

    inline void                     sendUniqMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg)
    {
    	m_pMsgSwitch->sendUniqMsg(iMsgType, pMsg);
    }
    inline void                     sendMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg)
    {
    	m_pMsgSwitch->sendMsg(iMsgType, pMsg);
    };

    inline void                     pushBackMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg)
    {
      m_pMsgPort->pushBackMsg(iMsgType, pMsg);
    };

    inline size_t                   msgQueueSize() const {return m_pMsgPort->msgQueueSize();};

    inline int                      nonBlockGetMsg(std::tr1::shared_ptr<MsgType_t> &pMsg)
    {
      return m_pMsgPort->nonBlockGetMsg(pMsg);
    }

    inline int                      getMsg(std::tr1::shared_ptr<MsgType_t> &pMsg)
    {
      return m_pMsgPort->getMsg(pMsg);
    }

  protected:
    typedef utl::MsgPortInternal<MT_>                   MsgPortInternal_t;
    typedef std::tr1::shared_ptr<MsgPortInternal_t>     MsgPortInternalSharedPtr_t;

    MsgPortInternalSharedPtr_t      m_pMsgPort;
    MsgSwitch_t	*					m_pMsgSwitch;
  };

#include "inl/msg_port_inl.h"

};//end namespace utl;
#endif
