#pragma once

#include "..\math.hpp"
#include "vector3.hpp"

/*namespace nl {
	class quaternion {
	private:
		f64 w, x, y, z;
	public:
		constexpr quaternion() { w = 0.0;  x = 0.0; y = 0.0; z = 0.0; };
		constexpr quaternion(const f64& t) { w = t; x = 0; y = 0; z = 0; };
		constexpr quaternion(const f64& a, const f64& b, const f64& c) { w = 0; x = a; y = b; z = c; };
		constexpr quaternion(const f64& t, const f64& a, const f64& b, const f64& c) { w = t; x = a; y = b; z = c; };

		constexpr f64vec3 vector() { return f64vec3(x, y, z); };
		constexpr f64vec3 vector() const { return f64vec3(x, y, z); };

		constexpr f64 real() { return w; };
		constexpr f64 real() const { return w; };

		constexpr quaternion conjugate() { return quaternion(w, -x, -y, -z); };
		constexpr quaternion conjugate() const { return quaternion(w, -x, -y, -z); };

		constexpr quaternion(const f64vec3& axis, const angle& a) {
			w = std::cos(a.val() / 2.0);
			f64 sin = std::sin(a.val() / 2.0);
			x = axis.x / sin; y = axis.y / sin; z = axis.z / sin;
		};

		static constexpr quaternion multiply(const quaternion& q0, const quaternion& q1) {

		};
	};
};*/