#ifndef SIMPLE_MOTION_ALGORITHM_HPP
#define SIMPLE_MOTION_ALGORITHM_HPP
#include <utility>
#include "common.hpp"

namespace simple::motion
{

// welcome to proper looping
template <typename Motion, typename Target>
int loop(Target&& target, Motion& motion, typename Motion::duration delta)
{
	int count = 0;
	while(auto result =
		motion.move(std::forward<Target>(target), delta))
	{
		motion.reset();
		delta = result.remaining;
		++count;
	}
	return count;
}

// and proper sequencing
// NOTE: The done flag indicates that the last updated motion is done, not necessarily the whole input sequence.
template <typename Stepper, typename Duration, typename Function>
constexpr
multi_advance_result<Duration, Stepper>
sequence(Stepper begin, Stepper end, Duration delta, Function&& advance)
{
	assert(begin != end);

	support::range<Stepper> updated{begin, begin};

	do
	{
		auto result = std::apply(
			std::forward<Function>(advance),
			std::forward_as_tuple(begin, delta)
		);
		++updated.upper();

		if(result.remaining <= Duration{})
			return {result, updated};

		delta = result.remaining;
		++begin;
	}
	while(begin != end);

	return {true, delta, updated};
}

} // namespace simple::motion

#endif /* end of include guard */
