#include "iaccess.hpp"

using namespace scith::iaccess;

namespace {

static_assert(!integral_array<void>);
static_assert(!integral_array<int>);
static_assert(!integral_array<float>);
static_assert(!integral_array<int8_t>);
static_assert(!integral_array<int16_t>);
static_assert(!integral_array<int32_t>);
static_assert(!integral_array<int64_t>);
static_assert(!integral_array<unsigned>);
static_assert(!integral_array<uint8_t>);
static_assert(!integral_array<uint16_t>);
static_assert(!integral_array<uint32_t>);
static_assert(!integral_array<uint64_t>);
static_assert( integral_array<int[4]>);
static_assert(!integral_array<float[4]>);
static_assert( integral_array<int8_t[4]>);
static_assert( integral_array<int16_t[4]>);
static_assert( integral_array<int32_t[4]>);
static_assert( integral_array<int64_t[4]>);
static_assert( integral_array<unsigned[4]>);
static_assert( integral_array<uint8_t[4]>);
static_assert( integral_array<uint16_t[4]>);
static_assert( integral_array<uint32_t[4]>);
static_assert( integral_array<uint64_t[4]>);
static_assert(!integral_array<std::array<void, 4>>);
static_assert( integral_array<std::array<int, 4>>);
static_assert(!integral_array<std::array<float, 4>>);
static_assert( integral_array<std::array<int8_t, 4>>);
static_assert( integral_array<std::array<int16_t, 4>>);
static_assert( integral_array<std::array<int32_t, 4>>);
static_assert( integral_array<std::array<int64_t, 4>>);
static_assert( integral_array<std::array<unsigned, 4>>);
static_assert( integral_array<std::array<uint8_t, 4>>);
static_assert( integral_array<std::array<uint16_t, 4>>);
static_assert( integral_array<std::array<uint32_t, 4>>);
static_assert( integral_array<std::array<uint64_t, 4>>);

static_assert(!integrals<void>);
static_assert( integrals<int>);
static_assert(!integrals<float>);
static_assert( integrals<int8_t>);
static_assert( integrals<int16_t>);
static_assert( integrals<int32_t>);
static_assert( integrals<int64_t>);
static_assert( integrals<unsigned>);
static_assert( integrals<uint8_t>);
static_assert( integrals<uint16_t>);
static_assert( integrals<uint32_t>);
static_assert( integrals<uint64_t>);
static_assert( integrals<int[4]>);
static_assert(!integrals<float[4]>);
static_assert( integrals<int8_t[4]>);
static_assert( integrals<int16_t[4]>);
static_assert( integrals<int32_t[4]>);
static_assert( integrals<int64_t[4]>);
static_assert( integrals<unsigned[4]>);
static_assert( integrals<uint8_t[4]>);
static_assert( integrals<uint16_t[4]>);
static_assert( integrals<uint32_t[4]>);
static_assert( integrals<uint64_t[4]>);
static_assert( integrals<std::array<int, 4>>);
static_assert(!integrals<std::array<float, 4>>);
static_assert( integrals<std::array<int8_t, 4>>);
static_assert( integrals<std::array<int16_t, 4>>);
static_assert( integrals<std::array<int32_t, 4>>);
static_assert( integrals<std::array<int64_t, 4>>);
static_assert( integrals<std::array<unsigned, 4>>);
static_assert( integrals<std::array<uint8_t, 4>>);
static_assert( integrals<std::array<uint16_t, 4>>);
static_assert( integrals<std::array<uint32_t, 4>>);
static_assert( integrals<std::array<uint64_t, 4>>);

static_assert(    0xff0 == lshift<uint64_t>(0xff,    4));
static_assert(0xff00000 == lshift<uint64_t>(0xff,   20));
static_assert(        0 == lshift<uint64_t>(0xff,  100));
static_assert(        0 == lshift<uint64_t>(0xff, -  4));
static_assert(        0 == lshift<uint64_t>(0xff, - 20));
static_assert(        0 == lshift<uint64_t>(0xff, -100));

static_assert(      0xf == rshift<uint64_t>(0xff,    4));
static_assert(        0 == rshift<uint64_t>(0xff,   20));
static_assert(        0 == rshift<uint64_t>(0xff,  100));
static_assert(        0 == rshift<uint64_t>(0xff, -  4));
static_assert(        0 == rshift<uint64_t>(0xff, - 20));
static_assert(        0 == rshift<uint64_t>(0xff, -100));

static_assert(    0xff0 == lrshift<uint64_t>(0xff,    4));
static_assert(0xff00000 == lrshift<uint64_t>(0xff,   20));
static_assert(        0 == lrshift<uint64_t>(0xff,  100));
static_assert(      0xf == lrshift<uint64_t>(0xff, -  4));
static_assert(        0 == lrshift<uint64_t>(0xff, - 20));
static_assert(        0 == lrshift<uint64_t>(0xff, -100));

template <integral T, integral ... ExpectTs>
constexpr bool expect(auto const & values, ExpectTs const ... expects) {
	if (access_as<T>(values)[-1] || bisect_as<T>(values)[-1]) {
		return false;
	}
	auto access{begin(access_as<T>(values))};
	auto bisect{begin(bisect_as<T>(values))};
	auto const bslice{udigits_v<T> / 2};
	auto const bmask{(unsigned_cast(T{1}) << bslice) - 1};
	for (auto const value :
	     {static_cast<std::make_unsigned_t<T>>(expects) ...}) {
		if (unsigned_cast<T>(*(access++)) != value           ||
		    unsigned_cast<T>(*(bisect++)) != (value & bmask) ||
		    unsigned_cast<T>(*(bisect++)) != (value >> bslice)) {
			return false;
		}
	}
	return true;
}

constexpr uint64_t const uint64{0xfedcba9876543210};
constexpr uint32_t const uint32{0x76543210};
constexpr uint16_t const uint16{0x3210};
constexpr uint8_t  const uint8 {0x10};

constexpr  int64_t const  int64{0x0123456789abcdef};
constexpr  int32_t const  int32(0x89abcdef);
constexpr  int16_t const  int16(0xcdef);
constexpr  int8_t  const  int8 (0xef);

static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64)[- 60]);
static_assert(0x1000000000000000 == digits_as<uint64_t>(uint64)[- 56]);
static_assert(0xedcba98765432100 == digits_as<uint64_t>(uint64)[-  4]);
static_assert(0xfedcba9876543210 == digits_as<uint64_t>(uint64)[   0]);
static_assert(0x0fedcba987654321 == digits_as<uint64_t>(uint64)[   4]);
static_assert(0x00000000000000fe == digits_as<uint64_t>(uint64)[  56]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64)[ 100]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64)[- 64]);
static_assert(0xf000000000000000 == digits_as<uint64_t>( int64)[- 60]);
static_assert(0xef00000000000000 == digits_as<uint64_t>( int64)[- 56]);
static_assert(0x123456789abcdef0 == digits_as<uint64_t>( int64)[-  4]);
static_assert(0x0123456789abcdef == digits_as<uint64_t>( int64)[   0]);
static_assert(0x00123456789abcde == digits_as<uint64_t>( int64)[   4]);
static_assert(0x0000000000000001 == digits_as<uint64_t>( int64)[  56]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64)[- 60]);
static_assert(0x1000000000000000 == digits_as< int64_t>(uint64)[- 56]);
static_assert(0xedcba98765432100 == digits_as< int64_t>(uint64)[-  4]);
static_assert(0xfedcba9876543210 == digits_as< int64_t>(uint64)[   0]);
static_assert(0x0fedcba987654321 == digits_as< int64_t>(uint64)[   4]);
static_assert(0x00000000000000fe == digits_as< int64_t>(uint64)[  56]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64)[- 64]);
static_assert(0xf000000000000000 == digits_as< int64_t>( int64)[- 60]);
static_assert(0xef00000000000000 == digits_as< int64_t>( int64)[- 56]);
static_assert(0x123456789abcdef0 == digits_as< int64_t>( int64)[-  4]);
static_assert(0x0123456789abcdef == digits_as< int64_t>( int64)[   0]);
static_assert(0x00123456789abcde == digits_as< int64_t>( int64)[   4]);
static_assert(0x0000000000000001 == digits_as< int64_t>( int64)[  56]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64)[ 100]);

static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[-  4]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>(uint64)[   0]);
static_assert(static_cast<uint8_t>(0x21) == digits_as<uint8_t>(uint64)[   4]);
static_assert(static_cast<uint8_t>(0xfe) == digits_as<uint8_t>(uint64)[  56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64)[ 100]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64)[- 56]);
static_assert(static_cast<uint8_t>(0xf0) == digits_as<uint8_t>( int64)[-  4]);
static_assert(static_cast<uint8_t>(0xef) == digits_as<uint8_t>( int64)[   0]);
static_assert(static_cast<uint8_t>(0xde) == digits_as<uint8_t>( int64)[   4]);
static_assert(static_cast<uint8_t>(0x01) == digits_as<uint8_t>( int64)[  56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[-  4]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>(uint64)[   0]);
static_assert(static_cast< int8_t>(0x21) == digits_as< int8_t>(uint64)[   4]);
static_assert(static_cast< int8_t>(0xfe) == digits_as< int8_t>(uint64)[  56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64)[- 56]);
static_assert(static_cast< int8_t>(0xf0) == digits_as< int8_t>( int64)[-  4]);
static_assert(static_cast< int8_t>(0xef) == digits_as< int8_t>( int64)[   0]);
static_assert(static_cast< int8_t>(0xde) == digits_as< int8_t>( int64)[   4]);
static_assert(static_cast< int8_t>(0x01) == digits_as< int8_t>( int64)[  56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64)[ 100]);

static_assert(!digits_as<bool>(uint64)[-200]);
static_assert(!digits_as<bool>(uint64)[- 64]);
static_assert(!digits_as<bool>(uint64)[- 60]);
static_assert(!digits_as<bool>(uint64)[- 56]);
static_assert(!digits_as<bool>(uint64)[-  4]);
static_assert(!digits_as<bool>(uint64)[   0]);
static_assert( digits_as<bool>(uint64)[   4]);
static_assert(!digits_as<bool>(uint64)[  56]);
static_assert(!digits_as<bool>(uint64)[ 100]);
static_assert(!digits_as<bool>( int64)[-200]);
static_assert(!digits_as<bool>( int64)[- 64]);
static_assert(!digits_as<bool>( int64)[- 60]);
static_assert(!digits_as<bool>( int64)[- 56]);
static_assert(!digits_as<bool>( int64)[-  4]);
static_assert( digits_as<bool>( int64)[   0]);
static_assert(!digits_as<bool>( int64)[   4]);
static_assert( digits_as<bool>( int64)[  56]);
static_assert(!digits_as<bool>( int64)[ 100]);

static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32)[- 60]);
static_assert(0x1000000000000000 == digits_as<uint64_t>(uint32)[- 56]);
static_assert(0x0000000765432100 == digits_as<uint64_t>(uint32)[-  4]);
static_assert(0x0000000076543210 == digits_as<uint64_t>(uint32)[   0]);
static_assert(0x0000000007654321 == digits_as<uint64_t>(uint32)[   4]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32)[  56]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32)[ 100]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32)[- 64]);
static_assert(0xf000000000000000 == digits_as<uint64_t>( int32)[- 60]);
static_assert(0xef00000000000000 == digits_as<uint64_t>( int32)[- 56]);
static_assert(0xfffffff89abcdef0 == digits_as<uint64_t>( int32)[-  4]);
static_assert(0xffffffff89abcdef == digits_as<uint64_t>( int32)[   0]);
static_assert(0xfffffffff89abcde == digits_as<uint64_t>( int32)[   4]);
static_assert(0xffffffffffffffff == digits_as<uint64_t>( int32)[  56]);
static_assert(0xffffffffffffffff == digits_as<uint64_t>( int32)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32)[- 60]);
static_assert(0x1000000000000000 == digits_as< int64_t>(uint32)[- 56]);
static_assert(0x0000000765432100 == digits_as< int64_t>(uint32)[-  4]);
static_assert(0x0000000076543210 == digits_as< int64_t>(uint32)[   0]);
static_assert(0x0000000007654321 == digits_as< int64_t>(uint32)[   4]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32)[  56]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32)[- 64]);
static_assert(0xf000000000000000 == digits_as< int64_t>( int32)[- 60]);
static_assert(0xef00000000000000 == digits_as< int64_t>( int32)[- 56]);
static_assert(0xfffffff89abcdef0 == digits_as< int64_t>( int32)[-  4]);
static_assert(0xffffffff89abcdef == digits_as< int64_t>( int32)[   0]);
static_assert(0xfffffffff89abcde == digits_as< int64_t>( int32)[   4]);
static_assert(0xffffffffffffffff == digits_as< int64_t>( int32)[  56]);
static_assert(0xffffffffffffffff == digits_as< int64_t>( int32)[ 100]);

