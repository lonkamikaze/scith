#ifndef _SCITH_PARSE_HPP_
#define _SCITH_PARSE_HPP_

#include "itraits.hpp"

#include <array>
#include <string_view>

namespace scith::parse {

using itraits::cdiv;
using itraits::log2;

constexpr std::size_t true_index() noexcept {
	return 0;
}

template <typename ... TailTs>
constexpr std::size_t true_index(bool const head, TailTs const ... tail) noexcept {
	return head ? 0 : (1 + true_index(tail ...));
}

template <typename T, typename ... Ts>
constexpr std::size_t match_index(T const ref, Ts const ... needles) {
	return true_index((ref == needles) ...);
}

template <typename ... Ts>
constexpr std::size_t prefix_index(std::string_view const ref, Ts const ... needles) {
	return true_index(ref.starts_with(needles) ...);
}

constexpr char tolower(char const ch) noexcept {
	return (ch >= 'A' && ch <= 'Z') ? ch - 'A' + 'a' : ch;
}

template <char ... Vs>
struct literal_view : std::string_view {
	static constexpr char const str[]{tolower(Vs) ..., '\0'};
	constexpr literal_view() noexcept : std::string_view{str} {}
};

template <char ... Vs>
constexpr auto radix() noexcept {
	struct result { std::string_view digits; unsigned radix; };
	constexpr literal_view<Vs ...> const view{};
	if constexpr(view.starts_with("0x")) {
		return result{view.substr(2), 16};
	} else if constexpr(view.starts_with("0b")) {
		return result{view.substr(2), 2};
	} else if constexpr(view == "0") {
		return result{view, 8};
	} else if constexpr(view.starts_with('0')) {
		return result{view.substr(1), 8};
	} else if constexpr(view[0] > '0' && view[0] <= '9') {
		return result{view, 10};
	}
	return result{"", 0};
}

template <char ... Vs>
constexpr auto const radix_v{radix<Vs ...>().radix};

constexpr int digit(char const ch) noexcept {
	return (ch >= 'a') * (ch - 'a' + 10) + (ch < 'a') * (ch - '0');
}

template <char ... Vs>
constexpr auto digits() noexcept {
	constexpr auto const digits_view{radix<Vs ...>().digits};
	std::array<int, digits_view.size()> result{};
	for (std::size_t i{0}; i < result.size(); ++i) {
		result[i] = digit(digits_view[i]);
	}
	return result;
}

template <char ... Vs>
constexpr auto const digits_intset{digits<Vs ...>()};

template <char ... Vs>
constexpr std::size_t const digits_v{0};

template <char ... Vs>
requires (radix_v<Vs ...> == (1 << log2(radix_v<Vs ...>)))
constexpr std::size_t const digits_v<Vs ...>{log2(radix_v<Vs ...>) * digits_intset<Vs ...>.size()};

template <char ... Vs>
requires (10 == radix_v<Vs ...>)
constexpr std::size_t const digits_v<Vs ...>{cdiv(digits_intset<Vs ...>.size() * 3322, 1000)};

template <char ... Vs>
constexpr bool valid() noexcept {
	for (auto const digit : digits_intset<Vs ...>) {
		if (digit < 0 || digit >= radix_v<Vs ...>) {
			return false;
		}
	}
	return digits_intset<Vs ...>.size() && (radix_v<Vs ...> > 0);
}

template <char ... Vs>
concept integer_literal = (valid<Vs ...>());

} /* namespace scith::parse */

#endif /* _SCITH_PARSE_HPP_ */
