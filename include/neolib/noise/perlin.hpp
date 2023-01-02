#pragma once

#include <unordered_map>

#include "..\vector\vector2.hpp"
#include "..\interpolation.hpp"

namespace nl {
	namespace perlin {
		class base2d {
		public:
			std::unordered_map<s32vec2, angle> map;
			u32 seed = 1;
			interpolation mode = interpolation::linear;

			bool sign = true;
			f64 offset = 0.0;
			bool abs = false;

			base2d() = default;
			base2d(u32 s) { seed = s; };
			base2d(const u32& s, const interpolation& ip) { seed = s; mode = ip; };

			base2d(const u32& s, const interpolation& ip, bool sgn, f64 off, bool a) {
				seed = s;
				mode = ip;

				sign = sgn;
				offset = off;
				abs = a;
			};

			angle getLatticeAngle(const s32vec2& coord) {
				if (map.contains(coord)) {
					return map[coord];
				}
				else {
					u32 x = coord.x;
					u32 y = coord.y;
					angle a = angle::random(random_u32(x ^ quarter_u32(y)) * seed);

					map[coord] = a;
					return a;
				};
			};

			f64vec2 getLatticeVector(const s32vec2& coord) {
				return f64vec2(getLatticeAngle(coord));
			};

			f64 getRawPoint(const s32vec2& icoord, const f64vec2& fcoord) {
				f64vec2 delta((fcoord.x - f64vec2(icoord).x) * 2.0, (fcoord.y - f64vec2(icoord).y) * 2.0);
				f64vec2 gradient = getLatticeVector(icoord);
				return delta & gradient;
			};

			f64 getPoint(const f64vec2& coord) {
				s32vec2 icoord(std::floor(coord.x), std::floor(coord.y));
				f64vec2 fcoord(fraction(coord.x), fraction(coord.y));

				f64 laa, lba, lab, lbb;

				switch (mode) {
				case(interpolation::nearest):
					return getRawPoint(icoord, icoord);
					break;
				case(interpolation::linear):
					laa = getRawPoint({ icoord.x, icoord.y }, coord);
					lba = getRawPoint({ icoord.x + 1, icoord.y }, coord);
					lab = getRawPoint({ icoord.x, icoord.y + 1 }, coord);
					lbb = getRawPoint({ icoord.x + 1, icoord.y + 1 }, coord);

					return bilinearInterpolation(laa, lba, lab, lbb, fcoord);
					break;
				case(interpolation::cubic):
					f64 aa = getRawPoint({ icoord.x - 1, icoord.y - 1 }, coord);
					f64 ba = getRawPoint({ icoord.x, icoord.y - 1 }, coord);
					f64 ca = getRawPoint({ icoord.x + 1, icoord.y - 1 }, coord);
					f64 da = getRawPoint({ icoord.x + 2, icoord.y - 1 }, coord);

					f64 ab = getRawPoint({ icoord.x - 1, icoord.y }, coord);
					f64 bb = getRawPoint({ icoord.x, icoord.y }, coord);
					f64 cb = getRawPoint({ icoord.x + 1, icoord.y }, coord);
					f64 db = getRawPoint({ icoord.x + 2, icoord.y }, coord);

					f64 ac = getRawPoint({ icoord.x - 1, icoord.y + 1 }, coord);
					f64 bc = getRawPoint({ icoord.x, icoord.y + 1 }, coord);
					f64 cc = getRawPoint({ icoord.x + 1, icoord.y + 1 }, coord);
					f64 dc = getRawPoint({ icoord.x + 2, icoord.y + 1 }, coord);

					f64 ad = getRawPoint({ icoord.x - 1, icoord.y + 2 }, coord);
					f64 bd = getRawPoint({ icoord.x, icoord.y + 2 }, coord);
					f64 cd = getRawPoint({ icoord.x + 1, icoord.y + 2 }, coord);
					f64 dd = getRawPoint({ icoord.x + 2, icoord.y + 2 }, coord);

					return bicubicInterpolation(
						aa, ba, ca, da,
						ab, bb, cb, db,
						ac, bc, cc, dc,
						ad, bd, cd, dd, fcoord);
					break;
				};
			};

