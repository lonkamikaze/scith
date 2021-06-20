#ifndef _SCITH_IACCESS_HPP_
#define _SCITH_IACCESS_HPP_

#include "itraits.hpp"

#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <array>

namespace scith::iaccess {

using namespace itraits;

template <typename T>
struct is_integral_array : std::false_type {};

template <integral T, std::size_t CountV>
struct is_integral_array<T[CountV]> : std::true_type {};

template <integral T, std::size_t CountV>
struct is_integral_array<std::array<T, CountV>> : std::true_type {};

template <integral T, std::size_t CountV>
struct is_integral_array<std::array<T, CountV> const> : std::true_type {};

template <typename T>
constexpr auto const is_integral_array_v{
	is_integral_array<T>::value
};

template <typename T>
concept integral_array = is_integral_array_v<T>;

template <typename T>
concept integrals = integral_array<T> || integral<T>;

template <integral T>
constexpr T * data_p(T & val) noexcept { return &val; }

template <integral T, std::size_t CountV>
constexpr T * data_p(T (& vals)[CountV]) noexcept { return vals; }

template <integral T, std::size_t CountV>
constexpr T * data_p(std::array<T, CountV> & vals) noexcept {
	return vals.data();
}

template <integral T, std::size_t CountV>
constexpr T const * data_p(std::array<T, CountV> const & vals) noexcept {
	return vals.data();
}

template <integrals>
constexpr std::size_t size_v{1};

template <integral T, std::size_t CountV>
constexpr std::size_t size_v<T[CountV]>{CountV};

template <integral T, std::size_t CountV>
constexpr std::size_t size_v<std::array<T, CountV>>{CountV};

template <integral T, std::size_t CountV>
constexpr std::size_t size_v<std::array<T, CountV> const>{CountV};

template <integrals T>
struct base_type;

template <integral T>
struct base_type<T> { using type = T; };

template <integral T, std::size_t CountV>
struct base_type<T[CountV]> : base_type<T> {};

template <integral T, std::size_t CountV>
struct base_type<std::array<T, CountV>> : base_type<T> {};

template <integral T, std::size_t CountV>
struct base_type<std::array<T, CountV> const> : base_type<T const> {};

template <integrals T>
using base_t = typename base_type<T>::type;

template <integral T>
constexpr T gcd(T const lhs, T const rhs) noexcept {
	return rhs ? gcd(rhs, lhs % rhs) : lhs;
}

template <unsigned_integral T>
constexpr T lshift(T const value, unsigned shift) noexcept {
	return shift < udigits_v<T> ? value << shift : 0;
}

template <unsigned_integral T>
constexpr T rshift(T const value, unsigned shift) noexcept {
	return shift < udigits_v<T> ? value >> shift : 0;
}

template <integral T, integrals IntsT>
class ints_access_reference {
	private:
	using UT = std::make_unsigned_t<T>;
	using DataT = base_t<IntsT>;
	using UDataT = std::make_unsigned_t<DataT>;

	static constexpr auto const access_digits{udigits_v<T>};
	static constexpr auto const value_digits{udigits_v<DataT>};
	static constexpr auto const values_size{size_v<IntsT>};
	static constexpr auto const values_digits{values_size * value_digits};
	static constexpr auto const slice_digits{gcd(access_digits, value_digits)};

	DataT * values;
	std::size_t i;

	public:
	constexpr ints_access_reference(DataT * const values, std::size_t const i) noexcept :
	    values{values}, i{i} {}

	constexpr operator T() const noexcept {
		constexpr auto const mask{static_cast<UT>(lshift<UT>(1, slice_digits) - 1)};
		auto const topfill{static_cast<UT>(-(values[values_size - 1] < 0))};
		auto const offset{i * access_digits};
		auto const fill{offset + access_digits};
		auto const limit{min(fill, values_digits)};
		UT result{topfill};
		for (auto digit{limit}; digit > offset;) {
			digit -= slice_digits;
			result = lshift(result, slice_digits);
			result |= ((values[digit / value_digits]) >>
			           (digit % value_digits)) & mask;
		}
		return result;
	}

	constexpr ints_access_reference & operator =(T const assign) noexcept {
		constexpr auto const mask{static_cast<UDataT>(lshift<UDataT>(1, slice_digits) - 1)};
		auto const offset{i * access_digits};
		auto const fill{offset + access_digits};
		auto const limit{min(fill, values_digits)};
		for (auto digit{offset}; digit < limit; digit += slice_digits) {
			auto & value{values[digit / value_digits]};
			auto result{unsigned_cast(value)};
			result &= ~(mask << (digit % value_digits));
			result |= (rshift(unsigned_cast(assign), digit - offset) & mask) <<
			          (digit % value_digits);
			value = result;
		}
		return *this;
	}
};

template <integral T, integrals IntsT>
class ints_access {
	private:
	using UT = std::make_unsigned_t<T>;
	using DataT = base_t<IntsT>;
	using reference = ints_access_reference<T, IntsT>;

	DataT * values;

	public:
	static constexpr auto const digits{udigits_v<DataT> * size_v<IntsT>};

	static constexpr auto size() noexcept {
		return cdiv(digits, udigits_v<T>);
	}