static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[-  4]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>(uint32)[   0]);
static_assert(static_cast<uint8_t>(0x21) == digits_as<uint8_t>(uint32)[   4]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[  56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32)[ 100]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32)[- 56]);
static_assert(static_cast<uint8_t>(0xf0) == digits_as<uint8_t>( int32)[-  4]);
static_assert(static_cast<uint8_t>(0xef) == digits_as<uint8_t>( int32)[   0]);
static_assert(static_cast<uint8_t>(0xde) == digits_as<uint8_t>( int32)[   4]);
static_assert(static_cast<uint8_t>(0xff) == digits_as<uint8_t>( int32)[  56]);
static_assert(static_cast<uint8_t>(0xff) == digits_as<uint8_t>( int32)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[-  4]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>(uint32)[   0]);
static_assert(static_cast< int8_t>(0x21) == digits_as< int8_t>(uint32)[   4]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[  56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32)[- 56]);
static_assert(static_cast< int8_t>(0xf0) == digits_as< int8_t>( int32)[-  4]);
static_assert(static_cast< int8_t>(0xef) == digits_as< int8_t>( int32)[   0]);
static_assert(static_cast< int8_t>(0xde) == digits_as< int8_t>( int32)[   4]);
static_assert(static_cast< int8_t>(0xff) == digits_as< int8_t>( int32)[  56]);
static_assert(static_cast< int8_t>(0xff) == digits_as< int8_t>( int32)[ 100]);

static_assert(!digits_as<bool>(uint32)[-200]);
static_assert(!digits_as<bool>(uint32)[- 64]);
static_assert(!digits_as<bool>(uint32)[- 60]);
static_assert(!digits_as<bool>(uint32)[- 56]);
static_assert(!digits_as<bool>(uint32)[-  4]);
static_assert(!digits_as<bool>(uint32)[   0]);
static_assert( digits_as<bool>(uint32)[   4]);
static_assert(!digits_as<bool>(uint32)[  56]);
static_assert(!digits_as<bool>(uint32)[ 100]);
static_assert(!digits_as<bool>( int32)[-200]);
static_assert(!digits_as<bool>( int32)[- 64]);
static_assert(!digits_as<bool>( int32)[- 60]);
static_assert(!digits_as<bool>( int32)[- 56]);
static_assert(!digits_as<bool>( int32)[-  4]);
static_assert( digits_as<bool>( int32)[   0]);
static_assert(!digits_as<bool>( int32)[   4]);
static_assert( digits_as<bool>( int32)[  56]);
static_assert( digits_as<bool>( int32)[ 100]);

template <integral T>
constexpr auto dassign(auto value, std::ptrdiff_t const i, auto const assign) {
	digits_as<T>(value)[i] = assign;
	return value;
}

