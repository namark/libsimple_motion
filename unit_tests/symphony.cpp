#include <cassert>
#include <chrono>
#include "simple/motion/movement.hpp"
#include "simple/motion/symphony.hpp"

using namespace simple::motion;
using duration = std::chrono::duration<float>;
using namespace std::literals;
using simple::support::way;

// TODO: use rational, can't rely on float

void SharpTurn()
{
	using float2 = std::array<float,2>;
	float2 block{0,0};

	using move = movement<duration, float>;

	auto sharp_turn = symphony(std::vector<move>{
		move{0,1, 1250ms},
		move{0,1, 1250ms}
	});


	sharp_turn.move(block, 500ms);
	assert( block[0] == way(0.f,1.f, 0.5f / 1.25f) );
	assert( block[1] == 0.f );

	sharp_turn.move(block, 500ms);
	assert( block[0] == way(0.f,1.f, 1.f / 1.25f) );
	assert( block[1] == 0.f );

	sharp_turn.move(block, 500ms);
	assert( block[0] == way(0.f,1.f, 1.f) );
	assert( block[1] == way(0.f,1.f, 0.25f / 1.25f) );

}

int main()
{
	SharpTurn();
	return 0;
}