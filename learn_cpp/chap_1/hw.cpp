#include <iostream>

int main()
{
	int width {5}; // copy assignment of value 5 into variable width

	std::cout << width << '\n'; // prints 5

	width = 7; // change value stored in variable width to 7

	std::cout << width << '\n'; // prints 7

	return 0;
}