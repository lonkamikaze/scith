#ifndef _SCITH_PARSE_HPP_
#define _SCITH_PARSE_HPP_

#include "itraits.hpp"

#include <array>
#include <compare>
#include <string_view>

namespace scith::parse {

using itraits::cdiv;
using itraits::log2;

constexpr std::size_t true_index() noexcept {
	return 0;
}

template <typename ... TailTs>
constexpr std::size_t true_index(bool const head, TailTs const ... tail) noexcept {
	return !head * (1 + true_index(tail ...));
}

template <typename T, typename ... Ts>
constexpr std::size_t match_index(T const ref, Ts const ... needles) {
	return true_index((ref == needles) ...);
}

template <typename ... Ts>
constexpr std::size_t prefix_index(std::string_view const ref, Ts const ... needles) {
	return true_index(ref.starts_with(needles) ...);
}

template <char ... Vs>
struct literal_view : std::string_view {
	static constexpr std::array const str{Vs ...};
	constexpr literal_view() noexcept : std::string_view{str.data(), str.size()} {}
};

constexpr unsigned radix(std::string_view const str) noexcept {
	return (str.size() && str[0] >= '0' && str[0] <= '9') *
	       std::array{16, 16, 2, 2, 8, 10}[prefix_index(str, "0x", "0X", "0b", "0B", '0')];
}

template <char ... Vs>
constexpr auto radix() noexcept {
	return radix(literal_view<Vs ...>{});
}

constexpr std::string_view radix_tail(std::string_view const str) noexcept {
	return str.substr(std::array{2, 2, 1, 0}[match_index((str != "0") * radix(str), 16U, 2U, 8U)]);
}

template <char ... Vs>
constexpr auto const radix_v{radix<Vs ...>()};

constexpr int digit(char const ch) noexcept {
	return std::array{(ch - 'a' + 10), (ch - 'A' + 10), -1, (ch - '0')}
	                 [true_index(ch >= 'a', ch >= 'A', ch > '9')];
}

template <class ContainerT, typename ValueT>
concept container = requires(ContainerT & c, ValueT v) {
	std::size_t{c.size()};
	c[0] <=> v;
};

struct digits_view {
	std::string_view str;

	constexpr digits_view(std::string_view const str) noexcept :
	    str{radix_tail(str)} {}

	constexpr std::size_t size() const noexcept {
		return str.size();
	}

	constexpr int operator [](std::size_t const i) const noexcept {
		return digit(str[i]);
	}

	constexpr auto operator <=>(container<int> auto const & op) const noexcept {
		auto result{size() <=> op.size()};
		for (std::size_t i{0}; result == 0 && i < size(); ++i) {
			result = (*this)[i] <=> op[i];
		}
		return result;
	}

	constexpr bool operator ==(container<int> auto const & op) const noexcept {
		return (*this <=> op) == 0;
	}
};

struct digits_view_iterator {
	digits_view const digits;
	std::size_t i;

	constexpr digits_view_iterator & operator ++() noexcept {
		return ++i, *this;
	}

	constexpr auto operator *() const noexcept {
		return digits[i];
	}

	constexpr auto operator <=>(digits_view_iterator const & op) const noexcept {
		return i <=> op.i;
	}

	constexpr auto operator ==(digits_view_iterator const & op) const noexcept {
		return i == op.i;
	}
};

constexpr digits_view_iterator begin(digits_view const set) noexcept {
	return {set, 0};
}

constexpr digits_view_iterator end(digits_view const set) noexcept {
	return {set, set.size()};
}

template <char ... Vs>
constexpr digits_view digits() noexcept {
	return literal_view<Vs ...>{};
}

template <char ... Vs>
constexpr std::size_t const digits_v{0};

template <char ... Vs>
requires (radix_v<Vs ...> == (1 << log2(radix_v<Vs ...>)))
constexpr std::size_t const digits_v<Vs ...>{log2(radix_v<Vs ...>) * digits<Vs ...>().size()};

template <char ... Vs>
requires (10 == radix_v<Vs ...>)
constexpr std::size_t const digits_v<Vs ...>{cdiv(digits<Vs ...>().size() * 3322, 1000)};

constexpr bool valid(int const digit, unsigned const radix) noexcept {
	return digit >= 0 && static_cast<unsigned>(digit) < radix;
}

constexpr bool valid(std::string_view const str) noexcept {
	auto const radix{parse::radix(str)};
	digits_view const digits{str};
	for (auto const digit : digits) {
		if (!valid(digit, radix)) {
			return false;
		}
	}
	return digits.size();
}

template <char ... Vs>
constexpr bool valid() noexcept {
	return valid(literal_view<Vs ...>{});
}

template <char ... Vs>
concept integer_literal = (valid<Vs ...>());

} /* namespace scith::parse */

#endif /* _SCITH_PARSE_HPP_ */
