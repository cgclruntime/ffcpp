/************************************************************************/
/* AthrunArthur 2011.7.1                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_INL_MSG_SWITCH_INL_H
#define FFCPP_UTILS_INL_MSG_SWITCH_INL_H


template <class MT_>
MsgSwitch<MT_>::~MsgSwitch()
{
}

template <class MT_>
void MsgSwitch<MT_>::sendUniqMsg(int iMsgType, const MsgTypeSharedPtr_t & pMsg)
{
  SharedLock_t lock(m_oMutex);
  typename Type2MsgPortsContainer_t::const_iterator it = m_oPorts.find(iMsgType);
  if(m_oPorts.end() == it)
  {
    assert( 0 && "Unexpected message type! This may because: 1. try to send a message before message ports' registering, 2. try to send a message with wrong type, 3. didn't register interesting types.");
    return ;
  }
  std::tr1::shared_ptr<MsgPortsContainer_t> pPortsWithSameType = it->second;
  assert(pPortsWithSameType &&"Unexpected message type or no message port to receive this message type!");

  int iPos = rand()%static_cast<int>(pPortsWithSameType->size()); //TODO(athrunarthur@gmail.com), don't use random!!
  MsgPortInternalSharedPtr_t pPort(pPortsWithSameType->at(iPos));
  pPort->addMsg(iMsgType, pMsg);
}


template <class MT_>
void MsgSwitch<MT_>::sendMsg(int iMsgType, const MsgTypeSharedPtr_t & pMsg)
{
  SharedLock_t lock(m_oMutex);

  typename Type2MsgPortsContainer_t::const_iterator it = m_oPorts.find(iMsgType);
  if(m_oPorts.end() == it)
  {
    assert( 0 && "Unexpected message type! This may because: 1. try to send a message before message ports' registering, 2. try to send a message with wrong type, 3. didn't register interesting types.");
    return ;
  }

  std::tr1::shared_ptr<MsgPortsContainer_t> pPortsWithSameType = it->second;
  assert(pPortsWithSameType &&"Unexpected message type or no message port to receive this message type!");

  for(typename MsgPortsContainer_t::size_type i = 0; i < pPortsWithSameType->size(); ++i)
  {
    MsgPortInternalSharedPtr_t pPort(pPortsWithSameType->at(i));
    if(pPort)
      pPort->addMsg(iMsgType, pMsg);
  }
};

template <class MT_>
void MsgSwitch<MT_>::registerInterestTypes(const MsgPortInternalSharedPtr_t &pPort, const std::vector<int> &oMsgTypes)
{
  UniqueLock_t lock(m_oMutex);

  typename Type2MsgPortsContainer_t::iterator it;
  std::tr1::shared_ptr<MsgPortsContainer_t> pPorts;

  for(std::vector<int>::size_type i = 0; i < oMsgTypes.size(); ++i)
  {
    it = m_oPorts.find(oMsgTypes[i]);
    if(m_oPorts.end() == it)
    {
      pPorts.reset(new MsgPortsContainer_t());
      std::pair<typename Type2MsgPortsContainer_t::iterator, bool> res = \
        m_oPorts.insert(std::make_pair(oMsgTypes[i], pPorts));

      if(!res.second)
      {
        assert(0 && "Map insert failed, but cannot find it, unknown error!");
        return ;
      }
    }
    else {
      pPorts = it->second;
    }

    MsgPortInternalWeakPtr_t pWeakPort(pPort);

    //Make sure there is only one unique message port!
    typename MsgSwitch<MT_>::MsgPortsContainer_t::size_type j;
    for(j = 0; pPorts && j < pPorts->size();  ++j)
    {
      MsgPortInternalSharedPtr_t pTemp(pPorts->at(j));
      if(pTemp == pPort)
        break;
    }
   
    if(pPorts && j >= pPorts->size())
    {
      pPorts->push_back(pWeakPort);
      m_iPortsNumber ++;
    }
  }
};

template <class MT_>
void MsgSwitch<MT_>::unregisterMsgPort(const MsgPortInternalSharedPtr_t &pPort)
{
  UniqueLock_t lock(m_oMutex);

  typename Type2MsgPortsContainer_t::iterator it;
  std::tr1::shared_ptr<MsgPortsContainer_t> pPorts;

RECHECK:
  for(it = m_oPorts.begin(); it != m_oPorts.end(); ++it)
  {
    pPorts = it->second;
    for(typename MsgSwitch<MT_>::MsgPortsContainer_t::iterator i = pPorts->begin(); i != pPorts->end(); ++i)
    {
      MsgPortInternalSharedPtr_t tmp(*i);
      if(tmp == pPort)
      {
        pPorts->erase(i);
        break;
      }
    }
    if(pPorts->empty())
    {
      m_oPorts.erase(it);
      goto RECHECK;
    }
  }
};



#endif