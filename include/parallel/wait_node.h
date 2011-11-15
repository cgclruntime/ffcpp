/************************************************************************/
/* AthrunArthur 2011.7.21                                               */
/************************************************************************/
#ifndef FFCPP_PARALLEL_WAIT_NODE_H
#define FFCPP_PARALLEL_WAIT_NODE_H
#include "utils/types.h"
#ifdef _DEBUG
#include <string>
#endif

namespace ff{
namespace details{

typedef utl::uint32_t pfid_t;

class WaitNode{
public:
  WaitNode(){ m_oInfo.intVal = 0;};   //intVal used for copy

  inline utl::uint64_t &			val(){return m_oInfo.intVal;};
  inline const utl::uint64_t &		val() const {return m_oInfo.intVal; };

  inline bool 						finished() const {return m_oInfo.D.F.finished;};
  inline void						setFinished(bool f = true) { m_oInfo.D.F.finished = f;};

  inline bool 						isPFID() const {return m_oInfo.D.F.isPFID;};
  inline void						setIsPFID(bool isPFID) { m_oInfo.D.F.isPFID = isPFID;};

  inline utl::uint32_t 				next() const {return m_oInfo.D.F.next;};
  inline void						setNext(utl::uint32_t n = WaitNode::max_next){m_oInfo.D.F.next = n;};

  inline pfid_t &					pfid(){return m_oInfo.D.PI.pfid;};
  inline const pfid_t &				pfid() const {return m_oInfo.D.PI.pfid; };

  inline utl::uint32_t 				type() const {return m_oInfo.D.PI.I.type;};
  inline void						setType(utl::uint32_t t){m_oInfo.D.PI.I.type = t;};

  inline utl::uint32_t 				waits() const {return m_oInfo.D.PI.I.waits;};
  inline void						setWaits(utl::uint32_t w){m_oInfo.D.PI.I.waits = w;};

  inline utl::uint32_t 				finishes()const {return m_oInfo.D.PI.I.finishes;};
  inline void						setFinishes(utl::uint32_t f){m_oInfo.D.PI.I.finishes = f;}

  WaitNode &				operator =(const WaitNode & w){	  val() = w.val();  return *this;}

  WaitNode &				createWithPfid(pfid_t iPFID);
  WaitNode &				createWithType(utl::uint32_t iType, utl::uint32_t iWaits);

#ifdef _DEBUG
  //! For log and debug
  std::string       toString() const;
#endif

public:
  enum{
	  max_next = 0xFFFFFFF   //?
  };
  enum{
    wnt_and = 0,
    wnt_or ,
    wnt_any ,
    wnt_all
  }WNT_Type;       // WaitNode Type
protected:
  typedef union{
	  utl::uint64_t		intVal;		///< used for copy!!  ??
	  struct {
			struct {
				bool			finished :1;
				bool			isPFID :1;
				utl::uint32_t next :30;
			} F;
			union {
				pfid_t		pfid;  // utl::uint32_t pfid
				struct {
					utl::uint32_t type :2;  // WaitNode type
					utl::uint32_t waits :15; // wait node numbers
					utl::uint32_t finishes :15; //finish node numbers
				} I;
			}PI;
		} D;
  }WaitInfo;

  WaitInfo 		m_oInfo;
};

};//end namespace details;
};//end namespace ff;

#endif