static_assert(0xfedcba9876543210 == dassign<uint64_t>(uint64, -200, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210 == dassign<uint64_t>(uint64, - 64, 0xcffffffffffffff5));
static_assert(0xfedcba987654321c == dassign<uint64_t>(uint64, - 60, 0xcffffffffffffff5));
static_assert(0xfedcba98765432cf == dassign<uint64_t>(uint64, - 56, 0xcffffffffffffff5));
static_assert(0xfcffffffffffffff == dassign<uint64_t>(uint64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5 == dassign<uint64_t>(uint64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff50 == dassign<uint64_t>(uint64,    4, 0xcffffffffffffff5));
static_assert(0xf5dcba9876543210 == dassign<uint64_t>(uint64,   56, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210 == dassign<uint64_t>(uint64,  100, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign<uint64_t>( int64, -200, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign<uint64_t>( int64, - 64, 0xcffffffffffffff5));
static_assert(0x0123456789abcdec == dassign<uint64_t>( int64, - 60, 0xcffffffffffffff5));
static_assert(0x0123456789abcdcf == dassign<uint64_t>( int64, - 56, 0xcffffffffffffff5));
static_assert(0x0cffffffffffffff == dassign<uint64_t>( int64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5 == dassign<uint64_t>( int64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff5f == dassign<uint64_t>( int64,    4, 0xcffffffffffffff5));
static_assert(0xf523456789abcdef == dassign<uint64_t>( int64,   56, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign<uint64_t>( int64,  100, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210 == dassign< int64_t>(uint64, -200, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210 == dassign< int64_t>(uint64, - 64, 0xcffffffffffffff5));
static_assert(0xfedcba987654321c == dassign< int64_t>(uint64, - 60, 0xcffffffffffffff5));
static_assert(0xfedcba98765432cf == dassign< int64_t>(uint64, - 56, 0xcffffffffffffff5));
static_assert(0xfcffffffffffffff == dassign< int64_t>(uint64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5 == dassign< int64_t>(uint64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff50 == dassign< int64_t>(uint64,    4, 0xcffffffffffffff5));
static_assert(0xf5dcba9876543210 == dassign< int64_t>(uint64,   56, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210 == dassign< int64_t>(uint64,  100, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign< int64_t>( int64, -200, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign< int64_t>( int64, - 64, 0xcffffffffffffff5));
static_assert(0x0123456789abcdec == dassign< int64_t>( int64, - 60, 0xcffffffffffffff5));
static_assert(0x0123456789abcdcf == dassign< int64_t>( int64, - 56, 0xcffffffffffffff5));
static_assert(0x0cffffffffffffff == dassign< int64_t>( int64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5 == dassign< int64_t>( int64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff5f == dassign< int64_t>( int64,    4, 0xcffffffffffffff5));
static_assert(0xf523456789abcdef == dassign< int64_t>( int64,   56, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef == dassign< int64_t>( int64,  100, 0xcffffffffffffff5));

static_assert(0xfedcba9876543210 == dassign<uint8_t>(uint64, -200, 0xc5));
static_assert(0xfedcba9876543210 == dassign<uint8_t>(uint64, - 64, 0xc5));
static_assert(0xfedcba9876543210 == dassign<uint8_t>(uint64, - 60, 0xc5));
static_assert(0xfedcba9876543210 == dassign<uint8_t>(uint64, - 56, 0xc5));
static_assert(0xfedcba987654321c == dassign<uint8_t>(uint64, -  4, 0xc5));
static_assert(0xfedcba98765432c5 == dassign<uint8_t>(uint64,    0, 0xc5));
static_assert(0xfedcba9876543c50 == dassign<uint8_t>(uint64,    4, 0xc5));
static_assert(0xc5dcba9876543210 == dassign<uint8_t>(uint64,   56, 0xc5));
static_assert(0xfedcba9876543210 == dassign<uint8_t>(uint64,  100, 0xc5));
static_assert(0x0123456789abcdef == dassign<uint8_t>( int64, -200, 0xc5));
static_assert(0x0123456789abcdef == dassign<uint8_t>( int64, - 64, 0xc5));
static_assert(0x0123456789abcdef == dassign<uint8_t>( int64, - 60, 0xc5));
static_assert(0x0123456789abcdef == dassign<uint8_t>( int64, - 56, 0xc5));
static_assert(0x0123456789abcdec == dassign<uint8_t>( int64, -  4, 0xc5));
static_assert(0x0123456789abcdc5 == dassign<uint8_t>( int64,    0, 0xc5));
static_assert(0x0123456789abcc5f == dassign<uint8_t>( int64,    4, 0xc5));
static_assert(0xc523456789abcdef == dassign<uint8_t>( int64,   56, 0xc5));
static_assert(0x0123456789abcdef == dassign<uint8_t>( int64,  100, 0xc5));
static_assert(0xfedcba9876543210 == dassign< int8_t>(uint64, -200, 0xc5));
static_assert(0xfedcba9876543210 == dassign< int8_t>(uint64, - 64, 0xc5));
static_assert(0xfedcba9876543210 == dassign< int8_t>(uint64, - 60, 0xc5));
static_assert(0xfedcba9876543210 == dassign< int8_t>(uint64, - 56, 0xc5));
static_assert(0xfedcba987654321c == dassign< int8_t>(uint64, -  4, 0xc5));
static_assert(0xfedcba98765432c5 == dassign< int8_t>(uint64,    0, 0xc5));
static_assert(0xfedcba9876543c50 == dassign< int8_t>(uint64,    4, 0xc5));
static_assert(0xc5dcba9876543210 == dassign< int8_t>(uint64,   56, 0xc5));
static_assert(0xfedcba9876543210 == dassign< int8_t>(uint64,  100, 0xc5));
static_assert(0x0123456789abcdef == dassign< int8_t>( int64, -200, 0xc5));
static_assert(0x0123456789abcdef == dassign< int8_t>( int64, - 64, 0xc5));
static_assert(0x0123456789abcdef == dassign< int8_t>( int64, - 60, 0xc5));
static_assert(0x0123456789abcdef == dassign< int8_t>( int64, - 56, 0xc5));
static_assert(0x0123456789abcdec == dassign< int8_t>( int64, -  4, 0xc5));
static_assert(0x0123456789abcdc5 == dassign< int8_t>( int64,    0, 0xc5));
static_assert(0x0123456789abcc5f == dassign< int8_t>( int64,    4, 0xc5));
static_assert(0xc523456789abcdef == dassign< int8_t>( int64,   56, 0xc5));
static_assert(0x0123456789abcdef == dassign< int8_t>( int64,  100, 0xc5));

static_assert(0xfedcba9876543210 == dassign<bool>(uint64, -200, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64, - 64, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64, - 60, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64, - 56, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64, -  4, 1));
static_assert(0xfedcba9876543211 == dassign<bool>(uint64,    0, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64,    4, 1));
static_assert(0xffdcba9876543210 == dassign<bool>(uint64,   56, 1));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64,    0, 0));
static_assert(0xfedcba9876543200 == dassign<bool>(uint64,    4, 0));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64,   56, 0));
static_assert(0xfedcba9876543210 == dassign<bool>(uint64,  100, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64, -200, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64, - 64, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64, - 60, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64, - 56, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64, -  4, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64,    0, 1));
static_assert(0x0123456789abcdff == dassign<bool>( int64,    4, 1));
static_assert(0x0123456789abcdef == dassign<bool>( int64,   56, 1));
static_assert(0x0123456789abcdee == dassign<bool>( int64,    0, 0));
static_assert(0x0123456789abcdef == dassign<bool>( int64,    4, 0));
static_assert(0x0023456789abcdef == dassign<bool>( int64,   56, 0));
static_assert(0x0123456789abcdef == dassign<bool>( int64,  100, 1));

static_assert(0x76543210 == dassign<uint64_t>(uint32, -200, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign<uint64_t>(uint32, - 64, 0xcffffffd6ffffff5));
static_assert(0x7654321c == dassign<uint64_t>(uint32, - 60, 0xcffffffd6ffffff5));
static_assert(0x765432cf == dassign<uint64_t>(uint32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff == dassign<uint64_t>(uint32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5 == dassign<uint64_t>(uint32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff50 == dassign<uint64_t>(uint32,    4, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign<uint64_t>(uint32,   56, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign<uint64_t>(uint32,  100, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign<uint64_t>( int32, -200, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign<uint64_t>( int32, - 64, 0xcffffffd6ffffff5));
static_assert(0x89abcdec == dassign<uint64_t>( int32, - 60, 0xcffffffd6ffffff5));
static_assert(0x89abcdcf == dassign<uint64_t>( int32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff == dassign<uint64_t>( int32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5 == dassign<uint64_t>( int32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff5f == dassign<uint64_t>( int32,    4, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign<uint64_t>( int32,   56, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign<uint64_t>( int32,  100, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign< int64_t>(uint32, -200, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign< int64_t>(uint32, - 64, 0xcffffffd6ffffff5));
static_assert(0x7654321c == dassign< int64_t>(uint32, - 60, 0xcffffffd6ffffff5));
static_assert(0x765432cf == dassign< int64_t>(uint32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff == dassign< int64_t>(uint32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5 == dassign< int64_t>(uint32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff50 == dassign< int64_t>(uint32,    4, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign< int64_t>(uint32,   56, 0xcffffffd6ffffff5));
static_assert(0x76543210 == dassign< int64_t>(uint32,  100, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign< int64_t>( int32, -200, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign< int64_t>( int32, - 64, 0xcffffffd6ffffff5));
static_assert(0x89abcdec == dassign< int64_t>( int32, - 60, 0xcffffffd6ffffff5));
static_assert(0x89abcdcf == dassign< int64_t>( int32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff == dassign< int64_t>( int32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5 == dassign< int64_t>( int32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff5f == dassign< int64_t>( int32,    4, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign< int64_t>( int32,   56, 0xcffffffd6ffffff5));
static_assert(0x89abcdef == dassign< int64_t>( int32,  100, 0xcffffffd6ffffff5));

static_assert(0x76543210 == dassign<uint8_t>(uint32, -200, 0xc5));
static_assert(0x76543210 == dassign<uint8_t>(uint32, - 64, 0xc5));
static_assert(0x76543210 == dassign<uint8_t>(uint32, - 60, 0xc5));
static_assert(0x76543210 == dassign<uint8_t>(uint32, - 56, 0xc5));
static_assert(0x7654321c == dassign<uint8_t>(uint32, -  4, 0xc5));
static_assert(0x765432c5 == dassign<uint8_t>(uint32,    0, 0xc5));
static_assert(0x76543c50 == dassign<uint8_t>(uint32,    4, 0xc5));
static_assert(0x76543210 == dassign<uint8_t>(uint32,   56, 0xc5));
static_assert(0x76543210 == dassign<uint8_t>(uint32,  100, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32, -200, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32, - 64, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32, - 60, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32, - 56, 0xc5));
static_assert(0x89abcdec == dassign<uint8_t>( int32, -  4, 0xc5));
static_assert(0x89abcdc5 == dassign<uint8_t>( int32,    0, 0xc5));
static_assert(0x89abcc5f == dassign<uint8_t>( int32,    4, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32,   56, 0xc5));
static_assert(0x89abcdef == dassign<uint8_t>( int32,  100, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32, -200, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32, - 64, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32, - 60, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32, - 56, 0xc5));
static_assert(0x7654321c == dassign< int8_t>(uint32, -  4, 0xc5));
static_assert(0x765432c5 == dassign< int8_t>(uint32,    0, 0xc5));
static_assert(0x76543c50 == dassign< int8_t>(uint32,    4, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32,   56, 0xc5));
static_assert(0x76543210 == dassign< int8_t>(uint32,  100, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32, -200, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32, - 64, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32, - 60, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32, - 56, 0xc5));
static_assert(0x89abcdec == dassign< int8_t>( int32, -  4, 0xc5));
static_assert(0x89abcdc5 == dassign< int8_t>( int32,    0, 0xc5));
static_assert(0x89abcc5f == dassign< int8_t>( int32,    4, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32,   56, 0xc5));
static_assert(0x89abcdef == dassign< int8_t>( int32,  100, 0xc5));

static_assert(0x76543210 == dassign<bool>(uint32, - 64, 1));
static_assert(0x76543210 == dassign<bool>(uint32, - 60, 1));
static_assert(0x76543210 == dassign<bool>(uint32, - 56, 1));
static_assert(0x76543210 == dassign<bool>(uint32, -  4, 1));
static_assert(0x76543211 == dassign<bool>(uint32,    0, 1));
static_assert(0x76543210 == dassign<bool>(uint32,    4, 1));
static_assert(0x76543210 == dassign<bool>(uint32,    0, 0));
static_assert(0x76543200 == dassign<bool>(uint32,    4, 0));
static_assert(0x76543210 == dassign<bool>(uint32,   56, 1));
static_assert(0x76543210 == dassign<bool>(uint32,  100, 1));
static_assert(0x89abcdef == dassign<bool>( int32, -200, 1));
static_assert(0x89abcdef == dassign<bool>( int32, - 64, 1));
static_assert(0x89abcdef == dassign<bool>( int32, - 60, 1));
static_assert(0x89abcdef == dassign<bool>( int32, - 56, 1));
static_assert(0x89abcdef == dassign<bool>( int32, -  4, 1));
static_assert(0x89abcdef == dassign<bool>( int32,    0, 1));
static_assert(0x89abcdff == dassign<bool>( int32,    4, 1));
static_assert(0x89abcdee == dassign<bool>( int32,    0, 0));
static_assert(0x89abcdef == dassign<bool>( int32,    4, 0));
static_assert(0x89abcdef == dassign<bool>( int32,   56, 1));
static_assert(0x89abcdef == dassign<bool>( int32,  100, 1));

enum class op { bit_and, bit_or, bit_xor, };

template <op OpV, integral T>
constexpr auto dassign(auto value, std::ptrdiff_t const i, auto const assign) {
	switch (OpV) {
	case op::bit_and:
		digits_as<T>(value)[i] &= assign;
		break;
	case op::bit_or:
		digits_as<T>(value)[i] |= assign;
		break;
	case op::bit_xor:
		digits_as<T>(value)[i] ^= assign;
		break;
	}
	return value;
}

static_assert(0x1010101010543210 == dassign<op::bit_and, uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xffddbb9977543210 == dassign<op::bit_or , uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xefcdab8967543210 == dassign<op::bit_xor, uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0x0101010101abcdef == dassign<op::bit_and, uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1133557799abcdef == dassign<op::bit_or , uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1032547698abcdef == dassign<op::bit_xor, uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1010101010543210 == dassign<op::bit_and,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xffddbb9977543210 == dassign<op::bit_or ,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xefcdab8967543210 == dassign<op::bit_xor,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0x0101010101abcdef == dassign<op::bit_and,  int64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1133557799abcdef == dassign<op::bit_or ,  int64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1032547698abcdef == dassign<op::bit_xor,  int64_t>( int64, 24, 0x1111111111111111));

static_assert(0xfedcba9876501210 == dassign<op::bit_and,  uint8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876553210 == dassign<op::bit_or ,  uint8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876552210 == dassign<op::bit_xor,  uint8_t>(uint64, 12, 0x11));
static_assert(0x0123456789a10def == dassign<op::bit_and,  uint8_t>( int64, 12, 0x11));
static_assert(0x0123456789abddef == dassign<op::bit_or ,  uint8_t>( int64, 12, 0x11));
static_assert(0x0123456789aaddef == dassign<op::bit_xor,  uint8_t>( int64, 12, 0x11));
static_assert(0xfedcba9876501210 == dassign<op::bit_and,   int8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876553210 == dassign<op::bit_or ,   int8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876552210 == dassign<op::bit_xor,   int8_t>(uint64, 12, 0x11));
static_assert(0x0123456789a10def == dassign<op::bit_and,   int8_t>( int64, 12, 0x11));
static_assert(0x0123456789abddef == dassign<op::bit_or ,   int8_t>( int64, 12, 0x11));
static_assert(0x0123456789aaddef == dassign<op::bit_xor,   int8_t>( int64, 12, 0x11));

static_assert(0xfedcba9876542210 == dassign<op::bit_and, bool>(uint64, 12, 0));
static_assert(0xfedcba9876543210 == dassign<op::bit_or , bool>(uint64, 12, 0));
static_assert(0xfedcba9876543210 == dassign<op::bit_xor, bool>(uint64, 12, 0));
static_assert(0x0123456789abcdef == dassign<op::bit_and, bool>( int64, 12, 0));
static_assert(0x0123456789abcdef == dassign<op::bit_or , bool>( int64, 12, 0));
static_assert(0x0123456789abcdef == dassign<op::bit_xor, bool>( int64, 12, 0));
static_assert(0xfedcba9876543210 == dassign<op::bit_and, bool>(uint64, 12, 1));
static_assert(0xfedcba9876543210 == dassign<op::bit_or , bool>(uint64, 12, 1));
static_assert(0xfedcba9876542210 == dassign<op::bit_xor, bool>(uint64, 12, 1));
static_assert(0x0123456789abcdef == dassign<op::bit_and, bool>( int64, 12, 1));
static_assert(0x0123456789abddef == dassign<op::bit_or , bool>( int64, 12, 1));
static_assert(0x0123456789abddef == dassign<op::bit_xor, bool>( int64, 12, 1));

static_assert(0x76542200 == dassign<op::bit_and, uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76543232 == dassign<op::bit_or,  uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76541032 == dassign<op::bit_xor, uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x89ab0022 == dassign<op::bit_and, uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefef == dassign<op::bit_or,  uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefcd == dassign<op::bit_xor, uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x76542200 == dassign<op::bit_and,  int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76543232 == dassign<op::bit_or,   int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76541032 == dassign<op::bit_xor,  int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x89ab0022 == dassign<op::bit_and,  int64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefef == dassign<op::bit_or,   int64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefcd == dassign<op::bit_xor,  int64_t>( int32, -48, 0x2222222222222222));

static_assert(0x16543210 == dassign<op::bit_and, uint8_t>(uint32, 28, 0x99));
static_assert(0xf6543210 == dassign<op::bit_or,  uint8_t>(uint32, 28, 0x99));
static_assert(0xe6543210 == dassign<op::bit_xor, uint8_t>(uint32, 28, 0x99));
static_assert(0x89abc98f == dassign<op::bit_and, uint8_t>( int32,  4, 0x99));
static_assert(0x89abcdff == dassign<op::bit_or,  uint8_t>( int32,  4, 0x99));
static_assert(0x89abc47f == dassign<op::bit_xor, uint8_t>( int32,  4, 0x99));
static_assert(0x16543210 == dassign<op::bit_and,  int8_t>(uint32, 28, 0x99));
static_assert(0xf6543210 == dassign<op::bit_or,   int8_t>(uint32, 28, 0x99));
static_assert(0xe6543210 == dassign<op::bit_xor,  int8_t>(uint32, 28, 0x99));
static_assert(0x89abc98f == dassign<op::bit_and,  int8_t>( int32,  4, 0x99));
static_assert(0x89abcdff == dassign<op::bit_or,   int8_t>( int32,  4, 0x99));
static_assert(0x89abc47f == dassign<op::bit_xor,  int8_t>( int32,  4, 0x99));

static_assert(0x66543210 == dassign<op::bit_and, bool>(uint32, 28, 0));
static_assert(0x76543210 == dassign<op::bit_or,  bool>(uint32, 28, 0));
static_assert(0x76543210 == dassign<op::bit_xor, bool>(uint32, 28, 0));
static_assert(0x89abcdef == dassign<op::bit_and, bool>( int32,  4, 0));
static_assert(0x89abcdef == dassign<op::bit_or,  bool>( int32,  4, 0));
static_assert(0x89abcdef == dassign<op::bit_xor, bool>( int32,  4, 0));
static_assert(0x76543210 == dassign<op::bit_and, bool>(uint32, 28, 1));
static_assert(0x76543210 == dassign<op::bit_or,  bool>(uint32, 28, 1));
static_assert(0x66543210 == dassign<op::bit_xor, bool>(uint32, 28, 1));
static_assert(0x89abcdef == dassign<op::bit_and, bool>( int32,  4, 1));
static_assert(0x89abcdff == dassign<op::bit_or,  bool>( int32,  4, 1));
static_assert(0x89abcdff == dassign<op::bit_xor, bool>( int32,  4, 1));

static_assert(expect<uint64_t>(uint64, 0xfedcba9876543210,  0,  0));
static_assert(expect< int64_t>(uint64, 0xfedcba9876543210,  0,  0));
static_assert(expect<uint64_t>(uint32,         0x76543210,  0,  0));
static_assert(expect< int64_t>(uint32,         0x76543210,  0,  0));
static_assert(expect<uint64_t>(uint16,             0x3210,  0,  0));
static_assert(expect< int64_t>(uint16,             0x3210,  0,  0));
static_assert(expect<uint64_t>( uint8,               0x10,  0,  0));
static_assert(expect< int64_t>( uint8,               0x10,  0,  0));

static_assert(expect<uint64_t>( int64, 0x0123456789abcdef,  0,  0));
static_assert(expect< int64_t>( int64, 0x0123456789abcdef,  0,  0));
static_assert(expect<uint64_t>( int32, 0xffffffff89abcdef, -1, -1));
static_assert(expect< int64_t>( int32, 0xffffffff89abcdef, -1, -1));
static_assert(expect<uint64_t>( int16, 0xffffffffffffcdef, -1, -1));
static_assert(expect< int64_t>( int16, 0xffffffffffffcdef, -1, -1));
static_assert(expect<uint64_t>(  int8, 0xffffffffffffffef, -1, -1));
static_assert(expect< int64_t>(  int8, 0xffffffffffffffef, -1, -1));

static_assert(expect<uint32_t>(uint64, 0x76543210, 0xfedcba98,  0,  0));
static_assert(expect< int32_t>(uint64, 0x76543210, 0xfedcba98,  0,  0));
static_assert(expect<uint32_t>(uint32,             0x76543210,  0,  0));
static_assert(expect< int32_t>(uint32,             0x76543210,  0,  0));
static_assert(expect<uint32_t>(uint16,                 0x3210,  0,  0));
static_assert(expect< int32_t>(uint16,                 0x3210,  0,  0));
static_assert(expect<uint32_t>( uint8,                   0x10,  0,  0));
static_assert(expect< int32_t>( uint8,                   0x10,  0,  0));

static_assert(expect<uint32_t>( int64, 0x89abcdef, 0x01234567,  0,  0));
static_assert(expect< int32_t>( int64, 0x89abcdef, 0x01234567,  0,  0));
static_assert(expect<uint32_t>( int32,             0x89abcdef, -1, -1));
static_assert(expect< int32_t>( int32,             0x89abcdef, -1, -1));
static_assert(expect<uint32_t>( int16,             0xffffcdef, -1, -1));
static_assert(expect< int32_t>( int16,             0xffffcdef, -1, -1));
static_assert(expect<uint32_t>(  int8,             0xffffffef, -1, -1));
static_assert(expect< int32_t>(  int8,             0xffffffef, -1, -1));

static_assert(expect<uint16_t>(uint64, 0x3210, 0x7654, 0xba98, 0xfedc,  0,  0));
static_assert(expect< int16_t>(uint64, 0x3210, 0x7654, 0xba98, 0xfedc,  0,  0));
static_assert(expect<uint16_t>(uint32,                 0x3210, 0x7654,  0,  0));
static_assert(expect< int16_t>(uint32,                 0x3210, 0x7654,  0,  0));
static_assert(expect<uint16_t>(uint16,                         0x3210,  0,  0));
static_assert(expect< int16_t>(uint16,                         0x3210,  0,  0));
static_assert(expect<uint16_t>( uint8,                           0x10,  0,  0));
static_assert(expect< int16_t>( uint8,                           0x10,  0,  0));

static_assert(expect<uint16_t>( int64, 0xcdef, 0x89ab, 0x4567, 0x0123,  0,  0));
static_assert(expect< int16_t>( int64, 0xcdef, 0x89ab, 0x4567, 0x0123,  0,  0));
static_assert(expect<uint16_t>( int32,                 0xcdef, 0x89ab, -1, -1));
static_assert(expect< int16_t>( int32,                 0xcdef, 0x89ab, -1, -1));
static_assert(expect<uint16_t>( int16,                         0xcdef, -1, -1));
static_assert(expect< int16_t>( int16,                         0xcdef, -1, -1));
static_assert(expect<uint16_t>(  int8,                         0xffef, -1, -1));
static_assert(expect< int16_t>(  int8,                         0xffef, -1, -1));

static_assert(expect<uint8_t>(uint64, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,  0,  0));
static_assert(expect< int8_t>(uint64, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,  0,  0));
static_assert(expect<uint8_t>(uint32,                         0x10, 0x32, 0x54, 0x76,  0,  0));
static_assert(expect< int8_t>(uint32,                         0x10, 0x32, 0x54, 0x76,  0,  0));
static_assert(expect<uint8_t>(uint16,                                     0x10, 0x32,  0,  0));
static_assert(expect< int8_t>(uint16,                                     0x10, 0x32,  0,  0));
static_assert(expect<uint8_t>( uint8,                                           0x10,  0,  0));
static_assert(expect< int8_t>( uint8,                                           0x10,  0,  0));

static_assert(expect<uint8_t>( int64, 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,  0,  0));
static_assert(expect< int8_t>( int64, 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,  0,  0));
static_assert(expect<uint8_t>( int32,                         0xef, 0xcd, 0xab, 0x89, -1, -1));
static_assert(expect< int8_t>( int32,                         0xef, 0xcd, 0xab, 0x89, -1, -1));
static_assert(expect<uint8_t>( int16,                                     0xef, 0xcd, -1, -1));
static_assert(expect< int8_t>( int16,                                     0xef, 0xcd, -1, -1));
static_assert(expect<uint8_t>(  int8,                                           0xef, -1, -1));
static_assert(expect< int8_t>(  int8,                                           0xef, -1, -1));

template <unsigned_integral T>
constexpr bool masked_comp(T const mask, T const lhs, T const rhs) {
	return 0 == ((lhs ^ rhs) & mask);
}

template <integral T>
constexpr bool assign(auto const & values, auto const assign) {
	using UT = std::make_unsigned_t<T>;
	auto const access{access_as<T>(values)};
	auto const slices{bisect_as<T>(values)};

	for (std::size_t i{0}; i < access.size(); ++i) {
		auto copy{values};
		auto v{access_as<T>(copy)};
		v[i] = assign;
		for (std::size_t p{0}; p < access.size(); ++p) {
			auto const digits{access.digits - i * digits_v<UT>};
			auto const mask{lshift<UT>(1, digits) - 1};
			if ((p == i && !masked_comp<UT>(mask, v[p], assign)) ||
			    (p != i && !masked_comp<UT>(mask, v[p], access[p]))) {
				return false;
			}
		}
	}

	for (std::size_t i{0}; i < slices.size(); ++i) {
		constexpr auto const slice_mask{lshift<UT>(1, digits_v<UT> / 2) - 1};
		auto copy{values};
		auto b{bisect_as<T>(copy)};
		b[i] = assign;
		for (std::size_t p{0}; p < b.size(); ++p) {
			auto const digits{access.digits - i * (digits_v<UT> / 2)};
			auto const mask{(lshift<UT>(1, digits) - 1) & slice_mask};
			if ((p == i && !masked_comp<UT>(mask, b[p], assign)) ||
			    (p != i && !masked_comp<UT>(mask, b[p], slices[p]))) {
				return false;
			}
		}
	}

	return true;
}

static_assert(assign<uint32_t>(uint32, 0xfedcba98));
static_assert(assign<uint32_t>(uint32,          0));
static_assert(assign<uint32_t>( int32, 0xfedcba98));
static_assert(assign<uint32_t>( int32,          0));
static_assert(assign< int32_t>(uint32, 0xfedcba98));
static_assert(assign< int32_t>(uint32,          0));
static_assert(assign< int32_t>( int32, 0xfedcba98));
static_assert(assign< int32_t>( int32,          0));

static_assert(assign< uint8_t>(uint64, 0xff));
static_assert(assign< uint8_t>(uint64,    0));
static_assert(assign< uint8_t>( int64, 0xff));
static_assert(assign< uint8_t>( int64,    0));
static_assert(assign<  int8_t>(uint64,   -1));
static_assert(assign<  int8_t>(uint64,    0));
static_assert(assign<  int8_t>( int64,   -1));
static_assert(assign<  int8_t>( int64,    0));

static_assert(assign<uint32_t>(uint16, 0xffffffff));
static_assert(assign<uint32_t>(uint16,          0));
static_assert(assign<uint32_t>( int16, 0xffffffff));
static_assert(assign<uint32_t>( int16,          0));
static_assert(assign< int32_t>(uint16, 0xffffffff));
static_assert(assign< int32_t>(uint16,          0));
static_assert(assign< int32_t>( int16, 0xffffffff));
static_assert(assign< int32_t>( int16,          0));

template <op OpV, integral T>
constexpr auto bassign(auto value, std::ptrdiff_t const i, auto const assign) {
	switch (OpV) {
	case op::bit_and:
		bisect_as<T>(value)[i] &= assign;
		break;
	case op::bit_or:
		bisect_as<T>(value)[i] |= assign;
		break;
	case op::bit_xor:
		bisect_as<T>(value)[i] ^= assign;
		break;
	}
	return value;
}

static_assert(0xfedcba9810101010 == bassign<op::bit_and, uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9877553311 == bassign<op::bit_or,  uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9867452301 == bassign<op::bit_xor, uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0x0123456701010101 == bassign<op::bit_and, uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456799bbddff == bassign<op::bit_or , uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456798badcfe == bassign<op::bit_xor, uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0xfedcba9810101010 == bassign<op::bit_and,  int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9877553311 == bassign<op::bit_or,   int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9867452301 == bassign<op::bit_xor,  int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0x0123456701010101 == bassign<op::bit_and,  int64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456799bbddff == bassign<op::bit_or ,  int64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456798badcfe == bassign<op::bit_xor,  int64_t>( int64, 0, 0x1111111111111111));

static_assert(0xfedcba9876541210 == bassign<op::bit_and,  uint8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876543210 == bassign<op::bit_or ,  uint8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876542210 == bassign<op::bit_xor,  uint8_t>(uint64, 3, 0x11));
static_assert(0x0123456789ab0def == bassign<op::bit_and,  uint8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef == bassign<op::bit_or ,  uint8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef == bassign<op::bit_xor,  uint8_t>( int64, 3, 0x11));
static_assert(0xfedcba9876541210 == bassign<op::bit_and,   int8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876543210 == bassign<op::bit_or ,   int8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876542210 == bassign<op::bit_xor,   int8_t>(uint64, 3, 0x11));
static_assert(0x0123456789ab0def == bassign<op::bit_and,   int8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef == bassign<op::bit_or ,   int8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef == bassign<op::bit_xor,   int8_t>( int64, 3, 0x11));

static_assert(0x22002200 == bassign<op::bit_and, uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x76763232 == bassign<op::bit_or,  uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x54761032 == bassign<op::bit_xor, uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x00220022 == bassign<op::bit_and, uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0xababefef == bassign<op::bit_or,  uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0xab89efcd == bassign<op::bit_xor, uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0x22002200 == bassign<op::bit_and,  int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x76763232 == bassign<op::bit_or,   int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x54761032 == bassign<op::bit_xor,  int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x00220022 == bassign<op::bit_and,  int64_t>( int32, 0, 0x2222222222222222));
static_assert(0xababefef == bassign<op::bit_or,   int64_t>( int32, 0, 0x2222222222222222));
static_assert(0xab89efcd == bassign<op::bit_xor,  int64_t>( int32, 0, 0x2222222222222222));

static_assert(0x16543210 == bassign<op::bit_and, uint8_t>(uint32, 7, 0x99));
static_assert(0xf6543210 == bassign<op::bit_or,  uint8_t>(uint32, 7, 0x99));
static_assert(0xe6543210 == bassign<op::bit_xor, uint8_t>(uint32, 7, 0x99));
static_assert(0x89abcd8f == bassign<op::bit_and, uint8_t>( int32, 1, 0x99));
static_assert(0x89abcdff == bassign<op::bit_or,  uint8_t>( int32, 1, 0x99));
static_assert(0x89abcd7f == bassign<op::bit_xor, uint8_t>( int32, 1, 0x99));
static_assert(0x16543210 == bassign<op::bit_and,  int8_t>(uint32, 7, 0x99));
static_assert(0xf6543210 == bassign<op::bit_or,   int8_t>(uint32, 7, 0x99));
static_assert(0xe6543210 == bassign<op::bit_xor,  int8_t>(uint32, 7, 0x99));
static_assert(0x89abcd8f == bassign<op::bit_and,  int8_t>( int32, 1, 0x99));
static_assert(0x89abcdff == bassign<op::bit_or,   int8_t>( int32, 1, 0x99));
static_assert(0x89abcd7f == bassign<op::bit_xor,  int8_t>( int32, 1, 0x99));

constexpr std::array<uint64_t, 2> const uint64a{
	0x7766554433221100, 0xffeeddccbbaa9988
};

constexpr std::array<uint32_t, 3> const uint32a{
	0x33221100, 0x77665544, 0xbbaa9988
};

constexpr std::array<uint16_t, 7> const uint16a{
	0x1100, 0x3322, 0x5544, 0x7766, 0x9988, 0xbbaa, 0xddcc
};

constexpr std::array<uint8_t, 15> const uint8a{
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee
};

constexpr std::array<int64_t, 2> const int64a{
	0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)
};

constexpr std::array<int32_t, 3> const int32a{
	0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa9988)
};

constexpr std::array<int16_t, 7> const int16a{
	0x1100, 0x3322, 0x5544, 0x7766,
	static_cast<int16_t>(0x9988), static_cast<int16_t>(0xbbaa),
	static_cast<int16_t>(0xddcc)
};

constexpr std::array<int8_t, 15> const int8a{
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	static_cast<int8_t>(0x88), static_cast<int8_t>(0x99),
	static_cast<int8_t>(0xaa), static_cast<int8_t>(0xbb),
	static_cast<int8_t>(0xcc), static_cast<int8_t>(0xdd),
	static_cast<int8_t>(0xee)
};

static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64a)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64a)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64a)[- 60]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint64a)[- 56]);
static_assert(0x7665544332211000 == digits_as<uint64_t>(uint64a)[-  4]);
static_assert(0x7766554433221100 == digits_as<uint64_t>(uint64a)[   0]);
static_assert(0x8776655443322110 == digits_as<uint64_t>(uint64a)[   4]);
static_assert(0xeeddccbbaa998877 == digits_as<uint64_t>(uint64a)[  56]);
static_assert(0x000000000ffeeddc == digits_as<uint64_t>(uint64a)[ 100]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64a)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64a)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64a)[- 60]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int64a)[- 56]);
static_assert(0x7665544332211000 == digits_as<uint64_t>( int64a)[-  4]);
static_assert(0x7766554433221100 == digits_as<uint64_t>( int64a)[   0]);
static_assert(0x8776655443322110 == digits_as<uint64_t>( int64a)[   4]);
static_assert(0xeeddccbbaa998877 == digits_as<uint64_t>( int64a)[  56]);
static_assert(0xfffffffffffeeddc == digits_as<uint64_t>( int64a)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64a)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64a)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64a)[- 60]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint64a)[- 56]);
static_assert(0x7665544332211000 == digits_as< int64_t>(uint64a)[-  4]);
static_assert(0x7766554433221100 == digits_as< int64_t>(uint64a)[   0]);
static_assert(0x8776655443322110 == digits_as< int64_t>(uint64a)[   4]);
static_assert(0xeeddccbbaa998877 == digits_as< int64_t>(uint64a)[  56]);
static_assert(0x000000000ffeeddc == digits_as< int64_t>(uint64a)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64a)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64a)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64a)[- 60]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int64a)[- 56]);
static_assert(0x7665544332211000 == digits_as< int64_t>( int64a)[-  4]);
static_assert(0x7766554433221100 == digits_as< int64_t>( int64a)[   0]);
static_assert(0x8776655443322110 == digits_as< int64_t>( int64a)[   4]);
static_assert(0xeeddccbbaa998877 == digits_as< int64_t>( int64a)[  56]);
static_assert(0xfffffffffffeeddc == digits_as< int64_t>( int64a)[ 100]);

static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[-  4]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint64a)[   0]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>(uint64a)[   4]);
static_assert(static_cast<uint8_t>(0x77) == digits_as<uint8_t>(uint64a)[  56]);
static_assert(static_cast<uint8_t>(0xdc) == digits_as<uint8_t>(uint64a)[ 100]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[-  4]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int64a)[   0]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>( int64a)[   4]);
static_assert(static_cast<uint8_t>(0x77) == digits_as<uint8_t>( int64a)[  56]);
static_assert(static_cast<uint8_t>(0xdc) == digits_as<uint8_t>( int64a)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[-  4]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint64a)[   0]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>(uint64a)[   4]);
static_assert(static_cast< int8_t>(0x77) == digits_as< int8_t>(uint64a)[  56]);
static_assert(static_cast< int8_t>(0xdc) == digits_as< int8_t>(uint64a)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[-  4]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int64a)[   0]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>( int64a)[   4]);
static_assert(static_cast< int8_t>(0x77) == digits_as< int8_t>( int64a)[  56]);
static_assert(static_cast< int8_t>(0xdc) == digits_as< int8_t>( int64a)[ 100]);

static_assert(!digits_as<bool>(uint64a)[-200]);
static_assert(!digits_as<bool>(uint64a)[- 64]);
static_assert(!digits_as<bool>(uint64a)[- 60]);
static_assert(!digits_as<bool>(uint64a)[- 56]);
static_assert(!digits_as<bool>(uint64a)[-  4]);
static_assert(!digits_as<bool>(uint64a)[   0]);
static_assert(!digits_as<bool>(uint64a)[   4]);
static_assert( digits_as<bool>(uint64a)[  56]);
static_assert(!digits_as<bool>(uint64a)[ 100]);
static_assert(!digits_as<bool>( int64a)[-200]);
static_assert(!digits_as<bool>( int64a)[- 64]);
static_assert(!digits_as<bool>( int64a)[- 60]);
static_assert(!digits_as<bool>( int64a)[- 56]);
static_assert(!digits_as<bool>( int64a)[-  4]);
static_assert(!digits_as<bool>( int64a)[   0]);
static_assert(!digits_as<bool>( int64a)[   4]);
static_assert( digits_as<bool>( int64a)[  56]);
static_assert(!digits_as<bool>( int64a)[ 100]);

static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32a)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32a)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32a)[- 60]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32a)[- 56]);
static_assert(0x7665544332211000 == digits_as<uint64_t>(uint32a)[-  4]);
static_assert(0x7766554433221100 == digits_as<uint64_t>(uint32a)[   0]);
static_assert(0x8776655443322110 == digits_as<uint64_t>(uint32a)[   4]);
static_assert(0x000000bbaa998877 == digits_as<uint64_t>(uint32a)[  56]);
static_assert(0x0000000000000000 == digits_as<uint64_t>(uint32a)[ 100]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32a)[-200]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32a)[- 64]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32a)[- 60]);
static_assert(0x0000000000000000 == digits_as<uint64_t>( int32a)[- 56]);
static_assert(0x7665544332211000 == digits_as<uint64_t>( int32a)[-  4]);
static_assert(0x7766554433221100 == digits_as<uint64_t>( int32a)[   0]);
static_assert(0x8776655443322110 == digits_as<uint64_t>( int32a)[   4]);
static_assert(0xffffffbbaa998877 == digits_as<uint64_t>( int32a)[  56]);
static_assert(0xffffffffffffffff == digits_as<uint64_t>( int32a)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32a)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32a)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32a)[- 60]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32a)[- 56]);
static_assert(0x7665544332211000 == digits_as< int64_t>(uint32a)[-  4]);
static_assert(0x7766554433221100 == digits_as< int64_t>(uint32a)[   0]);
static_assert(0x8776655443322110 == digits_as< int64_t>(uint32a)[   4]);
static_assert(0x000000bbaa998877 == digits_as< int64_t>(uint32a)[  56]);
static_assert(0x0000000000000000 == digits_as< int64_t>(uint32a)[ 100]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32a)[-200]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32a)[- 64]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32a)[- 60]);
static_assert(0x0000000000000000 == digits_as< int64_t>( int32a)[- 56]);
static_assert(0x7665544332211000 == digits_as< int64_t>( int32a)[-  4]);
static_assert(0x7766554433221100 == digits_as< int64_t>( int32a)[   0]);
static_assert(0x8776655443322110 == digits_as< int64_t>( int32a)[   4]);
static_assert(0xffffffbbaa998877 == digits_as< int64_t>( int32a)[  56]);
static_assert(0xffffffffffffffff == digits_as< int64_t>( int32a)[ 100]);

static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[-  4]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[   0]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>(uint32a)[   4]);
static_assert(static_cast<uint8_t>(0x77) == digits_as<uint8_t>(uint32a)[  56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>(uint32a)[ 100]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[-200]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[- 64]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[- 60]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[- 56]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[-  4]);
static_assert(static_cast<uint8_t>(0x00) == digits_as<uint8_t>( int32a)[   0]);
static_assert(static_cast<uint8_t>(0x10) == digits_as<uint8_t>( int32a)[   4]);
static_assert(static_cast<uint8_t>(0x77) == digits_as<uint8_t>( int32a)[  56]);
static_assert(static_cast<uint8_t>(0xff) == digits_as<uint8_t>( int32a)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[-  4]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[   0]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>(uint32a)[   4]);
static_assert(static_cast< int8_t>(0x77) == digits_as< int8_t>(uint32a)[  56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>(uint32a)[ 100]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[-200]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[- 64]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[- 60]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[- 56]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[-  4]);
static_assert(static_cast< int8_t>(0x00) == digits_as< int8_t>( int32a)[   0]);
static_assert(static_cast< int8_t>(0x10) == digits_as< int8_t>( int32a)[   4]);
static_assert(static_cast< int8_t>(0x77) == digits_as< int8_t>( int32a)[  56]);
static_assert(static_cast< int8_t>(0xff) == digits_as< int8_t>( int32a)[ 100]);

static_assert(!digits_as<bool>(uint32a)[-200]);
static_assert(!digits_as<bool>(uint32a)[- 64]);
static_assert(!digits_as<bool>(uint32a)[- 60]);
static_assert(!digits_as<bool>(uint32a)[- 56]);
static_assert(!digits_as<bool>(uint32a)[-  4]);
static_assert(!digits_as<bool>(uint32a)[   0]);
static_assert(!digits_as<bool>(uint32a)[   4]);
static_assert( digits_as<bool>(uint32a)[  56]);
static_assert(!digits_as<bool>(uint32a)[ 100]);
static_assert(!digits_as<bool>( int32a)[-200]);
static_assert(!digits_as<bool>( int32a)[- 64]);
static_assert(!digits_as<bool>( int32a)[- 60]);
static_assert(!digits_as<bool>( int32a)[- 56]);
static_assert(!digits_as<bool>( int32a)[-  4]);
static_assert(!digits_as<bool>( int32a)[   0]);
static_assert(!digits_as<bool>( int32a)[   4]);
static_assert( digits_as<bool>( int32a)[  56]);
static_assert( digits_as<bool>( int32a)[ 100]);

static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a, -200, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a, - 64, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x776655443322110c, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a, - 60, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x77665544332211cf, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a, - 56, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7cffffffffffffff, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a, -  4, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xcffffffffffffff5, 0xffeeddccbbaa9988} == dassign<uint64_t>(uint64a,    0, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xffffffffffffff50, 0xffeeddccbbaa998c} == dassign<uint64_t>(uint64a,    4, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xf566554433221100, 0xffcfffffffffffff} == dassign<uint64_t>(uint64a,   56, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffffff5cbbaa9988} == dassign<uint64_t>(uint64a,  100, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a, -200, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a, - 64, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x776655443322110c), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a, - 60, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x77665544332211cf), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a, - 56, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7cffffffffffffff), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a, -  4, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xcffffffffffffff5), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint64_t>( int64a,    0, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xffffffffffffff50), static_cast<int64_t>(0xffeeddccbbaa998c)}
              == dassign<uint64_t>( int64a,    4, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xf566554433221100), static_cast<int64_t>(0xffcfffffffffffff)}
              == dassign<uint64_t>( int64a,   56, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffffff5cbbaa9988)}
              == dassign<uint64_t>( int64a,  100, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a, -200, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a, - 64, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x776655443322110c, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a, - 60, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x77665544332211cf, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a, - 56, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7cffffffffffffff, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a, -  4, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xcffffffffffffff5, 0xffeeddccbbaa9988} == dassign< int64_t>(uint64a,    0, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xffffffffffffff50, 0xffeeddccbbaa998c} == dassign< int64_t>(uint64a,    4, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0xf566554433221100, 0xffcfffffffffffff} == dassign< int64_t>(uint64a,   56, 0xcffffffffffffff5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffffff5cbbaa9988} == dassign< int64_t>(uint64a,  100, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a, -200, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a, - 64, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x776655443322110c), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a, - 60, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x77665544332211cf), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a, - 56, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7cffffffffffffff), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a, -  4, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xcffffffffffffff5), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int64_t>( int64a,    0, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xffffffffffffff50), static_cast<int64_t>(0xffeeddccbbaa998c)}
              == dassign< int64_t>( int64a,    4, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xf566554433221100), static_cast<int64_t>(0xffcfffffffffffff)}
              == dassign< int64_t>( int64a,   56, 0xcffffffffffffff5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffffff5cbbaa9988)}
              == dassign< int64_t>( int64a,  100, 0xcffffffffffffff5));

