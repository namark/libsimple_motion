#ifndef SIMPLE_MOTION_MELODY_HPP
#define SIMPLE_MOTION_MELODY_HPP
#include "simple/support/range.hpp"
#include "simple/support/tuple_utils.hpp"
#include "common.hpp"

//TODO: might want to separate the iterator type (elapsed, current_index) from the structure type (total, start, target)

namespace simple::motion
{

template <typename Duration>
struct multi_advance_result : public advance_result<Duration>
{
	support::range<size_t> updated {0,0};
	using advance_result<Duration>::operator bool;
};

template <typename... Motions>
class melody
{
	public:

	using duration = std::common_type_t<typename Motions::duration...>;

	melody() = default;
	melody(Motions... motions) : movements{motions...} {}

	bool done()
	{
		return std::get<sizeof...(Motions) - 1>(movements).done();
	}

	// auto advance(duration delta)
	// {
	// 	return advance(delta, {current_index, current_index + 1});
	// }

	// WIP: TODO: need complete restructure can't nest melodies
	multi_advance_result<duration> advance(duration delta)
	{
		support::range<size_t> updated{current_index, current_index};
		for(; current_index < sizeof...(Motions); ++current_index)
		{
			auto result = support::apply_for(current_index, [&delta](auto&& movement)
			{
				return movement.advance(delta);
			}, movements);
			++updated.upper();

			delta = result.remaining;

			if(delta <= duration{})
				return {true, delta, updated};
		}

		return {false, delta, updated};
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
	advance_result<duration> move(T& target, duration delta)
	{
		auto result = advance(delta);
		support::apply_for(result.updated.upper()-1, [&target](auto&& movement)
		{
			target = movement.value();
		}, movements);
		return result;
	}

	template <typename... T,
		std::enable_if_t<sizeof...(T) == sizeof...(Motions)>* = nullptr>
	advance_result<duration> move(std::tuple<T...>&& targets, duration delta)
	{
		auto r = advance(delta);
		support::apply_for(r.updated, [](auto&& movement, auto&& target)
		{
			target = movement.value();
		}, movements, std::forward<std::tuple<T...>>(targets));
		return r;
	}

	template <typename T, size_t Size,
		std::enable_if_t<Size == sizeof...(Motions)>* = nullptr>
	advance_result<duration> move(std::array<T,Size>& targets, duration delta)
	{
		//TODO: mostly same as tuple version above
		auto r = advance(delta);
		support::apply_for(r.updated, [](auto&& movement, auto&& target)
		{
			target = movement.value();
		}, movements, std::forward<std::array<T,Size>>(targets));
		return r;
	}

	private:

	std::tuple<Motions...> movements;
	size_t current_index = 0;

	template<typename F, size_t I = sizeof...(Motions) - 1>
	void for_all(F&& f)
	{
		static_assert(I >= 0 && I < sizeof...(Motions));
		std::forward<F>(f)(std::get<I>(movements));
		if constexpr (I > 0)
			for_all<F, I-1>(std::forward<F>(f));
	}

	multi_advance_result<duration> advance(duration delta, support::range<size_t> updated)
	{
		auto [success, remaining] = support::apply_for(current_index, [&delta](auto&& movement)
		{
			return movement.advance(delta);
		}, movements);

		if(!success)
		{
			if(current_index == sizeof...(Motions) - 1)
				return {success, remaining, updated};

			++current_index;

			if(remaining > duration{})
				return advance(remaining, {updated.lower(), updated.upper()+1});
			else
				return {true, remaining, updated};
		}
		else
		{
			return {success, remaining, updated};
		}
	}

};

} // namespace simple::motion

#endif /* end of include guard */
