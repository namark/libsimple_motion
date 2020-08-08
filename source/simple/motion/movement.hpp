#ifndef SIMPLE_MOTION_MOVEMENT_HPP
#define SIMPLE_MOTION_MOVEMENT_HPP
#include "simple/support/algorithm.hpp"
#include "common.hpp"

namespace simple::motion
{

template <typename Number>
Number linear_curve(Number x) { return x; };
template <typename Number>
Number quadratic_curve(Number x) { return x*x; };
template <typename Number>
Number cubic_curve(Number x) { return x*x*x; };

template <typename Number>
using curve_t = decltype(&linear_curve<Number>);

template <typename Duration, typename Type, typename Ratio = float, curve_t<Ratio> curve = linear_curve<Ratio>>
struct movement
{
	using duration = Duration;
	Type start;
	Type end;
	Duration total {};
	Duration elapsed {};

	decltype(auto) value()
	{
		auto ratio = total == Duration{} ? Ratio{} :
			(Ratio{} + elapsed.count()) /
			(Ratio{} + total.count());
		using support::way;
		return way(start, end, curve(ratio));
	}

	bool done()
	{
		return elapsed >= total;
	}

	advance_result<Duration> advance(Duration delta)
	{
		elapsed += delta;
		if(done())
		{
			auto remaining = elapsed - total;
			elapsed = total;
			return {false, remaining};
		}
		return {true};
	}

	void reset()
	{
		elapsed = Duration{};
	}

	advance_result<Duration> move(Type& target, Duration delta)
	{
		auto result = advance(delta);
		target = value();
		return result;
	}
};

} // namespace simple::motion

#endif /* end of include guard */
