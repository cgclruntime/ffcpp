#include "parallel/wait_node.h"
#include <sstream>

namespace ff{
namespace details{

WaitNode & WaitNode::createWithPfid(pfid_t iPFID)
{
	setFinished(false);
	pfid() = iPFID;
	setIsPFID(true);
	setNext();
	return *this;
}

WaitNode & WaitNode::createWithType(utl::uint32_t iType, utl::uint32_t iWaits)
{
	setFinished(false);
	setWaits(iWaits);
	setFinishes(0);
	setIsPFID(false);
	setNext();
	setType(iType);
	return *this;
}

#ifdef _DEBUG
std::string WaitNode::toString() const
{
  std::ostringstream ss;
  ss<<"finished:"<<finished();
  ss<<" is_pfid:"<<isPFID();
  ss<<" next:"<<next();
  if(isPFID())
  {
	  ss<<" pfid:"<<pfid();
  }
  else{
	  ss<<" type:";
	  switch(type())
	  {
	  case wnt_all:
		  ss<<"all";
		  break;
	  case wnt_and:
		  ss<<"and";
		  break;
	  case wnt_any:
		  ss<<"any";
		  break;
	  case wnt_or:
		  ss<<"or";
		  break;
	  default:
		  ss<<"unknown("<<type()<<")";
		  break;
	  }//end switch
	  ss<<" waits:"<<waits();
	  ss<<" finishes:"<<finishes();
  }
  return ss.str();
}
#endif

};//end namespace details;
};//end namespace ff;


