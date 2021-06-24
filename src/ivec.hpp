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

using namespace scith::itraits;
using namespace scith::traits;
using iaccess::integrals;
using iaccess::access_as;
using iaccess::bisect_as;

template <integral BaseT, std::size_t DigitsV>
struct integer;

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

struct ctag {
	static constexpr struct raw_type {} const raw{};
	static constexpr struct narrowing_type {} const narrowing{};
};

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
constexpr auto access_as(IntsT & value) {
	return access_as<T>(value.values);
}

template <integral T, integer_variant IntsT>
constexpr auto bisect_as(IntsT & value) {
	return bisect_as<T>(value.values);
}

template <integral BaseT, std::size_t DigitsV>
template <integer_compatible T>
constexpr integer<BaseT, DigitsV>::
integer(ctag::narrowing_type, T const & value) noexcept : values{} {
	auto const values{access_as<BaseT>(value)};
	for (std::size_t i{0}; i < size(); ++i) {
		this->values[i] = values[i];
	}
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

enum class radix : unsigned {
	base2  =  2,
	base8  =  8,
	base10 = 10,
	base16 = 16,
};

template <typename T>
constexpr auto const to_value(T const v) noexcept {
	return static_cast<std::underlying_type_t<T>>(v);
}

template <radix RadixV>
constexpr unsigned char parse_digit(char const ch) noexcept {
	return max(0, min(to_value(RadixV) - 1, ch - '0'));
};

template <>
constexpr unsigned char parse_digit<radix::base16>(char const ch) noexcept {
	return min(max(ch >= 'a' ? ch - 'a' + 10 :
	               ch >= 'A' ? ch - 'A' + 10 :
	               ch - '0', 0), 15);
}

template <unsigned V>
constexpr bool is_power_of_two_v{!(V % 2) && is_power_of_two_v<V / 2>};

template <>
constexpr bool is_power_of_two_v<1>{1};

template <>
constexpr bool is_power_of_two_v<0>{0};

template <unsigned_integral T>
constexpr unsigned bits(T const value) noexcept {
	return (value > 0) ? 1 + bits(value / 2) : 0;
}

template <radix RadixV>
struct integer_base;

template <radix RadixV> requires is_power_of_two_v<to_value(RadixV)>
struct integer_base<RadixV> {
	static constexpr auto const lshift{bits(to_value(RadixV) - 1)};

	template <integer_variant ResultT>
	static constexpr auto parse(std::string_view const str) noexcept {
		using uvalue = value_t<ResultT>;
		constexpr auto const rshift{digits_v<uvalue> - lshift};
		ResultT result;
		auto values{access_as<uvalue>(result)};

		for (auto const ch : str) {
			uvalue carry{parse_digit<RadixV>(ch)};
			for (auto && value : values) {
				auto const overflow{value >> rshift};
				value = (value << lshift) + carry;
				carry = overflow;
			}
		}
		return result;
	}

	static constexpr std::size_t digits(std::size_t const count) noexcept {
		return max(count * lshift, 1);
	}
};

template <radix RadixV> requires (!is_power_of_two_v<to_value(RadixV)>)
struct integer_base<RadixV> {
	template <integer_variant ResultT>
	static constexpr auto parse(std::string_view const str) noexcept {
		ResultT result;
		using uvalue = value_t<ResultT>;
		auto slices{bisect_as<uvalue>(result)};

		for (auto const ch : str) {
			uvalue carry{parse_digit<RadixV>(ch)};
			for (auto && slice : slices) {
				carry += slice * to_value(RadixV);
				slice = carry;
				carry >>= slice.digits;
			}
		}
		return result;
	}

	static constexpr std::size_t digits(std::size_t const count) noexcept {
		if constexpr (RadixV == radix::base10) {
			return max(cdiv(count * 3322, 1000), 1);
		}
	}
};

template <typename ... Ts>
constexpr auto has_prefix(std::string_view const str, Ts const & ... pfxs) noexcept {
	return (str.starts_with(pfxs) || ...);
}

template <unsigned_integral BaseT, char ... StrVs>
constexpr auto integer_literal_parse() noexcept {
	constexpr char const str[]{StrVs ..., 0};
	if constexpr (has_prefix(str, "0b", "0B")) {
		using base = integer_base<radix::base2>;
		return base::parse<integer<BaseT, base::digits(sizeof...(StrVs) - 2)>>(str + 2);
	} else if constexpr (has_prefix(str, "0x", "0X")) {
		using base = integer_base<radix::base16>;
		return base::parse<integer<BaseT, base::digits(sizeof...(StrVs) - 2)>>(str + 2);
	} else if constexpr (has_prefix(str, "0")) {
		using base = integer_base<radix::base8>;
		return base::parse<integer<BaseT, base::digits(sizeof...(StrVs) - 1)>>(str + 1);
	} else {
		using base = integer_base<radix::base10>;
		return base::parse<integer<BaseT, base::digits(sizeof...(StrVs))>>(str);
	}
}

namespace literals {

template <char ... StrVs>
constexpr auto operator ""_ivec() noexcept {
	return integer_literal_parse<uint_fast8_t, StrVs ...>();
}

template <char ... StrVs>
constexpr auto operator ""_ivec8() noexcept {
	return integer_literal_parse<uint8_t, StrVs ...>();
}

template <char ... StrVs>
constexpr auto operator ""_ivec16() noexcept {
	return integer_literal_parse<uint16_t, StrVs ...>();
}

template <char ... StrVs>
constexpr auto operator ""_ivec32() noexcept {
	return integer_literal_parse<uint32_t, StrVs ...>();
}

template <char ... StrVs>
constexpr auto operator ""_ivec64() noexcept {
	return integer_literal_parse<uint64_t, StrVs ...>();
}

} /* namespace literals */

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
	using value  = value_t<T>;
	using svalue = std::make_signed_t<value>;
	return integer<svalue, digits_v<T>>{op};
}

