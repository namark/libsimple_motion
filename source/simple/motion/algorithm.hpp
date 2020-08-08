#ifndef SIMPLE_MOTION_ALGORITHM_HPP
#define SIMPLE_MOTION_ALGORITHM_HPP
#include <utility>

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

} // namespace simple::motion

#endif /* end of include guard */
