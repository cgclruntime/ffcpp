#include "parallel/wait_conds.h"
#include "parallel/wait_conds_expr.h"
#include "parallel/parallel.h"

namespace ff{
namespace details{

WaitConds operator||(WaitConds w1, WaitConds w2)
{
	WaitConds t;
	t.m_pWaitTree = WaitTree::andOrOr(w1.m_pWaitTree, w2.m_pWaitTree);
	delete w1.m_pWaitTree;
	delete w2.m_pWaitTree;

	t.m_pWaitTree->get(0).setType(WaitNode::wnt_or);
	return t;
}



WaitConds operator&&(WaitConds w1, WaitConds w2)
{
	WaitConds t;
	t.m_pWaitTree = WaitTree::andOrOr(w1.m_pWaitTree, w2.m_pWaitTree);
	delete w1.m_pWaitTree;
	delete w2.m_pWaitTree;

	t.m_pWaitTree->get(0).setType(WaitNode::wnt_and);
	return t;
}


}//end namespace details
}//end namespace ff

