#pragma once

#include <cstdlib>

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

	template<typename T> constexpr T snap(const T& x, const T& y) {
		return std::floor(x / y) * y;
	};

	template<typename T> constexpr T abssnap(const T& x, const T& y) {
		return absfloor(x / y) * y;
	};

	template<typename T> constexpr T map(const T& v, const T& minin, const T& maxin, const T& minout, const T& maxout) {
		return ((v - minin) / (maxin - minin)) * (maxout - minout) + minout;
	};

	template<typename T> constexpr T map(T v, const T& minin, const T& maxin, const T& minout, const T& maxout, bool clamp) {
		if (clamp) v = std::min(std::max(v, minin), maxin);
		return ((v - minin) / (maxin - minin)) * (maxout - minout) + minout;
	};

	template<typename T> constexpr T smoothClamp(const T& v) {
		return v / sqrt(1 + v * v);
	};

	template<typename T> constexpr T smooothClamp(const T& v) {
		return v / sqrt(sqrt((1 + v * v * v * v)));
	};

	template<typename T> constexpr T smoothClamp(const T& v, const T& e) {
		return v / pow(1 + pow(v, nl::f64(e)), 1.0 / f64(e));
	};

	template<typename T> constexpr T smoothClamp(const T& v, const T& min, const T& max) {
		return map(smoothClamp(map(v, min, max, -1.0, 1.0)), min, max);
	};

	template<typename T> constexpr T smooothClamp(const T& v, const T& min, const T& max) {
		return map(smooothClamp(map(v, min, max, -1.0, 1.0)), min, max);
	};
}