static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a, -200, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a, - 64, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a, - 60, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a, - 56, 0xc5));
static_assert(std::array<uint64_t, 2>{0x776655443322110c, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a, -  4, 0xc5));
static_assert(std::array<uint64_t, 2>{0x77665544332211c5, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a,    0, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221c50, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a,    4, 0xc5));
static_assert(std::array<uint64_t, 2>{0xc566554433221100, 0xffeeddccbbaa9988} == dassign<uint8_t>(uint64a,   56, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeedc5cbbaa9988} == dassign<uint8_t>(uint64a,  100, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a, -200, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a, - 64, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a, - 60, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a, - 56, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x776655443322110c), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a, -  4, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x77665544332211c5), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a,    0, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221c50), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a,    4, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xc566554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign<uint8_t>( int64a,   56, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeedc5cbbaa9988)}
              == dassign<uint8_t>( int64a,  100, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a, -200, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a, - 64, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a, - 60, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a, - 56, 0xc5));
static_assert(std::array<uint64_t, 2>{0x776655443322110c, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a, -  4, 0xc5));
static_assert(std::array<uint64_t, 2>{0x77665544332211c5, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a,    0, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221c50, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a,    4, 0xc5));
static_assert(std::array<uint64_t, 2>{0xc566554433221100, 0xffeeddccbbaa9988} == dassign< int8_t>(uint64a,   56, 0xc5));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeedc5cbbaa9988} == dassign< int8_t>(uint64a,  100, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a, -200, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a, - 64, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a, - 60, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a, - 56, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x776655443322110c), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a, -  4, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x77665544332211c5), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a,    0, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221c50), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a,    4, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0xc566554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)}
              == dassign< int8_t>( int64a,   56, 0xc5));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeedc5cbbaa9988)}
              == dassign< int8_t>( int64a,  100, 0xc5));

