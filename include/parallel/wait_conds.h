#ifndef FFCPP_PARALLEL_WAIT_CONDS_H
#define FFCPP_PARALLEL_WAIT_CONDS_H

#include "parallel/func_type_traits.h"
#include "parallel/wait_tree.h"
#include <vector>


namespace ff{
namespace details{


class WaitConds
{
public:
	WaitConds()
	: m_pWaitTree(NULL)
	{
	}

public:
	WaitTree * m_pWaitTree;
};

}//end namespace details
}//end namespace ff
#endif
