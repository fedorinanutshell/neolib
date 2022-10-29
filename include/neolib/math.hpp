#pragma once

#include <cmath>

#include "base.hpp"

namespace nl {
	template<typename T> constexpr T absfloor(const T& x) {
		return std::signbit(x) ? std::floor(x) : std::ceil(x);
	};

	template<typename T> constexpr T fraction(const T& x) {
		return x - std::floor(x);
	};

	template<typename T> constexpr T absfraction(const T& x) {
		return std::abs(x) - absfloor(x);
	};

	template<typename T> constexpr T mod(const T& x, const T& y) {
		return fraction(x / y) * y;
	};

	template<typename T> constexpr T absmod(const T& x, const T& y) {
		return absfraction(x / y) * std::abs(y);
	};
}