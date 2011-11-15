/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_MSG_PORT_INL_H
#define FFCPP_UTILS_MSG_PORT_INL_H

template <class MT_>
MsgPort<MT_>::MsgPort()
: m_pMsgSwitch(NULL)
{
  m_pMsgPort = MsgPortInternalSharedPtr_t(new MsgPortInternal<MsgType_t>());
  if(!m_pMsgPort)
  {
    assert(0 && "memory alloc failed!");
    exit(0);
  }
}

template <class MT_>
MsgPort<MT_>::~MsgPort()
{
	//m_pMsgSwitch->unregisterMsgPort(m_pMsgPort);
}

template<class MT_>
void  MsgPort<MT_>::registerInterestTypes(const std::vector<int> &oTypes)
{
	m_pMsgSwitch->registerInterestTypes(m_pMsgPort, oTypes);
}

template <class MT_>
void  MsgPort<MT_>::unregister()
{
	m_pMsgSwitch->unregisterMsgPort(m_pMsgPort);
}

#endif
