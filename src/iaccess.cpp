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

constexpr   int8_t operator  ""_sint8(unsigned long long v) noexcept { return v; }
constexpr  uint8_t operator  ""_uint8(unsigned long long v) noexcept { return v; }
constexpr  int32_t operator ""_sint32(unsigned long long v) noexcept { return v; }
constexpr uint32_t operator ""_uint32(unsigned long long v) noexcept { return v; }
constexpr  int64_t operator ""_sint64(unsigned long long v) noexcept { return v; }
constexpr uint64_t operator ""_uint64(unsigned long long v) noexcept { return v; }

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

static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64)[- 60]);
static_assert(0x1000000000000000_uint64 == digits_as<uint64_t>(uint64)[- 56]);
static_assert(0xedcba98765432100_uint64 == digits_as<uint64_t>(uint64)[-  4]);
static_assert(0xfedcba9876543210_uint64 == digits_as<uint64_t>(uint64)[   0]);
static_assert(0x0fedcba987654321_uint64 == digits_as<uint64_t>(uint64)[   4]);
static_assert(0x00000000000000fe_uint64 == digits_as<uint64_t>(uint64)[  56]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64)[ 100]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64)[- 64]);
static_assert(0xf000000000000000_uint64 == digits_as<uint64_t>( int64)[- 60]);
static_assert(0xef00000000000000_uint64 == digits_as<uint64_t>( int64)[- 56]);
static_assert(0x123456789abcdef0_uint64 == digits_as<uint64_t>( int64)[-  4]);
static_assert(0x0123456789abcdef_uint64 == digits_as<uint64_t>( int64)[   0]);
static_assert(0x00123456789abcde_uint64 == digits_as<uint64_t>( int64)[   4]);
static_assert(0x0000000000000001_uint64 == digits_as<uint64_t>( int64)[  56]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64)[- 60]);
static_assert(0x1000000000000000_sint64 == digits_as< int64_t>(uint64)[- 56]);
static_assert(0xedcba98765432100_sint64 == digits_as< int64_t>(uint64)[-  4]);
static_assert(0xfedcba9876543210_sint64 == digits_as< int64_t>(uint64)[   0]);
static_assert(0x0fedcba987654321_sint64 == digits_as< int64_t>(uint64)[   4]);
static_assert(0x00000000000000fe_sint64 == digits_as< int64_t>(uint64)[  56]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64)[- 64]);
static_assert(0xf000000000000000_sint64 == digits_as< int64_t>( int64)[- 60]);
static_assert(0xef00000000000000_sint64 == digits_as< int64_t>( int64)[- 56]);
static_assert(0x123456789abcdef0_sint64 == digits_as< int64_t>( int64)[-  4]);
static_assert(0x0123456789abcdef_sint64 == digits_as< int64_t>( int64)[   0]);
static_assert(0x00123456789abcde_sint64 == digits_as< int64_t>( int64)[   4]);
static_assert(0x0000000000000001_sint64 == digits_as< int64_t>( int64)[  56]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64)[ 100]);

static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[-  4]);
static_assert(0x10_uint8 == digits_as<uint8_t>(uint64)[   0]);
static_assert(0x21_uint8 == digits_as<uint8_t>(uint64)[   4]);
static_assert(0xfe_uint8 == digits_as<uint8_t>(uint64)[  56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64)[ 100]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64)[- 56]);
static_assert(0xf0_uint8 == digits_as<uint8_t>( int64)[-  4]);
static_assert(0xef_uint8 == digits_as<uint8_t>( int64)[   0]);
static_assert(0xde_uint8 == digits_as<uint8_t>( int64)[   4]);
static_assert(0x01_uint8 == digits_as<uint8_t>( int64)[  56]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[-  4]);
static_assert(0x10_sint8 == digits_as< int8_t>(uint64)[   0]);
static_assert(0x21_sint8 == digits_as< int8_t>(uint64)[   4]);
static_assert(0xfe_sint8 == digits_as< int8_t>(uint64)[  56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64)[- 56]);
static_assert(0xf0_sint8 == digits_as< int8_t>( int64)[-  4]);
static_assert(0xef_sint8 == digits_as< int8_t>( int64)[   0]);
static_assert(0xde_sint8 == digits_as< int8_t>( int64)[   4]);
static_assert(0x01_sint8 == digits_as< int8_t>( int64)[  56]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64)[ 100]);

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

static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32)[- 60]);
static_assert(0x1000000000000000_uint64 == digits_as<uint64_t>(uint32)[- 56]);
static_assert(0x0000000765432100_uint64 == digits_as<uint64_t>(uint32)[-  4]);
static_assert(0x0000000076543210_uint64 == digits_as<uint64_t>(uint32)[   0]);
static_assert(0x0000000007654321_uint64 == digits_as<uint64_t>(uint32)[   4]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32)[  56]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32)[ 100]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32)[- 64]);
static_assert(0xf000000000000000_uint64 == digits_as<uint64_t>( int32)[- 60]);
static_assert(0xef00000000000000_uint64 == digits_as<uint64_t>( int32)[- 56]);
static_assert(0xfffffff89abcdef0_uint64 == digits_as<uint64_t>( int32)[-  4]);
static_assert(0xffffffff89abcdef_uint64 == digits_as<uint64_t>( int32)[   0]);
static_assert(0xfffffffff89abcde_uint64 == digits_as<uint64_t>( int32)[   4]);
static_assert(0xffffffffffffffff_uint64 == digits_as<uint64_t>( int32)[  56]);
static_assert(0xffffffffffffffff_uint64 == digits_as<uint64_t>( int32)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32)[- 60]);
static_assert(0x1000000000000000_sint64 == digits_as< int64_t>(uint32)[- 56]);
static_assert(0x0000000765432100_sint64 == digits_as< int64_t>(uint32)[-  4]);
static_assert(0x0000000076543210_sint64 == digits_as< int64_t>(uint32)[   0]);
static_assert(0x0000000007654321_sint64 == digits_as< int64_t>(uint32)[   4]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32)[  56]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32)[- 64]);
static_assert(0xf000000000000000_sint64 == digits_as< int64_t>( int32)[- 60]);
static_assert(0xef00000000000000_sint64 == digits_as< int64_t>( int32)[- 56]);
static_assert(0xfffffff89abcdef0_sint64 == digits_as< int64_t>( int32)[-  4]);
static_assert(0xffffffff89abcdef_sint64 == digits_as< int64_t>( int32)[   0]);
static_assert(0xfffffffff89abcde_sint64 == digits_as< int64_t>( int32)[   4]);
static_assert(0xffffffffffffffff_sint64 == digits_as< int64_t>( int32)[  56]);
static_assert(0xffffffffffffffff_sint64 == digits_as< int64_t>( int32)[ 100]);

static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[-  4]);
static_assert(0x10_uint8 == digits_as<uint8_t>(uint32)[   0]);
static_assert(0x21_uint8 == digits_as<uint8_t>(uint32)[   4]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[  56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32)[ 100]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32)[- 56]);
static_assert(0xf0_uint8 == digits_as<uint8_t>( int32)[-  4]);
static_assert(0xef_uint8 == digits_as<uint8_t>( int32)[   0]);
static_assert(0xde_uint8 == digits_as<uint8_t>( int32)[   4]);
static_assert(0xff_uint8 == digits_as<uint8_t>( int32)[  56]);
static_assert(0xff_uint8 == digits_as<uint8_t>( int32)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[-  4]);
static_assert(0x10_sint8 == digits_as< int8_t>(uint32)[   0]);
static_assert(0x21_sint8 == digits_as< int8_t>(uint32)[   4]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[  56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32)[- 56]);
static_assert(0xf0_sint8 == digits_as< int8_t>( int32)[-  4]);
static_assert(0xef_sint8 == digits_as< int8_t>( int32)[   0]);
static_assert(0xde_sint8 == digits_as< int8_t>( int32)[   4]);
static_assert(0xff_sint8 == digits_as< int8_t>( int32)[  56]);
static_assert(0xff_sint8 == digits_as< int8_t>( int32)[ 100]);

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

