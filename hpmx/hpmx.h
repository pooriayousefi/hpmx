
/***************************************************************************************/
/*                                                                                     */
/*                        high-performance matrix implementation                       */
/*                                                                                     */
/*                                    Pooria Yousefi                                   */
/*                                 pooriayousefi@aol.com                               */
/*                       https://www.linkedin.com/in/pooriayousefi/                    */
/*                                         2021                                        */
/*                                                                                     */
/***************************************************************************************/

#pragma once

#include <type_traits>
#include <algorithm>
#include <execution>
#include <numeric>

#include <initializer_list>
#include <valarray>
#include <vector>
#include <tuple>
#include <list>
#include <set>

/***************************************************************************************/
/* high-performance matrix space                                                       */
/***************************************************************************************/
namespace hpmx {

	/***********************************************************************************/
	/* real space                                                                      */
	/***********************************************************************************/
	namespace real {

		/*******************************************************************************/
		/* real-valued space                                                           */
		/*******************************************************************************/
		namespace valued {

			/***************************************************************************/
			/* real-valued matrix space                                                */
			/***************************************************************************/
			namespace matrix {

				// real-valued matrix data structure
				template<typename T>
				using _ = std::enable_if_t<std::is_arithmetic_v<T>, std::tuple<size_t, size_t, std::valarray<T> > >;

				// generic matrix protocols
				constexpr size_t number_of_rows_ = 0u;
				constexpr size_t number_of_columns_ = 1u;
				constexpr size_t data_ = 2u;

				// matrix constructor
				auto constructor = [](size_t number_of_rows, size_t number_of_columns, const auto & data) {
					using data_type = std::remove_cv_t<std::remove_reference_t<decltype(data)> >;
					if constexpr (std::is_arithmetic_v<data_type>) {
						return std::make_tuple(number_of_rows,
							number_of_columns,
							std::valarray<data_type>(data, number_of_rows * number_of_columns));
					}
					else {
						using data_value_type = typename data_type::value_type;
						auto retmat{ std::make_tuple(number_of_rows, number_of_columns,
						std::valarray<data_value_type>(data_value_type(0), number_of_rows * number_of_columns)) };
						auto minsize{ std::min(std::size(data), std::get<data_>(retmat).size()) };
						auto it = std::begin(data);
						for (auto i{ 0u }; i < minsize; ++i) {
							std::get<data_>(retmat)[i] = *it;
							it = std::next(it);
						}
						return retmat;
					}
				};

				// matrix display
				auto display = [](std::string_view title, const auto& data) {
					using data_type = std::remove_cv_t<std::remove_reference_t<decltype(data)> >;
					using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
					if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >) {
						std::cout.right;
						std::cout << title << " (" << std::get<number_of_rows_>(data) << " x " << std::get<number_of_columns_>(data) << ")\n\n";
						for (auto i{ 0u }; i < std::get<number_of_rows_>(data); ++i) {
							for (auto j{ 0u }; j < std::get<number_of_columns_>(data); ++j)
								std::cout << std::setw(16) << std::get<data_>(data)[i * std::get<number_of_columns_>(data) + j];
							std::cout << std::endl;
						}
					}
				};

				/***********************************************************************/
				/* real-valued matrix space real-valued matrix getters space           */
				/***********************************************************************/
				namespace get {

					/*******************************************************************/
					/* row getters space                                               */
					/*******************************************************************/
					namespace row {

						// row reference getter
						auto ref = [](auto & data, size_t i) {
							using data_type = std::remove_reference_t<decltype(data)>;
							using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
							if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
								return std::get<data_>(data)[std::slice(i * std::get<number_of_columns_>(data),
									std::get<number_of_columns_>(data),
									1u)];
						};

						// row copy getter
						auto copy = [](const auto & data, size_t i) {
							using data_type = std::remove_cv_t<std::remove_reference_t<decltype(data)> >;
							using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
							if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
								return std::get<data_>(data)[std::slice(i * std::get<number_of_columns_>(data),
									std::get<number_of_columns_>(data),
									1u)];
						};
					}

					/*******************************************************************/
					/* column getters space                                            */
					/*******************************************************************/
					namespace column {
						
						// column reference getter
						auto ref = [](auto & data, size_t j) {
							using data_type = std::remove_reference_t<decltype(data)>;
							using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
							if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
								return std::get<data_>(data)[std::slice(j, 
									std::get<number_of_rows_>(data),
									std::get<number_of_columns_>(data))];
						};

						// column copy getter
						auto copy = [](const auto & data, size_t j) {
							using data_type = std::remove_cv_t<std::remove_reference_t<decltype(data)> >;
							using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
							if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
								return std::get<data_>(data)[std::slice(j,
									std::get<number_of_rows_>(data),
									std::get<number_of_columns_>(data))];
						};
					}

					// data[i][j] value getter
					auto data = [](const auto & mat, size_t i, size_t j) {
						using data_type = std::remove_cv_t<std::remove_reference_t<decltype(mat)> >;
						using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
						if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
							return std::get<data_>(mat)[i * std::get<number_of_columns_>(mat) + j];
					};
				}

