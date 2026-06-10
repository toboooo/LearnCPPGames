#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <chrono>
#include <random>

namespace Random {
	inline std::mt19937 init_engine() {
		std::random_device rd;
		std::seed_seq ss {
			static_cast<std::seed_seq::result_type>(
				std::chrono::steady_clock::now().time_since_epoch().count()
			),
			rd(), rd(), rd(), rd(), rd(), rd(), rd()
		};
		return std::mt19937(ss);
	}

	//inline std::mt19937 rng = init_engine();
	//static std::mt19937 rng = init_engine();
	namespace {
		std::mt19937 rng = init_engine();
	};

	inline int get(int min, int max) {
		return std::uniform_int_distribution<int>(min, max)(rng);
	}
};

#endif
