#ifndef SIMPLE_MOTION_MELODY_HPP
#define SIMPLE_MOTION_MELODY_HPP
#include "simple/support/tuple_utils.hpp"
#include "common.hpp"
#include "algorithm.hpp"

//TODO: might want to separate the iterator type (elapsed, current_index) from the structure type (total, start, target)

namespace simple::motion
{

template <typename... Motions>
class melody
{
	public:

	using duration = std::common_type_t<typename Motions::duration...>;
	using result_t = multi_advance_result<duration>;

	melody() = default;
	melody(Motions... motions) :
		movements{motions...},
		current_index{}
	{}

	bool done()
	{
		return current_index == sizeof...(Motions);
	}

	// WIP: TODO: need complete restructure can't nest melodies
	result_t advance(duration delta)
	{
		assert(!done());
		auto r = sequence
		(
			current_index, sizeof...(Motions),
			delta,
			[this](auto current, auto delta)
			{
				return support::apply_for(current,
					[&delta](auto&& move)
					{
						return move.advance(delta);
					},
					movements
				);
			}
		);

		bool last_done = r.done;
		current_index = r.updated.upper() - !last_done;

		if(last_done && current_index == sizeof...(Motions))
			return {true, r.remaining, r.updated};

		return {false, r.remaining, r.updated};
	}

	template <size_t... I>
	std::tuple<decltype(std::declval<Motions>().value())...> value(std::index_sequence<I...>)
	{
		return {std::get<I>(movements).value()...};
	}

	std::tuple<decltype(std::declval<Motions>().value())...> value()
	{
		return value(std::make_index_sequence<sizeof...(Motions)>{});
	}

	void reset()
	{
		for_all([](auto& movement) {movement.reset();});
		current_index = 0;
	}

	template <typename T>
	result_t move(T& target, duration delta)
	{
		if(done())
			return {true, delta};

		auto result = advance(delta);
		support::apply_for(result.updated.upper()-1, [&target](auto&& movement)
		{
			target = movement.value();
		}, movements);
		return result;
	}

	template <typename... T,
		std::enable_if_t<sizeof...(T) == sizeof...(Motions)>* = nullptr>
	result_t move(std::tuple<T...>&& targets, duration delta)
	{
		if(done())
			return {true, delta};

		auto r = advance(delta);
		support::apply_for(r.updated, [](auto&& movement, auto&& target)
		{
			target = movement.value();
		}, movements, std::forward<std::tuple<T...>>(targets));
		return r;
	}

	template <typename T, size_t Size,
		std::enable_if_t<Size == sizeof...(Motions)>* = nullptr>
	result_t move(std::array<T,Size>& targets, duration delta)
	{
		//TODO: mostly same as tuple version above
		if(done())
			return {true, delta};

		auto r = advance(delta);
		support::apply_for(r.updated, [](auto&& movement, auto&& target)
		{
			target = movement.value();
		}, movements, std::forward<std::array<T,Size>>(targets));
		return r;
	}

	private:

	std::tuple<Motions...> movements;
	size_t current_index = sizeof...(Motions);

	template<typename F, size_t I = sizeof...(Motions) - 1>
	void for_all(F&& f)
	{
		static_assert(I >= 0 && I < sizeof...(Motions));
		std::forward<F>(f)(std::get<I>(movements));
		if constexpr (I > 0)
			for_all<F, I-1>(std::forward<F>(f));
	}

};

} // namespace simple::motion

#endif /* end of include guard */