static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a, -200, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a, - 64, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a, - 60, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a, - 56, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a, -  4, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,    0, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,    4, 0));
static_assert(std::array<uint64_t, 2>{0x7666554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,   56, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,  100, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221101, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,    0, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221110, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,    4, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<bool>(uint64a,   56, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeedddcbbaa9988} == dassign<bool>(uint64a,  100, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a, -200, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a, - 64, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a, - 60, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a, - 56, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a, -  4, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,    0, 0));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,    4, 0));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7666554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,   56, 0));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,  100, 0));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221101), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,    0, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221110), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,    4, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<bool>( int64a,   56, 1));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffeedddcbbaa9988)} == dassign<bool>( int64a,  100, 1));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint64_t>(uint32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint64_t>(uint32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x3322110c, 0x77665544, 0xbbaa9988} == dassign<uint64_t>(uint32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x332211cf, 0x77665544, 0xbbaa9988} == dassign<uint64_t>(uint32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0xd6ffffff, 0x7cffffff, 0xbbaa9988} == dassign<uint64_t>(uint32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x6ffffff5, 0xcffffffd, 0xbbaa9988} == dassign<uint64_t>(uint32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0xffffff50, 0xffffffd6, 0xbbaa998c} == dassign<uint64_t>(uint32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xf5665544, 0xfd6fffff} == dassign<uint64_t>(uint32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint64_t>(uint32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x3322110c), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x332211cf), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0xd6ffffff), static_cast<int32_t>(0x7cffffff), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x6ffffff5), static_cast<int32_t>(0xcffffffd), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0xffffff50), static_cast<int32_t>(0xffffffd6), static_cast<int32_t>(0xbbaa998c)}
              == dassign<uint64_t>( int32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0xf5665544), static_cast<int32_t>(0xfd6fffff)}
              == dassign<uint64_t>( int32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<uint64_t>( int32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int64_t>(uint32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int64_t>(uint32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x3322110c, 0x77665544, 0xbbaa9988} == dassign< int64_t>(uint32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x332211cf, 0x77665544, 0xbbaa9988} == dassign< int64_t>(uint32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0xd6ffffff, 0x7cffffff, 0xbbaa9988} == dassign< int64_t>(uint32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x6ffffff5, 0xcffffffd, 0xbbaa9988} == dassign< int64_t>(uint32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0xffffff50, 0xffffffd6, 0xbbaa998c} == dassign< int64_t>(uint32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xf5665544, 0xfd6fffff} == dassign< int64_t>(uint32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int64_t>(uint32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x3322110c), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x332211cf), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0xd6ffffff), static_cast<int32_t>(0x7cffffff), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x6ffffff5), static_cast<int32_t>(0xcffffffd), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0xffffff50), static_cast<int32_t>(0xffffffd6), static_cast<int32_t>(0xbbaa998c)}
              == dassign< int64_t>( int32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0xf5665544), static_cast<int32_t>(0xfd6fffff)}
              == dassign< int64_t>( int32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign< int64_t>( int32a,  100, 0xcffffffd6ffffff5));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a, -200, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a, - 64, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a, - 60, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a, - 56, 0xc5));
