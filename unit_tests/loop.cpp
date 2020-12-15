#include <chrono>
#include "simple/motion/movement.hpp"
#include "simple/motion/melody.hpp"
#include "simple/motion/symphony.hpp"
#include "simple/motion/algorithm.hpp"

using namespace std::literals;
// using duration = std::chrono::duration<float>;
using duration = std::chrono::steady_clock::duration;
using simple::support::way;

// TODO: use rational, can't rely on float
using movement = simple::motion::movement<duration,float,float>;

void BasicLoop()
{
	auto justamo = movement{1250ms, 0,1};
	float x = 0.0f;
	assert( 0 == loop(x, justamo, 500ms) );
	assert( x == way(0.f,1.f, 0.5f / 1.25f) );
	assert( 0 == loop(x, justamo, 500ms) );
	assert( x == way(0.f,1.f, 1.0f / 1.25f) );
	assert( 1 == loop(x, justamo, 500ms) );
	assert( x == way(0.f,1.f, 0.25f / 1.25f) );
}

void LoopDontSkip()
{
	{
		auto sequence = simple::motion::melody(
			movement{10ms, 0,1},
			movement{10ms, 0,1},
			movement{10ms, 0,1},
			movement{10ms, 0,1}
		);
		std::array<float,4> values{};
		assert( 1 == loop(values, sequence, 60ms) ); // TODO: do 55ms with rational
		assert(( values == std::array{1.f, 1.f, 1.f, 1.f} ));
		assert( 2 == loop(values, sequence, 90ms) );
		assert(( values == std::array{1.f, 1.f, 1.f, 1.f} ));
	}

	{
		auto sequence = simple::motion::symphony(std::array{
			movement{10ms, 0,1},
			movement{10ms, 0,1},
			movement{10ms, 0,1},
			movement{10ms, 0,1}
		});
		std::array<float,4> values{};
		assert( 1 == loop(values, sequence, 60ms) ); // TODO: do 55ms with rational
		assert(( values == std::array{1.f, 1.f, 1.f, 1.f} ));
		assert( 2 == loop(values, sequence, 90ms) );
		assert(( values == std::array{1.f, 1.f, 1.f, 1.f} ));
	}
}

int main()
{
	BasicLoop();
	LoopDontSkip();
	return 0;
}
