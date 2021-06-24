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
constexpr T lshift(T const value, unsigned const shift) noexcept {
	return shift < udigits_v<T> ? value << shift : 0;
}

template <unsigned_integral T>
constexpr T rshift(T const value, unsigned const shift) noexcept {
	return shift < udigits_v<T> ? value >> shift : 0;
}

template <unsigned_integral T>
constexpr T lrshift(T const value, int const shift) noexcept {
	return shift >= 0 ? lshift<T>(value, shift) : rshift<T>(value, -shift);
}

template <integral T, integrals IntsT>
class ints_digits_reference {
	private:
	using UT = std::make_unsigned_t<T>;
	using DataT = base_t<IntsT>;
	using UDataT = std::make_unsigned_t<DataT>;

	static constexpr std::ptrdiff_t const access_digits{udigits_v<T>};
	static constexpr std::ptrdiff_t const value_digits{udigits_v<DataT>};
	static constexpr std::ptrdiff_t const values_size{size_v<IntsT>};
	static constexpr std::ptrdiff_t const values_digits{values_size * value_digits};

	DataT * values;
	std::ptrdiff_t i;

	public:
	constexpr ints_digits_reference(DataT * const values, std::ptrdiff_t const i) noexcept :
	    values{values}, i{i} {}

	constexpr operator T() const noexcept {
		auto const start{max(0, i)};
		auto const limit{min(i + access_digits, values_digits)};
		UT result{lshift<UT>(-(values[values_size - 1] < 0),
		                     max(0, values_digits - i))};
		for (auto digit{start}; digit < limit;
		     digit += value_digits - digit % value_digits) {
			auto const uvalue{unsigned_cast(values[digit / value_digits])};
			result |= lrshift(unsigned_cast(uvalue + UT{0}),
			                  digit - i - (digit % value_digits));
		}
		return result;
	}

	constexpr ints_digits_reference & operator =(T const assign) noexcept {
		auto const start{max(0, i)};
		auto const limit{min(i + access_digits, values_digits)};
		auto uassign{rshift<UT>(assign, max(0, -i))};
		auto mask   {rshift<UT>(-1,     max(0, -i))};
		for (auto digit{start}; digit < limit;
		     digit += value_digits - digit % value_digits) {
			auto const shift{digit % value_digits};
			auto uvalue{unsigned_cast(values[digit / value_digits])};
			uvalue &= ~(static_cast<UDataT>(mask) << shift);
			values[digit / value_digits] = uvalue | (static_cast<UDataT>(uassign) << shift);
			uassign = rshift(uassign, value_digits - shift);
			mask    = rshift(mask,    value_digits - shift);
		}
		return *this;
	}
};

template <integral T, integrals IntsT>
class ints_digits {
	private:
	using UT = std::make_unsigned_t<T>;
	using DataT = base_t<IntsT>;
	using reference = ints_digits_reference<T, IntsT>;

	DataT * values;

	public:
	static constexpr auto const digits{udigits_v<DataT> * size_v<IntsT>};

	static constexpr auto size() noexcept {
		return digits;
	}

	constexpr ints_digits(IntsT & values) noexcept :
	    values{data_p(values)} {}

	constexpr T operator [](std::ptrdiff_t const i) const noexcept {
		return reference{this->values, i};
	}

	constexpr auto operator [](std::ptrdiff_t const i) noexcept {
		return reference{this->values, i};
	}
};

template <integral T, integrals IntsT>
class ints_access {
	private:
	using DataT = base_t<IntsT>;

	ints_digits<T, IntsT> values;

	public:
	static constexpr auto const digits{udigits_v<DataT> * size_v<IntsT>};

	static constexpr auto size() noexcept {
		return cdiv(digits, udigits_v<T>);
	}

	constexpr ints_access(IntsT & values) noexcept :
	    values{values} {}

	constexpr T operator [](std::ptrdiff_t const i) const noexcept {
		return values[i * signed_cast(udigits_v<T>)];
	}

	constexpr auto operator [](std::ptrdiff_t const i) noexcept {
		return values[i * signed_cast(udigits_v<T>)];
	}
};

template <integral T, integrals IntsT>
struct ints_bisect_reference {
	using UT = std::make_unsigned_t<T>;

	static constexpr std::ptrdiff_t const digits{digits_v<UT> / 2};
	static constexpr auto const mask{(UT{1} << digits) - 1};

	ints_digits<T, IntsT> values;
	std::ptrdiff_t const i;

	constexpr operator T() const noexcept {
		return unsigned_cast(values[i * digits]) & mask;
	}

	constexpr ints_bisect_reference & operator =(T const assign) noexcept {
		values[i * digits] = (unsigned_cast<T>(values[i * digits]) & ~mask) |
		                     (unsigned_cast(assign) & mask);
		return *this;
	}
};


template <integral T, integrals IntsT>
struct ints_bisect {
	private:
	using UT = std::make_unsigned_t<T>;
	using reference = ints_bisect_reference<T, IntsT>;

	ints_digits<T, IntsT> values;

	public:
	static constexpr auto const digits{udigits_v<T> / 2};


	constexpr ints_bisect(IntsT & values) noexcept :
	    values{values} {}

	constexpr std::size_t size() const noexcept {
		return cdiv(values.digits, digits);
	}

	constexpr T operator [](std::ptrdiff_t const i) const noexcept {
		return reference{this->values, i};
	}

	constexpr auto operator [](std::ptrdiff_t const i) noexcept {
		return reference{this->values, i};
	}
};

template <integral T, integrals IntsT>
constexpr ints_digits<T, IntsT> digits_as(IntsT & values) noexcept {
	return {values};
}

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