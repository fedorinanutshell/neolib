#pragma once

#include <numbers>

#include "math.hpp"

namespace nl {
	struct angle {
	public:
		static constexpr f64 pi = std::numbers::pi;
		static constexpr f64 tau = 2.0 * pi;

		static constexpr f64 rtod = 180.0 / pi;
		static constexpr f64 dtor = pi / 180.0;

		enum class angleTypes {
			//		uclamped, sclamped,
			urad, srad,
			udegree, sdegree
		};
		using at = angleTypes;
	private:
		f64 a;
	public:
		static constexpr f64 from(const f64& x, const at& type) {
			switch (type) {
			case at::urad:
				return x;
				break;
			case at::srad:
				return x < 0.0 ? tau + x : x;
				break;
			case at::udegree:
				return x * dtor;
				break;
			case at::sdegree:
				return x < 0.0 ? (360.0 + x) * dtor : x * dtor;
				break;
			};
		};

		static constexpr f64 to(const f64& x, const at& type) {
			switch (type) {
			case at::urad:
				return x;
				break;
			case at::srad:
				return x > pi ? x - tau : x;
				break;
			case at::udegree:
				return x * rtod;
				break;
			case at::sdegree:
				return x > pi ? (x - tau) * rtod : x * rtod;
				break;
			};
		};

		static constexpr f64 convert(const f64& x, const at& type, at target) {
			return type == target ? x : to(from(x, type), target);
		};

		static constexpr f64 clamp(const f64& x, const at& type) {
			switch (type) {
			case at::urad:
				return mod(x, tau);
				break;
			case at::srad:
				return mod(x + pi, tau) - pi;
				break;
			case at::udegree:
				return mod(x, 360.0);
				break;
			case at::sdegree:
				return mod(x + 180.0, 360.0) - 180.0;
				break;
			};
		};

		static constexpr f64 clamp(const f64& x) {
			return clamp(x, at::urad);
		};

		constexpr operator double() { return a; };
		constexpr operator double() const { return a; };

		constexpr angle() {
			a = 0.0;
		};
		constexpr angle(const f64& x) {
			a = clamp(x);
		};
		constexpr angle(const f64& x, const at& type) {
			a = from(x, type);
		};

		constexpr angle& clamp() {
			a = clamp(a);
			return *this;
		};

		constexpr angle clamped() {
			angle res = a;
			return res.clamp();
		};
		constexpr angle clamped() const {
			angle res = a;
			return res.clamp();
		};

		constexpr f64 val() {
			return a;
		};
		constexpr f64 val(const at& type) {
			return to(a, type);
		};

		constexpr f64 val() const {
			return a;
		};
		constexpr f64 val(const at& type) const {
			return to(a, type);
		};

		constexpr f64 set(const f64& x) {
			return a = x;
		};
		constexpr f64 set(const f64& x, const at& type) {
			return a = from(x, type);
		};

		constexpr angle& operator+=(const f64& x) {
			a += x;
			return clamp();
		};
		constexpr angle& operator+=(const angle& x) {
			a += x.val();
			return clamp();
		};

		constexpr angle& operator*=(const f64& x) {
			a *= x;
			return clamp();
		};

		constexpr angle& operator/=(const f64& x) {
			a /= x;
			return clamp();
		};
	};

	constexpr angle operator+(angle x, const angle& y) { return x += y; };
	constexpr angle operator-(const angle& x, const angle& y) { return x.val() - y.val(); };
	constexpr angle operator*(angle x, const f64& y) { return x *= y; };
	constexpr angle operator/(angle x, const f64& y) { return x /= y; };
	constexpr angle operator-(angle x) { return x += angle::pi; };

	constexpr angle operator==(const angle& x, const angle& y) { return x.val() == y.val(); };
	constexpr angle operator<(const angle& x, const angle& y) { return x.val() < y.val(); };
	constexpr angle operator>(const angle& x, const angle& y) { return x.val() > y.val(); };
	constexpr angle operator<=(const angle& x, const angle& y) { return x.val() <= y.val(); };
	constexpr angle operator>=(const angle& x, const angle& y) { return x.val() >= y.val(); };

	std::ostream& operator<<(std::ostream& out, const angle& x) {
		return out << x.val();
	};
}