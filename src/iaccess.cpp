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

template <integral T, integral ... ExpectTs>
constexpr bool expect(auto const & values, ExpectTs const ... expects) {
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

} /* namespace */
