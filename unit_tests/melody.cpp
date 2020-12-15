#include <cassert>
#include <chrono>

#include "simple/motion/movement.hpp"
#include "simple/motion/melody.hpp"

using namespace std::literals;
using duration = std::chrono::duration<float>;
using simple::support::way;
using simple::motion::melody;


void SharpTurn()
{
	// TODO: use rational, can't rely on float
	using movement = simple::motion::movement<duration,float,float>;

	struct float2 { float x, y; };
	float2 block{0,0};

	auto sharp_turn = melody(
		movement{1250ms, 0,1},
		movement{1250ms, 0,1}
	);


	auto range = sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms).updated;
	assert( block.x == way(0.f,1.f, 0.5f / 1.25f) );
	assert( block.y == 0.f );
	assert( range.upper() - range.lower() == 1 );

	range = sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms).updated;
	assert( block.x == way(0.f,1.f, 1.f / 1.25f) );
	assert( block.y == 0.f );
	assert( range.upper() - range.lower() == 1 );

	range = sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms).updated;
	assert( block.x == way(0.f,1.f, 1.f) );
	assert( block.y == way(0.f,1.f, 0.25f / 1.25f) );
	assert( range.upper() - range.lower() == 2 );

	range = sharp_turn.move(std::forward_as_tuple(block.x, block.y), 500ms).updated;
	assert( block.x == way(0.f,1.f, 1.f) );
	assert( block.y == way(0.f,1.f, 0.75f / 1.25f) );
	assert( range.upper() - range.lower() == 1 );

}

void Advance()
{
	// TODO: use rational, can't rely on float
	using movement = simple::motion::movement<std::chrono::milliseconds,float,float>;

	auto sequence = melody(
		movement{10ms, 0,1},
		movement{10ms, 0,1},
		movement{10ms, 0,1},
		movement{10ms, 0,1}
	);

	auto result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == 0 );
	assert( result.updated.upper() == 1 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == 1 );
	assert( result.updated.upper() == 2 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( not result.done );
	assert( result.updated.lower() == 2 );
	assert( result.updated.upper() == 3 );
	assert( result.remaining == 0ms );

	result = sequence.advance(10ms);
	assert( result.done );
	assert( result.updated.lower() == 3 );
	assert( result.updated.upper() == 4 );
	assert( result.remaining == 0ms );

	sequence.reset();

	result = sequence.advance(15ms);
	assert( not result.done );
	assert( result.updated.lower() == 0 );
	assert( result.updated.upper() == 2 );
	assert( result.remaining == 0ms );

	result = sequence.advance(15ms);
	assert( not result.done );
	assert( result.updated.lower() == 1 );
	assert( result.updated.upper() == 3 );
	assert( result.remaining == 0ms );

	result = sequence.advance(15ms);
	assert( result.done );
	assert( result.updated.lower() == 3 );
	assert( result.updated.upper() == 4 );
	assert( result.remaining == 5ms );

}

void EmptyIsDone()
{
	using movement = simple::motion::movement<std::chrono::milliseconds,float,float>;
	using melody_t = melody<movement, movement>;
	melody_t m;
	assert(m.done());
}

int main()
{
	SharpTurn();
	Advance();
	EmptyIsDone();
	return 0;
}