			std::pair<f64, f64> range() {
				f64 min = sign ? (offset - 1.0) : offset;
				f64 max = offset + 1.0;

				if (abs) {
					f64 pmean = (min + max) / 2.0;

					if (pmean < 1.0) {
						min *= -1.0;
						max *= -1.0;
					}
					else if (pmean <= 0.0) {
						min = 0.0;
						max = -min;
					}
					else if (pmean <= 1.0) {
						min = 0.0;
					};
				};

				return std::make_pair(min, max);
			};

			f64 get(const f64vec2& coord) {
				f64 v = getPoint(coord);

				f64 val = smoothClamp(getPoint(coord));

				val = sign ? val : std::abs(val);
				val += offset;
				val = abs ? std::abs(val) : val;

				return val;
			};
		};

		class additive2d {
		public:
			std::vector<base2d> maps;
			std::pair<f64, f64> range{ -2.0, 2.0 };

			u32 seed = 1;
			u32 octaves = 1;
			f64 scale = 1.0;
			f64 amplitude = 1.0;

			f64 lacunarity = 2.0;
			f64 persistency = 0.5;

			bool sign = true;
			f64 offset = 0.0;
			bool abs = false;

			f64 level = 0.0;
			bool contour = false;

			void computeRange() {
				f64 min = 0.0;
				f64 max = 0.0;

				f64 influence = amplitude / persistency;

				for (u8 o = 0; o < octaves; o++) {
					influence *= persistency;

					std::pair<f64, f64> lpair = maps[o].range();
					lpair.first *= influence; lpair.second *= influence;

					min += std::min(lpair.first, lpair.second);
					max += std::max(lpair.first, lpair.second);
				};

				if (contour) {
					min -= level; max -= level;
					min = std::signbit(min * max) ? 0.0 : std::min(std::abs(min), std::abs(max));
					max = std::max(std::abs(min), std::abs(max));
				};

				range = std::make_pair(min, max);
			};

			void setupOctaves() {
				maps.resize(octaves);
				u32 s = seed;
				for (u8 o = 0; o < octaves; o++) {
					s *= s + 1;
					maps[o] = base2d(s, interpolation::cubic, sign, offset, abs);
				};
			};

			additive2d(u32 s, u32 oct) {
				seed = s;
				octaves = oct;

				setupOctaves();
				computeRange();
			};

			additive2d(u32 s, u32 oct, f64 sc) {
				seed = s;
				octaves = oct;
				scale = sc;

				setupOctaves();
				computeRange();
			};

			additive2d(u32 s, u32 oct, f64 sc, f64 amp) {
				seed = s;
				octaves = oct;
				scale = sc;
				amplitude = amp;
				
				setupOctaves();
				computeRange();
			};

			additive2d(u32 s, u32 oct, f64 sc, f64 amp, f64 lac, f64 persExp) {
				seed = s;
				octaves = oct;
				scale = sc;
				amplitude = amp;

				lacunarity = lac;
				persistency = std::pow(lacunarity, -persExp);

				setupOctaves();
				computeRange();
			};

			additive2d(u32 s, u32 oct, f64 sc, f64 amp, f64 lac, f64 persExp, f64 lvl, bool cont) {
				seed = s;
				octaves = oct;
				scale = sc;
				amplitude = amp;

				lacunarity = lac;
				persistency = std::pow(lacunarity, -persExp);

				level = lvl;
				contour = cont;

				setupOctaves();
				computeRange();
			};

			additive2d(u32 s, u32 oct, f64 sc, f64 amp, f64 lac, f64 persExp, f64 lvl, bool cont, bool sgn, f64 off, bool a) {
				seed = s;
				octaves = oct;
				scale = sc;
				amplitude = amp;

				lacunarity = lac;
				persistency = std::pow(lacunarity, -persExp);

				level = lvl;
				contour = cont;

				sign = sgn;
				offset = off;
				abs = a;

				setupOctaves();
				computeRange();
			};

			f64 getPoint(f64vec2 coord) {
				coord /= scale;
				f64 influence = amplitude / persistency;

				f64 res = 0.0;
				for (u8 o = 0; o < octaves; o++) {
					coord *= lacunarity;
					influence *= persistency;

					f64 val = smoothClamp(maps[o].getPoint(coord));

					val = sign ? val : std::abs(val);
					val += offset;
					val = abs ? std::abs(val) : val;

					res += val * influence;
				};

				res = contour ? std::abs(res - level) : res;
				return res;
			};

			f64 getPoint(f64vec2 coord, bool remap) {
				return remap ? map(getPoint(coord), range.first, range.second, -1.0, 1.0) : getPoint(coord);
			};
		};

	};
};