static_assert(0xfedcba9876543210_uint64 == dassign<uint64_t>(uint64, -200, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint64_t>(uint64, - 64, 0xcffffffffffffff5));
static_assert(0xfedcba987654321c_uint64 == dassign<uint64_t>(uint64, - 60, 0xcffffffffffffff5));
static_assert(0xfedcba98765432cf_uint64 == dassign<uint64_t>(uint64, - 56, 0xcffffffffffffff5));
static_assert(0xfcffffffffffffff_uint64 == dassign<uint64_t>(uint64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5_uint64 == dassign<uint64_t>(uint64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff50_uint64 == dassign<uint64_t>(uint64,    4, 0xcffffffffffffff5));
static_assert(0xf5dcba9876543210_uint64 == dassign<uint64_t>(uint64,   56, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint64_t>(uint64,  100, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint64_t>( int64, -200, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint64_t>( int64, - 64, 0xcffffffffffffff5));
static_assert(0x0123456789abcdec_sint64 == dassign<uint64_t>( int64, - 60, 0xcffffffffffffff5));
static_assert(0x0123456789abcdcf_sint64 == dassign<uint64_t>( int64, - 56, 0xcffffffffffffff5));
static_assert(0x0cffffffffffffff_sint64 == dassign<uint64_t>( int64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5_sint64 == dassign<uint64_t>( int64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff5f_sint64 == dassign<uint64_t>( int64,    4, 0xcffffffffffffff5));
static_assert(0xf523456789abcdef_sint64 == dassign<uint64_t>( int64,   56, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint64_t>( int64,  100, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210_uint64 == dassign< int64_t>(uint64, -200, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210_uint64 == dassign< int64_t>(uint64, - 64, 0xcffffffffffffff5));
static_assert(0xfedcba987654321c_uint64 == dassign< int64_t>(uint64, - 60, 0xcffffffffffffff5));
static_assert(0xfedcba98765432cf_uint64 == dassign< int64_t>(uint64, - 56, 0xcffffffffffffff5));
static_assert(0xfcffffffffffffff_uint64 == dassign< int64_t>(uint64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5_uint64 == dassign< int64_t>(uint64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff50_uint64 == dassign< int64_t>(uint64,    4, 0xcffffffffffffff5));
static_assert(0xf5dcba9876543210_uint64 == dassign< int64_t>(uint64,   56, 0xcffffffffffffff5));
static_assert(0xfedcba9876543210_uint64 == dassign< int64_t>(uint64,  100, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign< int64_t>( int64, -200, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign< int64_t>( int64, - 64, 0xcffffffffffffff5));
static_assert(0x0123456789abcdec_sint64 == dassign< int64_t>( int64, - 60, 0xcffffffffffffff5));
static_assert(0x0123456789abcdcf_sint64 == dassign< int64_t>( int64, - 56, 0xcffffffffffffff5));
static_assert(0x0cffffffffffffff_sint64 == dassign< int64_t>( int64, -  4, 0xcffffffffffffff5));
static_assert(0xcffffffffffffff5_sint64 == dassign< int64_t>( int64,    0, 0xcffffffffffffff5));
static_assert(0xffffffffffffff5f_sint64 == dassign< int64_t>( int64,    4, 0xcffffffffffffff5));
static_assert(0xf523456789abcdef_sint64 == dassign< int64_t>( int64,   56, 0xcffffffffffffff5));
static_assert(0x0123456789abcdef_sint64 == dassign< int64_t>( int64,  100, 0xcffffffffffffff5));

static_assert(0xfedcba9876543210_uint64 == dassign<uint8_t>(uint64, -200, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint8_t>(uint64, - 64, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint8_t>(uint64, - 60, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint8_t>(uint64, - 56, 0xc5));
static_assert(0xfedcba987654321c_uint64 == dassign<uint8_t>(uint64, -  4, 0xc5));
static_assert(0xfedcba98765432c5_uint64 == dassign<uint8_t>(uint64,    0, 0xc5));
static_assert(0xfedcba9876543c50_uint64 == dassign<uint8_t>(uint64,    4, 0xc5));
static_assert(0xc5dcba9876543210_uint64 == dassign<uint8_t>(uint64,   56, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign<uint8_t>(uint64,  100, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint8_t>( int64, -200, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint8_t>( int64, - 64, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint8_t>( int64, - 60, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint8_t>( int64, - 56, 0xc5));
static_assert(0x0123456789abcdec_sint64 == dassign<uint8_t>( int64, -  4, 0xc5));
static_assert(0x0123456789abcdc5_sint64 == dassign<uint8_t>( int64,    0, 0xc5));
static_assert(0x0123456789abcc5f_sint64 == dassign<uint8_t>( int64,    4, 0xc5));
static_assert(0xc523456789abcdef_sint64 == dassign<uint8_t>( int64,   56, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign<uint8_t>( int64,  100, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign< int8_t>(uint64, -200, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign< int8_t>(uint64, - 64, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign< int8_t>(uint64, - 60, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign< int8_t>(uint64, - 56, 0xc5));
static_assert(0xfedcba987654321c_uint64 == dassign< int8_t>(uint64, -  4, 0xc5));
static_assert(0xfedcba98765432c5_uint64 == dassign< int8_t>(uint64,    0, 0xc5));
static_assert(0xfedcba9876543c50_uint64 == dassign< int8_t>(uint64,    4, 0xc5));
static_assert(0xc5dcba9876543210_uint64 == dassign< int8_t>(uint64,   56, 0xc5));
static_assert(0xfedcba9876543210_uint64 == dassign< int8_t>(uint64,  100, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign< int8_t>( int64, -200, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign< int8_t>( int64, - 64, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign< int8_t>( int64, - 60, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign< int8_t>( int64, - 56, 0xc5));
static_assert(0x0123456789abcdec_sint64 == dassign< int8_t>( int64, -  4, 0xc5));
static_assert(0x0123456789abcdc5_sint64 == dassign< int8_t>( int64,    0, 0xc5));
static_assert(0x0123456789abcc5f_sint64 == dassign< int8_t>( int64,    4, 0xc5));
static_assert(0xc523456789abcdef_sint64 == dassign< int8_t>( int64,   56, 0xc5));
static_assert(0x0123456789abcdef_sint64 == dassign< int8_t>( int64,  100, 0xc5));

static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64, -200, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64, - 64, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64, - 60, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64, - 56, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64, -  4, 1));
static_assert(0xfedcba9876543211_uint64 == dassign<bool>(uint64,    0, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64,    4, 1));
static_assert(0xffdcba9876543210_uint64 == dassign<bool>(uint64,   56, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64,    0, 0));
static_assert(0xfedcba9876543200_uint64 == dassign<bool>(uint64,    4, 0));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64,   56, 0));
static_assert(0xfedcba9876543210_uint64 == dassign<bool>(uint64,  100, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64, -200, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64, - 64, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64, - 60, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64, - 56, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64, -  4, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64,    0, 1));
static_assert(0x0123456789abcdff_sint64 == dassign<bool>( int64,    4, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64,   56, 1));
static_assert(0x0123456789abcdee_sint64 == dassign<bool>( int64,    0, 0));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64,    4, 0));
static_assert(0x0023456789abcdef_sint64 == dassign<bool>( int64,   56, 0));
static_assert(0x0123456789abcdef_sint64 == dassign<bool>( int64,  100, 1));

static_assert(0x76543210_uint32 == dassign<uint64_t>(uint32, -200, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign<uint64_t>(uint32, - 64, 0xcffffffd6ffffff5));
static_assert(0x7654321c_uint32 == dassign<uint64_t>(uint32, - 60, 0xcffffffd6ffffff5));
static_assert(0x765432cf_uint32 == dassign<uint64_t>(uint32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff_uint32 == dassign<uint64_t>(uint32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5_uint32 == dassign<uint64_t>(uint32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff50_uint32 == dassign<uint64_t>(uint32,    4, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign<uint64_t>(uint32,   56, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign<uint64_t>(uint32,  100, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign<uint64_t>( int32, -200, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign<uint64_t>( int32, - 64, 0xcffffffd6ffffff5));
static_assert(0x89abcdec_sint32 == dassign<uint64_t>( int32, - 60, 0xcffffffd6ffffff5));
static_assert(0x89abcdcf_sint32 == dassign<uint64_t>( int32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff_sint32 == dassign<uint64_t>( int32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5_sint32 == dassign<uint64_t>( int32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff5f_sint32 == dassign<uint64_t>( int32,    4, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign<uint64_t>( int32,   56, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign<uint64_t>( int32,  100, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign< int64_t>(uint32, -200, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign< int64_t>(uint32, - 64, 0xcffffffd6ffffff5));
static_assert(0x7654321c_uint32 == dassign< int64_t>(uint32, - 60, 0xcffffffd6ffffff5));
static_assert(0x765432cf_uint32 == dassign< int64_t>(uint32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff_uint32 == dassign< int64_t>(uint32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5_uint32 == dassign< int64_t>(uint32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff50_uint32 == dassign< int64_t>(uint32,    4, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign< int64_t>(uint32,   56, 0xcffffffd6ffffff5));
static_assert(0x76543210_uint32 == dassign< int64_t>(uint32,  100, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign< int64_t>( int32, -200, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign< int64_t>( int32, - 64, 0xcffffffd6ffffff5));
static_assert(0x89abcdec_sint32 == dassign< int64_t>( int32, - 60, 0xcffffffd6ffffff5));
static_assert(0x89abcdcf_sint32 == dassign< int64_t>( int32, - 56, 0xcffffffd6ffffff5));
static_assert(0xd6ffffff_sint32 == dassign< int64_t>( int32, -  4, 0xcffffffd6ffffff5));
static_assert(0x6ffffff5_sint32 == dassign< int64_t>( int32,    0, 0xcffffffd6ffffff5));
static_assert(0xffffff5f_sint32 == dassign< int64_t>( int32,    4, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign< int64_t>( int32,   56, 0xcffffffd6ffffff5));
static_assert(0x89abcdef_sint32 == dassign< int64_t>( int32,  100, 0xcffffffd6ffffff5));

static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32, -200, 0xc5));
static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32, - 64, 0xc5));
static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32, - 60, 0xc5));
static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32, - 56, 0xc5));
static_assert(0x7654321c_uint32 == dassign<uint8_t>(uint32, -  4, 0xc5));
static_assert(0x765432c5_uint32 == dassign<uint8_t>(uint32,    0, 0xc5));
static_assert(0x76543c50_uint32 == dassign<uint8_t>(uint32,    4, 0xc5));
static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32,   56, 0xc5));
static_assert(0x76543210_uint32 == dassign<uint8_t>(uint32,  100, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32, -200, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32, - 64, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32, - 60, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32, - 56, 0xc5));
static_assert(0x89abcdec_sint32 == dassign<uint8_t>( int32, -  4, 0xc5));
static_assert(0x89abcdc5_sint32 == dassign<uint8_t>( int32,    0, 0xc5));
static_assert(0x89abcc5f_sint32 == dassign<uint8_t>( int32,    4, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32,   56, 0xc5));
static_assert(0x89abcdef_sint32 == dassign<uint8_t>( int32,  100, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32, -200, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32, - 64, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32, - 60, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32, - 56, 0xc5));
static_assert(0x7654321c_uint32 == dassign< int8_t>(uint32, -  4, 0xc5));
static_assert(0x765432c5_uint32 == dassign< int8_t>(uint32,    0, 0xc5));
static_assert(0x76543c50_uint32 == dassign< int8_t>(uint32,    4, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32,   56, 0xc5));
static_assert(0x76543210_uint32 == dassign< int8_t>(uint32,  100, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32, -200, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32, - 64, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32, - 60, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32, - 56, 0xc5));
static_assert(0x89abcdec_sint32 == dassign< int8_t>( int32, -  4, 0xc5));
static_assert(0x89abcdc5_sint32 == dassign< int8_t>( int32,    0, 0xc5));
static_assert(0x89abcc5f_sint32 == dassign< int8_t>( int32,    4, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32,   56, 0xc5));
static_assert(0x89abcdef_sint32 == dassign< int8_t>( int32,  100, 0xc5));

static_assert(0x76543210_uint32 == dassign<bool>(uint32, - 64, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32, - 60, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32, - 56, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32, -  4, 1));
static_assert(0x76543211_uint32 == dassign<bool>(uint32,    0, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32,    4, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32,    0, 0));
static_assert(0x76543200_uint32 == dassign<bool>(uint32,    4, 0));
static_assert(0x76543210_uint32 == dassign<bool>(uint32,   56, 1));
static_assert(0x76543210_uint32 == dassign<bool>(uint32,  100, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32, -200, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32, - 64, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32, - 60, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32, - 56, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32, -  4, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32,    0, 1));
static_assert(0x89abcdff_sint32 == dassign<bool>( int32,    4, 1));
static_assert(0x89abcdee_sint32 == dassign<bool>( int32,    0, 0));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32,    4, 0));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32,   56, 1));
static_assert(0x89abcdef_sint32 == dassign<bool>( int32,  100, 1));

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

static_assert(0x1010101010543210_uint64 == dassign<op::bit_and, uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xffddbb9977543210_uint64 == dassign<op::bit_or , uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xefcdab8967543210_uint64 == dassign<op::bit_xor, uint64_t>(uint64, 24, 0x1111111111111111));
static_assert(0x0101010101abcdef_sint64 == dassign<op::bit_and, uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1133557799abcdef_sint64 == dassign<op::bit_or , uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1032547698abcdef_sint64 == dassign<op::bit_xor, uint64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1010101010543210_uint64 == dassign<op::bit_and,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xffddbb9977543210_uint64 == dassign<op::bit_or ,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0xefcdab8967543210_uint64 == dassign<op::bit_xor,  int64_t>(uint64, 24, 0x1111111111111111));
static_assert(0x0101010101abcdef_sint64 == dassign<op::bit_and,  int64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1133557799abcdef_sint64 == dassign<op::bit_or ,  int64_t>( int64, 24, 0x1111111111111111));
static_assert(0x1032547698abcdef_sint64 == dassign<op::bit_xor,  int64_t>( int64, 24, 0x1111111111111111));

static_assert(0xfedcba9876501210_uint64 == dassign<op::bit_and,  uint8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876553210_uint64 == dassign<op::bit_or ,  uint8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876552210_uint64 == dassign<op::bit_xor,  uint8_t>(uint64, 12, 0x11));
static_assert(0x0123456789a10def_sint64 == dassign<op::bit_and,  uint8_t>( int64, 12, 0x11));
static_assert(0x0123456789abddef_sint64 == dassign<op::bit_or ,  uint8_t>( int64, 12, 0x11));
static_assert(0x0123456789aaddef_sint64 == dassign<op::bit_xor,  uint8_t>( int64, 12, 0x11));
static_assert(0xfedcba9876501210_uint64 == dassign<op::bit_and,   int8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876553210_uint64 == dassign<op::bit_or ,   int8_t>(uint64, 12, 0x11));
static_assert(0xfedcba9876552210_uint64 == dassign<op::bit_xor,   int8_t>(uint64, 12, 0x11));
static_assert(0x0123456789a10def_sint64 == dassign<op::bit_and,   int8_t>( int64, 12, 0x11));
static_assert(0x0123456789abddef_sint64 == dassign<op::bit_or ,   int8_t>( int64, 12, 0x11));
static_assert(0x0123456789aaddef_sint64 == dassign<op::bit_xor,   int8_t>( int64, 12, 0x11));

static_assert(0xfedcba9876542210_uint64 == dassign<op::bit_and, bool>(uint64, 12, 0));
static_assert(0xfedcba9876543210_uint64 == dassign<op::bit_or , bool>(uint64, 12, 0));
static_assert(0xfedcba9876543210_uint64 == dassign<op::bit_xor, bool>(uint64, 12, 0));
static_assert(0x0123456789abcdef_sint64 == dassign<op::bit_and, bool>( int64, 12, 0));
static_assert(0x0123456789abcdef_sint64 == dassign<op::bit_or , bool>( int64, 12, 0));
static_assert(0x0123456789abcdef_sint64 == dassign<op::bit_xor, bool>( int64, 12, 0));
static_assert(0xfedcba9876543210_uint64 == dassign<op::bit_and, bool>(uint64, 12, 1));
static_assert(0xfedcba9876543210_uint64 == dassign<op::bit_or , bool>(uint64, 12, 1));
static_assert(0xfedcba9876542210_uint64 == dassign<op::bit_xor, bool>(uint64, 12, 1));
static_assert(0x0123456789abcdef_sint64 == dassign<op::bit_and, bool>( int64, 12, 1));
static_assert(0x0123456789abddef_sint64 == dassign<op::bit_or , bool>( int64, 12, 1));
static_assert(0x0123456789abddef_sint64 == dassign<op::bit_xor, bool>( int64, 12, 1));

static_assert(0x76542200_uint32 == dassign<op::bit_and, uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76543232_uint32 == dassign<op::bit_or,  uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76541032_uint32 == dassign<op::bit_xor, uint64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x89ab0022_sint32 == dassign<op::bit_and, uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefef_sint32 == dassign<op::bit_or,  uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefcd_sint32 == dassign<op::bit_xor, uint64_t>( int32, -48, 0x2222222222222222));
static_assert(0x76542200_uint32 == dassign<op::bit_and,  int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76543232_uint32 == dassign<op::bit_or,   int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x76541032_uint32 == dassign<op::bit_xor,  int64_t>(uint32, -48, 0x2222222222222222));
static_assert(0x89ab0022_sint32 == dassign<op::bit_and,  int64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefef_sint32 == dassign<op::bit_or,   int64_t>( int32, -48, 0x2222222222222222));
static_assert(0x89abefcd_sint32 == dassign<op::bit_xor,  int64_t>( int32, -48, 0x2222222222222222));

static_assert(0x16543210_uint32 == dassign<op::bit_and, uint8_t>(uint32, 28, 0x99));
static_assert(0xf6543210_uint32 == dassign<op::bit_or,  uint8_t>(uint32, 28, 0x99));
static_assert(0xe6543210_uint32 == dassign<op::bit_xor, uint8_t>(uint32, 28, 0x99));
static_assert(0x89abc98f_sint32 == dassign<op::bit_and, uint8_t>( int32,  4, 0x99));
static_assert(0x89abcdff_sint32 == dassign<op::bit_or,  uint8_t>( int32,  4, 0x99));
static_assert(0x89abc47f_sint32 == dassign<op::bit_xor, uint8_t>( int32,  4, 0x99));
static_assert(0x16543210_uint32 == dassign<op::bit_and,  int8_t>(uint32, 28, 0x99));
static_assert(0xf6543210_uint32 == dassign<op::bit_or,   int8_t>(uint32, 28, 0x99));
static_assert(0xe6543210_uint32 == dassign<op::bit_xor,  int8_t>(uint32, 28, 0x99));
static_assert(0x89abc98f_sint32 == dassign<op::bit_and,  int8_t>( int32,  4, 0x99));
static_assert(0x89abcdff_sint32 == dassign<op::bit_or,   int8_t>( int32,  4, 0x99));
static_assert(0x89abc47f_sint32 == dassign<op::bit_xor,  int8_t>( int32,  4, 0x99));

static_assert(0x66543210_uint32 == dassign<op::bit_and, bool>(uint32, 28, 0));
static_assert(0x76543210_uint32 == dassign<op::bit_or,  bool>(uint32, 28, 0));
static_assert(0x76543210_uint32 == dassign<op::bit_xor, bool>(uint32, 28, 0));
static_assert(0x89abcdef_sint32 == dassign<op::bit_and, bool>( int32,  4, 0));
static_assert(0x89abcdef_sint32 == dassign<op::bit_or,  bool>( int32,  4, 0));
static_assert(0x89abcdef_sint32 == dassign<op::bit_xor, bool>( int32,  4, 0));
static_assert(0x76543210_uint32 == dassign<op::bit_and, bool>(uint32, 28, 1));
static_assert(0x76543210_uint32 == dassign<op::bit_or,  bool>(uint32, 28, 1));
static_assert(0x66543210_uint32 == dassign<op::bit_xor, bool>(uint32, 28, 1));
static_assert(0x89abcdef_sint32 == dassign<op::bit_and, bool>( int32,  4, 1));
static_assert(0x89abcdff_sint32 == dassign<op::bit_or,  bool>( int32,  4, 1));
static_assert(0x89abcdff_sint32 == dassign<op::bit_xor, bool>( int32,  4, 1));

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

static_assert(0xfedcba9810101010_uint64 == bassign<op::bit_and, uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9877553311_uint64 == bassign<op::bit_or,  uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9867452301_uint64 == bassign<op::bit_xor, uint64_t>(uint64, 0, 0x1111111111111111));
static_assert(0x0123456701010101_sint64 == bassign<op::bit_and, uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456799bbddff_sint64 == bassign<op::bit_or , uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456798badcfe_sint64 == bassign<op::bit_xor, uint64_t>( int64, 0, 0x1111111111111111));
static_assert(0xfedcba9810101010_uint64 == bassign<op::bit_and,  int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9877553311_uint64 == bassign<op::bit_or,   int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0xfedcba9867452301_uint64 == bassign<op::bit_xor,  int64_t>(uint64, 0, 0x1111111111111111));
static_assert(0x0123456701010101_sint64 == bassign<op::bit_and,  int64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456799bbddff_sint64 == bassign<op::bit_or ,  int64_t>( int64, 0, 0x1111111111111111));
static_assert(0x0123456798badcfe_sint64 == bassign<op::bit_xor,  int64_t>( int64, 0, 0x1111111111111111));

static_assert(0xfedcba9876541210_uint64 == bassign<op::bit_and,  uint8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876543210_uint64 == bassign<op::bit_or ,  uint8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876542210_uint64 == bassign<op::bit_xor,  uint8_t>(uint64, 3, 0x11));
static_assert(0x0123456789ab0def_sint64 == bassign<op::bit_and,  uint8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef_sint64 == bassign<op::bit_or ,  uint8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef_sint64 == bassign<op::bit_xor,  uint8_t>( int64, 3, 0x11));
static_assert(0xfedcba9876541210_uint64 == bassign<op::bit_and,   int8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876543210_uint64 == bassign<op::bit_or ,   int8_t>(uint64, 3, 0x11));
static_assert(0xfedcba9876542210_uint64 == bassign<op::bit_xor,   int8_t>(uint64, 3, 0x11));
static_assert(0x0123456789ab0def_sint64 == bassign<op::bit_and,   int8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef_sint64 == bassign<op::bit_or ,   int8_t>( int64, 3, 0x11));
static_assert(0x0123456789abddef_sint64 == bassign<op::bit_xor,   int8_t>( int64, 3, 0x11));

static_assert(0x22002200_uint32 == bassign<op::bit_and, uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x76763232_uint32 == bassign<op::bit_or,  uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x54761032_uint32 == bassign<op::bit_xor, uint64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x00220022_sint32 == bassign<op::bit_and, uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0xababefef_sint32 == bassign<op::bit_or,  uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0xab89efcd_sint32 == bassign<op::bit_xor, uint64_t>( int32, 0, 0x2222222222222222));
static_assert(0x22002200_uint32 == bassign<op::bit_and,  int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x76763232_uint32 == bassign<op::bit_or,   int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x54761032_uint32 == bassign<op::bit_xor,  int64_t>(uint32, 0, 0x2222222222222222));
static_assert(0x00220022_sint32 == bassign<op::bit_and,  int64_t>( int32, 0, 0x2222222222222222));
static_assert(0xababefef_sint32 == bassign<op::bit_or,   int64_t>( int32, 0, 0x2222222222222222));
static_assert(0xab89efcd_sint32 == bassign<op::bit_xor,  int64_t>( int32, 0, 0x2222222222222222));

static_assert(0x16543210_uint32 == bassign<op::bit_and, uint8_t>(uint32, 7, 0x99));
static_assert(0xf6543210_uint32 == bassign<op::bit_or,  uint8_t>(uint32, 7, 0x99));
static_assert(0xe6543210_uint32 == bassign<op::bit_xor, uint8_t>(uint32, 7, 0x99));
static_assert(0x89abcd8f_sint32 == bassign<op::bit_and, uint8_t>( int32, 1, 0x99));
static_assert(0x89abcdff_sint32 == bassign<op::bit_or,  uint8_t>( int32, 1, 0x99));
static_assert(0x89abcd7f_sint32 == bassign<op::bit_xor, uint8_t>( int32, 1, 0x99));
static_assert(0x16543210_uint32 == bassign<op::bit_and,  int8_t>(uint32, 7, 0x99));
static_assert(0xf6543210_uint32 == bassign<op::bit_or,   int8_t>(uint32, 7, 0x99));
static_assert(0xe6543210_uint32 == bassign<op::bit_xor,  int8_t>(uint32, 7, 0x99));
static_assert(0x89abcd8f_sint32 == bassign<op::bit_and,  int8_t>( int32, 1, 0x99));
static_assert(0x89abcdff_sint32 == bassign<op::bit_or,   int8_t>( int32, 1, 0x99));
static_assert(0x89abcd7f_sint32 == bassign<op::bit_xor,  int8_t>( int32, 1, 0x99));

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

static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64a)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64a)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64a)[- 60]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint64a)[- 56]);
static_assert(0x7665544332211000_uint64 == digits_as<uint64_t>(uint64a)[-  4]);
static_assert(0x7766554433221100_uint64 == digits_as<uint64_t>(uint64a)[   0]);
static_assert(0x8776655443322110_uint64 == digits_as<uint64_t>(uint64a)[   4]);
static_assert(0xeeddccbbaa998877_uint64 == digits_as<uint64_t>(uint64a)[  56]);
static_assert(0x000000000ffeeddc_uint64 == digits_as<uint64_t>(uint64a)[ 100]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64a)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64a)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64a)[- 60]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int64a)[- 56]);
static_assert(0x7665544332211000_uint64 == digits_as<uint64_t>( int64a)[-  4]);
static_assert(0x7766554433221100_uint64 == digits_as<uint64_t>( int64a)[   0]);
static_assert(0x8776655443322110_uint64 == digits_as<uint64_t>( int64a)[   4]);
static_assert(0xeeddccbbaa998877_uint64 == digits_as<uint64_t>( int64a)[  56]);
static_assert(0xfffffffffffeeddc_uint64 == digits_as<uint64_t>( int64a)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64a)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64a)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64a)[- 60]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint64a)[- 56]);
static_assert(0x7665544332211000_sint64 == digits_as< int64_t>(uint64a)[-  4]);
static_assert(0x7766554433221100_sint64 == digits_as< int64_t>(uint64a)[   0]);
static_assert(0x8776655443322110_sint64 == digits_as< int64_t>(uint64a)[   4]);
static_assert(0xeeddccbbaa998877_sint64 == digits_as< int64_t>(uint64a)[  56]);
static_assert(0x000000000ffeeddc_sint64 == digits_as< int64_t>(uint64a)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64a)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64a)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64a)[- 60]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int64a)[- 56]);
static_assert(0x7665544332211000_sint64 == digits_as< int64_t>( int64a)[-  4]);
static_assert(0x7766554433221100_sint64 == digits_as< int64_t>( int64a)[   0]);
static_assert(0x8776655443322110_sint64 == digits_as< int64_t>( int64a)[   4]);
static_assert(0xeeddccbbaa998877_sint64 == digits_as< int64_t>( int64a)[  56]);
static_assert(0xfffffffffffeeddc_sint64 == digits_as< int64_t>( int64a)[ 100]);

static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[-  4]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint64a)[   0]);
static_assert(0x10_uint8 == digits_as<uint8_t>(uint64a)[   4]);
static_assert(0x77_uint8 == digits_as<uint8_t>(uint64a)[  56]);
static_assert(0xdc_uint8 == digits_as<uint8_t>(uint64a)[ 100]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[-  4]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int64a)[   0]);
static_assert(0x10_uint8 == digits_as<uint8_t>( int64a)[   4]);
static_assert(0x77_uint8 == digits_as<uint8_t>( int64a)[  56]);
static_assert(0xdc_uint8 == digits_as<uint8_t>( int64a)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[-  4]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint64a)[   0]);
static_assert(0x10_sint8 == digits_as< int8_t>(uint64a)[   4]);
static_assert(0x77_sint8 == digits_as< int8_t>(uint64a)[  56]);
static_assert(0xdc_sint8 == digits_as< int8_t>(uint64a)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[-  4]);
static_assert(0x00_sint8 == digits_as< int8_t>( int64a)[   0]);
static_assert(0x10_sint8 == digits_as< int8_t>( int64a)[   4]);
static_assert(0x77_sint8 == digits_as< int8_t>( int64a)[  56]);
static_assert(0xdc_sint8 == digits_as< int8_t>( int64a)[ 100]);

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

static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32a)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32a)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32a)[- 60]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32a)[- 56]);
static_assert(0x7665544332211000_uint64 == digits_as<uint64_t>(uint32a)[-  4]);
static_assert(0x7766554433221100_uint64 == digits_as<uint64_t>(uint32a)[   0]);
static_assert(0x8776655443322110_uint64 == digits_as<uint64_t>(uint32a)[   4]);
static_assert(0x000000bbaa998877_uint64 == digits_as<uint64_t>(uint32a)[  56]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>(uint32a)[ 100]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32a)[-200]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32a)[- 64]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32a)[- 60]);
static_assert(0x0000000000000000_uint64 == digits_as<uint64_t>( int32a)[- 56]);
static_assert(0x7665544332211000_uint64 == digits_as<uint64_t>( int32a)[-  4]);
static_assert(0x7766554433221100_uint64 == digits_as<uint64_t>( int32a)[   0]);
static_assert(0x8776655443322110_uint64 == digits_as<uint64_t>( int32a)[   4]);
static_assert(0xffffffbbaa998877_uint64 == digits_as<uint64_t>( int32a)[  56]);
static_assert(0xffffffffffffffff_uint64 == digits_as<uint64_t>( int32a)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32a)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32a)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32a)[- 60]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32a)[- 56]);
static_assert(0x7665544332211000_sint64 == digits_as< int64_t>(uint32a)[-  4]);
static_assert(0x7766554433221100_sint64 == digits_as< int64_t>(uint32a)[   0]);
static_assert(0x8776655443322110_sint64 == digits_as< int64_t>(uint32a)[   4]);
static_assert(0x000000bbaa998877_sint64 == digits_as< int64_t>(uint32a)[  56]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>(uint32a)[ 100]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32a)[-200]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32a)[- 64]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32a)[- 60]);
static_assert(0x0000000000000000_sint64 == digits_as< int64_t>( int32a)[- 56]);
static_assert(0x7665544332211000_sint64 == digits_as< int64_t>( int32a)[-  4]);
static_assert(0x7766554433221100_sint64 == digits_as< int64_t>( int32a)[   0]);
static_assert(0x8776655443322110_sint64 == digits_as< int64_t>( int32a)[   4]);
static_assert(0xffffffbbaa998877_sint64 == digits_as< int64_t>( int32a)[  56]);
static_assert(0xffffffffffffffff_sint64 == digits_as< int64_t>( int32a)[ 100]);

