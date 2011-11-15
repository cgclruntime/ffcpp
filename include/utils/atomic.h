/************************************************************************/
/* AthrunArthur 2011.8.17                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_ATOMIC_H
#define FFCPP_UTILS_ATOMIC_H

#if defined WIN32 || defined _WIN32
#include <Windows.h>
#endif


#include "utils/types.h"
#include "utils/atomic/atomic_basic.h"

namespace utl
{
    ///! Atomic memory access
    template <typename Ty_>
    class Atomic{
    };

    template <>
    class Atomic<utl::int8_t> : public utl::details::AtomicBasic<utl::int8_t>{};

    template <>
    class Atomic<utl::uint8_t> : public utl::details::AtomicBasic<utl::uint8_t>{};

    template <>
    class Atomic<utl::int16_t> : public utl::details::AtomicBasic<utl::int16_t>{};

    template <>
    class Atomic<utl::uint16_t> : public utl::details::AtomicBasic<utl::uint16_t>{};

    template <>
    class Atomic<utl::int32_t> : public utl::details::AtomicBasic<utl::int32_t>{};

    template <>
    class Atomic<utl::uint32_t> : public utl::details::AtomicBasic<utl::uint32_t>{};

    template <>
    class Atomic<utl::int64_t> : public utl::details::AtomicBasic<utl::int64_t>{};

    template <>
    class Atomic<utl::uint64_t> : public utl::details::AtomicBasic<utl::uint64_t>{};

    struct mem{};
    template<>
    class Atomic<mem>{
    public:
    	//! Memory barrier.
    	static inline void sync(){
#if defined WIN32 || defined _WIN32
    		MemoryBarrier();
#elif defined __GNUC__
    		__sync_synchronize();
#endif
    	};//end void sync();
    };//end class Atomic

};//end namespace utl;
#endif
