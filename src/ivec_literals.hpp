#ifndef _SCITH_IVEC_LITERALS_HPP_
#define _SCITH_IVEC_LITERALS_HPP_

#include "parse.hpp"
#include "ivec.hpp"

namespace scith::ivec::literals {

using parse::integer_literal;

template <integer_variant T>
constexpr auto parse(std::string_view const str) noexcept {
	auto const radix{parse::radix(str)};
	T result{};
	auto && slices{bisect_as<uvalue_t<T>>(result)};
	for (auto const digit : parse::digits_view{str}) {
		if (!parse::valid(digit, radix)) {
			/* stop parse on invalid digit */
			return result;
		}
		auto carry{static_cast<uvalue_t<T>>(digit)};
		for (auto && slice : slices) {
			carry += slice * radix;
			slice = carry;
			carry >>= slice.digits;
		}
	}
	return result;
}

template <integral BaseT, char ... Vs> requires integer_literal<Vs ...>
constexpr auto parse() noexcept {
	constexpr auto const digits{parse::digits_v<Vs ...>};
	return parse<integer<BaseT, digits>>(parse::literal_view<Vs ...>{});
}

template <char ... Vs> requires integer_literal<Vs ...>
constexpr auto operator ""_ivec() noexcept {
	return parse<uint_fast8_t, Vs ...>();
}

template <char ... Vs> requires integer_literal<Vs ...>
constexpr auto operator ""_ivec8() noexcept {
	return parse<uint8_t, Vs ...>();
}

template <char ... Vs> requires integer_literal<Vs ...>
constexpr auto operator ""_ivec16() noexcept {
	return parse<uint16_t, Vs ...>();
}

template <char ... Vs> requires integer_literal<Vs ...>
constexpr auto operator ""_ivec32() noexcept {
	return parse<uint32_t, Vs ...>();
}

template <char ... Vs> requires integer_literal<Vs ...>
constexpr auto operator ""_ivec64() noexcept {
	return parse<uint64_t, Vs ...>();
}

} /* namespace scith::ivec::literals */

#endif /* _SCITH_IVEC_LITERALS_HPP_ */