static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[-  4]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[   0]);
static_assert(0x10_uint8 == digits_as<uint8_t>(uint32a)[   4]);
static_assert(0x77_uint8 == digits_as<uint8_t>(uint32a)[  56]);
static_assert(0x00_uint8 == digits_as<uint8_t>(uint32a)[ 100]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[-200]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[- 64]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[- 60]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[- 56]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[-  4]);
static_assert(0x00_uint8 == digits_as<uint8_t>( int32a)[   0]);
static_assert(0x10_uint8 == digits_as<uint8_t>( int32a)[   4]);
static_assert(0x77_uint8 == digits_as<uint8_t>( int32a)[  56]);
static_assert(0xff_uint8 == digits_as<uint8_t>( int32a)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[-  4]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[   0]);
static_assert(0x10_sint8 == digits_as< int8_t>(uint32a)[   4]);
static_assert(0x77_sint8 == digits_as< int8_t>(uint32a)[  56]);
static_assert(0x00_sint8 == digits_as< int8_t>(uint32a)[ 100]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[-200]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[- 64]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[- 60]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[- 56]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[-  4]);
static_assert(0x00_sint8 == digits_as< int8_t>( int32a)[   0]);
static_assert(0x10_sint8 == digits_as< int8_t>( int32a)[   4]);
static_assert(0x77_sint8 == digits_as< int8_t>( int32a)[  56]);
static_assert(0xff_sint8 == digits_as< int8_t>( int32a)[ 100]);

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

