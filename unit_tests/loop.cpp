#include <chrono>
#include "simple/motion/movement.hpp"
#include "simple/motion/melody.hpp"
#include "simple/motion/algorithm.hpp"

using namespace simple::motion;
using duration = std::chrono::duration<float>;
using namespace std::literals;
using simple::support::way;

// TODO: use rational, can't rely on float

void BasicLoop()
{
	auto justamo = movement<duration,float>{0,1, 1250ms};
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
	auto sequence = melody(
		movement<duration, float>{0,1, 10ms},
		movement<duration, float>{0,1, 10ms},
		movement<duration, float>{0,1, 10ms},
		movement<duration, float>{0,1, 10ms}
	);
	std::array<float,4> values{};
	assert( 1 == loop(values, sequence, 60ms) ); // TODO: do 55ms with rational
	assert(( values == std::array{1.f, 1.f, 1.f, 1.f} ));
}

int main()
{
	BasicLoop();
	LoopDontSkip();
	return 0;
}