template <integer_variant T>
constexpr auto operator -(T const & op) noexcept {
	using value  = value_t<T>;
	using svalue = std::make_signed_t<value>;
	using uvalue = std::make_unsigned_t<value>;
	auto const vop{access_as<uvalue>(op)};

	integer<svalue, digits_v<T> + std::is_signed_v<value>> result;
	uvalue carry{1};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = ~vop[i] + carry;
		carry &= !vop[i];
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
	return integer<select_common_value_t<LT, RT>, digits_v<LT>>{};
}

template <integer_compatible LT, integer_compatible RT>
constexpr auto operator %(LT const & num, RT const & denom) noexcept {
	return integer<select_common_value_t<LT, RT>, min_digits_v<LT, RT>>{};
}

template <integer_compatible T>
constexpr T operator <<(T const & lop, int const rop) noexcept {
	return T{};
}

template <integer_compatible T>
constexpr T operator >>(T const & lop, int const rop) noexcept {
	return T{};
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

template <integer_variant ResultT>
constexpr ResultT select(int const i) noexcept {
	return {};
}

template <integer_variant ResultT,
          integer_compatible HeadT, integer_compatible ... TailTs>
constexpr ResultT
select(int const i, HeadT const & head, TailTs const & ... tail) noexcept {
	return i == sizeof...(tail)
	       ? ResultT{ctag::narrowing, head}
	       : select<ResultT>(i, tail ...);
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
	return select<integer<value, digits>>(index_of_max(values ...), values ...);
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

} /* namespace scith::ivec */

namespace std {

template <scith::ivec::integer_variant T>
struct numeric_limits<T> : numeric_limits<scith::traits::value_t<T>> {
	static constexpr auto const digits{T::digits};
};

template <scith::ivec::integer_variant T>
struct make_unsigned<T> {
	using type = scith::ivec::integer<std::make_unsigned_t<scith::traits::value_t<T>>, scith::itraits::digits_v<T>>;
};

template <scith::ivec::integer_variant T>
struct make_signed<T> {
	using type = scith::ivec::integer<std::make_signed_t<scith::traits::value_t<T>>, scith::itraits::digits_v<T>>;
};

} /* namespace std */

#endif /* _SCITH_IVEC_HPP_ */
