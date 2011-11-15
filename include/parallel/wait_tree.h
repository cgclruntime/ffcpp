/************************************************************************/
/* AthrunArthur 2011.7.21                                               */
/************************************************************************/
#ifndef FFCPP_PARALLEL_WAIT_TREE_H
#define FFCPP_PARALLEL_WAIT_TREE_H
#include <vector>
#include "parallel/wait_node.h"

namespace ff {

namespace details {

class WaitTree
{
public:
	WaitTree();
	~WaitTree();

	bool							notifyFinish(pfid_t iPFID);

	bool							isFinished() const;

	static WaitTree *				createWithNode(const WaitNode& n);
	static WaitTree *				andOrOr(const WaitNode &a1, const WaitNode &a2);
	static WaitTree *				andOrOr(WaitTree *t1, WaitTree *t2);
	static WaitTree *				allOrAny(const std::vector<pfid_t>& pfids);

	void							getWaitPFIDs(std::vector<pfid_t> & oPFIDs) const;

	void							getUnfinishedPFIDs(std::vector<pfid_t> & oPFIDs) const;

	WaitNode &						operator [](size_t i){
		return m_pNodes[i];
	}

	WaitNode &						get(size_t i){return m_pNodes[i];}

	size_t							size() const {return m_iSize;};

#ifdef _DEBUG
	std::string toString() const;
#endif

protected:
	WaitNode * m_pNodes;  //
	size_t m_iSize;  //
};
//end class WaitTree

}
;
//end namespace details;
}
;
//end namespace ff;
#endif

