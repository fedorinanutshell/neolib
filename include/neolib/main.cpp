#include <iostream>
#include <iomanip>

#include "angle.hpp"
#include "random.hpp"
#include "vector/vector2.hpp"

int main() {
	nl::f32 x32 = 0.12345678910111213141516171819;
	nl::f64 x64 = 0.12345678910111213141516171819;
	std::cout << "f32 " << std::setprecision(64) << x32 << "\n";
	std::cout << "f64 " << std::setprecision(64) << x64 << "\n";

	std::cout << std::setprecision(16) << nl::f64vec2(nl::angle(1.57)) << "\n";
};