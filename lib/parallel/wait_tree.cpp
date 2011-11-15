#include "parallel/wait_tree.h"
#include "parallel/wait_conds.h"
#include "common/log.h"
#include "utils/atomic.h"
#include <cassert>
#include <cstring>
#ifdef _DEBUG
#include <sstream>
#endif

namespace ff{
namespace details{

WaitTree::WaitTree( )
: m_pNodes(NULL)
, m_iSize(0)
{
}

WaitTree::~WaitTree()
{
  if(m_pNodes)
	  delete[] m_pNodes;
}

WaitTree * WaitTree::allOrAny(const std::vector<pfid_t> & pfids)
{
	WaitTree *res = new WaitTree();
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}
	res->m_iSize = pfids.size()+1;
	res->m_pNodes = new WaitNode[res->m_iSize];
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}

	res->get(0).createWithType(WaitNode::wnt_all, pfids.size());	//The type should be modified!

	for(size_t i = 1; i < res->m_iSize; ++i)
	{
		res->get(i).createWithPfid(pfids[i-1]);
		res->get(i).setNext(0);
	}
	return res;
}

WaitTree * WaitTree::andOrOr(const WaitNode &a1, const WaitNode & a2)
{
	WaitTree *res = new WaitTree();
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}
	res->m_iSize = 3;
	res->m_pNodes = new WaitNode[res->m_iSize];
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}

	res->get(0).createWithType(WaitNode::wnt_and, 2); //The type should be modified!
	res->get(1) = a1;
	res->get(2) = a2;
	res->get(1).setNext(0);
	res->get(2).setNext(0);

	return res;
}

WaitTree * WaitTree::andOrOr(WaitTree *t1, WaitTree *t2)
{
	WaitTree *res = new WaitTree();
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}
	res->m_iSize = t1->size()+ t2->size() + 1;
	res->m_pNodes = new WaitNode[res->m_iSize];
	if(!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}

	res->get(0).createWithType(WaitNode::wnt_all, 2);	//The type should be modified!

	size_t base = 1;
	t1->get(0).setNext(0);
	t2->get(0).setNext(0);
	for(size_t i = 1; i < t1->size(); ++i)
	{
		t1->get(i).setNext(t1->get(i).next() + base);
	}
	base += t1->size();
	for(size_t i = 1; i < t2->size(); ++i)
	{
		t2->get(i).setNext(t2->get(i).next() + base);
	}

	memcpy(res->m_pNodes+1, t1->m_pNodes, t1->size()*sizeof(WaitNode));
	memcpy(res->m_pNodes+base, t2->m_pNodes, t2->size() * sizeof(WaitNode));

	return res;
}

WaitTree * WaitTree::createWithNode(const WaitNode &n)
{
	WaitTree *res = new WaitTree();
	if (!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}
	res->m_iSize = + 1;
	res->m_pNodes = new WaitNode[res->m_iSize];
	if (!res)
	{
		assert(0 && "bad alloc!");
		exit(1);
	}
	res->get(0) = n; //  WaitNode m_pNodes[0] = n
	return res;
}

bool WaitTree::isFinished() const
{
	return m_pNodes[0].finished();  // WaitNode m_pNodes[0].finished
}

bool WaitTree::notifyFinish(pfid_t iPFID)
{
	size_t pos = m_iSize -1;
	bool isFind = false;
	for(; pos >= 0; pos--)
	{
		if(m_pNodes[pos].isPFID() && m_pNodes[pos].pfid() == iPFID)
		{
			isFind = true;
			break;
		}
	}
	if(!isFind)
		return false;

	// __sync_synchronize()
	utl::Atomic<utl::mem>::sync(); // Atomic memory access : Memory Barrier

	if(m_pNodes[pos].finished())
		return false;

	m_pNodes[pos].setFinished(true);
	bool last_finish = true;
	pos = m_pNodes[pos].next();  // pos = m_oInfo.D.F.next

	while(pos!=WaitNode::max_next)
	{
		if(!last_finish)
			break;
		m_pNodes[pos].setFinishes(m_pNodes[pos].finishes() + 1);
		switch(m_pNodes[pos].type())
		{
		case WaitNode::wnt_and:
		case WaitNode::wnt_all:
			if(m_pNodes[pos].finishes() == m_pNodes[pos].waits())
				m_pNodes[pos].setFinished(true);
			break;
		case WaitNode::wnt_or:
		case WaitNode::wnt_any:
			m_pNodes[pos].setFinished(true);
			break;
		default:
			assert(0 && "unknown type");
			exit(0);
		}//end switch
		last_finish = m_pNodes[pos].finished(); // last_finish = m_oInfo.D.F.finished
		pos = m_pNodes[pos].next();
	}//end while
	return m_pNodes[0].finished();
}

#ifdef _DEBUG
std::string WaitTree::toString() const
{
  std::ostringstream ss;
  ss<<"size:"<<m_iSize;
  for(size_t i = 0; i < m_iSize; ++i)
  {
	  ss<<" "<<i<<":"<<m_pNodes[i].toString()<<";";
  }
  return ss.str();
}
#endif

/*
 */
void WaitTree::getWaitPFIDs(std::vector<pfid_t> & oPFIDs) const
{
	for(size_t i = 0 ; i < m_iSize; ++i)
	{
		if(m_pNodes[i].isPFID())
			oPFIDs.push_back(m_pNodes[i].pfid());
	}
}

void WaitTree::getUnfinishedPFIDs(std::vector<pfid_t> & oPFIDs) const
{
	for(size_t i = 0; i < m_iSize; ++i)
		if(m_pNodes[i].isPFID() && !m_pNodes[i].finished())
			oPFIDs.push_back(m_pNodes[i].pfid());
}

}//end namespace details;
}//end namespace ff
