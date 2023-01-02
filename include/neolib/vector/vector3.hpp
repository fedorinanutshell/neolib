#pragma once

#include <iostream>

#include "..\math.hpp"
#include "..\angle.hpp"
#include "vector2.hpp"

namespace nl {
	template<typename T> class vector3 {
	public:
		T x, y, z;

		constexpr vector3() { x = 0; y = 0; z = 0; };

		template<typename TT> constexpr vector3(const TT& a, const TT& b, const TT& c) { x = a; y = b; z = c; };
		template<typename TT> constexpr vector3(const TT& a) { x = a; y = a; z = a; };

		template<typename TT> constexpr vector3(const vector3<TT>& v) { x = v.x; y = v.y; z = v.z; };
		template<typename TT> constexpr vector3(const std::tuple<TT, TT, TT> t) { x = std::get<0>(t); y = std::get<1>(t); z = std::get<2>(t); };

		constexpr T scalar() { return x * x + y * y + z * z; };
		constexpr T scalar() const { return x * x + y * y + z * z; };

		constexpr vector3& normalize() {
			T s = scalar();
			if (s == 0) {
				x = 0; y = 0; z = 0;
			}
			else {
				x /= s; y /= s; z /= s;
			};
			return *this;
		};

		constexpr vector3 normalized() { return vector3(x, y, z).normalize(); };
		constexpr vector3 normalized() const { return vector3(x, y, z).normalize(); };

		template<typename TT> constexpr vector3& operator+=(const vector3<TT>& v) { x += v.x; y += v.y; z += v.z; return *this; };
		template<typename TT> constexpr vector3& operator-=(const vector3<TT>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; };

		template<typename TT> constexpr vector3& operator*=(const TT& v) { x *= v; y *= v; z *= v; return *this; };
		template<typename TT> constexpr vector3& operator/=(const TT& v) { x /= v; y /= v; z /= v; return *this; };

		template<typename TT> constexpr T dotprod(const vector3<TT>& v) { return x * v.x + y * v.y + z * v.z; };
		template<typename TT> constexpr T dotprod(const vector3<TT>& v) const { return x * v.x + y * v.y + z * v.z; };

		template<typename TT> constexpr vector3& operator%=(const vector3<TT>& v) { return (*this) *= normalized().dotprod(v.normalized()); };

		template<typename TT> void store(TT* target) {
			target[0] = x; target[1] = y; target[2] = z;
		};
		template<typename TT> void store(TT* target) const {
			target[0] = x; target[1] = y; target[2] = z;
		};
	};
};

template<typename T> std::ostream& operator<<(std::ostream& out, const nl::vector3<T>& v) {
	return out << v.x << " " << v.y << " " << v.z;
};

template<typename T> constexpr T operator~(const nl::vector3<T>& v) { return v.scalar(); };
template<typename T> constexpr nl::vector3<T> operator!(const nl::vector3<T>& v) { return v.normalized(); };

template<typename T, typename TT> constexpr nl::vector3<T> operator+(nl::vector3<T> vx, const nl::vector3<TT>& vy) { return vx += vy; };
template<typename T, typename TT> constexpr nl::vector3<T> operator-(nl::vector3<T> vx, const nl::vector3<TT>& vy) { return vx -= vy; };

template<typename T, typename TT> constexpr nl::vector3<T> operator*(nl::vector3<T> vx, const TT& y) { return vx *= y; };
template<typename T, typename TT> constexpr nl::vector3<T> operator/(nl::vector3<T> vx, const TT& y) { return vx /= y; };

template<typename T, typename TT> constexpr nl::vector3<T> operator%(nl::vector3<T> vx, const nl::vector3<TT>& y) { return vx %= y; };

template<typename T, typename TT> constexpr T operator&(const nl::vector3<T>& vx, const nl::vector3<TT>& vy) { return vx.dotprod(vy); };
template<typename T, typename TT> constexpr T operator|(const nl::vector3<T>& vx, const nl::vector3<TT>& vy) { return (!vx) & (!vy); };
template<typename T, typename TT> constexpr bool operator||(const nl::vector3<T>& vx, const nl::vector3<TT>& vy) { return vx.normalized() == vy.normalized(); };

namespace nl {
	using u8vec3 = nl::vector3<nl::u8>;
	using u16vec3 = nl::vector3<nl::u16>;
	using u32vec3 = nl::vector3<nl::u32>;
	using u64vec3 = nl::vector3<nl::u64>;
	using s8vec3 = nl::vector3<nl::s8>;
	using s16vec3 = nl::vector3<nl::s16>;
	using s32vec3 = nl::vector3<nl::s32>;
	using s64vec3 = nl::vector3<nl::s64>;

	using f32vec3 = nl::vector3<nl::f32>;
	using f64vec3 = nl::vector3<nl::f64>;
};

template<typename T> constexpr bool operator==(const nl::vector3<T>& lvec, const nl::vector3<T>& rvec) {
	return (lvec.x == rvec.x) && (lvec.y == rvec.y) && (lvec.z == rvec.z);
};

template<typename T> struct std::equal_to<nl::vector3<T>> {
	bool operator()(const nl::vector3<T>& lvec, const nl::vector3<T>& rvec) const {
		return (lvec.x == rvec.x) && (lvec.y == rvec.y) && (lvec.z == rvec.z);
	};
};

template<typename T> struct std::hash<nl::vector3<T>> {
	std::size_t operator()(nl::vector3<T> vec) const {
		std::size_t res;
		res = (res << 19) ^ (res >> 13); res ^= std::hash<T>{}(vec.x);
		res = (res << 19) ^ (res >> 13); res ^= std::hash<T>{}(vec.y);
		res = (res << 19) ^ (res >> 13); res ^= std::hash<T>{}(vec.z);
		return res;
	};
};