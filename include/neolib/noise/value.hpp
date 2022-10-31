#pragma once

#include <unordered_map>

#include "vector/vector2.hpp"
#include "interpolation.hpp"
#include "random.hpp"

namespace nl {
	class baseNoise2d {
	public:
		std::unordered_map<nl::s32vec2, f64> map;
		u64 seed = 1;
		interpolation mode = interpolation::linear;

		baseNoise2d() = default;
		baseNoise2d(const u64& s) { seed = s; };
		baseNoise2d(const u64& s, const interpolation& ip) { seed = s; mode = ip; };

		f64 getLatticePoint(const s32vec2& coord) {
			if (map.contains(coord)) {
				return map[coord];
			}
			else {
				u32 x = coord.x;
				u32 y = coord.y;
				f64 val = random_clamped_uf64(random_u32(x ^ quarter_u32(y)) * seed);
				map[coord] = val;
				return val;
			};
		};

		f64 getPoint(const f64vec2& coord) {
			f64vec2 fcoord(fraction(coord.x), fraction(coord.y));
			s32vec2 icoord(std::floor(coord.x), std::floor(coord.y));

			f64 laa = getLatticePoint({ icoord.x, icoord.y });
			f64 lba = getLatticePoint({ icoord.x + 1, icoord.y });
			f64 lab = getLatticePoint({ icoord.x, icoord.y + 1 });
			f64 lbb = getLatticePoint({ icoord.x + 1, icoord.y + 1 });

			f64 aa = getLatticePoint({ icoord.x - 1, icoord.y - 1 });
			f64 ba = getLatticePoint({ icoord.x, icoord.y - 1 });
			f64 ca = getLatticePoint({ icoord.x + 1, icoord.y - 1 });
			f64 da = getLatticePoint({ icoord.x + 2, icoord.y - 1 });

			f64 ab = getLatticePoint({ icoord.x - 1, icoord.y });
			f64 bb = getLatticePoint({ icoord.x, icoord.y });
			f64 cb = getLatticePoint({ icoord.x + 1, icoord.y });
			f64 db = getLatticePoint({ icoord.x + 2, icoord.y });

			f64 ac = getLatticePoint({ icoord.x - 1, icoord.y + 1 });
			f64 bc = getLatticePoint({ icoord.x, icoord.y + 1 });
			f64 cc = getLatticePoint({ icoord.x + 1, icoord.y + 1 });
			f64 dc = getLatticePoint({ icoord.x + 2, icoord.y + 1 });

			f64 ad = getLatticePoint({ icoord.x - 1, icoord.y + 2 });
			f64 bd = getLatticePoint({ icoord.x, icoord.y + 2 });
			f64 cd = getLatticePoint({ icoord.x + 1, icoord.y + 2 });
			f64 dd = getLatticePoint({ icoord.x + 2, icoord.y + 2 });

			switch (mode) {
			case(interpolation::discrete):
				return getLatticePoint(icoord);
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
		};
	};
};