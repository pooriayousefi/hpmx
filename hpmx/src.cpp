
#include <functional>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>

#include "hpmx.h"

using namespace hpmx;

// real::valued::matrix test function
void real_valued_matrix_test(void) {

	auto mat1{ real::valued::matrix::constructor(3u, 3u, 'x')};
	real::valued::matrix::display("\n--------------------\n\nmat1", mat1);

	std::initializer_list<float> initlst0{ -4.1f, -3.2f, -2.3f, -1.4f };
	auto mat2{ real::valued::matrix::constructor(5u, 2u, initlst0) };
	real::valued::matrix::display("\n--------------------\n\nmat2", mat2);

	std::valarray<double> val0{ 1.1, 1.2, 1.3, 1.4, 1.5, 1.6 };
	auto mat3{ real::valued::matrix::constructor(1u, 4u, val0) };
	real::valued::matrix::display("\n--------------------\n\nmat3", mat3);

	std::vector<unsigned int> vec0{ 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u };
	auto mat4{ real::valued::matrix::constructor(2u, 4u, vec0) };
	real::valued::matrix::display("\n--------------------\n\nmat4", mat4);

	std::set<int, std::less<int> > set0{ 8, 3, 0, 4, 5, -2, 3 };
	auto mat5{ real::valued::matrix::constructor(7u, 1u, set0) };
	real::valued::matrix::display("\n--------------------\n\nmat5", mat5);

	std::multiset<int, std::greater<int> > set1{ 7, 5, 2, 2, 0, 3, 1, 5};
	auto mat6{ real::valued::matrix::constructor(4u, 2u, set1) };
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	std::list<char> lst0{ 'a', 'R', '.', ',', '`', '~' };
	auto mat7{ real::valued::matrix::constructor(2u, 2u, lst0) };
	real::valued::matrix::display("\n--------------------\n\nmat7", mat7);

	auto mat7row1ref{ real::valued::matrix::get::row::ref(mat7, 1u) };
	auto mat7row0copy{ real::valued::matrix::get::row::copy(mat7, 0u) };
	auto mat7col0ref{ real::valued::matrix::get::column::ref(mat7, 0u) };
	auto may7col1copy{ real::valued::matrix::get::column::copy(mat7, 1u) };
	auto mat7data10{ real::valued::matrix::get::data(mat7, 1u, 0u) };

	real::valued::matrix::set::row(mat6, 2u, -1);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	auto fn0 = [](float a, float b) { return a + b; };
	real::valued::matrix::set::row(mat6, 1u, fn0, -3.2f, 0.2f);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	real::valued::matrix::set::column(mat6, 0u, 3);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	auto fn1 = []() { return 0; };
	real::valued::matrix::set::column(mat6, 1u, fn1);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	real::valued::matrix::set::data(mat6, 1u, 0u, -5);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	auto fn2 = [](int a, int b) { return a - b; };
	real::valued::matrix::set::data(mat6, 2u, 1u, fn2, -2, -4);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	auto fn3 = [](int a, int b, int c) { std::cout << (a * b / c) << std::endl; };
	real::valued::matrix::set::row(mat6, 3u, fn3, -2, -4, 2);
	real::valued::matrix::display("\n--------------------\n\nmat6", mat6);

	std::random_device rd;
	std::default_random_engine rng{ rd() };
	std::uniform_real_distribution<float> rnd(0.0f, 1.0f);
	auto fn4 = [&rng, &rnd]() { return rnd(rng); };
	auto mat8{ real::valued::matrix::generator(4u, 5u, fn4) };
	real::valued::matrix::display("\n--------------------\n\nmat8", mat8);

	size_t c = 0u;
	auto fn5 = [&c](size_t u) { return c++ + u; };
	auto mat9{ real::valued::matrix::generator(3u, 2u, fn5, 0) };
	real::valued::matrix::display("\n--------------------\n\nmat9", mat9);

	auto b0{ real::valued::matrix::integrity_check(mat9) };
	std::get<real::valued::matrix::number_of_columns_>(mat9)++;
	auto b1{ real::valued::matrix::integrity_check(mat9) };
	std::get<real::valued::matrix::number_of_columns_>(mat9)--;

	auto fn10 = [](size_t i) { return i + 1u; };
	auto mat10{ real::valued::matrix::apply(mat9, fn10) };
	real::valued::matrix::display("\n--------------------\n\nmat10", mat10);

	//auto fn11 = [](size_t i) { std::cout << i << std::endl; };
	//auto mat11{ real::valued::matrix::apply(mat10, fn11) };// ------------------------- ERROR
	//real::valued::matrix::display("\n--------------------\n\nmat10", mat10);

	auto mat12{ real::valued::matrix::constructor(3u, 6u, 1) };
	real::valued::matrix::display("\n--------------------\n\nmat12", mat12);

	auto mat13{ real::valued::matrix::constructor(3u, 6u, 2) };
	real::valued::matrix::display("\n--------------------\n\nmat13", mat13);

	auto mat14{ real::valued::matrix::addition(mat12, mat13) };
	real::valued::matrix::display("\n--------------------\n\nmat14", mat14);

	auto mat15{ real::valued::matrix::subtraction(mat12, mat13) };
	real::valued::matrix::display("\n--------------------\n\nmat15", mat15);

	int cnt = 0;
	auto fn12 = [&cnt](int u) { return cnt++ + u; };
	auto mat16{ real::valued::matrix::generator(3u, 2u, fn12, 0) };
	real::valued::matrix::display("\n--------------------\n\nmat16", mat16);

	auto mat17{ real::valued::matrix::generator(2u, 4u, fn12, 0) };
	real::valued::matrix::display("\n--------------------\n\nmat17", mat17);

	auto mat18{ real::valued::matrix::multiplication(mat16, mat17) };
	real::valued::matrix::display("\n--------------------\n\nmat18", mat18);
}

// entry point
auto main() -> int {
	try {	
		real_valued_matrix_test();
   		return EXIT_SUCCESS;
	}
	catch (const std::exception & xxx) {
		std::cerr << xxx.what() << std::endl;
		return EXIT_FAILURE;
	}
}