	constexpr ints_access(IntsT & values) noexcept :
	    values{data_p(values)} {}

	constexpr T operator [](std::size_t const i) const noexcept {
		return reference{this->values, i};
	}

	constexpr auto operator [](std::size_t const i) noexcept {
		return reference{this->values, i};
	}
};

template <integral T, integrals IntsT>
struct ints_bisect_reference {
	using UT = std::make_unsigned_t<T>;

	static constexpr auto const digits{digits_v<UT> / 2};
	static constexpr auto const mask{(UT{1} << digits) - 1};

	ints_access<T, IntsT> values;
	std::size_t const i;

	constexpr operator T() const noexcept {
		return (unsigned_cast(values[i / 2]) >> (digits * (i % 2))) & mask;
	}

	constexpr ints_bisect_reference & operator =(T const assign) noexcept {
		values[i / 2] =
			(unsigned_cast<T>(values[i / 2]) & ~(mask << (digits * (i % 2)))) |
			((unsigned_cast(assign) & mask) << (digits * (i % 2)));
		return *this;
	}
};


template <integral T, integrals IntsT>
struct ints_bisect {
	using UT = std::make_unsigned_t<T>;

	using reference = ints_bisect_reference<T, IntsT>;

	static constexpr auto const digits{udigits_v<T> / 2};

	ints_access<T, IntsT> values;

	constexpr std::size_t size() const noexcept {
		return cdiv(values.digits, digits);
	}

	constexpr T operator [](std::size_t const i) const noexcept {
		return reference{this->values, i};
	}

	constexpr auto operator [](std::size_t const i) noexcept {
		return reference{this->values, i};
	}
};

template <integral T, integrals IntsT>
constexpr ints_access<T, IntsT> access_as(IntsT & values) noexcept {
	return {values};
}

template <integral T, integrals IntsT>
constexpr ints_bisect<T, IntsT> bisect_as(IntsT & values) noexcept {
	return {values};
}

template <integral T, integrals IntsT>
struct ints_access_iterator {
	ints_access<T, IntsT> access;
	std::size_t i;

	constexpr ints_access_iterator & operator ++() noexcept {
		return ++i, *this;
	}

	constexpr ints_access_iterator operator ++(int) noexcept {
		auto const copy{*this};
		return ++i, copy;
	}

	constexpr auto operator *() const noexcept {
		return access[i];
	}

	constexpr auto operator *() noexcept {
		return access[i];
	}

	constexpr auto
	operator <=>(ints_access_iterator const & op) const noexcept {
		return i <=> op.i;
	}

	constexpr bool
	operator ==(ints_access_iterator const & op) const noexcept {
		return (i <=> op.i) == 0;
	}
};

template <integral T, integrals IntsT>
constexpr ints_access_iterator<T, IntsT>
begin(ints_access<T, IntsT> const & access) noexcept {
	return {access, 0};
}

template <integral T, integrals IntsT>
constexpr ints_access_iterator<T, IntsT const>
cbegin(ints_access<T, IntsT> const & access) noexcept {
	return {access, 0};
}

template <integral T, integrals IntsT>
constexpr ints_access_iterator<T, IntsT>
end(ints_access<T, IntsT> const & access) noexcept {
	return {access, access.size()};
}

template <integral T, integrals IntsT>
constexpr ints_access_iterator<T, IntsT const>
cend(ints_access<T, IntsT> const & access) noexcept {
	return {access, access.size()};
}

template <integral T, integrals IntsT>
struct ints_bisect_iterator {
	ints_bisect<T, IntsT> bisect;
	std::size_t i;

	constexpr ints_bisect_iterator & operator ++() noexcept {
		return ++i, *this;
	}

	constexpr ints_bisect_iterator operator ++(int) noexcept {
		auto const copy{*this};
		return ++i, copy;
	}

	constexpr auto operator *() const noexcept {
		return bisect[i];
	}

	constexpr auto operator *() noexcept {
		return bisect[i];
	}

	constexpr auto
	operator <=>(ints_bisect_iterator const & op) const noexcept {
		return i <=> op.i;
	}

	constexpr bool
	operator ==(ints_bisect_iterator const & op) const noexcept {
		return (i <=> op.i) == 0;
	}
};

template <integral T, integrals IntsT>
constexpr ints_bisect_iterator<T, IntsT>
begin(ints_bisect<T, IntsT> const & bisect) noexcept {
	return {bisect, 0};
}

template <integral T, integrals IntsT>
constexpr ints_bisect_iterator<T, IntsT const>
cbegin(ints_bisect<T, IntsT> const & bisect) noexcept {
	return {bisect, 0};
}

template <integral T, integrals IntsT>
constexpr ints_bisect_iterator<T, IntsT>
end(ints_bisect<T, IntsT> const & bisect) noexcept {
	return {bisect, bisect.size()};
}

template <integral T, integrals IntsT>
constexpr ints_bisect_iterator<T, IntsT const>
cend(ints_bisect<T, IntsT> const & bisect) noexcept {
	return {bisect, bisect.size()};
}

} /* namespace scith::iaccess */

#endif /* _SCITH_IACCESS_HPP_ */
