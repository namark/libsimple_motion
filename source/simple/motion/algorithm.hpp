#ifndef SIMPLE_MOTION_ALGORITHM_HPP
#define SIMPLE_MOTION_ALGORITHM_HPP
#include <utility>
#include "common.hpp"

namespace simple::motion
{

// welcome to proper looping
// TODO: for known total duration mod(remaining, total) optimization
template <typename Motion, typename Target>
int loop(Target&& target, Motion& motion, typename Motion::duration delta)
{
	int count = 0;
	// better way to write this??
	while(true) if(auto [success, remaining] =
		motion.move(std::forward<Target>(target), delta);
		!success
	)
	{
		motion.reset();
		delta = remaining;
		++count;
	}
	else break;
	return count;
}

template <typename It, typename Duration, typename Function>
constexpr
multi_advance_result<Duration, It> sequence(It begin, It end, Duration delta, Function&& advance)
{
	support::range<It> updated{begin, begin};
	for(; begin != end; ++begin)
	{
		auto result = std::apply(
			std::forward<Function>(advance),
			std::forward_as_tuple(begin, delta)
		);
		++updated.upper();

		delta = result.remaining;

		if(delta <= Duration{})
			return {true, delta, updated};
	}

	return {false, delta, updated};
}

} // namespace simple::motion

#endif /* end of include guard */
