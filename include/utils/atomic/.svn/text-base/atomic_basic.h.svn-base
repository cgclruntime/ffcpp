/************************************************************************/
/* AthrunArthur 2011.8.17                                               */
/************************************************************************/
#ifndef FFCPP_UTILS_ATOMIC_ATOMIC_BASIC_H_
#define FFCPP_UTILS_ATOMIC_ATOMIC_BASIC_H_

#include <boost/noncopyable.hpp>
#if defined WIN32 || defined _WIN32     //For windows
#include <Windows.h>
#endif


namespace utl
{
namespace details{

 ///! Atomic memory access
 template <typename Ty_>
 class AtomicBasic : public boost::noncopyable
 {
 public:
     //! Atomically increment an Ty_ by 1
     //! "mem": pointer to the object
     //! Returns the old value pointed to by mem
     static inline Ty_      inc(volatile Ty_ *mem)
#if defined WIN32 || defined _WIN32
		{
			return static_cast<Ty_>(InterlockedIncrement(reinterpret_cast<volatile long*>(mem))-1);
		}
#endif
;

     //! Atomically decrement an Ty_ by 1
     //! "mem": pointer to the object
     //! Returns the old value pointed to by mem
     static inline Ty_      dec(volatile Ty_ *mem)
#if defined WIN32 || defined _WIN32
		{
			return static_cast<Ty_>(InterlockedDecrement(reinterpret_cast<volatile long*>(mem))+1);
		}
#endif
;

     //! Atomically increment an Ty_ by val
     //! "mem": pointer to the object
     //! Returns the old value pointed to by mem
     static inline Ty_      add(volatile Ty_ *mem, Ty_ val)
#if defined WIN32 || defined _WIN32
		{
			Ty_ lOldSize;
			while(1)
			{
				lOldSize = *mem;
				if(lOldSize == InterlockedCompareExchange(reinterpret_cast<volatile long*>(mem), lOldSize+val, lOldSize))
				{
					break;
				}
			}
			return lOldSize;
		}
#endif
;

     //! Atomically read an Ty_ from memory
     static inline Ty_      read(volatile Ty_ *mem)
#if defined WIN32 || defined _WIN32
		{
			return static_cast<Ty_>(*mem);
		}
#endif
;

     //! Atomically set an Ty_ in memory
     //! "mem": pointer to the object
     //! "val": val value that the object will assume
     static inline Ty_      write(volatile Ty_ *mem, Ty_ val)
#if defined WIN32 || defined _WIN32
		{
			return static_cast<Ty_>(InterlockedExchange(reinterpret_cast<volatile long*>(mem), val));
		}
#endif
;

     //! Compare an Ty_'s value with "cmp".
     //! If they are the same swap the value with "with"
     //! "mem": pointer to the value
     //! "with": what to swap it with
     //! "cmp": the value to compare it to
     //! Returns the old value of *mem
     static inline Ty_      val_cas(volatile Ty_ *mem, Ty_ with, Ty_ cmp)
#if defined WIN32 || defined _WIN32
		{
			return static_cast<Ty_>(InterlockedCompareExchange(reinterpret_cast<volatile long*>(mem), with, cmp));
		}
#endif
;

     //! Compare an Ty_'s value with "cmp".
     //! If they are the same swap the value with "with"
     //! "mem": pointer to the value
     //! "with": what to swap it with
     //! "cmp": the value to compare it to
     //! Returns true if mem is swapped, or false
     static inline bool   bool_cas(volatile Ty_ *mem, Ty_ with, Ty_ cmp)
#if defined WIN32 || defined _WIN32
		{
			Ty_ temp=*mem;
			if(val_cas(mem, with, cmp)==temp)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
#endif
;
 };

#if defined WIN32 || defined _WIN32     //For windows

template <>
class AtomicBasic<utl::int64_t> : public boost::noncopyable
{
public:
	static inline utl::int64_t inc(volatile utl::int64_t *mem)
	{
		return static_cast<utl::int64_t>(InterlockedIncrement64(reinterpret_cast<volatile LONGLONG*>(mem))-1);
	};

 static inline utl::int64_t dec(volatile utl::int64_t *mem)
	{
		return static_cast<utl::int64_t>(InterlockedDecrement64(reinterpret_cast<volatile LONGLONG*>(mem))+1);
	};

