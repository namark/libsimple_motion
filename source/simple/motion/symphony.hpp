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

template <typename Range>
class symphony
{
	public:
	using iterator = decltype(std::begin(std::declval<Range&>()));
	using motion = std::remove_reference_t<decltype(*std::declval<iterator>())>;
	using duration = typename motion::duration;
	using result_t = multi_advance_result<duration,iterator>;

	explicit symphony(Range motions) :
		motions(std::move(motions)),
		current(std::begin(this->motions))
	{
	}

	bool done()
	{
		return current == std::end(motions);
	}

	void reset()
	{
		for(auto&& motion : motions)
			motion.reset();
		current = std::begin(motions);
	}

	result_t advance(duration delta)
	{
		assert(!done());
		auto r = sequence
		(
			current, std::end(motions),
			delta,
			[](auto current, auto delta)
			{
				return current->advance(delta);
			}
		);

		// TODO follows a duplicate code from melody melody
		// maybe make another function sequence_update
		bool last_done = r.done;
		current = r.updated.upper() - !last_done;

		if(last_done && current == std::end(motions))
			return {true, r.remaining, r.updated};

		return {false, r.remaining, r.updated};
	}

	template <typename TargetRange>
	result_t move(TargetRange&& target, duration delta)
	{
		if(done())
			return {true, delta};

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

	const Range& range()
	{
		return motions;
	}

	private:
	Range motions;
	iterator current;

};

} // namespace simple::motion

#endif /* end of include guard */
