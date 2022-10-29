#include <iostream>

#include "angle.hpp"

int main() {
	nl::angle a = 3;
	std::cout << a << "\n";
	a.set(-90.0, nl::angle::at::sdegree);
	std::cout << a << " " << a.val(nl::angle::at::sdegree) << "\n";
	nl::angle b = 0.5;
	std::cout << b << " " << a + b << " " << -(a + b) << " " << "\n";
};