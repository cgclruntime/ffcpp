

#ifndef FFCPP_UTILS_TYPES_H_
#define FFCPP_UTILS_TYPES_H_

#include <boost/cstdint.hpp>
#include <sstream>
#include <string>

namespace utl{

  typedef unsigned char byte;

  typedef boost::uint8_t uint8_t;
  typedef boost::uint16_t uint16_t;
  typedef boost::uint32_t uint32_t;
  typedef boost::uint64_t uint64_t;

  typedef boost::int8_t  int8_t;
  typedef boost::int16_t int16_t;
  typedef boost::int32_t int32_t;
  typedef boost::int64_t int64_t;

  template <typename Ty_>
  std::string 	str(const Ty_ & t){
  	std::ostringstream ss;
  	ss<<t;
  	return ss.str();
  }
};//end namespace utl;
#endif