static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a, -200, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a, - 64, 0xcffffffffffffff5));
static_assert(std::array{0x776655443322110c_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a, - 60, 0xcffffffffffffff5));
static_assert(std::array{0x77665544332211cf_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a, - 56, 0xcffffffffffffff5));
static_assert(std::array{0x7cffffffffffffff_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a, -  4, 0xcffffffffffffff5));
static_assert(std::array{0xcffffffffffffff5_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint64_t>(uint64a,    0, 0xcffffffffffffff5));
static_assert(std::array{0xffffffffffffff50_uint64, 0xffeeddccbbaa998c_uint64} == dassign<uint64_t>(uint64a,    4, 0xcffffffffffffff5));
static_assert(std::array{0xf566554433221100_uint64, 0xffcfffffffffffff_uint64} == dassign<uint64_t>(uint64a,   56, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_uint64, 0xffffff5cbbaa9988_uint64} == dassign<uint64_t>(uint64a,  100, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a, -200, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a, - 64, 0xcffffffffffffff5));
static_assert(std::array{0x776655443322110c_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a, - 60, 0xcffffffffffffff5));
static_assert(std::array{0x77665544332211cf_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a, - 56, 0xcffffffffffffff5));
static_assert(std::array{0x7cffffffffffffff_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a, -  4, 0xcffffffffffffff5));
static_assert(std::array{0xcffffffffffffff5_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint64_t>( int64a,    0, 0xcffffffffffffff5));
static_assert(std::array{0xffffffffffffff50_sint64, 0xffeeddccbbaa998c_sint64} == dassign<uint64_t>( int64a,    4, 0xcffffffffffffff5));
static_assert(std::array{0xf566554433221100_sint64, 0xffcfffffffffffff_sint64} == dassign<uint64_t>( int64a,   56, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffffff5cbbaa9988_sint64} == dassign<uint64_t>( int64a,  100, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a, -200, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a, - 64, 0xcffffffffffffff5));
static_assert(std::array{0x776655443322110c_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a, - 60, 0xcffffffffffffff5));
static_assert(std::array{0x77665544332211cf_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a, - 56, 0xcffffffffffffff5));
static_assert(std::array{0x7cffffffffffffff_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a, -  4, 0xcffffffffffffff5));
static_assert(std::array{0xcffffffffffffff5_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int64_t>(uint64a,    0, 0xcffffffffffffff5));
static_assert(std::array{0xffffffffffffff50_uint64, 0xffeeddccbbaa998c_uint64} == dassign< int64_t>(uint64a,    4, 0xcffffffffffffff5));
static_assert(std::array{0xf566554433221100_uint64, 0xffcfffffffffffff_uint64} == dassign< int64_t>(uint64a,   56, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_uint64, 0xffffff5cbbaa9988_uint64} == dassign< int64_t>(uint64a,  100, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a, -200, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a, - 64, 0xcffffffffffffff5));
static_assert(std::array{0x776655443322110c_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a, - 60, 0xcffffffffffffff5));
static_assert(std::array{0x77665544332211cf_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a, - 56, 0xcffffffffffffff5));
static_assert(std::array{0x7cffffffffffffff_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a, -  4, 0xcffffffffffffff5));
static_assert(std::array{0xcffffffffffffff5_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int64_t>( int64a,    0, 0xcffffffffffffff5));
static_assert(std::array{0xffffffffffffff50_sint64, 0xffeeddccbbaa998c_sint64} == dassign< int64_t>( int64a,    4, 0xcffffffffffffff5));
static_assert(std::array{0xf566554433221100_sint64, 0xffcfffffffffffff_sint64} == dassign< int64_t>( int64a,   56, 0xcffffffffffffff5));
static_assert(std::array{0x7766554433221100_sint64, 0xffffff5cbbaa9988_sint64} == dassign< int64_t>( int64a,  100, 0xcffffffffffffff5));

static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a, -200, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a, - 64, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a, - 60, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a, - 56, 0xc5));
static_assert(std::array{0x776655443322110c_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a, -  4, 0xc5));
static_assert(std::array{0x77665544332211c5_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a,    0, 0xc5));
static_assert(std::array{0x7766554433221c50_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a,    4, 0xc5));
static_assert(std::array{0xc566554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<uint8_t>(uint64a,   56, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeedc5cbbaa9988_uint64} == dassign<uint8_t>(uint64a,  100, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a, -200, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a, - 64, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a, - 60, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a, - 56, 0xc5));
static_assert(std::array{0x776655443322110c_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a, -  4, 0xc5));
static_assert(std::array{0x77665544332211c5_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a,    0, 0xc5));
static_assert(std::array{0x7766554433221c50_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a,    4, 0xc5));
static_assert(std::array{0xc566554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<uint8_t>( int64a,   56, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeedc5cbbaa9988_sint64} == dassign<uint8_t>( int64a,  100, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a, -200, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a, - 64, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a, - 60, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a, - 56, 0xc5));
static_assert(std::array{0x776655443322110c_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a, -  4, 0xc5));
static_assert(std::array{0x77665544332211c5_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a,    0, 0xc5));
static_assert(std::array{0x7766554433221c50_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a,    4, 0xc5));
static_assert(std::array{0xc566554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign< int8_t>(uint64a,   56, 0xc5));
static_assert(std::array{0x7766554433221100_uint64, 0xffeedc5cbbaa9988_uint64} == dassign< int8_t>(uint64a,  100, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a, -200, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a, - 64, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a, - 60, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a, - 56, 0xc5));
static_assert(std::array{0x776655443322110c_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a, -  4, 0xc5));
static_assert(std::array{0x77665544332211c5_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a,    0, 0xc5));
static_assert(std::array{0x7766554433221c50_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a,    4, 0xc5));
static_assert(std::array{0xc566554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign< int8_t>( int64a,   56, 0xc5));
static_assert(std::array{0x7766554433221100_sint64, 0xffeedc5cbbaa9988_sint64} == dassign< int8_t>( int64a,  100, 0xc5));

static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a, -200, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a, - 64, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a, - 60, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a, - 56, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a, -  4, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,    0, 0));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,    4, 0));
static_assert(std::array{0x7666554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,   56, 0));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,  100, 0));
static_assert(std::array{0x7766554433221101_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,    0, 1));
static_assert(std::array{0x7766554433221110_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,    4, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<bool>(uint64a,   56, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeedddcbbaa9988_uint64} == dassign<bool>(uint64a,  100, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a, -200, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a, - 64, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a, - 60, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a, - 56, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a, -  4, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,    0, 0));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,    4, 0));
static_assert(std::array{0x7666554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,   56, 0));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,  100, 0));
static_assert(std::array{0x7766554433221101_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,    0, 1));
static_assert(std::array{0x7766554433221110_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,    4, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<bool>( int64a,   56, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeedddcbbaa9988_sint64} == dassign<bool>( int64a,  100, 1));

static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array{0x3322110c_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array{0x332211cf_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array{0xd6ffffff_uint32, 0x7cffffff_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array{0x6ffffff5_uint32, 0xcffffffd_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array{0xffffff50_uint32, 0xffffffd6_uint32, 0xbbaa998c_uint32} == dassign<uint64_t>(uint32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0xf5665544_uint32, 0xfd6fffff_uint32} == dassign<uint64_t>(uint32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint64_t>(uint32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array{0x3322110c_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array{0x332211cf_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array{0xd6ffffff_sint32, 0x7cffffff_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array{0x6ffffff5_sint32, 0xcffffffd_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array{0xffffff50_sint32, 0xffffffd6_sint32, 0xbbaa998c_sint32} == dassign<uint64_t>( int32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0xf5665544_sint32, 0xfd6fffff_sint32} == dassign<uint64_t>( int32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint64_t>( int32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array{0x3322110c_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array{0x332211cf_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array{0xd6ffffff_uint32, 0x7cffffff_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array{0x6ffffff5_uint32, 0xcffffffd_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array{0xffffff50_uint32, 0xffffffd6_uint32, 0xbbaa998c_uint32} == dassign< int64_t>(uint32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0xf5665544_uint32, 0xfd6fffff_uint32} == dassign< int64_t>(uint32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int64_t>(uint32a,  100, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a, -200, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a, - 64, 0xcffffffd6ffffff5));
static_assert(std::array{0x3322110c_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a, - 60, 0xcffffffd6ffffff5));
static_assert(std::array{0x332211cf_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a, - 56, 0xcffffffd6ffffff5));
static_assert(std::array{0xd6ffffff_sint32, 0x7cffffff_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a, -  4, 0xcffffffd6ffffff5));
static_assert(std::array{0x6ffffff5_sint32, 0xcffffffd_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a,    0, 0xcffffffd6ffffff5));
static_assert(std::array{0xffffff50_sint32, 0xffffffd6_sint32, 0xbbaa998c_sint32} == dassign< int64_t>( int32a,    4, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0xf5665544_sint32, 0xfd6fffff_sint32} == dassign< int64_t>( int32a,   56, 0xcffffffd6ffffff5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int64_t>( int32a,  100, 0xcffffffd6ffffff5));

static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a, -200, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a, - 64, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a, - 60, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a, - 56, 0xc5));
static_assert(std::array{0x3322110c_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a, -  4, 0xc5));
static_assert(std::array{0x332211c5_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a,    0, 0xc5));
static_assert(std::array{0x33221c50_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a,    4, 0xc5));
static_assert(std::array{0x33221100_uint32, 0xc5665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a,   56, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<uint8_t>(uint32a,  100, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a, -200, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a, - 64, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a, - 60, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a, - 56, 0xc5));
static_assert(std::array{0x3322110c_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a, -  4, 0xc5));
static_assert(std::array{0x332211c5_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a,    0, 0xc5));
static_assert(std::array{0x33221c50_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a,    4, 0xc5));
static_assert(std::array{0x33221100_sint32, 0xc5665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a,   56, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<uint8_t>( int32a,  100, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a, -200, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a, - 64, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a, - 60, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a, - 56, 0xc5));
static_assert(std::array{0x3322110c_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a, -  4, 0xc5));
static_assert(std::array{0x332211c5_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a,    0, 0xc5));
static_assert(std::array{0x33221c50_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a,    4, 0xc5));
static_assert(std::array{0x33221100_uint32, 0xc5665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a,   56, 0xc5));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign< int8_t>(uint32a,  100, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a, -200, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a, - 64, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a, - 60, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a, - 56, 0xc5));
static_assert(std::array{0x3322110c_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a, -  4, 0xc5));
static_assert(std::array{0x332211c5_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a,    0, 0xc5));
static_assert(std::array{0x33221c50_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a,    4, 0xc5));
static_assert(std::array{0x33221100_sint32, 0xc5665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a,   56, 0xc5));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign< int8_t>( int32a,  100, 0xc5));

static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a, -200, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a, - 64, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a, - 60, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a, - 56, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a, -  4, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,    0, 0));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,    4, 0));
static_assert(std::array{0x33221100_uint32, 0x76665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,   56, 0));
static_assert(std::array{0x33221101_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,    0, 1));
static_assert(std::array{0x33221110_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,    4, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,   56, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<bool>(uint32a,  100, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a, -200, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a, - 64, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a, - 60, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a, - 56, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a, -  4, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,    0, 0));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,    4, 0));
static_assert(std::array{0x33221100_sint32, 0x76665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,   56, 0));
static_assert(std::array{0x33221101_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,    0, 1));
static_assert(std::array{0x33221110_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,    4, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,   56, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<bool>( int32a,  100, 1));

static_assert(std::array{0x1166554433221100_uint64, 0xff00110011001100_uint64} == dassign<op::bit_and, uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x7766554433221100_uint64, 0xffffddddbbbb9999_uint64} == dassign<op::bit_or,  uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x6666554433221100_uint64, 0xffffccddaabb8899_uint64} == dassign<op::bit_xor, uint64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x1166554433221100_sint64, 0xff00110011001100_sint64} == dassign<op::bit_and, uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array{0x7766554433221100_sint64, 0xffffddddbbbb9999_sint64} == dassign<op::bit_or,  uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array{0x6666554433221100_sint64, 0xffffccddaabb8899_sint64} == dassign<op::bit_xor, uint64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array{0x1166554433221100_uint64, 0xff00110011001100_uint64} == dassign<op::bit_and,  int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x7766554433221100_uint64, 0xffffddddbbbb9999_uint64} == dassign<op::bit_or,   int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x6666554433221100_uint64, 0xffffccddaabb8899_uint64} == dassign<op::bit_xor,  int64_t>(uint64a, 56, 0x1111111111111111));
static_assert(std::array{0x1166554433221100_sint64, 0xff00110011001100_sint64} == dassign<op::bit_and,  int64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array{0x7766554433221100_sint64, 0xffffddddbbbb9999_sint64} == dassign<op::bit_or,   int64_t>( int64a, 56, 0x1111111111111111));
static_assert(std::array{0x6666554433221100_sint64, 0xffffccddaabb8899_sint64} == dassign<op::bit_xor,  int64_t>( int64a, 56, 0x1111111111111111));

static_assert(std::array{0x1766554433221100_uint64, 0xffeeddccbbaa9980_uint64} == dassign<op::bit_and, uint8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9989_uint64} == dassign<op::bit_or,  uint8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x6766554433221100_uint64, 0xffeeddccbbaa9989_uint64} == dassign<op::bit_xor, uint8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x1766554433221100_sint64, 0xffeeddccbbaa9980_sint64} == dassign<op::bit_and, uint8_t>( int64a, 60, 0x11));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9989_sint64} == dassign<op::bit_or,  uint8_t>( int64a, 60, 0x11));
static_assert(std::array{0x6766554433221100_sint64, 0xffeeddccbbaa9989_sint64} == dassign<op::bit_xor, uint8_t>( int64a, 60, 0x11));
static_assert(std::array{0x1766554433221100_uint64, 0xffeeddccbbaa9980_uint64} == dassign<op::bit_and,  int8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9989_uint64} == dassign<op::bit_or,   int8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x6766554433221100_uint64, 0xffeeddccbbaa9989_uint64} == dassign<op::bit_xor,  int8_t>(uint64a, 60, 0x11));
static_assert(std::array{0x1766554433221100_sint64, 0xffeeddccbbaa9980_sint64} == dassign<op::bit_and,  int8_t>( int64a, 60, 0x11));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9989_sint64} == dassign<op::bit_or,   int8_t>( int64a, 60, 0x11));
static_assert(std::array{0x6766554433221100_sint64, 0xffeeddccbbaa9989_sint64} == dassign<op::bit_xor,  int8_t>( int64a, 60, 0x11));

