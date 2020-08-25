#ifndef SIMPLE_MOTION_SYMPHONY_HPP
#define SIMPLE_MOTION_SYMPHONY_HPP
#include <vector>
#include <variant>
#include "simple/support/algorithm.hpp"
#include "common.hpp"
#include "algorithm.hpp"

namespace simple::motion
{

template <typename... Motions>
struct variant : public std::variant<Motions...>
{
	using duration = std::common_type_t<typename Motions::duration...>;

	using std::variant<Motions...>::variant;

	decltype(auto) advance(duration delta)
	{
		return std::visit([&delta](auto&& motion)
		{
			return motion.advance(delta);
		}, *this);
	}

	decltype(auto) value()
	{
		return std::visit([](auto&& motion)
		{
			return motion.value();
		}, *this);
	}

};

template <typename Motion,
	template<typename T> typename Range =
		std::vector>
class symphony
{
	public:
	using duration = typename Motion::duration;
	using iterator = typename Range<Motion>::iterator;

	explicit symphony(Range<Motion> motions) :
		motions(std::move(motions)),
		current(std::begin(this->motions))
	{
	}

	multi_advance_result<duration, iterator>
	advance(duration delta)
	{
		return sequence
		(
			current, motions.end(),
			delta,
			[this](auto current, auto delta)
			{
				return (*current).advance(delta);
			}
		);
	}

	template <typename TargetRange>
	advance_result<duration> move(TargetRange&& target, duration delta)
	{
		using std::begin;
		using std::end;
		using support::map_range;
		using support::reverse;

		auto r = advance(delta);

		auto to_update = reverse(map_range(
			r.updated, target, begin(motions) ));
		auto updated = begin(reverse(r.updated));

		for(auto&& i : to_update)
			i = (*updated++).value();
		return r;
	}

	private:
	Range<Motion> motions;
	iterator current;

};

} // namespace simple::motion

#endif /* end of include guard */
