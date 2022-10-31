#pragma once

#include <unordered_map>

#include "..\vector\vector2.hpp"
#include "..\interpolation.hpp"

namespace nl {
	class basePerlin2d {
	public:
		std::unordered_map<s32vec2, angle> map;
		u32 seed = 1;
		interpolation mode = interpolation::linear;

		basePerlin2d() = default;
		basePerlin2d(u32 s) { seed = s; };
		basePerlin2d(const u32& s, const interpolation& ip) { seed = s; mode = ip; };

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
			//std::cout << delta.x << " " << delta.y << " | " << gradient.x << " " << gradient.y << "\n";
			return delta & gradient;
		};

		f64 getPoint(const f64vec2& coord) {
			s32vec2 icoord(std::floor(coord.x), std::floor(coord.y));
			f64vec2 fcoord(fraction(coord.x), fraction(coord.y));

			f64 laa = getRawPoint({ icoord.x, icoord.y }, coord);
			f64 lba = getRawPoint({ icoord.x + 1, icoord.y }, coord);
			f64 lab = getRawPoint({ icoord.x, icoord.y + 1 }, coord);
			f64 lbb = getRawPoint({ icoord.x + 1, icoord.y + 1 }, coord);

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

			switch (mode) {
			case(interpolation::discrete):
				return getRawPoint(icoord, icoord);
				break;
			case(interpolation::linear):
				return bilinearInterpolation(laa, lba, lab, lbb, fcoord);
				break;
			case(interpolation::cubic):
				return bicubicInterpolation(
					aa, ba, ca, da,
					ab, bb, cb, db,
					ac, bc, cc, dc,
					ad, bd, cd, dd, fcoord);
				break;
			};
		}
	};

	struct noiseParameters {
		f64 lacunarity = 0.5;
		f64 persistency = 0.5;
		bool sign = true;

		f64 scale = 1.0;
		u8 octaves = 1;
		u32 seed = 1;
		bool abs = false;

		noiseParameters() = default;
		noiseParameters(const u32& s) { seed = s; };
		noiseParameters(const u32& s, const u8& o) { seed = s; octaves = o; };
		noiseParameters(const u32& s, const u8& o, const f64& sc) { seed = s; octaves = o; scale = sc; };
	};

	class perlin2d {
	public:
		std::unordered_map<u8, basePerlin2d> map;
		noiseParameters mode;

		perlin2d() = default;
		perlin2d(const noiseParameters& src) {
			mode = src;
			u32 seed = src.seed;

			for (u8 o = 0; o < mode.octaves; o++) {
				seed *= mode.seed + 1;
				map[o] = basePerlin2d(seed, interpolation::cubic);
			};
		};

		f64 getPoint(f64vec2 coord) {
			coord /= mode.scale;
			f64 lacunarity = 1.0;
			f64 persistency = 1.0;

			f64 res = 0.0;
			for (u8 o = 0; o < mode.octaves; o++) {
				lacunarity *= mode.lacunarity;
				persistency *= mode.persistency;

				f64 val = map[o].getPoint({ coord.x / lacunarity, coord.y / lacunarity });
				//std::cout << coord.x << "\n";
				val = mode.sign ? val : (val * 0.5 + 0.5);
				val *= persistency;

				res += val;
			};

			res = mode.abs ? std::abs(res) : res;
			return res;
		};
	};
};