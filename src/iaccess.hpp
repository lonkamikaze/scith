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
constexpr auto const is_integral_array_v{false};

template <integral T, std::size_t CountV>
constexpr auto const is_integral_array_v<T[CountV]>{true};

template <integral T, std::size_t CountV>
constexpr auto const is_integral_array_v<std::array<T, CountV>>{true};

template <integral T, std::size_t CountV>
constexpr auto const is_integral_array_v<std::array<T, CountV> const>{true};

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

template <unsigned_integral T>
constexpr T lshift(T const value, std::size_t const shift) noexcept {
	return shift < udigits_v<T> ? value << shift : 0;
}

template <unsigned_integral T>
constexpr T rshift(T const value, std::size_t const shift) noexcept {
	return shift < udigits_v<T> ? value >> shift : 0;
}

template <unsigned_integral T>
constexpr T lrshift(T const value, std::ptrdiff_t const shift) noexcept {
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

	template <UDataT(* FuncV)(UDataT, UDataT) noexcept>
	constexpr ints_digits_reference & assign(T const op) noexcept {
		auto const start{max(0, i)};
		auto const limit{min(i + access_digits, values_digits)};
		auto uop {rshift<UT>(op, max(0, -i))};
		auto mask{rshift<UT>(-1,     max(0, -i))};
		for (auto digit{start}; digit < limit;) {
			auto const slice_index {digit / value_digits};
			auto const slice_offset{digit % value_digits};
			auto const slice_digits{value_digits - slice_offset};
			auto uvalue{unsigned_cast(values[slice_index])};
			UDataT uassign{FuncV(uvalue >> slice_offset, uop)};
			uvalue &= ~(static_cast<UDataT>(mask) << slice_offset);
			values[slice_index] = uvalue | (uassign << slice_offset);
			uop    = rshift(uop,  slice_digits);
			mask   = rshift(mask, slice_digits);
			digit += slice_digits;
		}
		return *this;
	}

	public:
	constexpr ints_digits_reference(DataT * const values, std::ptrdiff_t const i) noexcept :
	    values{values}, i{i} {}

	constexpr operator T() const noexcept {
		auto const start{max(0, i)};
		auto const limit{min(i + access_digits, values_digits)};
		UT result{lshift<UT>(-(values[values_size - 1] < 0),
		                     max(0, values_digits - i))};
		for (auto digit{start}; digit < limit;) {
			auto const slice_index {digit / value_digits};
			auto const slice_offset{digit % value_digits};
			auto const slice_digits{value_digits - slice_offset};
			auto const uvalue{unsigned_cast(values[slice_index])};
			result |= lrshift(unsigned_cast(uvalue + UT{0}),
			                  digit - i - (slice_offset));
			digit += slice_digits;
		}
		return result;
	}

	constexpr auto & operator =(T const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return r;}};
		return assign<op>(value);
	}

	constexpr auto & operator &=(T const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l & r;}};
		return assign<op>(value);
	}

	constexpr auto & operator |=(T const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l | r;}};
		return assign<op>(value);
	}

	constexpr auto & operator ^=(T const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l ^ r;}};
		return assign<op>(value);
	}
};

template <integrals IntsT>
class ints_digits_reference<bool, IntsT> {
	private:
	using DataT = base_t<IntsT>;
	using UDataT = std::make_unsigned_t<DataT>;

	static constexpr std::ptrdiff_t const value_digits{udigits_v<DataT>};
	static constexpr std::ptrdiff_t const values_size{size_v<IntsT>};
	static constexpr std::ptrdiff_t const values_digits{values_size * value_digits};

	DataT * values;
	std::ptrdiff_t i;

	template <UDataT(* FuncV)(UDataT, UDataT) noexcept>
	constexpr auto& assign(bool const op) noexcept {
		if(i >= 0 && i < values_digits) {
			auto const index {i / value_digits};
			auto const offset{i % value_digits};
			auto & value{values[index]};
			value = FuncV(value, UDataT{op} << offset);
		}
		return *this;
	}

	public:
	constexpr ints_digits_reference(DataT * const values, std::ptrdiff_t const i) noexcept :
	    values{values}, i{i} {}

	constexpr operator bool() const noexcept {
		return i < 0 ? 0
		       : i >= values_digits ? values[values_size - 1] < 0
		         : (values[i / value_digits] >> (i % value_digits)) & 1;
	}

	constexpr auto & operator =(bool const value) noexcept {
		return value ? *this |= 1 : *this &= 0;
	}

	constexpr auto & operator &=(bool const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l & ~r;}};
		return assign<op>(!value);
	}

	constexpr auto & operator |=(bool const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l | r;}};
		return assign<op>(value);
	}

	constexpr auto & operator ^=(bool const value) noexcept {
		constexpr auto const op{[](UDataT l, UDataT r) noexcept -> UDataT {return l ^ r;}};
		return assign<op>(value);
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

template <integrals IntsT>
class ints_digits<bool, IntsT> {
	private:
	using DataT = base_t<IntsT>;
	using reference = ints_digits_reference<bool, IntsT>;

	DataT * values;

	public:
	static constexpr auto const digits{udigits_v<DataT> * size_v<IntsT>};

	static constexpr auto size() noexcept {
		return digits;
	}

	constexpr ints_digits(IntsT & values) noexcept :
	    values{data_p(values)} {}

	constexpr bool operator [](std::ptrdiff_t const i) const noexcept {
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
class ints_bisect_reference {
	private:
	using UT = std::make_unsigned_t<T>;

	ints_digits<T, IntsT> values;
	std::ptrdiff_t const i;

	public:
	static constexpr std::ptrdiff_t const digits{digits_v<UT> / 2};
	static constexpr auto const mask{(UT{1} << digits) - 1};

	constexpr ints_bisect_reference(ints_digits<T, IntsT> const values,
	                                std::ptrdiff_t const i) noexcept :
	    values{values}, i{i} {}

	constexpr operator T() const noexcept {
		return unsigned_cast(values[i * digits]) & mask;
	}

	constexpr auto & operator =(T const value) noexcept {
		(values[i * digits] &= ~mask) |= (unsigned_cast<T>(value) & mask);
		return *this;
	}

	constexpr auto & operator &=(T const value) noexcept {
		values[i * digits] &= (unsigned_cast<T>(value) | ~mask);
		return *this;
	}

	constexpr auto & operator |=(T const value) noexcept {
		values[i * digits] |= (unsigned_cast<T>(value) & mask);
		return *this;
	}

	constexpr auto & operator ^=(T const value) noexcept {
		values[i * digits] ^= (unsigned_cast<T>(value) & mask);
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
