/* AthrunArthur 2011.8.23
 * */
#ifndef FFCPP_MESSAGE_MESSAGE_H_
#define FFCPP_MESSAGE_MESSAGE_H_
#include <boost/tr1/memory.hpp>
#include "utils/types.h"

namespace ff{
namespace details{

typedef utl::uint32_t pfid_t;

class FFMsg{
public:
	enum{
		mt_terminate = 1 ,	//For system quit.
		mt_pf_finish = 2	//For parallel functions' finish.
	};

public:
	//! Constructor.
	/// When the type is mt_terminate, pfid is no use.
	FFMsg( utl::uint8_t type, pfid_t pfid)
	: m_iType(type)
	, m_iPFID(pfid){};

	~FFMsg(){};

	utl::uint8_t		getType() const {return m_iType; }

	pfid_t				getPFID() const {return m_iPFID; }

protected:
	utl::uint8_t				m_iType;   // m_iType is either of mt_terminate and mt_pf_finish
	pfid_t						m_iPFID;  //  m_iPFID is the ID of FFMsg
};

typedef std::tr1::shared_ptr<FFMsg> FFMsgPtr; // FFMsgPtr is the pointer of FFMsg

};//end namespace details
};//end namespace ff
#endif