				/***********************************************************************/
				/* real-valued matrix space real-valued matrix setters space           */
				/***********************************************************************/
				namespace set {

					/*******************************************************************/
					/* row setters space                                               */
					/*******************************************************************/
					auto row = [](auto & data, size_t i, auto&& value, auto... args) {
						using data_type = std::remove_reference_t<decltype(data)>;
						using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
						if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >) {
							using value_type = std::remove_reference_t<decltype(value)>;
							if constexpr (std::is_arithmetic_v<value_type>)
								std::get<data_>(data)[std::slice(i * std::get<number_of_columns_>(data),
									std::get<number_of_columns_>(data),
									1u)] = value;
							else if constexpr (std::is_invocable_v<value_type, decltype(args)...> &&
								std::is_arithmetic_v<std::invoke_result_t<value_type, decltype(args)...> >) {
								std::get<data_>(data)[std::slice(i * std::get<number_of_columns_>(data),
									std::get<number_of_columns_>(data),
									1u)] = value(args...);
							}
						}
					};

					/*******************************************************************/
					/* column setters space                                            */
					/*******************************************************************/
					auto column = [](auto & data, size_t j, auto && value, auto... args) {
						using data_type = std::remove_reference_t<decltype(data)>;
						using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
						if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >) {
							using value_type = std::remove_reference_t<decltype(value)>;
							if constexpr (std::is_arithmetic_v<value_type>)
								std::get<data_>(data)[std::slice(j,
									std::get<number_of_rows_>(data),
									std::get<number_of_columns_>(data))] = value;
							else if constexpr (std::is_invocable_v<value_type, decltype(args)...> &&
								std::is_arithmetic_v<std::invoke_result_t<value_type, decltype(args)...> >) {
								std::get<data_>(data)[std::slice(j,
									std::get<number_of_rows_>(data),
									std::get<number_of_columns_>(data))] = value(args...);
							}
						}
					};

