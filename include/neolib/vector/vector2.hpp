#pragma once

#include "..\math.hpp"
#include "..\angle.hpp"

namespace nl {
	template<typename T> class vector2 {
	public:
		T x, y;

		constexpr vector2() { x = 0; y = 0; };

		template<typename TT> constexpr vector2(const TT& a, const TT& b) { x = a; y = b; };
		template<typename TT> constexpr vector2(const TT& a) { x = a; y = a; };

		template<typename TT> constexpr vector2(const vector2<TT>& v) { x = v.x; y = v.y; };
		template<typename TT, typename TTT> constexpr vector2(const std::pair<TT, TTT>& p) { x = p.first; y = p.second; };

		constexpr T scalar() { return std::sqrt(x * x + y * y); };
		constexpr T scalar() const { return std::sqrt(x * x + y * y); };

		constexpr vector2& normalize() { x = scalar() == 0 ? 0 : x / scalar(); y = scalar() == 0 ? 0 : y / scalar(); return *this; };

		constexpr vector2 normalized() { return vector2(x, y).normalize(); };
		constexpr vector2 normalized() const { return vector2(x, y).normalize(); };

		template<typename TT> constexpr vector2& operator+=(const vector2<TT>& v) { x += v.x; y += v.y; return *this; };
		template<typename TT> constexpr vector2& operator-=(const vector2<TT>& v) { x -= v.x; y -= v.y; return *this; };

		template<typename TT> constexpr vector2& operator*=(const TT& v) { x *= v; y *= v; return *this; };
		template<typename TT> constexpr vector2& operator/=(const TT& v) { x /= v; y /= v; return *this; };

		template<typename TT> constexpr T dotprod(const vector2<TT>& v) { return x * v.x + y * v.y; };
		template<typename TT> constexpr T dotprod(const vector2<TT>& v) const { return x * v.x + y * v.y; };

		template<typename TT> constexpr vector2& operator%=(const vector2<TT>& v) { return (*this) *= normalized().dotprod(v.normalized()); };

		template<typename TT> void store(TT* target) {
			target[0] = x; target[1] = y;
		};
		template<typename TT> void store(TT* target) const {
			target[0] = x; target[1] = y;
		};

		constexpr vector2<T> xy() {
			return vector2<T>(x, y);
		};
		constexpr vector2<T> yx() {
			return vector2<T>(y, x);
		};

		constexpr angle dirAngle() {
			return y > 0.0 ? std::acos(normalized().x) : (angle::tau - std::acos(normalized().x));
		};
		constexpr angle dirAngle() const {
			return y > 0.0 ? std::acos(normalized().x) : (angle::tau - std::acos(normalized().x));
		};

		constexpr vector2(const angle& a) { x = std::cos(a.val()); y = std::sin(a.val()); };
	};
};

template<typename T> std::ostream& operator<<(std::ostream& out, const nl::vector2<T>& v) {
	return out << v.x << " " << v.y;
};

template<typename T> constexpr T operator~(const nl::vector2<T>& v) { return v.scalar(); };
template<typename T> constexpr nl::vector2<T> operator!(const nl::vector2<T>& v) { return v.normalized(); };

template<typename T, typename TT> constexpr nl::vector2<T> operator+(nl::vector2<T> vx, const nl::vector2<TT>& vy) { return vx += vy; };
template<typename T, typename TT> constexpr nl::vector2<T> operator-(nl::vector2<T> vx, const nl::vector2<TT>& vy) { return vx -= vy; };

template<typename T, typename TT> constexpr nl::vector2<T> operator*(nl::vector2<T> vx, const TT& y) { return vx *= y; };
template<typename T, typename TT> constexpr nl::vector2<T> operator/(nl::vector2<T> vx, const TT& y) { return vx /= y; };

template<typename T, typename TT> constexpr nl::vector2<T> operator%(nl::vector2<T> vx, const nl::vector2<TT>& y) { return vx %= y; };

template<typename T, typename TT> constexpr T operator&(const nl::vector2<T>& vx, const nl::vector2<TT>& vy) { return vx.dotprod(vy); };
template<typename T, typename TT> constexpr T operator|(const nl::vector2<T>& vx, const nl::vector2<TT>& vy) { return (!vx) & (!vy); };
template<typename T, typename TT> constexpr bool operator||(const nl::vector2<T>& vx, const nl::vector2<TT>& vy) { return vx.normalized() == vy.normalized(); };

namespace nl {
	using u8vec2 = nl::vector2<nl::u8>;
	using u16vec2 = nl::vector2<nl::u16>;
	using u32vec2 = nl::vector2<nl::u32>;
	using u64vec2 = nl::vector2<nl::u64>;
	using s8vec2 = nl::vector2<nl::s8>;
	using s16vec2 = nl::vector2<nl::s16>;
	using s32vec2 = nl::vector2<nl::s32>;
	using s64vec2 = nl::vector2<nl::s64>;

	using f32vec2 = nl::vector2<nl::f32>;
	using f64vec2 = nl::vector2<nl::f64>;
};

template<typename T> constexpr bool operator==(const nl::vector2<T>& lvec, const nl::vector2<T>& rvec) {
	return (lvec.x == rvec.x) && (lvec.y == rvec.y);
};

template<typename T> struct std::equal_to<nl::vector2<T>> {
	bool operator()(const nl::vector2<T>& lvec, const nl::vector2<T>& rvec) const {
		return (lvec.x == rvec.x) && (lvec.y == rvec.y);
	};
};

template<typename T> struct std::hash<nl::vector2<T>> {
	std::size_t operator()(nl::vector2<T> vec) const {
		std::size_t res;
		res = (res << 19) ^ (res >> 13); res ^= std::hash<T>{}(vec.x);
		res = (res << 19) ^ (res >> 13); res ^= std::hash<T>{}(vec.y);
		return res;
	};
};