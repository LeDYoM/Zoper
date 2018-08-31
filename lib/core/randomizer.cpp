#include "randomizer.hpp"
#include <random>

#include <lib/include/core/log.hpp>

namespace lib::core
{
	class Randomizer::RandomizerPrivate
	{
	public:
		std::random_device rd;
		std::mt19937 mt;

		RandomizerPrivate()
			: mt{ rd() } {}

		std::uniform_int_distribution<int> dist;
	};

	Randomizer::Randomizer(Host &host) 
        : AppService{ host }, 
        p_rPriv { muptr<RandomizerPrivate>() } {}

    Randomizer::~Randomizer() = default;

	u32 Randomizer::getUInt(const size_type max, const size_type min) const
	{
		log_debug_info("Asked for random number between ", min, " and ", max);
		__ASSERT(min != max, "The min and max parameters must be different");
		__ASSERT(max > min, "The max paramter must be greater than min");
		auto g(p_rPriv->dist(p_rPriv->mt) % (max - min));
		log_debug_info("\tGot ", g);
		log_debug_info("\tReturning ", min + g);
		return min + g;
	}
}