static_assert(std::array{0x6766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_and, bool>(uint64a, 60, 0));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_or,  bool>(uint64a, 60, 0));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_xor, bool>(uint64a, 60, 0));
static_assert(std::array{0x6766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_and, bool>( int64a, 60, 0));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_or,  bool>( int64a, 60, 0));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_xor, bool>( int64a, 60, 0));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_and, bool>(uint64a, 60, 1));
static_assert(std::array{0x7766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_or,  bool>(uint64a, 60, 1));
static_assert(std::array{0x6766554433221100_uint64, 0xffeeddccbbaa9988_uint64} == dassign<op::bit_xor, bool>(uint64a, 60, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_and, bool>( int64a, 60, 1));
static_assert(std::array{0x7766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_or,  bool>( int64a, 60, 1));
static_assert(std::array{0x6766554433221100_sint64, 0xffeeddccbbaa9988_sint64} == dassign<op::bit_xor, bool>( int64a, 60, 1));

static_assert(std::array{0x23221100_uint32, 0x22220000_uint32, 0xb2220000_uint32} == dassign<op::bit_and, uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x33221100_uint32, 0x77667766_uint32, 0xbbaabbaa_uint32} == dassign<op::bit_or,  uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x13221100_uint32, 0x55447766_uint32, 0xb988bbaa_uint32} == dassign<op::bit_xor, uint64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x23221100_sint32, 0x22220000_sint32, 0xb2220000_sint32} == dassign<op::bit_and, uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array{0x33221100_sint32, 0x77667766_sint32, 0xbbaabbaa_sint32} == dassign<op::bit_or,  uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array{0x13221100_sint32, 0x55447766_sint32, 0xb988bbaa_sint32} == dassign<op::bit_xor, uint64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array{0x23221100_uint32, 0x22220000_uint32, 0xb2220000_uint32} == dassign<op::bit_and,  int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x33221100_uint32, 0x77667766_uint32, 0xbbaabbaa_uint32} == dassign<op::bit_or,   int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x13221100_uint32, 0x55447766_uint32, 0xb988bbaa_uint32} == dassign<op::bit_xor,  int64_t>(uint32a, 28, 0x2222222222222222));
static_assert(std::array{0x23221100_sint32, 0x22220000_sint32, 0xb2220000_sint32} == dassign<op::bit_and,  int64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array{0x33221100_sint32, 0x77667766_sint32, 0xbbaabbaa_sint32} == dassign<op::bit_or,   int64_t>( int32a, 28, 0x2222222222222222));
static_assert(std::array{0x13221100_sint32, 0x55447766_sint32, 0xb988bbaa_sint32} == dassign<op::bit_xor,  int64_t>( int32a, 28, 0x2222222222222222));

