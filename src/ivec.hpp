#ifndef _SCITH_IVEC_HPP_
#define _SCITH_IVEC_HPP_

#include "traits.hpp"
#include "itraits.hpp"
#include "iaccess.hpp"

#include <cstdint>
#include <type_traits>
#include <compare>
#include <string_view>
#include <limits>

namespace scith::ivec {

using itraits::integral;
using itraits::signed_integral;
using itraits::unsigned_integral;
using itraits::digits_v;
using itraits::udigits_v;
using itraits::min;
using itraits::max;
using itraits::log2;
using itraits::cdiv;
using itraits::make_signed_if;
using itraits::make_signed_if_t;
using itraits::make_unsigned_if;
using itraits::make_unsigned_if_t;
using itraits::min_digits_v;
using itraits::max_digits_v;
using itraits::sum_digits_v;
using itraits::has_signed_v;
using itraits::has_unsigned_v;
using itraits::unsigned_cast;

using traits::value_t;
using traits::svalue_t;
using traits::uvalue_t;

using iaccess::integrals;
using iaccess::digits_as;
using iaccess::access_as;
using iaccess::bisect_as;

template <integral BaseT, std::size_t DigitsV>
struct integer;

template<integral T>
integer(T const) -> integer<T, digits_v<T>>;

template <typename T>
struct is_integer_variant : std::false_type {};

template <integral BaseT, std::size_t DigitsV>
struct is_integer_variant<integer<BaseT, DigitsV>> : std::true_type {};

template <integral BaseT, std::size_t DigitsV>
struct is_integer_variant<integer<BaseT, DigitsV> const> : std::true_type {};

template <typename T>
constexpr auto const is_integer_variant_v{is_integer_variant<T>::value};

template <typename T>
concept integer_variant = is_integer_variant_v<T>;

template <typename T>
concept integer_compatible = integer_variant<T> || integral<T>;

namespace ctag {
	static constexpr struct raw_type {} const raw{};
	static constexpr struct narrowing_type {} const narrowing{};
} /* namespace ctag */

template <integral T, std::size_t DigitsV>
constexpr std::size_t const count_of_v{
	cdiv(DigitsV + std::is_signed_v<T>, udigits_v<T>)
};

template <integral BaseT, std::size_t DigitsV>
struct integer {
	using value_type  = BaseT;
	using values_type = std::array<BaseT, count_of_v<BaseT, DigitsV>>;
	static constexpr auto const digits{DigitsV};

	constexpr integer() noexcept : values{} {}

	template <integral ... Ts>
	constexpr integer(ctag::raw_type, Ts const ... values) noexcept :
	    values{static_cast<BaseT>(values) ...} {}

	template <integer_compatible T>
	constexpr integer(ctag::narrowing_type, T const & value) noexcept;

	template <integer_compatible T>
	requires (DigitsV >= digits_v<T> && signed_integral<BaseT> >= signed_integral<value_t<T>>)
	constexpr integer(T const & value) noexcept;

	values_type values;

	static constexpr std::size_t size() noexcept { return count_of_v<BaseT, DigitsV>; }

	constexpr BaseT const & operator [](std::size_t const i) const noexcept {
		return values[i];
	}

	constexpr BaseT & operator [](std::size_t const i) noexcept {
		return values[i];
	}

	constexpr BaseT * data() noexcept { return values.data(); }
	constexpr BaseT const * data() const noexcept { return values.data(); }

