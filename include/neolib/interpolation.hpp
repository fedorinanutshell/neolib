#pragma once

#include "vector\vector2.hpp"

namespace nl {
	enum class interpolation {
		nearest, linear, cubic
	};

	constexpr f64 linearInterpolation(f64 a, f64 b, f64 pos, f64 range) {
		return a + (b - a) * (pos / range);
	};
	constexpr f64 linearInterpolation(f64 a, f64 b, f64 pos) {
		return linearInterpolation(a, b, pos, 1.0f);
	};

	constexpr f64 bilinearInterpolation(f64 aa, f64 ba, f64 ab, f64 bb, const f64vec2& pos, const f64vec2& range) {
		f64 ia = linearInterpolation(aa, ba, pos.x, range.x);
		f64 ib = linearInterpolation(ab, bb, pos.y, range.y);

		return linearInterpolation(ia, ib, pos.x, range.y);
	};
	constexpr f64 bilinearInterpolation(f64 aa, f64 ba, f64 ab, f64 bb, const f64vec2& pos, f64 range) {
		return bilinearInterpolation(aa, ba, ab, bb, pos, f64vec2(range));
	};
	constexpr f64 bilinearInterpolation(f64 aa, f64 ba, f64 ab, f64 bb, const f64vec2& pos) {
		return bilinearInterpolation(aa, ba, ab, bb, pos, f64vec2(1.0f));
	};

	/*constexpr f64 trilinearInterpolation(f64 aaa, f64 baa, f64 aba, f64 bba, f64 aab, f64 bab, f64 abb, f64 bbb, const vec3<float>& pos, const vec3<float>& range) {
		f64 iia = bilinearInterpolation(aaa, baa, aba, bba, f64vec2({ pos[0], pos[1] }), f64vec2({ range[0], range[1] }));
		f64 iib = bilinearInterpolation(aab, bab, abb, bbb, f64vec2({ pos[0], pos[1] }), f64vec2({ range[0], range[1] }));

		return linearInterpolation(iia, iib, pos[2], range[2]);
	};
	constexpr f64 trilinearInterpolation(f64 aaa, f64 baa, f64 aba, f64 bba, f64 aab, f64 bab, f64 abb, f64 bbb, const vec3<float>& pos, f64 range) {
		return trilinearInterpolation(aaa, baa, aba, bba, aab, bab, abb, bbb, pos, f64vec2(range));
	};
	constexpr f64 trilinearInterpolation(f64 aaa, f64 baa, f64 aba, f64 bba, f64 aab, f64 bab, f64 abb, f64 bbb, const vec3<float>& pos) {
		return trilinearInterpolation(aaa, baa, aba, bba, aab, bab, abb, bbb, pos, 1.0f);
	};*/

	constexpr f64 cubicInterpolation(f64 a, f64 b, f64 c, f64 d, f64 pos, f64 range) {
		pos /= range;

		f64 p0 = b * (2.0f * pos * pos * pos - 3.0f * pos * pos + 1.0f);
		f64 m0 = (c - a) / 2.0f * (pos * pos * pos - 2.0f * pos * pos + pos);
		f64 p1 = c * (-2.0f * pos * pos * pos + 3 * pos * pos);
		f64 m1 = (d - b) / 2.0f * (pos * pos * pos - pos * pos);

		return p0 + m0 + p1 + m1;
	};
	constexpr f64 cubicInterpolation(f64 a, f64 b, f64 c, f64 d, f64 pos) {
		return cubicInterpolation(a, b, c, d, pos, 1.0f);
	};

	constexpr f64 bicubicInterpolation(
		f64 aa, f64 ba, f64 ca, f64 da,
		f64 ab, f64 bb, f64 cb, f64 db,
		f64 ac, f64 bc, f64 cc, f64 dc,
		f64 ad, f64 bd, f64 cd, f64 dd,
		const f64vec2& pos, const f64vec2& range) {
		f64 ia = cubicInterpolation(aa, ba, ca, da, pos.x, range.x);
		f64 ib = cubicInterpolation(ab, bb, cb, db, pos.x, range.x);
		f64 ic = cubicInterpolation(ac, bc, cc, dc, pos.x, range.x);
		f64 id = cubicInterpolation(ad, bd, cd, dd, pos.x, range.x);

		return cubicInterpolation(ia, ib, ic, id, pos.y, range.y);
	};
	constexpr f64 bicubicInterpolation(
		f64 aa, f64 ba, f64 ca, f64 da,
		f64 ab, f64 bb, f64 cb, f64 db,
		f64 ac, f64 bc, f64 cc, f64 dc,
		f64 ad, f64 bd, f64 cd, f64 dd,
		const f64vec2& pos, f64 range) {
		return bicubicInterpolation(aa, ba, ca, da, ab, bb, cb, db, ac, bc, cc, dc, ad, bd, cd, dd, pos, f64vec2(range));
	};
	constexpr f64 bicubicInterpolation(
		f64 aa, f64 ba, f64 ca, f64 da,
		f64 ab, f64 bb, f64 cb, f64 db,
		f64 ac, f64 bc, f64 cc, f64 dc,
		f64 ad, f64 bd, f64 cd, f64 dd,
		const f64vec2& pos) {
		return bicubicInterpolation(aa, ba, ca, da, ab, bb, cb, db, ac, bc, cc, dc, ad, bd, cd, dd, pos, 1.0f);
	};
};