static_assert(std::array{0x33221100_uint32, 0x17665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_and, uint8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_uint32, 0xf7665544_uint32, 0xbbaa9989_uint32} == dassign<op::bit_or,  uint8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_uint32, 0xe7665544_uint32, 0xbbaa9981_uint32} == dassign<op::bit_xor, uint8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0x17665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_and, uint8_t>( int32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0xf7665544_sint32, 0xbbaa9989_sint32} == dassign<op::bit_or,  uint8_t>( int32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0xe7665544_sint32, 0xbbaa9981_sint32} == dassign<op::bit_xor, uint8_t>( int32a, 60, 0x99));
static_assert(std::array{0x33221100_uint32, 0x17665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_and,  int8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_uint32, 0xf7665544_uint32, 0xbbaa9989_uint32} == dassign<op::bit_or,   int8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_uint32, 0xe7665544_uint32, 0xbbaa9981_uint32} == dassign<op::bit_xor,  int8_t>(uint32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0x17665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_and,  int8_t>( int32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0xf7665544_sint32, 0xbbaa9989_sint32} == dassign<op::bit_or,   int8_t>( int32a, 60, 0x99));
static_assert(std::array{0x33221100_sint32, 0xe7665544_sint32, 0xbbaa9981_sint32} == dassign<op::bit_xor,  int8_t>( int32a, 60, 0x99));