	static inline utl::int64_t add(volatile utl::int64_t *mem, utl::int64_t val)
 {
		utl::int64_t llOldSize;
		while(1)
		{
			llOldSize = *mem;
			if(llOldSize == InterlockedCompareExchange64(reinterpret_cast<volatile LONGLONG*>(mem), llOldSize+val, llOldSize))
			{
				break;
			}
		}
    return llOldSize;
 };

 static inline utl::int64_t read(volatile utl::int64_t *mem)
 {
		return static_cast<utl::int64_t>(*mem);
 };

	static inline utl::int64_t write(volatile utl::int64_t *mem, utl::int64_t val)
	{
		return static_cast<utl::int64_t>(InterlockedExchange64(reinterpret_cast<volatile LONGLONG*>(mem), val));
	};

	static inline utl::int64_t val_cas(volatile utl::int64_t *mem, utl::int64_t with, utl::int64_t cmp)
	{
		return static_cast<utl::int64_t>(InterlockedCompareExchange64(reinterpret_cast<volatile LONGLONG*>(mem), with, cmp));
	};

 static inline bool bool_cas(volatile utl::int64_t *mem, utl::int64_t with, utl::int64_t cmp)
	{
		utl::int64_t temp=*mem;
		if(val_cas(mem, with, cmp)==temp)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
};

template <>
class AtomicBasic<utl::uint64_t> : public boost::noncopyable
{
public:
	static inline utl::uint64_t inc(volatile utl::uint64_t *mem)
	{
		return static_cast<utl::uint64_t>(InterlockedIncrement64(reinterpret_cast<volatile LONGLONG*>(mem))-1);
	};

 static inline utl::uint64_t dec(volatile utl::uint64_t *mem)
	{
		return static_cast<utl::uint64_t>(InterlockedDecrement64(reinterpret_cast<volatile LONGLONG*>(mem))+1);
	};

	static inline utl::uint64_t add(volatile utl::uint64_t *mem, utl::uint64_t val)
 {
		utl::uint64_t llOldSize;
		while(1)
		{
			llOldSize = *mem;
			if(llOldSize == InterlockedCompareExchange64(reinterpret_cast<volatile LONGLONG*>(mem), llOldSize+val, llOldSize))
			{
				break;
			}
		}
    return llOldSize;
 };

 static inline utl::uint64_t read(volatile utl::uint64_t *mem)
 {
		return static_cast<utl::uint64_t>(*mem);
 };

	static inline utl::uint64_t write(volatile utl::uint64_t *mem, utl::uint64_t val)
	{
		return static_cast<utl::uint64_t>(InterlockedExchange64(reinterpret_cast<volatile LONGLONG*>(mem), val));
	};

	static inline utl::uint64_t val_cas(volatile utl::uint64_t *mem, utl::uint64_t with, utl::uint64_t cmp)
	{
		return static_cast<utl::uint64_t>(InterlockedCompareExchange64(reinterpret_cast<volatile LONGLONG*>(mem), with, cmp));
	};

 static inline bool bool_cas(volatile utl::uint64_t *mem, utl::uint64_t with, utl::uint64_t cmp)
	{
		utl::uint64_t temp=*mem;
		if(val_cas(mem, with, cmp)==temp)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
};

#elif defined __GNUC__  //For gcc
    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::inc(volatile Ty_ *mem)
    {
        return AtomicBasic<Ty_>::add(mem, 1);
    };

    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::dec(volatile Ty_ *mem)
    {
        return AtomicBasic<Ty_>::add(mem, -1);
    };

    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::add(volatile Ty_ *mem, Ty_ val)
    {
       return __sync_fetch_and_add(mem, val);
    };

    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::read(volatile Ty_ *mem)
    {
        return *mem;
    };

    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::write(volatile Ty_ *mem, Ty_ val)
    {
        *mem = val;
        return *mem;
    };

    template <typename Ty_>
    inline bool AtomicBasic<Ty_>::bool_cas(volatile Ty_ *mem, Ty_ with, Ty_ cmp)
    {
        return __sync_bool_compare_and_swap(mem, cmp, with);
    };

    template <typename Ty_>
    inline Ty_ AtomicBasic<Ty_>::val_cas(volatile Ty_ *mem, Ty_ with, Ty_ cmp)
    {
        return __sync_val_compare_and_swap(mem, cmp, with);
    };
#endif

};//end namespace details

};//end namespace utl;

#endif