static_assert(std::array<uint32_t, 3>{0x3322110c, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a, -  4, 0xc5));
static_assert(std::array<uint32_t, 3>{0x332211c5, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a,    0, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221c50, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a,    4, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xc5665544, 0xbbaa9988} == dassign<uint8_t>(uint32a,   56, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<uint8_t>(uint32a,  100, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a, -200, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a, - 64, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a, - 60, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a, - 56, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x3322110c), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a, -  4, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x332211c5), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a,    0, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221c50), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a,    4, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0xc5665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a,   56, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<uint8_t>( int32a,  100, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a, -200, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a, - 64, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a, - 60, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a, - 56, 0xc5));
static_assert(std::array<uint32_t, 3>{0x3322110c, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a, -  4, 0xc5));
static_assert(std::array<uint32_t, 3>{0x332211c5, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a,    0, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221c50, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a,    4, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xc5665544, 0xbbaa9988} == dassign< int8_t>(uint32a,   56, 0xc5));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign< int8_t>(uint32a,  100, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a, -200, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a, - 64, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a, - 60, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a, - 56, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x3322110c), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a, -  4, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x332211c5), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a,    0, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221c50), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a,    4, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0xc5665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a,   56, 0xc5));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign< int8_t>( int32a,  100, 0xc5));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a, -200, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a, - 64, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a, - 60, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a, - 56, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a, -  4, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,    0, 0));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,    4, 0));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x76665544, 0xbbaa9988} == dassign<bool>(uint32a,   56, 0));
static_assert(std::array<uint32_t, 3>{0x33221101, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,    0, 1));
static_assert(std::array<uint32_t, 3>{0x33221110, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,    4, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,   56, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<bool>(uint32a,  100, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a, -200, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a, - 64, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a, - 60, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a, - 56, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a, -  4, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,    0, 0));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,    4, 0));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x76665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,   56, 0));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221101), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,    0, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221110), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,    4, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,   56, 1));
static_assert(std::array< int32_t, 3>{static_cast<int32_t>(0x33221100), static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<bool>( int32a,  100, 1));

static_assert(std::array<uint64_t, 2>{0x1166554433221100, 0xff00110011001100}
              == dassign<op::bit_and, uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffffddddbbbb9999}
              == dassign<op::bit_or,  uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array<uint64_t, 2>{0x6666554433221100, 0xffffccddaabb8899}
              == dassign<op::bit_xor, uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x1166554433221100, static_cast<int64_t>(0xff00110011001100)}
              == dassign<op::bit_and, uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffffddddbbbb9999)}
              == dassign<op::bit_or,  uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x6666554433221100, static_cast<int64_t>(0xffffccddaabb8899)}
              == dassign<op::bit_xor, uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array<uint64_t, 2>{0x1166554433221100, 0xff00110011001100}
              == dassign<op::bit_and,  int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffffddddbbbb9999}
              == dassign<op::bit_or,   int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array<uint64_t, 2>{0x6666554433221100, 0xffffccddaabb8899}
              == dassign<op::bit_xor,  int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x1166554433221100, static_cast<int64_t>(0xff00110011001100)}
              == dassign<op::bit_and,  int64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffffddddbbbb9999)}
              == dassign<op::bit_or,   int64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array< int64_t, 2>{0x6666554433221100, static_cast<int64_t>(0xffffccddaabb8899)}
              == dassign<op::bit_xor,  int64_t>( int64a, 56, 0x1111111111111111));

