#ifndef _SCITH_IVEC_LITERALS_HPP_
#define _SCITH_IVEC_LITERALS_HPP_

#include "parse.hpp"
#include "ivec.hpp"

namespace scith::ivec::literals {

using parse::integer_literal;

template <integral BaseT, char ... Vs> requires integer_literal<Vs ...>
constexpr auto parse() noexcept {
	using uvalue = std::make_unsigned_t<BaseT>;
	constexpr auto const radix{parse::radix_v<Vs ...>};
	constexpr auto const digits{parse::digits_v<Vs ...>};
	integer<BaseT, digits> result{};
	auto && slices{bisect_as<uvalue>(result)};

	for (auto const digit : parse::digits_intset<Vs ...>) {
		auto carry{static_cast<uvalue>(digit)};
		for (auto && slice : slices) {
			carry += slice * radix;
			slice = carry;
			carry >>= slice.digits;
		}
	}
	return result;
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