					// data[i][j] value setter
					auto data = [](auto & mat, size_t i, size_t j, auto&& value, auto... args) {
						using data_type = std::remove_reference_t<decltype(mat)>;
						using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
						if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >) {
							using value_type = std::remove_reference_t<decltype(value)>;
							if constexpr (std::is_arithmetic_v<value_type>)
								std::get<data_>(mat)[i * std::get<number_of_columns_>(mat) + j] = value;
							else if constexpr (std::is_invocable_v<value_type, decltype(args)...> &&
								std::is_arithmetic_v<std::invoke_result_t<value_type, decltype(args)...> >) {
								std::get<data_>(mat)[i * std::get<number_of_columns_>(mat) + j] = value(args...);
							}
						}
					};
				}

				// real-valued matrix generator
				auto generator = [](size_t number_of_rows, size_t number_of_columns, auto && fn, auto... args) {
					using data_type = std::remove_reference_t<decltype(fn)>;
					if constexpr (std::is_invocable_v<data_type, decltype(args)...>) {
						using return_type = std::invoke_result_t<data_type, decltype(args)...>;
						auto retmat{ std::make_tuple(number_of_rows,
							number_of_columns,
							std::valarray<return_type>(return_type(0), number_of_rows * number_of_columns)) };
						for (auto& elem : std::get<data_>(retmat))
							elem = fn(args...);
						return retmat;
					}
				};

				// real-valued matrix integrity check
				auto integrity_check = [](const auto& data) {
					using data_type = std::remove_cv_t<std::remove_reference_t<decltype(data)> >;
					using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
					if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >)
						if ((std::get<number_of_rows_>(data) * std::get<number_of_columns_>(data)) == std::get<data_>(data).size())
							return true;
						else return false;
					else return false;
				};

				// real-valued matrix apply function
				auto apply = [](auto & data, auto&& fn) {
					using data_type = std::remove_reference_t<decltype(data)>;
					using data_value_type = typename std::tuple_element_t<data_, data_type>::value_type;
					if constexpr (std::is_same_v<data_type, real::valued::matrix::_<data_value_type> >) {
						using fn_type = std::remove_reference_t<decltype(fn)>;
						if constexpr (std::is_invocable_r_v<data_value_type, fn_type, const data_value_type&>) {
							return std::make_tuple(std::get<number_of_rows_>(data),
								std::get<number_of_columns_>(data),
								std::get<data_>(data).apply(std::forward<fn_type>(fn)));
						}
					}
				};

				// real-valued matrix addition function
				auto addition = [](const auto & lhs, const auto & rhs) {

					using lhs_type = std::remove_cv_t<std::remove_reference_t<decltype(lhs)> >;
					using lhs_value_type = typename std::tuple_element_t<data_, lhs_type>::value_type;

					using rhs_type = std::remove_cv_t<std::remove_reference_t<decltype(rhs)> >;
					using rhs_value_type = typename std::tuple_element_t<data_, rhs_type>::value_type;

					if constexpr (std::is_same_v<lhs_type, real::valued::matrix::_<lhs_value_type> > &&
						std::is_same_v<rhs_type, real::valued::matrix::_<rhs_value_type> >) {

						if (real::valued::matrix::integrity_check(lhs) && real::valued::matrix::integrity_check(rhs)) {
							if ((std::get<number_of_rows_>(lhs) == std::get<number_of_rows_>(rhs)) &&
								(std::get<number_of_columns_>(lhs) == std::get<number_of_columns_>(rhs))) {

								return std::make_tuple(std::get<number_of_rows_>(lhs),
									std::get<number_of_columns_>(lhs),
									std::get<data_>(lhs) + std::get<data_>(rhs));
							}
							else
								throw std::exception("Exception from real::valued:matrix::addition: "
									"matrices dimensions are not compatible with each other.");
						}
						else
							throw std::exception("Exception from real::valued:matrix::addition: "
								"matrix integrity adjustment is breached.");
					}
				};

				// real-valued matrix subtraction function
				auto subtraction = [](const auto & lhs, const auto & rhs) {

					using lhs_type = std::remove_cv_t<std::remove_reference_t<decltype(lhs)> >;
					using lhs_value_type = typename std::tuple_element_t<data_, lhs_type>::value_type;

					using rhs_type = std::remove_cv_t<std::remove_reference_t<decltype(rhs)> >;
					using rhs_value_type = typename std::tuple_element_t<data_, rhs_type>::value_type;

					if constexpr (std::is_same_v<lhs_type, real::valued::matrix::_<lhs_value_type> > &&
						std::is_same_v<rhs_type, real::valued::matrix::_<rhs_value_type> >) {

						if (real::valued::matrix::integrity_check(lhs) && real::valued::matrix::integrity_check(rhs)) {
							if ((std::get<number_of_rows_>(lhs) == std::get<number_of_rows_>(rhs)) &&
								(std::get<number_of_columns_>(lhs) == std::get<number_of_columns_>(rhs))) {

								return std::make_tuple(std::get<number_of_rows_>(lhs),
									std::get<number_of_columns_>(lhs),
									std::get<data_>(lhs) - std::get<data_>(rhs));
							}
							else
								throw std::exception("Exception from real::valued:matrix::subtraction: "
									"matrices dimensions are not compatible with each other.");
						}
						else
							throw std::exception("Exception from real::valued:matrix::subtraction: "
								"matrix integrity adjustment is breached.");
					}
				};

				// real-valued matrix multiplication function
				auto multiplication = [](auto & lhs, auto & rhs) {

					using lhs_type = std::remove_reference_t<decltype(lhs)>;
					using lhs_value_type = typename std::tuple_element_t<data_, lhs_type>::value_type;

					using rhs_type = std::remove_reference_t<decltype(rhs)>;
					using rhs_value_type = typename std::tuple_element_t<data_, rhs_type>::value_type;

					if constexpr (std::is_same_v<lhs_type, real::valued::matrix::_<lhs_value_type> > &&
						std::is_same_v<rhs_type, real::valued::matrix::_<rhs_value_type> >) {

						if (real::valued::matrix::integrity_check(lhs) && real::valued::matrix::integrity_check(rhs)) {
							if (std::get<number_of_columns_>(lhs) == std::get<number_of_rows_>(rhs)) {

								auto nrows{ std::get<number_of_rows_>(lhs) };
								auto ncols{ std::get<number_of_columns_>(rhs) };
								using return_value_type = std::common_type_t<lhs_value_type, rhs_value_type>;
								auto retmat{ std::make_tuple(nrows,
									ncols,
									std::valarray<return_value_type>(return_value_type(0), nrows * ncols)) };

								for (auto i{ 0u }; i < nrows; ++i) {
									for (auto j{ 0u }; j < ncols; ++j) {

										std::valarray<return_value_type> lhsi{ std::get<data_>(lhs)[std::slice(i * std::get<number_of_columns_>(lhs), std::get<number_of_columns_>(lhs), 1u)] };
										std::valarray<return_value_type> rhsj{ std::get<data_>(rhs)[std::slice(j, std::get<number_of_rows_>(rhs), std::get<number_of_columns_>(rhs))] };
										
										std::get<data_>(retmat)[i * ncols + j] = std::transform_reduce(std::execution::seq,
											std::begin(lhsi),
											std::end(lhsi),
											std::begin(rhsj),
											return_value_type(0),
											std::plus<return_value_type>(),
											std::multiplies<return_value_type>());
									}
								}

								return retmat;
							}
							else
								throw std::exception("Exception from real::valued:matrix::multiplication: "
									"matrices dimensions are not compatible for matrix multiplication. "
									"It means that the number of columns in the left-hand side matrix is not equal to "
									"the number of rows in the right-hand side matrix.");
						}
						else
							throw std::exception("Exception from real::valued:matrix::multiplication: "
								"matrix integrity adjustment is breached.");
					}
				};
			}
		}
	}
}