static_assert(std::array<uint64_t, 2>{0x1766554433221100, 0xffeeddccbbaa9980}
              == dassign<op::bit_and, uint8_t>(uint64a, 60, 0x11));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9989}
              == dassign<op::bit_or,  uint8_t>(uint64a, 60, 0x11));
static_assert(std::array<uint64_t, 2>{0x6766554433221100, 0xffeeddccbbaa9989}
              == dassign<op::bit_xor, uint8_t>(uint64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x1766554433221100, static_cast<int64_t>(0xffeeddccbbaa9980)}
              == dassign<op::bit_and, uint8_t>( int64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9989)}
              == dassign<op::bit_or,  uint8_t>( int64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x6766554433221100, static_cast<int64_t>(0xffeeddccbbaa9989)}
              == dassign<op::bit_xor, uint8_t>( int64a, 60, 0x11));
static_assert(std::array<uint64_t, 2>{0x1766554433221100, 0xffeeddccbbaa9980}
              == dassign<op::bit_and,  int8_t>(uint64a, 60, 0x11));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9989}
              == dassign<op::bit_or,   int8_t>(uint64a, 60, 0x11));
static_assert(std::array<uint64_t, 2>{0x6766554433221100, 0xffeeddccbbaa9989}
              == dassign<op::bit_xor,  int8_t>(uint64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x1766554433221100, static_cast<int64_t>(0xffeeddccbbaa9980)}
              == dassign<op::bit_and,  int8_t>( int64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9989)}
              == dassign<op::bit_or,   int8_t>( int64a, 60, 0x11));
static_assert(std::array< int64_t, 2>{0x6766554433221100, static_cast<int64_t>(0xffeeddccbbaa9989)}
              == dassign<op::bit_xor,  int8_t>( int64a, 60, 0x11));

static_assert(std::array<uint64_t, 2>{0x6766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_and, bool>(uint64a, 60, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_or,  bool>(uint64a, 60, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_xor, bool>(uint64a, 60, 0));
static_assert(std::array< int64_t, 2>{0x6766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_and, bool>( int64a, 60, 0));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_or,  bool>( int64a, 60, 0));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_xor, bool>( int64a, 60, 0));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_and, bool>(uint64a, 60, 1));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_or,  bool>(uint64a, 60, 1));
static_assert(std::array<uint64_t, 2>{0x6766554433221100, 0xffeeddccbbaa9988} == dassign<op::bit_xor, bool>(uint64a, 60, 1));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_and, bool>( int64a, 60, 1));
static_assert(std::array< int64_t, 2>{0x7766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_or,  bool>( int64a, 60, 1));
static_assert(std::array< int64_t, 2>{0x6766554433221100, static_cast<int64_t>(0xffeeddccbbaa9988)} == dassign<op::bit_xor, bool>( int64a, 60, 1));

static_assert(std::array<uint32_t, 3>{0x23221100, 0x22220000, 0xb2220000}
              == dassign<op::bit_and, uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77667766, 0xbbaabbaa}
              == dassign<op::bit_or,  uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array<uint32_t, 3>{0x13221100, 0x55447766, 0xb988bbaa}
              == dassign<op::bit_xor, uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x23221100, 0x22220000, static_cast<int32_t>(0xb2220000)}
              == dassign<op::bit_and, uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77667766, static_cast<int32_t>(0xbbaabbaa)}
              == dassign<op::bit_or,  uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x13221100, 0x55447766, static_cast<int32_t>(0xb988bbaa)}
              == dassign<op::bit_xor, uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array<uint32_t, 3>{0x23221100, 0x22220000, 0xb2220000}
              == dassign<op::bit_and,  int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77667766, 0xbbaabbaa}
              == dassign<op::bit_or,   int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array<uint32_t, 3>{0x13221100, 0x55447766, 0xb988bbaa}
              == dassign<op::bit_xor,  int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x23221100, 0x22220000, static_cast<int32_t>(0xb2220000)}
              == dassign<op::bit_and,  int64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77667766, static_cast<int32_t>(0xbbaabbaa)}
              == dassign<op::bit_or,   int64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array< int32_t, 3>{0x13221100, 0x55447766, static_cast<int32_t>(0xb988bbaa)}
              == dassign<op::bit_xor,  int64_t>( int32a, 28, 0x2222222222222222));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x17665544, 0xbbaa9988}
              == dassign<op::bit_and, uint8_t>(uint32a, 60, 0x99));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xf7665544, 0xbbaa9989}
              == dassign<op::bit_or,  uint8_t>(uint32a, 60, 0x99));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xe7665544, 0xbbaa9981}
              == dassign<op::bit_xor, uint8_t>(uint32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x17665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<op::bit_and, uint8_t>( int32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xf7665544), static_cast<int32_t>(0xbbaa9989)}
              == dassign<op::bit_or,  uint8_t>( int32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xe7665544), static_cast<int32_t>(0xbbaa9981)}
              == dassign<op::bit_xor, uint8_t>( int32a, 60, 0x99));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x17665544, 0xbbaa9988}
              == dassign<op::bit_and,  int8_t>(uint32a, 60, 0x99));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xf7665544, 0xbbaa9989}
              == dassign<op::bit_or,   int8_t>(uint32a, 60, 0x99));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xe7665544, 0xbbaa9981}
              == dassign<op::bit_xor,  int8_t>(uint32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x17665544), static_cast<int32_t>(0xbbaa9988)}
              == dassign<op::bit_and,  int8_t>( int32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xf7665544), static_cast<int32_t>(0xbbaa9989)}
              == dassign<op::bit_or,   int8_t>( int32a, 60, 0x99));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xe7665544), static_cast<int32_t>(0xbbaa9981)}
              == dassign<op::bit_xor,  int8_t>( int32a, 60, 0x99));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x67665544, 0xbbaa9988} == dassign<op::bit_and, bool>(uint32a, 60, 0));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<op::bit_or,  bool>(uint32a, 60, 0));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<op::bit_xor, bool>(uint32a, 60, 0));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x67665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_and, bool>( int32a, 60, 0));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_or,  bool>( int32a, 60, 0));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_xor, bool>( int32a, 60, 0));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<op::bit_and, bool>(uint32a, 60, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9988} == dassign<op::bit_or,  bool>(uint32a, 60, 1));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x67665544, 0xbbaa9988} == dassign<op::bit_xor, bool>(uint32a, 60, 1));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_and, bool>( int32a, 60, 1));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x77665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_or,  bool>( int32a, 60, 1));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x67665544), static_cast<int32_t>(0xbbaa9988)} == dassign<op::bit_xor, bool>( int32a, 60, 1));

static_assert(expect<uint64_t>(uint64a, 0x7766554433221100, 0xffeeddccbbaa9988,  0,  0));
static_assert(expect< int64_t>(uint64a, 0x7766554433221100, 0xffeeddccbbaa9988,  0,  0));
static_assert(expect<uint64_t>( int64a, 0x7766554433221100, 0xffeeddccbbaa9988, -1, -1));
static_assert(expect< int64_t>( int64a, 0x7766554433221100, 0xffeeddccbbaa9988, -1, -1));
static_assert(expect<uint64_t>(uint32a, 0x7766554433221100, 0x00000000bbaa9988,  0,  0));
static_assert(expect< int64_t>(uint32a, 0x7766554433221100, 0x00000000bbaa9988,  0,  0));
static_assert(expect<uint64_t>( int32a, 0x7766554433221100, 0xffffffffbbaa9988, -1, -1));
static_assert(expect< int64_t>( int32a, 0x7766554433221100, 0xffffffffbbaa9988, -1, -1));
static_assert(expect<uint64_t>(uint16a, 0x7766554433221100, 0x0000ddccbbaa9988,  0,  0));
static_assert(expect< int64_t>(uint16a, 0x7766554433221100, 0x0000ddccbbaa9988,  0,  0));
static_assert(expect<uint64_t>( int16a, 0x7766554433221100, 0xffffddccbbaa9988, -1, -1));
static_assert(expect< int64_t>( int16a, 0x7766554433221100, 0xffffddccbbaa9988, -1, -1));
static_assert(expect<uint64_t>( uint8a, 0x7766554433221100, 0x00eeddccbbaa9988,  0,  0));
static_assert(expect< int64_t>( uint8a, 0x7766554433221100, 0x00eeddccbbaa9988,  0,  0));
static_assert(expect<uint64_t>(  int8a, 0x7766554433221100, 0xffeeddccbbaa9988, -1, -1));
static_assert(expect< int64_t>(  int8a, 0x7766554433221100, 0xffeeddccbbaa9988, -1, -1));

