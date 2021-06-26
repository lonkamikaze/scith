#ifndef _SCITH_TRAITS_HPP_
#define _SCITH_TRAITS_HPP_

#include <type_traits>

namespace scith::traits {

template <typename T, class = void>
struct value_type { using type = T; };

template <typename T>
struct value_type<T, std::void_t<typename T::value_type>> {
	using type = typename T::value_type;
};

template <typename T>
using value_t = typename value_type<T>::type;

template <typename T>
using uvalue_t = std::make_unsigned_t<value_t<T>>;

template <typename T>
using svalue_t = std::make_signed_t<value_t<T>>;

template <typename T, class = void>
struct values_type { using type = T; };

template <typename T>
struct values_type<T, std::void_t<typename T::values_type>> {
	using type = typename T::values_type;
};

template <typename T>
using values_t = typename values_type<T>::type;

} /* namespace scith::traits */

#endif /* _SCITH_TRAITS_HPP_ */