	explicit constexpr operator bool() const noexcept { return *this != 0; }
};

template <integral LT, integral RT>
struct select_greater_digits_type { using type = LT; };

template <integral LT, integral RT>
requires (digits_v<LT> < digits_v<RT>)
struct select_greater_digits_type<LT, RT> { using type = RT; };

template <integral LT, integral RT>
using select_greater_digits_t =
    typename select_greater_digits_type<LT, RT>::type;

template <integral ... Ts>
struct select_max_digits_type;

template <integral ... Ts>
using select_max_digits_t =
    typename select_max_digits_type<Ts ...>::type;

template <integral LT, integral ... RTs>
struct select_max_digits_type<LT, RTs ...> :
    select_greater_digits_type<LT, select_max_digits_t<RTs ...>> {};

template <integral T>
struct select_max_digits_type<T> { using type = T; };

template <integral ... Ts>
struct select_common_type : make_signed_if<has_signed_v<Ts ...>, select_max_digits_t<Ts ...>> {};

template <integral ... Ts>
using select_common_t = typename select_common_type<Ts ...>::type;

template <integer_compatible ... Ts>
using select_common_value_t = select_common_t<value_t<Ts> ...>;

template <integral T, integer_variant IntsT>
constexpr auto digits_as(IntsT & value) {
	return digits_as<T>(value.values);
}

template <integral T, integer_variant IntsT>
constexpr auto access_as(IntsT & value) {
	return access_as<T>(value.values);
}

template <integral T, integer_variant IntsT>
constexpr auto bisect_as(IntsT & value) {
	return bisect_as<T>(value.values);
}

template <integer_variant T> requires signed_integral<value_t<T>>
constexpr T & clonesign(T & dst, auto const & src) {
	return digits_as<value_t<T>>(dst)[T::digits] = -(src < 0), dst;
}

template <integer_variant T> requires unsigned_integral<value_t<T>>
constexpr T & clonesign(T & dst, auto const &) {
	return digits_as<value_t<T>>(dst)[T::digits] = 0, dst;
}

template <integer_variant T> requires unsigned_integral<value_t<T>>
constexpr T & fix_pad_if_unsigned(T & value) {
	return digits_as<value_t<T>>(value)[T::digits] = 0, value;
}

template <integer_variant T> requires signed_integral<value_t<T>>
constexpr T & fix_pad_if_unsigned(T & value) { return value; }

template <integral BaseT, std::size_t DigitsV>
template <integer_compatible T>
constexpr integer<BaseT, DigitsV>::
integer(ctag::narrowing_type, T const & value) noexcept : values{} {
	auto const values{access_as<BaseT>(value)};
	for (std::size_t i{0}; i < size(); ++i) {
		this->values[i] = values[i];
	}
	clonesign(*this, value);
}

template <integral BaseT, std::size_t DigitsV>
template <integer_compatible T>
requires (DigitsV >= digits_v<T> &&
          signed_integral<BaseT> >= signed_integral<value_t<T>>)
constexpr integer<BaseT, DigitsV>::
integer(T const & value) noexcept : values{} {
	auto const values{access_as<BaseT>(value)};
	for (std::size_t i{0}; i < size(); ++i) {
		this->values[i] = values[i];
	}
}

template <integer_compatible T>
constexpr bool isnan(T const & value) {
	auto const top{digits_as<value_t<T>>(value)[T::digits]};
	return (top != 0) && (top != -signed_integral<value_t<T>>);
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator <=>(LT const & lop, RT const & rop) noexcept {
	using value = select_common_value_t<LT, RT>;
	using uvalue = std::make_unsigned_t<value>;
	auto const lslices{access_as<uvalue>(lop)};
	auto const rslices{access_as<uvalue>(rop)};
	auto const top{max(lslices.size(), rslices.size())};
	/* check sign, flipping left and right, because a negative
	 * number has a greater sign than a positive number */
	auto result{rslices[top] <=> lslices[top]};
	for (auto i{top - 1}; result == 0 && i < top; --i) {
		result = (lslices[i] <=> rslices[i]);
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr bool operator ==(LT const & lop, RT const & rop) noexcept {
	return (lop <=> rop) == 0;
}

template <integer_variant T>
constexpr auto operator +(T const & op) noexcept {
	return integer<svalue_t<T>, digits_v<T>>{op};
}

template <integer_variant T>
constexpr auto operator -(T const & op) noexcept {
	using value  = value_t<T>;
	using uvalue = uvalue_t<T>;
	auto const vop{access_as<uvalue>(op)};

	integer<svalue_t<T>, digits_v<T> + std::is_signed_v<value>> result;
	uvalue carry{1};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = ~vop[i] + carry;
		carry &= !vop[i];
	}
	return result;
}

template <integer_variant T>
constexpr auto operator ~(T const & op) noexcept {
	T result{};
	for (std::size_t i{0}; i < op.size(); ++i) {
		result[i] = ~unsigned_cast(op[i]);
	}
	return fix_pad_if_unsigned(result);
}

template <integer_variant T>
constexpr T operator <<(T const & value, std::ptrdiff_t const lshift) noexcept {
	using value_type = value_t<T>;
	constexpr std::ptrdiff_t const value_digits{udigits_v<value_type>};
	auto const && values{digits_as<value_type>(value)};
	T result{};
	for (std::ptrdiff_t i{0}; i < result.size(); ++i) {
		result[i] = values[i * value_digits - lshift];
	}
	if (lshift > 0) {
		clonesign(result, value);
	}
	return result;
}

template <integer_variant T>
constexpr T operator >>(T const & value, std::ptrdiff_t const rshift) noexcept {
	return value << -rshift;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator &(LT const & lhs, RT const & rhs) noexcept {
	using value =
	    make_unsigned_if_t<(unsigned_integral<value_t<LT>> && digits_v<LT> <= digits_v<RT>) ||
	                       (unsigned_integral<value_t<RT>> && digits_v<RT> <= digits_v<LT>),
	                       select_common_value_t<LT, RT>>;
	using uvalue = std::make_unsigned_t<value>;
	constexpr auto const digits{
		signed_integral<value>
		? max_digits_v<LT, RT>
		: min_digits_v<LT, RT>
	};
	integer<value, digits> result;
	auto const && lvals{access_as<uvalue>(lhs)};
	auto const && rvals{access_as<uvalue>(rhs)};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = lvals[i] & rvals[i];
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator |(LT const & lhs, RT const & rhs) noexcept {
	using value = select_common_value_t<LT, RT>;
	using uvalue = std::make_unsigned_t<value>;
	integer<value, max_digits_v<LT, RT>> result;
	auto && lvals{access_as<uvalue>(lhs)};
	auto && rvals{access_as<uvalue>(rhs)};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = lvals[i] | rvals[i];
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator ^(LT const & lhs, RT const & rhs) noexcept {
	using value = select_common_value_t<LT, RT>;
	using uvalue = std::make_unsigned_t<value>;
	integer<value, max_digits_v<LT, RT>> result;
	auto && lvals{access_as<uvalue>(lhs)};
	auto && rvals{access_as<uvalue>(rhs)};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = lvals[i] ^ rvals[i];
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator +(LT const & lop, RT const & rop) noexcept {
	using value = select_common_value_t<LT, RT>;
	using uvalue = std::make_unsigned_t<value>;
	constexpr auto const digits{digits_v<uvalue>};
	constexpr auto const slice{digits / 2};

	integer<value, (max_digits_v<LT, RT> + 1)> result{};
	auto slices{bisect_as<uvalue>(result)};
	auto const lslices{bisect_as<uvalue>(lop)};
	auto const rslices{bisect_as<uvalue>(rop)};
	uvalue carry{0};
	for (std::size_t i{0}; i < slices.size(); ++i) {
		carry += lslices[i] + rslices[i];
		slices[i] = carry;
		carry >>= slice;
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator -(LT const & lop, RT const & rop) noexcept {
	using value = std::make_signed_t<select_common_value_t<LT, RT>>;
	using uvalue = std::make_unsigned_t<value>;
	constexpr auto const digits{digits_v<uvalue>};
	constexpr auto const slice{digits / 2};
	constexpr auto const mask{(uvalue{1} << slice) - 1};

	integer<value, (max_digits_v<LT, RT> + 1)> result{};
	auto slices{bisect_as<uvalue>(result)};
	auto const lslices{bisect_as<uvalue>(lop)};
	auto const rslices{bisect_as<uvalue>(rop)};
	/* negate the rhs on the fly and use addition, to avoid relying
	 * on implementation defined behaviour */
	uvalue carry{1};
	for (std::size_t i{0}; i < slices.size(); ++i) {
		carry += lslices[i] + (rslices[i] ^ mask);
		slices[i] = carry;
		carry >>= slice;
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator *(LT const & lop, RT const & rop) noexcept {
	using value = select_common_value_t<LT, RT>;
	using uvalue = std::make_unsigned_t<value>;
	constexpr auto const digits{digits_v<uvalue>};
	constexpr auto const slice{digits / 2};

	integer<value, sum_digits_v<LT, RT>> result{};
	auto slices{bisect_as<uvalue>(result)};
	auto const lslices{bisect_as<uvalue>(lop)};
	auto const rslices{bisect_as<uvalue>(rop)};
	for (std::size_t i{0}; i < slices.size(); ++i) {
		for (std::size_t p{0}; p <= i; ++p) {
			uvalue carry = lslices[p] * rslices[i - p];
			for (std::size_t q{i}; carry && q < slices.size(); ++q) {
				slices[q] = (carry += slices[q]);
				carry >>= slice;
			}
		}
	}
	return result;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator /(LT const & num, RT const & denom) noexcept {
	return div(num, denom).quot;
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator %(LT const & num, RT const & denom) noexcept {
	return div(num, denom).rem;
}

template <integral T>
constexpr auto abs(T const value) noexcept {
	using UT = std::make_unsigned_t<T>;
	return integer<UT, udigits_v<T>>{static_cast<UT>(value < 0 ? -value : value)};
}

template <integer_variant T>
requires signed_integral<value_t<T>>
constexpr auto abs(T const & value) noexcept {
	using uvalue = std::make_unsigned_t<value_t<T>>;
	integer<uvalue, digits_v<T> + 1> result{};
	auto slices{access_as<uvalue>(result)};
	auto const values{access_as<uvalue>(value)};
	if (value < 0) {
		uvalue carry{1};
		for (std::size_t i{0}; i < slices.size(); ++i) {
			slices[i] = ~values[i] + carry;
			carry &= !slices[i];
		}
	} else {
		for (std::size_t i{0}; i < slices.size(); ++i) {
			slices[i] = values[i];
		}
	}
	return result;
}

template <integer_variant T>
requires unsigned_integral<value_t<T>>
constexpr auto abs(T value) noexcept {
	return value;
}

template <integer_variant ResultT, class ...>
constexpr ResultT select(int const i) noexcept {
	return {};
}

template <integer_variant ResultT, class ... TagT,
          integer_compatible HeadT, integer_compatible ... TailTs>
constexpr ResultT
select(int const i, HeadT const & head, TailTs const & ... tail) noexcept {
	return i == sizeof...(tail)
	       ? ResultT{TagT{} ..., head}
	       : select<ResultT, TagT ...>(i, tail ...);
}

template <int IV>
constexpr int index_of_max(integer_compatible auto const &) noexcept {
	return IV;
}

template <int IV>
constexpr int index_of_max(integer_compatible auto const & lhs,
                           integer_compatible auto const & rhs,
                           integer_compatible auto const & ... tail) noexcept {
	return lhs >= rhs
	       ? index_of_max<IV>(lhs, tail ...)
	       : index_of_max<sizeof...(tail)>(rhs, tail ...);
}

constexpr int index_of_max(integer_compatible auto const & ... values) noexcept {
	return index_of_max<sizeof...(values) - 1>(values ...);
}

template <integer_compatible ... Ts>
constexpr auto max(Ts const & ... values) noexcept {
	/* if there is at least one unsigned value the result
	 * must be positive, so return an unsigned integer */
	using value = make_unsigned_if_t<has_unsigned_v<value_t<Ts> ...>,
	                                 select_common_value_t<Ts ...>>;
	constexpr auto const digits{max_digits_v<Ts ...>};
	return select<integer<value, digits>, ctag::narrowing_type>
	             (index_of_max(values ...), values ...);
}

template <int IV>
constexpr int index_of_min(integer_compatible auto const &) noexcept {
	return IV;
}

template <int IV>
constexpr int index_of_min(integer_compatible auto const & lhs,
                           integer_compatible auto const & rhs,
                           integer_compatible auto const & ... tail) noexcept {
	return lhs <= rhs
	       ? index_of_min<IV>(lhs, tail ...)
	       : index_of_min<sizeof...(tail)>(rhs, tail ...);
}

constexpr int index_of_min(integer_compatible auto const & ... values) noexcept {
	return index_of_min<sizeof...(values) - 1>(values ...);
}

template <integer_compatible ... Ts>
constexpr auto min(Ts const & ... values) noexcept {
	using value = select_common_value_t<Ts ...>;
	constexpr auto const digits{max_digits_v<Ts ...>};
	return select<integer<value, digits>>(index_of_min(values ...), values ...);
}

template <integer_variant T>
constexpr std::ptrdiff_t log2(T const & value) noexcept {
	if (value < 0) {
		return digits_v<T> + 1;
	}
	using itraits::log2;
	auto i{value.size() - 1};
	value_t<T> subv{0};
	for (; (subv = value[i]) == 0 && i > 0; --i);
	return i * udigits_v<value_t<T>> + log2(subv);
}

template <integer_compatible NumT, integer_compatible DenomT>
struct div_t {
	using value_type = select_common_value_t<NumT, DenomT>;
	integer<value_type, digits_v<NumT>>             quot;
	integer<value_type, min_digits_v<NumT, DenomT>> rem;
};

template <integer_compatible NumT, integer_compatible DenomT>
constexpr div_t<NumT, DenomT> div(NumT const & num, DenomT const & denom) noexcept {
	using divisor_type = decltype(max(abs(num), abs(denom)));
	using quot_type = decltype(abs(num));
	divisor_type const divisor{abs(denom)};
	quot_type quotient;
	auto quot{digits_as<bool>(quotient)};
	auto rem{abs(integer{num})};
	auto const l2div{log2(divisor)};
	for (auto pow{log2(rem) - l2div}; pow >= 0;
	     pow = min(log2(rem) - l2div, pow - 1)) {
		auto const divpow{divisor << pow};
		if (rem >= divpow) {
			quot[pow] |= 1;
			rem = {ctag::narrowing, (rem - divpow)};
		}
	}
	return {
		{ctag::narrowing, (num < 0) != (denom < 0) ? -quotient : quotient},
		{ctag::narrowing, num < 0 ? -rem : rem}
	};
}

} /* namespace scith::ivec */

namespace std {

template <scith::ivec::integer_variant T>
struct numeric_limits<T> : numeric_limits<scith::traits::value_t<T>> {
	static constexpr auto const digits{T::digits};
	static constexpr auto const digits10{digits * 1000 / 3322};
	static constexpr auto const max_digits10{scith::ivec::cdiv(digits * 1000, 3321)};
};

} /* namespace std */

#endif /* _SCITH_IVEC_HPP_ */
