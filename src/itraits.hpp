#ifndef _SCITH_ITRAITS_HPP_
#define _SCITH_ITRAITS_HPP_

#include <type_traits>
#include <limits>

namespace scith::itraits {

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
concept unsigned_integral = integral<T> && std::is_unsigned_v<T>;

template <typename T>
concept signed_integral = integral<T> && std::is_signed_v<T>;

template <integral T>
constexpr auto max(T const v) noexcept { return v; }

template <integral T, integral ... Ts>
constexpr auto max(T const v, Ts const ... vs) noexcept {
	auto const rv{max(vs ...)};
	return v > rv ? v : rv;
}

template <integral T>
constexpr auto min(T const v) noexcept { return v; }

template <integral T, integral ... Ts>
constexpr auto min(T const v, Ts const ... vs) noexcept {
	auto const rv{min(vs ...)};
	return v <= rv ? v : rv;
}

template <typename T>
constexpr std::size_t const digits_v{std::numeric_limits<T>::digits};

template <typename T>
constexpr auto const udigits_v{digits_v<std::make_unsigned_t<T>>};

template <typename T>
constexpr auto const sdigits_v{digits_v<std::make_signed_t<T>>};

template <typename ... Ts>
constexpr auto const sum_digits_v{(digits_v<Ts> + ...)};

template <typename ... Ts>
constexpr auto const sum_udigits_v{(udigits_v<Ts> + ...)};

template <typename ... Ts>
constexpr auto const sum_sdigits_v{(sdigits_v<Ts> + ...)};

template <typename ... Ts>
constexpr auto const max_digits_v{max(digits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const max_udigits_v{max(udigits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const max_sdigits_v{max(sdigits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const min_digits_v{min(digits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const min_udigits_v{min(udigits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const min_sdigits_v{min(sdigits_v<Ts> ...)};

template <typename ... Ts>
constexpr auto const all_signed_v{(std::is_signed_v<Ts> && ...)};

template <typename ... Ts>
constexpr auto const has_signed_v{(std::is_signed_v<Ts> || ...)};

template <typename ... Ts>
constexpr auto const all_unsigned_v{(std::is_unsigned_v<Ts> && ...)};

template <typename ... Ts>
constexpr auto const has_unsigned_v{(std::is_unsigned_v<Ts> || ...)};

template <bool CondV, integral T>
struct make_signed_if { using type = T; };

template <integral T>
struct make_signed_if<true, T> : std::make_signed<T> {};

template <bool CondV, integral T>
using make_signed_if_t = typename make_signed_if<CondV, T>::type;

template <bool CondV, integral T>
struct make_unsigned_if { using type = T; };

template <integral T>
struct make_unsigned_if<true, T> : std::make_unsigned<T> {};

template <bool CondV, integral T>
using make_unsigned_if_t = typename make_unsigned_if<CondV, T>::type;

template <integral T>
constexpr std::make_signed_t<T> signed_cast(T const value) noexcept {
	return value;
}

template <integral T>
constexpr std::make_unsigned_t<T> unsigned_cast(T const value) noexcept {
	return value;
}

constexpr int sign() noexcept { return 0; }

constexpr int sign(integral auto const ... values) noexcept {
	return (((values > 0) - (values < 0)) * ...);
}

constexpr auto abs(integral auto const value) noexcept {
	return sign(value) * value;
}

constexpr auto cdiv(integral auto const num,
                    integral auto const denom) noexcept {
	return sign(num, denom) *
	       (abs(num) + abs(denom) - 1) / abs(denom);
}

} /* namespace scith::itraits */

#endif /* _SCITH_ITRAITS_HPP_ */