static_assert(std::array{0x33221100_uint32, 0x67665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_and, bool>(uint32a, 60, 0));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_or,  bool>(uint32a, 60, 0));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_xor, bool>(uint32a, 60, 0));
static_assert(std::array{0x33221100_sint32, 0x67665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_and, bool>( int32a, 60, 0));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_or,  bool>( int32a, 60, 0));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_xor, bool>( int32a, 60, 0));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_and, bool>(uint32a, 60, 1));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_or,  bool>(uint32a, 60, 1));
static_assert(std::array{0x33221100_uint32, 0x67665544_uint32, 0xbbaa9988_uint32} == dassign<op::bit_xor, bool>(uint32a, 60, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_and, bool>( int32a, 60, 1));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_or,  bool>( int32a, 60, 1));
static_assert(std::array{0x33221100_sint32, 0x67665544_sint32, 0xbbaa9988_sint32} == dassign<op::bit_xor, bool>( int32a, 60, 1));

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

static_assert(std::array{0x5544554433221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_and, uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x7777555533221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_or,  uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x2233001133221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_xor, uint64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x5544554433221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_and, uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array{0x7777555533221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_or,  uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array{0x2233001133221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_xor, uint64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array{0x5544554433221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_and,  int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x7777555533221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_or,   int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x2233001133221100_uint64, 0xffeeddccbbaa9988_uint64} == bassign<op::bit_xor,  int64_t>(uint64a, 1, 0x5555555555555555));
static_assert(std::array{0x5544554433221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_and,  int64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array{0x7777555533221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_or,   int64_t>( int64a, 1, 0x5555555555555555));
static_assert(std::array{0x2233001133221100_sint64, 0xffeeddccbbaa9988_sint64} == bassign<op::bit_xor,  int64_t>( int64a, 1, 0x5555555555555555));

static_assert(std::array{0x7766554433221100_uint64, 0xff4eddccbbaa9988_uint64} == bassign<op::bit_and, uint8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_uint64, 0xfffeddccbbaa9988_uint64} == bassign<op::bit_or,  uint8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_uint64, 0xffbeddccbbaa9988_uint64} == bassign<op::bit_xor, uint8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xff4eddccbbaa9988_sint64} == bassign<op::bit_and, uint8_t>( int64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xfffeddccbbaa9988_sint64} == bassign<op::bit_or,  uint8_t>( int64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xffbeddccbbaa9988_sint64} == bassign<op::bit_xor, uint8_t>( int64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_uint64, 0xff4eddccbbaa9988_uint64} == bassign<op::bit_and,  int8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_uint64, 0xfffeddccbbaa9988_uint64} == bassign<op::bit_or,   int8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_uint64, 0xffbeddccbbaa9988_uint64} == bassign<op::bit_xor,  int8_t>(uint64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xff4eddccbbaa9988_sint64} == bassign<op::bit_and,  int8_t>( int64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xfffeddccbbaa9988_sint64} == bassign<op::bit_or,   int8_t>( int64a, 29, 0x55));
static_assert(std::array{0x7766554433221100_sint64, 0xffbeddccbbaa9988_sint64} == bassign<op::bit_xor,  int8_t>( int64a, 29, 0x55));

