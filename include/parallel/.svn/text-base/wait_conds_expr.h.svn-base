#ifndef FFCPP_PARALLEL_WAIT_CONDS_EXPR_H
#define FFCPP_PARALLEL_WAIT_CONDS_EXPR_H
#include "parallel/parallel.h"


ff::details::WaitConds operator||(ff::details::WaitConds w1, ff::details::WaitConds w2);


template<typename FTx_, FTx_ * pfx, typename FTy_, FTy_ *pfy>
ff::details::WaitConds operator||(const ff::Parallel<FTx_, pfx>& px
					, const ff::Parallel<FTy_, pfy>& py)
{
	ff::details::WaitConds t;
	ff::details::WaitNode w1, w2;
	w1.createWithPfid(px.identifier());
	w2.createWithPfid(px.identifier());
	t.m_pWaitTree = ff::details::WaitTree::andOrOr(w1, w2);
	t.m_pWaitTree->get(0).setType(ff::details::WaitNode::wnt_or);
	return t;
}

ff::details::WaitConds operator&&(ff::details::WaitConds w1, ff::details::WaitConds w2);


template<typename FTx_, FTx_ * pfx, typename FTy_, FTy_ *pfy>
ff::details::WaitConds operator&&(const ff::Parallel<FTx_, pfx>& px
					, const ff::Parallel<FTy_, pfy>& py)
{
	ff::details::WaitConds t;
	ff::details::WaitNode w1, w2;
	w1.createWithPfid(px.identifier());
	w2.createWithPfid(py.identifier());
	t.m_pWaitTree = ff::details::WaitTree::andOrOr(w1, w2);
	t.m_pWaitTree->get(0).setType(ff::details::WaitNode::wnt_and);
	return t;
}

template<typename FT_, FT_ * func_addr, int N>
ff::details::WaitConds all(const ff::Parallel<FT_, func_addr>(&pf)[N])
{
	std::vector<utl::uint32_t> oPFIDs;
	for (size_t i = 0; i < N; i++)
	{
		if (!pf[i].isConstructed())
			oPFIDs.push_back(pf[i].identifier());
	}
	ff::details::WaitConds t;
	t.m_pWaitTree = ff::details::WaitTree::allOrAny(oPFIDs);
	t.m_pWaitTree->get(0).setType(ff::details::WaitNode::wnt_all);
	return t;
}

template<typename FT_, FT_ * func_addr, int N>
ff::details::WaitConds any(const ff::Parallel<FT_, func_addr>(&pf)[N])
{
	std::vector<utl::uint32_t> oPFIDs;
	for (size_t i = 0; i < N; i++)
	{
		if (!pf[i].isConstructed())
			oPFIDs.push_back(pf[i].identifier());
	}
	ff::details::WaitConds t;
	t.m_pWaitTree = ff::details::WaitTree::allOrAny(oPFIDs);
	t.m_pWaitTree->get(0).setType(ff::details::WaitNode::wnt_any);
	return t;
}


#endif

