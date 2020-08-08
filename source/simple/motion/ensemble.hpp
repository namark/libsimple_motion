#ifndef SIMPLE_MOTION_ENSEMBLE_HPP
#define SIMPLE_MOTION_ENSEMBLE_HPP
#include <utility>
#include "common.hpp"

namespace simple::motion
{

template <typename... Motions>
class ensemble // TODO
{
	using duration = std::common_type_t<typename Motions::duration...>;
	std::tuple<Motions...> instruments;
	bool done();
	advance_result<duration> advance(duration delta);
	void reset();
	template <typename T>
	advance_result<duration> move(T& target, duration delta);
};

} // namespace simple::motion

#endif /* end of include guard */
