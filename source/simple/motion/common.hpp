#ifndef SIMPLE_MOTION_COMMON_HPP
#define SIMPLE_MOTION_COMMON_HPP
#include "simple/support/range.hpp"

namespace simple::motion
{

template <typename Duration>
struct advance_result
{
	bool success = false;
	Duration remaining{};
	explicit operator bool() { return success; }
};

template <typename Duration, typename It = size_t>
struct multi_advance_result : public advance_result<Duration>
{
	support::range<It> updated {};
	using advance_result<Duration>::operator bool;
};


} // namespace simple::motion

#endif /* end of include guard */
