#include <cassert>
#include <chrono>
#include "simple/motion/movement.hpp"
#include "simple/motion/symphony.hpp"

using namespace std::literals;
using duration = std::chrono::duration<float>;
using simple::support::way;
using simple::motion::symphony;

// TODO: use rational, can't rely on float
using movement = simple::motion::movement<duration,float,float>;

void SharpTurn()
{
	using float2 = std::array<float,2>;
	float2 block{0,0};

	auto sharp_turn = symphony(std::vector<movement>{
		movement{1250ms, 0,1},
		movement{1250ms, 0,1}
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