static_assert(expect<uint32_t>(uint64a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc,  0,  0));
static_assert(expect< int32_t>(uint64a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc,  0,  0));
static_assert(expect<uint32_t>( int64a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc, -1, -1));
static_assert(expect< int32_t>( int64a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc, -1, -1));
static_assert(expect<uint32_t>(uint32a, 0x33221100, 0x77665544, 0xbbaa9988, 0x00000000,  0,  0));
static_assert(expect< int32_t>(uint32a, 0x33221100, 0x77665544, 0xbbaa9988, 0x00000000,  0,  0));
static_assert(expect<uint32_t>( int32a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffffffff, -1, -1));
static_assert(expect< int32_t>( int32a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffffffff, -1, -1));
static_assert(expect<uint32_t>(uint16a, 0x33221100, 0x77665544, 0xbbaa9988, 0x0000ddcc,  0,  0));
static_assert(expect< int32_t>(uint16a, 0x33221100, 0x77665544, 0xbbaa9988, 0x0000ddcc,  0,  0));
static_assert(expect<uint32_t>( int16a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffffddcc, -1, -1));
static_assert(expect< int32_t>( int16a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffffddcc, -1, -1));
static_assert(expect<uint32_t>( uint8a, 0x33221100, 0x77665544, 0xbbaa9988, 0x00eeddcc,  0,  0));
static_assert(expect< int32_t>( uint8a, 0x33221100, 0x77665544, 0xbbaa9988, 0x00eeddcc,  0,  0));
static_assert(expect<uint32_t>(  int8a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc, -1, -1));
static_assert(expect< int32_t>(  int8a, 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc, -1, -1));

static_assert(expect<uint8_t>(uint64a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,  0,  0));
static_assert(expect< int8_t>(uint64a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,  0,  0));
static_assert(expect<uint8_t>( int64a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, -1, -1));
static_assert(expect< int8_t>( int64a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, -1, -1));
static_assert(expect<uint8_t>(uint32a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0x00, 0x00, 0x00, 0x00,  0,  0));
static_assert(expect< int8_t>(uint32a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0x00, 0x00, 0x00, 0x00,  0,  0));
static_assert(expect<uint8_t>( int32a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xff, 0xff, 0xff, 0xff, -1, -1));
static_assert(expect< int8_t>( int32a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xff, 0xff, 0xff, 0xff, -1, -1));
static_assert(expect<uint8_t>(uint16a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0x00, 0x00,  0,  0));
static_assert(expect< int8_t>(uint16a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0x00, 0x00,  0,  0));
static_assert(expect<uint8_t>( int16a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff, 0xff, -1, -1));
static_assert(expect< int8_t>( int16a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff, 0xff, -1, -1));
static_assert(expect<uint8_t>( uint8a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x00,  0,  0));
static_assert(expect< int8_t>( uint8a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x00,  0,  0));
static_assert(expect<uint8_t>(  int8a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, -1, -1));
static_assert(expect< int8_t>(  int8a, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                       0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, -1, -1));

static_assert(assign<uint64_t>(uint64a, 0xaffeaffeaffeaffe));
static_assert(assign<uint64_t>( int64a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>(uint64a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>( int64a, 0xaffeaffeaffeaffe));

static_assert(assign<uint32_t>(uint64a, 0xaffeaffe));
static_assert(assign<uint32_t>( int64a, 0xaffeaffe));
static_assert(assign< int32_t>(uint64a, 0xaffeaffe));
static_assert(assign< int32_t>( int64a, 0xaffeaffe));

static_assert(assign<uint16_t>(uint64a, 0xaffe));
static_assert(assign<uint16_t>( int64a, 0xaffe));
static_assert(assign< int16_t>(uint64a, 0xaffe));
static_assert(assign< int16_t>( int64a, 0xaffe));

static_assert(assign< uint8_t>(uint64a, 0xae));
static_assert(assign< uint8_t>( int64a, 0xae));
static_assert(assign<  int8_t>(uint64a, 0xae));
static_assert(assign<  int8_t>( int64a, 0xae));

static_assert(assign<uint64_t>(uint32a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>(uint32a, 0xaffeaffeaffeaffe));
static_assert(assign<uint64_t>( int32a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>( int32a, 0xaffeaffeaffeaffe));

static_assert(assign<uint32_t>(uint32a, 0xaffeaffe));
static_assert(assign<uint32_t>( int32a, 0xaffeaffe));
static_assert(assign< int32_t>(uint32a, 0xaffeaffe));
static_assert(assign< int32_t>( int32a, 0xaffeaffe));

static_assert(assign<uint16_t>(uint32a, 0xaffe));
static_assert(assign<uint16_t>( int32a, 0xaffe));
static_assert(assign< int16_t>(uint32a, 0xaffe));
static_assert(assign< int16_t>( int32a, 0xaffe));

static_assert(assign< uint8_t>(uint32a, 0xae));
static_assert(assign< uint8_t>( int32a, 0xae));
static_assert(assign<  int8_t>(uint32a, 0xae));
static_assert(assign<  int8_t>( int32a, 0xae));

static_assert(assign<uint64_t>(uint16a, 0xaffeaffeaffeaffe));
static_assert(assign<uint64_t>( int16a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>(uint16a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>( int16a, 0xaffeaffeaffeaffe));

static_assert(assign<uint32_t>(uint16a, 0xaffeaffe));
static_assert(assign<uint32_t>( int16a, 0xaffeaffe));
static_assert(assign< int32_t>(uint16a, 0xaffeaffe));
static_assert(assign< int32_t>( int16a, 0xaffeaffe));

static_assert(assign<uint16_t>(uint16a, 0xaffe));
static_assert(assign<uint16_t>( int16a, 0xaffe));
static_assert(assign< int16_t>(uint16a, 0xaffe));
static_assert(assign< int16_t>( int16a, 0xaffe));

static_assert(assign< uint8_t>(uint16a, 0xae));
static_assert(assign< uint8_t>( int16a, 0xae));
static_assert(assign<  int8_t>(uint16a, 0xae));
static_assert(assign<  int8_t>( int16a, 0xae));

static_assert(assign<uint64_t>( uint8a, 0xaffeaffeaffeaffe));
static_assert(assign<uint64_t>(  int8a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>( uint8a, 0xaffeaffeaffeaffe));
static_assert(assign< int64_t>(  int8a, 0xaffeaffeaffeaffe));

static_assert(assign<uint32_t>( uint8a, 0xaffeaffe));
static_assert(assign<uint32_t>(  int8a, 0xaffeaffe));
static_assert(assign< int32_t>( uint8a, 0xaffeaffe));
static_assert(assign< int32_t>(  int8a, 0xaffeaffe));

static_assert(assign<uint16_t>( uint8a, 0xaffe));
static_assert(assign<uint16_t>(  int8a, 0xaffe));
static_assert(assign< int16_t>( uint8a, 0xaffe));
static_assert(assign< int16_t>(  int8a, 0xaffe));

static_assert(assign< uint8_t>( uint8a, 0xae));
static_assert(assign< uint8_t>(  int8a, 0xae));
static_assert(assign<  int8_t>( uint8a, 0xae));
static_assert(assign<  int8_t>(  int8a, 0xae));

static_assert(std::array<uint64_t, 2>{0x5544554433221100, 0xffeeddccbbaa9988} == bassign<op::bit_and, uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array<uint64_t, 2>{0x7777555533221100, 0xffeeddccbbaa9988} == bassign<op::bit_or,  uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array<uint64_t, 2>{0x2233001133221100, 0xffeeddccbbaa9988} == bassign<op::bit_xor, uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x5544554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_and, uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7777555533221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_or,  uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x2233001133221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_xor, uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array<uint64_t, 2>{0x5544554433221100, 0xffeeddccbbaa9988} == bassign<op::bit_and,  int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array<uint64_t, 2>{0x7777555533221100, 0xffeeddccbbaa9988} == bassign<op::bit_or,   int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array<uint64_t, 2>{0x2233001133221100, 0xffeeddccbbaa9988} == bassign<op::bit_xor,  int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x5544554433221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_and,  int64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7777555533221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_or,   int64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x2233001133221100), static_cast<int64_t>(0xffeeddccbbaa9988)} == bassign<op::bit_xor,  int64_t>( int64a, 1, 0x5555555555555555));

static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xff4eddccbbaa9988} == bassign<op::bit_and, uint8_t>(uint64a, 29, 0x55));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xfffeddccbbaa9988} == bassign<op::bit_or,  uint8_t>(uint64a, 29, 0x55));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffbeddccbbaa9988} == bassign<op::bit_xor, uint8_t>(uint64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xff4eddccbbaa9988)} == bassign<op::bit_and, uint8_t>( int64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xfffeddccbbaa9988)} == bassign<op::bit_or,  uint8_t>( int64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffbeddccbbaa9988)} == bassign<op::bit_xor, uint8_t>( int64a, 29, 0x55));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xff4eddccbbaa9988} == bassign<op::bit_and,  int8_t>(uint64a, 29, 0x55));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xfffeddccbbaa9988} == bassign<op::bit_or,   int8_t>(uint64a, 29, 0x55));
static_assert(std::array<uint64_t, 2>{0x7766554433221100, 0xffbeddccbbaa9988} == bassign<op::bit_xor,  int8_t>(uint64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xff4eddccbbaa9988)} == bassign<op::bit_and,  int8_t>( int64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xfffeddccbbaa9988)} == bassign<op::bit_or,   int8_t>( int64a, 29, 0x55));
static_assert(std::array< int64_t, 2>{static_cast<int64_t>(0x7766554433221100), static_cast<int64_t>(0xffbeddccbbaa9988)} == bassign<op::bit_xor,  int8_t>( int64a, 29, 0x55));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x11001100, 0xbbaa9988} == bassign<op::bit_and, uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xffffdddd, 0xbbaa9988} == bassign<op::bit_or,  uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xeeffccdd, 0xbbaa9988} == bassign<op::bit_xor, uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x11001100), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_and, uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xffffdddd), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_or,  uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xeeffccdd), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_xor, uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x11001100, 0xbbaa9988} == bassign<op::bit_and,  int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xffffdddd, 0xbbaa9988} == bassign<op::bit_or,   int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array<uint32_t, 3>{0x33221100, 0xeeffccdd, 0xbbaa9988} == bassign<op::bit_xor,  int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0x11001100), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_and,  int64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xffffdddd), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_or,   int64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array< int32_t, 3>{0x33221100, static_cast<int32_t>(0xeeffccdd), static_cast<int32_t>(0xbbaa9988)} == bassign<op::bit_xor,  int64_t>( int32a, 1, 0x5555555599999999));

static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9980} == bassign<op::bit_and, uint8_t>(uint32a, 16, 0x55));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa998d} == bassign<op::bit_or,  uint8_t>(uint32a, 16, 0x55));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa998d} == bassign<op::bit_xor, uint8_t>(uint32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa9980)} == bassign<op::bit_and, uint8_t>( int32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa998d)} == bassign<op::bit_or,  uint8_t>( int32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa998d)} == bassign<op::bit_xor, uint8_t>( int32a, 16, 0x55));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa9980} == bassign<op::bit_and,  int8_t>(uint32a, 16, 0x55));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa998d} == bassign<op::bit_or,   int8_t>(uint32a, 16, 0x55));
static_assert(std::array<uint32_t, 3>{0x33221100, 0x77665544, 0xbbaa998d} == bassign<op::bit_xor,  int8_t>(uint32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa9980)} == bassign<op::bit_and,  int8_t>( int32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa998d)} == bassign<op::bit_or,   int8_t>( int32a, 16, 0x55));
static_assert(std::array< int32_t, 3>{0x33221100, 0x77665544, static_cast<int32_t>(0xbbaa998d)} == bassign<op::bit_xor,  int8_t>( int32a, 16, 0x55));

} /* namespace */
