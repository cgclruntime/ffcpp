#ifndef FFCPP_THREAD_POOL_ATOMIC_STAMPED_REFERENCE_H_
#define FFCPP_THREAD_POOL_ATOMIC_STAMPED_REFERENCE_H_

#include <boost/noncopyable.hpp>
#include "utils/types.h"
#include "utils/atomic.h"

namespace utl{
	namespace details{


    template <typename Ty_>
    class AtomicStampedBasic : public boost::noncopyable
    {
    public:
    	AtomicStampedBasic()
    	: m_iStampAndReference(0){};

    	AtomicStampedBasic(Ty_ initial)
    	{
    		StampedReference t;
    		t.m_iReference = initial;
    		t.m_iStamp = 1;
    		m_iStampAndReference = t.m_iData;
    	};
        virtual ~AtomicStampedBasic(){};

        bool        compareAndSet(const Ty_ & expectedVal,
								  const Ty_ & swapedVal,
								  utl::uint32_t expectedStamp,
								  utl::uint32_t swapedStamp){
        	StampedReference t;
        	t.m_iReference = expectedVal;
        	t.m_iStamp = expectedStamp;

        	StampedReference swaped;
        	swaped.m_iReference = swapedVal;
        	swaped.m_iStamp = swapedStamp;

        	return utl::Atomic<utl::uint64_t>::bool_cas(&m_iStampAndReference, swaped.m_iData, t.m_iData);
        };

        inline Ty_	get() const{
        	StampedReference t;
        	t.m_iData = m_iStampAndReference;
        	return static_cast<Ty_>(t.m_iReference);
        }

        inline Ty_  get(utl::uint32_t &oldStamp) const{
        	StampedReference t;
        	t.m_iData = m_iStampAndReference;
        	oldStamp = t.m_iStamp;
        	return static_cast<Ty_>(t.m_iReference);
        };

        inline void        set(const Ty_ & newVal, utl::uint32_t newStamp){
        	StampedReference t;
        	t.m_iStamp = newStamp;
        	t.m_iReference = newVal;
        	m_iStampAndReference = t.m_iData;
        };

        inline bool			equalReference(const AtomicStampedBasic<Ty_> & c)
        {
        	StampedReference t1, t2;
        	t1.m_iData = c.m_iStampAndReference;
        	t2.m_iData = m_iStampAndReference;
        	return t1.m_iReference == t2.m_iReference;
        }

    protected:
        typedef union{
        	utl::uint64_t			m_iData;
        	struct{
        		utl::uint32_t		m_iReference;
        		utl::uint32_t		m_iStamp;
        	};
        }StampedReference;

        volatile utl::uint64_t       m_iStampAndReference;
    };//end class AtomicStampedBasic

	};//end namespace details;


	template <typename Ty_> class AtomicStampedReference{};

    template <>
    class AtomicStampedReference<utl::int32_t> :
		public utl::details::AtomicStampedBasic<utl::int32_t>{
		public:
    	AtomicStampedReference(utl::int32_t initial)
			: AtomicStampedBasic<utl::int32_t>(initial){};
    };

    template <>
    class AtomicStampedReference<utl::uint32_t> :
		public utl::details::AtomicStampedBasic<utl::uint32_t>{
		public:
    	AtomicStampedReference(utl::uint32_t initial)
			: AtomicStampedBasic<utl::uint32_t>(initial){};
    };

    template <>
    class AtomicStampedReference<utl::int16_t> :
		public utl::details::AtomicStampedBasic<utl::int16_t>{
		public:
		AtomicStampedReference(utl::int16_t initial)
			: AtomicStampedBasic<utl::int16_t>(initial){};
    };

    template <>
    class AtomicStampedReference<utl::uint16_t> :
		public utl::details::AtomicStampedBasic<utl::uint16_t>{
		public:
		AtomicStampedReference(utl::uint16_t initial)
			: AtomicStampedBasic<utl::uint16_t>(initial){};
    };

    template <>
    class AtomicStampedReference<utl::int8_t> :
		public utl::details::AtomicStampedBasic<utl::int8_t>{
		public:
		AtomicStampedReference(utl::int8_t initial)
			: AtomicStampedBasic<utl::int8_t>(initial){};
    };

    template <>
    class AtomicStampedReference<utl::uint8_t> :
		public utl::details::AtomicStampedBasic<utl::uint8_t>{
		public:
		AtomicStampedReference(utl::uint8_t initial)
			: AtomicStampedBasic<utl::uint8_t>(initial){};
    };

};//end namespace utl

#endif

