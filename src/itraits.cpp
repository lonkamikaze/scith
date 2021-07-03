#include "itraits.hpp"

#include <cstdint>

using namespace scith::itraits;

namespace {

static_assert(!integral<void>);
static_assert( integral<int>);
static_assert(!integral<float>);
static_assert( integral<int8_t>);
static_assert( integral<int16_t>);
static_assert( integral<int32_t>);
static_assert( integral<int64_t>);
static_assert( integral<unsigned>);
static_assert( integral<uint8_t>);
static_assert( integral<uint16_t>);
static_assert( integral<uint32_t>);
static_assert( integral<uint64_t>);
static_assert(!integral<int[4]>);
static_assert(!integral<int8_t[4]>);
static_assert(!integral<int16_t[4]>);
static_assert(!integral<int32_t[4]>);
static_assert(!integral<int64_t[4]>);
static_assert(!integral<unsigned[4]>);
static_assert(!integral<uint8_t[4]>);
static_assert(!integral<uint16_t[4]>);
static_assert(!integral<uint32_t[4]>);
static_assert(!integral<uint64_t[4]>);

static_assert(!unsigned_integral<void>);
static_assert(!unsigned_integral<int>);
static_assert(!unsigned_integral<float>);
static_assert(!unsigned_integral<int8_t>);
static_assert(!unsigned_integral<int16_t>);
static_assert(!unsigned_integral<int32_t>);
static_assert(!unsigned_integral<int64_t>);
static_assert( unsigned_integral<unsigned>);
static_assert( unsigned_integral<uint8_t>);
static_assert( unsigned_integral<uint16_t>);
static_assert( unsigned_integral<uint32_t>);
static_assert( unsigned_integral<uint64_t>);
static_assert(!unsigned_integral<int[4]>);
static_assert(!unsigned_integral<int8_t[4]>);
static_assert(!unsigned_integral<int16_t[4]>);
static_assert(!unsigned_integral<int32_t[4]>);
static_assert(!unsigned_integral<int64_t[4]>);
static_assert(!unsigned_integral<unsigned[4]>);
static_assert(!unsigned_integral<uint8_t[4]>);
static_assert(!unsigned_integral<uint16_t[4]>);
static_assert(!unsigned_integral<uint32_t[4]>);
static_assert(!unsigned_integral<uint64_t[4]>);

static_assert(!signed_integral<void>);
static_assert( signed_integral<int>);
static_assert(!signed_integral<float>);
static_assert( signed_integral<int8_t>);
static_assert( signed_integral<int16_t>);
static_assert( signed_integral<int32_t>);
static_assert( signed_integral<int64_t>);
static_assert(!signed_integral<unsigned>);
static_assert(!signed_integral<uint8_t>);
static_assert(!signed_integral<uint16_t>);
static_assert(!signed_integral<uint32_t>);
static_assert(!signed_integral<uint64_t>);
static_assert(!signed_integral<int[4]>);
static_assert(!signed_integral<int8_t[4]>);
static_assert(!signed_integral<int16_t[4]>);
static_assert(!signed_integral<int32_t[4]>);
static_assert(!signed_integral<int64_t[4]>);
static_assert(!signed_integral<unsigned[4]>);
static_assert(!signed_integral<uint8_t[4]>);
static_assert(!signed_integral<uint16_t[4]>);
static_assert(!signed_integral<uint32_t[4]>);
static_assert(!signed_integral<uint64_t[4]>);

static_assert(3 == max(      3));
static_assert(2 == max(      2));
static_assert(1 == max(      1));
static_assert(3 == max(   3, 2));
static_assert(3 == max(   2, 3));
static_assert(2 == max(   2, 1));
static_assert(2 == max(   1, 2));
static_assert(3 == max(3, 2, 1));
static_assert(3 == max(3, 1, 2));
static_assert(3 == max(1, 3, 2));
static_assert(3 == max(2, 3, 1));
static_assert(3 == max(2, 1, 3));
static_assert(3 == max(1, 2, 3));

static_assert(3 == min(      3));
static_assert(2 == min(      2));
static_assert(1 == min(      1));
static_assert(2 == min(   3, 2));
static_assert(2 == min(   2, 3));
static_assert(1 == min(   2, 1));
static_assert(1 == min(   1, 2));
static_assert(1 == min(3, 2, 1));
static_assert(1 == min(3, 1, 2));
static_assert(1 == min(1, 3, 2));
static_assert(1 == min(2, 3, 1));
static_assert(1 == min(2, 1, 3));
static_assert(1 == min(1, 2, 3));

static_assert( 7 ==  digits_v<  int8_t>);
static_assert( 8 ==  digits_v< uint8_t>);
static_assert(15 ==  digits_v< int16_t>);
static_assert(16 ==  digits_v<uint16_t>);
static_assert(31 ==  digits_v< int32_t>);
static_assert(32 ==  digits_v<uint32_t>);
static_assert(63 ==  digits_v< int64_t>);
static_assert(64 ==  digits_v<uint64_t>);

static_assert( 8 == udigits_v<  int8_t>);
static_assert( 8 == udigits_v< uint8_t>);
static_assert(16 == udigits_v< int16_t>);
static_assert(16 == udigits_v<uint16_t>);
static_assert(32 == udigits_v< int32_t>);
static_assert(32 == udigits_v<uint32_t>);
static_assert(64 == udigits_v< int64_t>);
static_assert(64 == udigits_v<uint64_t>);

static_assert( 7 == sdigits_v<  int8_t>);
static_assert( 7 == sdigits_v< uint8_t>);
static_assert(15 == sdigits_v< int16_t>);
static_assert(15 == sdigits_v<uint16_t>);
static_assert(31 == sdigits_v< int32_t>);
static_assert(31 == sdigits_v<uint32_t>);
static_assert(63 == sdigits_v< int64_t>);
static_assert(63 == sdigits_v<uint64_t>);

static_assert(150 ==  sum_digits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert(152 == sum_udigits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert(148 == sum_sdigits_v<int8_t, uint16_t, int64_t, uint64_t>);

static_assert( 64 ==  max_digits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert( 64 == max_udigits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert( 63 == max_sdigits_v<int8_t, uint16_t, int64_t, uint64_t>);

static_assert(  7 ==  min_digits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert(  8 == min_udigits_v<int8_t, uint16_t, int64_t, uint64_t>);
static_assert(  7 == min_sdigits_v<int8_t, uint16_t, int64_t, uint64_t>);

static_assert(!all_signed_v< int8_t, uint16_t,  int64_t, uint64_t>);
static_assert(!all_signed_v<uint8_t, uint16_t, uint64_t, uint64_t>);
static_assert( all_signed_v< int8_t,  int16_t,  int64_t,  int64_t>);

static_assert( has_signed_v< int8_t, uint16_t,  int64_t, uint64_t>);
static_assert(!has_signed_v<uint8_t, uint16_t, uint64_t, uint64_t>);
static_assert( has_signed_v< int8_t,  int16_t,  int64_t,  int64_t>);

static_assert(!all_unsigned_v< int8_t, uint16_t,  int64_t, uint64_t>);
static_assert( all_unsigned_v<uint8_t, uint16_t, uint64_t, uint64_t>);
static_assert(!all_unsigned_v< int8_t,  int16_t,  int64_t,  int64_t>);

static_assert( has_unsigned_v< int8_t, uint16_t,  int64_t, uint64_t>);
static_assert( has_unsigned_v<uint8_t, uint16_t, uint64_t, uint64_t>);
static_assert(!has_unsigned_v< int8_t,  int16_t,  int64_t,  int64_t>);

static_assert(std::is_same_v<     int, make_signed_if_t<true,  unsigned>>);
static_assert(std::is_same_v<     int, make_signed_if_t<true,  int>>);
static_assert(std::is_same_v<unsigned, make_signed_if_t<false, unsigned>>);
static_assert(std::is_same_v<     int, make_signed_if_t<false, int>>);

static_assert(std::is_same_v< int16_t, make_signed_if_t<true,  uint16_t>>);
static_assert(std::is_same_v< int16_t, make_signed_if_t<true,  int16_t>>);
static_assert(std::is_same_v<uint16_t, make_signed_if_t<false, uint16_t>>);
static_assert(std::is_same_v< int16_t, make_signed_if_t<false, int16_t>>);

static_assert(std::is_same_v<unsigned, make_unsigned_if_t<true,  unsigned>>);
static_assert(std::is_same_v<unsigned, make_unsigned_if_t<true,  int>>);
static_assert(std::is_same_v<unsigned, make_unsigned_if_t<false, unsigned>>);
static_assert(std::is_same_v<     int, make_unsigned_if_t<false, int>>);

static_assert(std::is_same_v<uint16_t, make_unsigned_if_t<true,  uint16_t>>);
static_assert(std::is_same_v<uint16_t, make_unsigned_if_t<true,  int16_t>>);
static_assert(std::is_same_v<uint16_t, make_unsigned_if_t<false, uint16_t>>);
static_assert(std::is_same_v< int16_t, make_unsigned_if_t<false, int16_t>>);

static_assert( 0 == sign());

static_assert( 0 == sign(    0));
static_assert( 1 == sign(    1));
static_assert( 1 == sign( 1337));
static_assert( 0 == sign(-   0));
static_assert(-1 == sign(-   1));
static_assert(-1 == sign(-1337));

static_assert( 0 == sign( 0,  0));
static_assert( 0 == sign( 0,  9));
static_assert( 0 == sign( 5,  0));
static_assert( 1 == sign( 1,  7));

static_assert( 0 == sign( 0,  0));
static_assert( 0 == sign( 0, -7));
static_assert( 0 == sign(-2,  0));
static_assert( 1 == sign(-7, -4));

static_assert( 1 == sign( 9,  4));
static_assert(-1 == sign( 1, -6));
static_assert(-1 == sign(-1,  6));
static_assert( 1 == sign(-3, -7));

static_assert( 0 == sign( 0,  0,  0));
static_assert( 0 == sign( 0,  0,  8));
static_assert( 0 == sign( 0,  4,  0));
static_assert( 0 == sign( 0,  9,  5));
static_assert( 0 == sign( 4,  0,  0));
static_assert( 0 == sign( 5,  0,  2));
static_assert( 0 == sign( 1,  9,  0));
static_assert( 1 == sign( 2,  4,  1));

static_assert( 0 == sign( 0,  0,  0));
static_assert( 0 == sign( 0,  0, -8));
static_assert( 0 == sign( 0, -4,  0));
static_assert( 0 == sign( 0, -2, -4));
static_assert( 0 == sign(-5,  0,  0));
static_assert( 0 == sign(-8,  0, -1));
static_assert( 0 == sign(-3, -6,  0));
static_assert(-1 == sign(-5, -9, -2));

static_assert( 1 == sign( 4,  6,  1));
static_assert(-1 == sign( 3,  8, -2));
static_assert(-1 == sign( 8, -4,  3));
static_assert( 1 == sign( 7, -5, -7));
static_assert(-1 == sign(-7,  2,  9));
static_assert( 1 == sign(-7,  4, -3));
static_assert( 1 == sign(-5, -6,  7));
static_assert(-1 == sign(-8, -3, -8));

static_assert( 0 == sign( 3,  0, -2));
static_assert( 0 == sign( 0, -4,  3));
static_assert( 0 == sign( 7, -0, -7));
static_assert( 0 == sign(-7,  2,  0));
static_assert( 0 == sign(-0,  4, -3));
static_assert( 0 == sign(-5, -0,  7));

static_assert( 0 == abs(  0));
static_assert( 0 == abs(- 0));
static_assert( 1 == abs(  1));
static_assert( 1 == abs(- 1));
static_assert(23 == abs( 23));
static_assert(42 == abs(-42));

static_assert( 5 == cdiv( 25,  5));
static_assert(-5 == cdiv( 25, -5));
static_assert(-5 == cdiv(-25,  5));
static_assert( 5 == cdiv(-25, -5));

static_assert( 4 == cdiv( 110,  32));
static_assert(-4 == cdiv( 110, -32));
static_assert(-4 == cdiv(-110,  32));
static_assert( 4 == cdiv(-110, -32));

static_assert( 0 == cdiv(  0,  3));
static_assert( 1 == cdiv(  1,  3));
static_assert( 1 == cdiv(  2,  3));
static_assert( 1 == cdiv(  3,  3));
static_assert( 2 == cdiv(  4,  3));
static_assert( 2 == cdiv(  5,  3));
static_assert( 2 == cdiv(  6,  3));
static_assert( 3 == cdiv(  7,  3));

static_assert( 0 == cdiv(  0, -3));
static_assert(-1 == cdiv(  1, -3));
static_assert(-1 == cdiv(  2, -3));
static_assert(-1 == cdiv(  3, -3));
static_assert(-2 == cdiv(  4, -3));
static_assert(-2 == cdiv(  5, -3));
static_assert(-2 == cdiv(  6, -3));
static_assert(-3 == cdiv(  7, -3));

static_assert(-1 == cdiv( -1,  3));
static_assert(-1 == cdiv( -2,  3));
static_assert(-1 == cdiv( -3,  3));
static_assert(-2 == cdiv( -4,  3));
static_assert(-2 == cdiv( -5,  3));
static_assert(-2 == cdiv( -6,  3));
static_assert(-3 == cdiv( -7,  3));

static_assert( 1 == cdiv( -1, -3));
static_assert( 1 == cdiv( -2, -3));
static_assert( 1 == cdiv( -3, -3));
static_assert( 2 == cdiv( -4, -3));
static_assert( 2 == cdiv( -5, -3));
static_assert( 2 == cdiv( -6, -3));
static_assert( 3 == cdiv( -7, -3));

static_assert(- 1 == log2(0));
static_assert(  0 == log2(1));
static_assert(  1 == log2(2));
static_assert(  1 == log2(3));
static_assert(  2 == log2(4));
static_assert(  2 == log2(5));
static_assert(  2 == log2(6));
static_assert(  2 == log2(7));
static_assert(  3 == log2(8));
static_assert( 63 == log2<int64_t>(-1));
static_assert( 19 == log2(0x080850));
static_assert( 19 == log2(0x0fffff));
static_assert( 20 == log2(0x100000));
static_assert( 20 == log2(0x1fffff));
static_assert(  7 == log2<int8_t>(-128));
static_assert(  7 == log2<int8_t>(-64));
static_assert(  7 == log2<int8_t>(-1));
static_assert(  7 == log2<uint8_t>(128));
static_assert(  6 == log2<uint8_t>(64));
static_assert(  0 == log2<uint8_t>(1));

} /* namespace */
