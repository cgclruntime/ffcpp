#include "threadpool/function_scheduler.h"
#include "parallel/parallel.h"
#include <vector>
#include <iostream>
#include "common/log.h"


namespace ff{
namespace details {

FFFunctionScheduler::FFFunctionScheduler(FFFunctionThreadPool *pPool)
: m_oFinishes(pPool)
, m_oWaits(pPool)
, m_oPort()
, m_oEagerScheduler(pPool)
, m_pPool(pPool)
{
}

FFFunctionScheduler::~FFFunctionScheduler() {

}
void FFFunctionScheduler::initialSwitch(utl::MsgSwitch<FFMsg> & msgSwitch)
{
	m_oPort.setMsgSwitch(msgSwitch);
	m_oEagerScheduler.setMsgSwitch(msgSwitch);
	std::vector<int> oTypes;
	oTypes.push_back(FFMsg::mt_pf_finish);
	oTypes.push_back(FFMsg::mt_terminate);
	m_oPort.registerInterestTypes(oTypes);
}
void FFFunctionScheduler::run() {
	FFMsgPtr pMsg;
	int type;
	while((type = m_oPort.getMsg(pMsg))!= FFMsg::mt_terminate)
	{
		switch(type)
		{
		case FFMsg::mt_pf_finish:
			handlePFFinishMsg(*pMsg);
			break;
		case FFMsg::mt_terminate:
			break;
		}//end switch
	}//end while
#ifdef _DEBUG
	log_scheduler("FFFunctionScheduler", "run(), quite!");
#endif
}

void FFFunctionScheduler::handlePFFinishMsg(const FFMsg & msg)
{
#ifdef _DEBUG
	log_scheduler("FFFunctionScheduler", "handlePFFinishMsg(), id:" + utl::str(msg.getPFID()));
#endif
	//The atomic operation must be ensured!
	acquireUniqueAtomic();
#ifdef _DEBUG
	log_scheduler("FFFunctionScheduler", "handlePFFinishMsg(), Enter atomic. id:" + utl::str(msg.getPFID()));
#endif
	m_oFinishes.pushBack(msg.getPFID());
	m_oFinishes.notify();
	releaseUniqueAtomic();

	m_oWaits.handleFinishedPF(msg.getPFID());
#ifdef _DEBUG
	log_scheduler("FFFunctionScheduler", "handlePFFinishMsg(), handle over!");
#endif
}

};//end namespace detail
};//end namespace ff
