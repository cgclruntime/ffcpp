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
	log_scheduler("FFFunctionScheduler", "run(), quite!");
}

void FFFunctionScheduler::handlePFFinishMsg(const FFMsg & msg)
{
	log_scheduler("FFFunctionScheduler", "handlePFFinishMsg(), id:" + utl::str(msg.getPFID()));

	//The atomic operation must be ensured!
	acquireUniqueAtomic();
	log_scheduler("FFFunctionScheduler", "handlePFFinishMsg(), Enter atomic. id:" + utl::str(msg.getPFID()));
	m_oFinishes.pushBack(msg.getPFID());
	m_oFinishes.notify();
	releaseUniqueAtomic();

	m_oWaits.handleFinishedPF(msg.getPFID());
}

};//end namespace detail
};//end namespace ff
