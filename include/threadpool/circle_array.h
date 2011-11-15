/************************************************************************/
/* AthrunArthur 2011.8.17
 * This class defines a circluar array.
 * Each element in the array must have default constructor and operator =()
 *                                           */
/************************************************************************/
#ifndef FFCPP2_THREAD_POOL_CIRCLE_ARRAY_H_
#define FFCPP2_THREAD_POOL_CIRCLE_ARRAY_H_

#include <boost/noncopyable.hpp>
#include <cassert>
#include "utils/atomic.h"

namespace ff{
namespace details{

template <typename Ty_>
class CircleArray : public boost::noncopyable
{
public:
	explicit CircleArray(utl::uint32_t iCapacity = 64)
	: m_iCapacity(iCapacity)
	{
		m_pArray = new Ty_[m_iCapacity];
		if(m_pArray == NULL)
		{
			assert(0 && "new failed!");
			exit(0);
		}
	};

	virtual ~CircleArray(){
		delete[] m_pArray;
	};

	inline utl::uint32_t		getCapacity() const{
		return m_iCapacity;
	};

	inline Ty_ &				operator [](size_t index){
		utl::Atomic<utl::mem>::sync();
		return m_pArray[index%m_iCapacity];
	};

	inline Ty_ 				get(size_t index){
		utl::Atomic<utl::mem>::sync();
		return m_pArray[index%m_iCapacity];
	};
	inline const Ty_ &				operator [](size_t index) const {
		utl::Atomic<utl::mem>::sync();
		return m_pArray[index%m_iCapacity];
	}

	void				resize(utl::uint32_t iNewCapacity)
	{
		Ty_ *p = new Ty_[iNewCapacity];
		if(p == NULL)
		{
			assert(0 && "new failed!");
			exit(0);
		}
		for(int i = 0; i<m_iCapacity && i<iNewCapacity; ++i)
		{
			p[i%iNewCapacity] = m_pArray[i%m_iCapacity];
		}
		Ty_ *t = m_pArray;

		m_pArray = p, m_iCapacity = iNewCapacity;
		utl::Atomic<utl::mem>::sync();
		delete[] t;
	};

	void				resizeAndCopy(utl::uint32_t iAddCapacity,
									  utl::int32_t iTop,
									  utl::int32_t iBottom)
	{
		utl::uint32_t iNewCapacity = m_iCapacity + iAddCapacity;
		Ty_ *p = new Ty_[iNewCapacity];
		if(p == NULL)
		{
			assert(0 && "new failed!");
			exit(0);
		}
		for(int i = iBottom; i<=iTop; ++i)
		{
			p[i%iNewCapacity] = m_pArray[i%m_iCapacity];
		}
		Ty_ *t = m_pArray;

		m_pArray = p, m_iCapacity = iNewCapacity;
		utl::Atomic<utl::mem>::sync();
		delete[] t;
	};

protected:
	utl::uint32_t		m_iCapacity;
	Ty_ *				m_pArray;
};//end class CircleArray;

};//end namespace details;
};//end namespace ff;


#endif


