#include <cassert>
#include <chrono>

#include "simple/motion/movement.hpp"
#include "simple/motion/melody.hpp"

using namespace simple::motion;
using duration = std::chrono::duration<float>;
using namespace std::literals;
using simple::support::way;

// TODO: use rational, can't rely on float

void SharpTurn()
{
	struct float2 { float x, y; };
	float2 block{0,0};

	auto sharp_turn = melody(
		movement<duration, float>{0,1, 1250ms},
		movement<duration, float>{0,1, 1250ms}
	);


	sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms);
	assert( block.x == way(0.f,1.f, 0.5f / 1.25f) );
	assert( block.y == 0.f );

	sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms);
	assert( block.x == way(0.f,1.f, 1.f / 1.25f) );
	assert( block.y == 0.f );

	sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms);
	assert( block.x == way(0.f,1.f, 1.f) );
	assert( block.y == way(0.f,1.f, 0.25f / 1.25f) );

}

int main()
{
	SharpTurn();
	return 0;
}
