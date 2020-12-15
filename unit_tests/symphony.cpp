#include <cassert>
#include <chrono>
#include "simple/motion/movement.hpp"
#include "simple/motion/symphony.hpp"

using namespace std::literals;
using duration = std::chrono::duration<float>;
using simple::support::way;
using simple::motion::symphony;


template <typename T> class show_type;

void SharpTurn()
{
	// TODO: use rational, can't rely on float
	using movement = simple::motion::movement<duration,float,float>;
	std::array<float,2> block{0,0};

	auto sharp_turn = symphony(std::vector<movement>{
		movement{1250ms, 0,1},
		movement{1250ms, 0,1}
	});


	auto range = sharp_turn.move(block, 500ms).updated;
	assert( block[0] == way(0.f,1.f, 0.5f / 1.25f) );
	assert( block[1] == 0.f );
	assert( range.end() - range.begin() == 1 );

	range = sharp_turn.move(block, 500ms).updated;
	assert( block[0] == way(0.f,1.f, 1.f / 1.25f) );
	assert( block[1] == 0.f );
	assert( range.end() - range.begin() == 1 );

	range = sharp_turn.move(block, 500ms).updated;
	assert( block[0] == way(0.f,1.f, 1.f) );
	assert( block[1] == way(0.f,1.f, 0.25f / 1.25f) );
	assert( range.end() - range.begin() == 2 );

	range = sharp_turn.move(block, 500ms).updated;
	assert( block[0] == way(0.f,1.f, 1.f) );
	assert( block[1] == way(0.f,1.f, 0.75f / 1.25f) );
	assert( range.end() - range.begin() == 1 );

}

void Advance()
{
	// TODO: use rational, can't rely on float
	using movement = simple::motion::movement<std::chrono::milliseconds,float,float>;

	auto sequence = symphony(std::vector{
		movement{10ms, 0,1},
		movement{10ms, 0,1},
		movement{10ms, 0,1},
		movement{10ms, 0,1}
	});

	const auto begin = sequence.range().begin();

	auto result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == begin+0 );
	assert( result.updated.upper() == begin+1 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == begin+1 );
	assert( result.updated.upper() == begin+2 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == begin+2 );
	assert( result.updated.upper() == begin+3 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( result.done );
	assert( result.updated.lower() == begin+3 );
	assert( result.updated.upper() == begin+4 );
	assert( result.remaining == 0ms );

	sequence.reset();

	result = sequence.advance(15ms);
	assert( not result.done );
	assert( result.updated.lower() == begin+0 );
	assert( result.updated.upper() == begin+2 );
	assert( result.remaining == 0ms );

	result = sequence.advance(15ms);
	assert( not result.done );
	assert( result.updated.lower() == begin+1 );
	assert( result.updated.upper() == begin+3 );
	assert( result.remaining == 0ms );

	result = sequence.advance(15ms);
	assert( result.done );
	assert( result.updated.lower() == begin+3 );
	assert( result.updated.upper() == begin+4 );
	assert( result.remaining == 5ms );

}

int main()
{
	SharpTurn();
	Advance();
	return 0;
}