static_assert(std::array{0x33221100_uint32, 0x11001100_uint32, 0xbbaa9988_uint32} == bassign<op::bit_and, uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_uint32, 0xffffdddd_uint32, 0xbbaa9988_uint32} == bassign<op::bit_or,  uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_uint32, 0xeeffccdd_uint32, 0xbbaa9988_uint32} == bassign<op::bit_xor, uint64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0x11001100_sint32, 0xbbaa9988_sint32} == bassign<op::bit_and, uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0xffffdddd_sint32, 0xbbaa9988_sint32} == bassign<op::bit_or,  uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0xeeffccdd_sint32, 0xbbaa9988_sint32} == bassign<op::bit_xor, uint64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_uint32, 0x11001100_uint32, 0xbbaa9988_uint32} == bassign<op::bit_and,  int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_uint32, 0xffffdddd_uint32, 0xbbaa9988_uint32} == bassign<op::bit_or,   int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_uint32, 0xeeffccdd_uint32, 0xbbaa9988_uint32} == bassign<op::bit_xor,  int64_t>(uint32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0x11001100_sint32, 0xbbaa9988_sint32} == bassign<op::bit_and,  int64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0xffffdddd_sint32, 0xbbaa9988_sint32} == bassign<op::bit_or,   int64_t>( int32a, 1, 0x5555555599999999));
static_assert(std::array{0x33221100_sint32, 0xeeffccdd_sint32, 0xbbaa9988_sint32} == bassign<op::bit_xor,  int64_t>( int32a, 1, 0x5555555599999999));

static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9980_uint32} == bassign<op::bit_and, uint8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa998d_uint32} == bassign<op::bit_or,  uint8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa998d_uint32} == bassign<op::bit_xor, uint8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9980_sint32} == bassign<op::bit_and, uint8_t>( int32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa998d_sint32} == bassign<op::bit_or,  uint8_t>( int32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa998d_sint32} == bassign<op::bit_xor, uint8_t>( int32a, 16, 0x55));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa9980_uint32} == bassign<op::bit_and,  int8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa998d_uint32} == bassign<op::bit_or,   int8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_uint32, 0x77665544_uint32, 0xbbaa998d_uint32} == bassign<op::bit_xor,  int8_t>(uint32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa9980_sint32} == bassign<op::bit_and,  int8_t>( int32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa998d_sint32} == bassign<op::bit_or,   int8_t>( int32a, 16, 0x55));
static_assert(std::array{0x33221100_sint32, 0x77665544_sint32, 0xbbaa998d_sint32} == bassign<op::bit_xor,  int8_t>( int32a, 16, 0x55));

} /* namespace */
