/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_INL_MSG_PORT_INTERNAL_INL_H
#define FFCPP_UTILS_INL_MSG_PORT_INTERNAL_INL_H


template <class MT_>
void MsgPortInternal<MT_>::pushBackMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg)
{
  addMsg(iMsgType, pMsg);
}

template <class MT_>
size_t MsgPortInternal<MT_>::msgQueueSize() const
{
  UniqueLock_t l(m_oMutex);
  return m_oMsgs.size();
}

template <class MT_>
int MsgPortInternal<MT_>::nonBlockGetMsg(std::tr1::shared_ptr<MsgType_t> &pMsg)
{
  UniqueLock_t l(m_oMutex);

  if(m_oMsgs.empty())
    return invalid_msg_type;

  typename MsgPortInternal<MT_>::TypeAndMsg_t & e = m_oMsgs.front();
  pMsg = e.second;
  m_oMsgs.pop();
  return e.first;
};

template <class MT_>
int MsgPortInternal<MT_>::getMsg(std::tr1::shared_ptr<MsgType_t> &pMsg)
{
  UniqueLock_t l(m_oMutex);
  
  while(m_oMsgs.empty())
  {
    m_oEmptyCond.wait(l);
  }

  typename MsgPortInternal<MT_>::TypeAndMsg_t &e = m_oMsgs.front();
  pMsg = e.second;
  m_oMsgs.pop();
  return e.first;
};

template <class MT_>
void MsgPortInternal<MT_>::addMsg(int iMsgType, const std::tr1::shared_ptr<MsgType_t> &pMsg)
{
  UniqueLock_t l(m_oMutex);
  typename MsgPortInternal<MT_>::TypeAndMsg_t e;
  e.first = iMsgType;
  e.second = pMsg;
  m_oMsgs.push(e);

  if(m_oMsgs.size() == 1)
    m_oEmptyCond.notify_all();
};

#endif
