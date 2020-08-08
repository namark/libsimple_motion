#ifndef SIMPLE_MOTION_COMMON_HPP
#define SIMPLE_MOTION_COMMON_HPP

namespace simple::motion
{

template <typename Duration>
struct advance_result
{
	bool success = false;
	Duration remaining{};
	explicit operator bool() { return success; }
};

} // namespace simple::motion

#endif /* end of include guard */
