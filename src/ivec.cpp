#include "ivec.hpp"

using namespace scith::ivec;

namespace {

static_assert(std::is_same_v< int8_t, select_greater_digits_t< int8_t,  int8_t>>);
static_assert(std::is_same_v<int16_t, select_greater_digits_t< int8_t, int16_t>>);
static_assert(std::is_same_v<int32_t, select_greater_digits_t< int8_t, int32_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t< int8_t, int64_t>>);
static_assert(std::is_same_v<int16_t, select_greater_digits_t<int16_t, int16_t>>);
static_assert(std::is_same_v<int32_t, select_greater_digits_t<int16_t, int32_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int16_t, int64_t>>);
static_assert(std::is_same_v<int32_t, select_greater_digits_t<int32_t, int32_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int32_t, int64_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int64_t, int64_t>>);

static_assert(std::is_same_v<int16_t, select_greater_digits_t<int16_t,  int8_t>>);
static_assert(std::is_same_v<int32_t, select_greater_digits_t<int32_t,  int8_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int64_t,  int8_t>>);
static_assert(std::is_same_v<int32_t, select_greater_digits_t<int32_t, int16_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int64_t, int16_t>>);
static_assert(std::is_same_v<int64_t, select_greater_digits_t<int64_t, int32_t>>);

static_assert(std::is_same_v< uint8_t, select_greater_digits_t< int8_t,  uint8_t>>);
static_assert(std::is_same_v<uint16_t, select_greater_digits_t< int8_t, uint16_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t< int8_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t< int8_t, uint64_t>>);
static_assert(std::is_same_v<uint16_t, select_greater_digits_t<int16_t, uint16_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<int16_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<int16_t, uint64_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<int32_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<int32_t, uint64_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<int64_t, uint64_t>>);

static_assert(std::is_same_v< uint8_t, select_greater_digits_t< uint8_t,  int8_t>>);
static_assert(std::is_same_v< int16_t, select_greater_digits_t< uint8_t, int16_t>>);
static_assert(std::is_same_v< int32_t, select_greater_digits_t< uint8_t, int32_t>>);
static_assert(std::is_same_v< int64_t, select_greater_digits_t< uint8_t, int64_t>>);
static_assert(std::is_same_v<uint16_t, select_greater_digits_t<uint16_t, int16_t>>);
static_assert(std::is_same_v< int32_t, select_greater_digits_t<uint16_t, int32_t>>);
static_assert(std::is_same_v< int64_t, select_greater_digits_t<uint16_t, int64_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<uint32_t, int32_t>>);
static_assert(std::is_same_v< int64_t, select_greater_digits_t<uint32_t, int64_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint64_t, int64_t>>);

static_assert(std::is_same_v< uint8_t, select_greater_digits_t< uint8_t,  uint8_t>>);
static_assert(std::is_same_v<uint16_t, select_greater_digits_t< uint8_t, uint16_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t< uint8_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t< uint8_t, uint64_t>>);
static_assert(std::is_same_v<uint16_t, select_greater_digits_t<uint16_t, uint16_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<uint16_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint16_t, uint64_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<uint32_t, uint32_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint32_t, uint64_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint64_t, uint64_t>>);

static_assert(std::is_same_v<uint16_t, select_greater_digits_t<uint16_t,  uint8_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<uint32_t,  uint8_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint64_t,  uint8_t>>);
static_assert(std::is_same_v<uint32_t, select_greater_digits_t<uint32_t, uint16_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint64_t, uint16_t>>);
static_assert(std::is_same_v<uint64_t, select_greater_digits_t<uint64_t, uint32_t>>);

static_assert(std::is_same_v< int8_t, select_max_digits_t< int8_t>>);
static_assert(std::is_same_v<int16_t, select_max_digits_t<int16_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t<int32_t>>);

static_assert(std::is_same_v<int32_t, select_max_digits_t<int32_t, int16_t,  int8_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t< int8_t, int32_t, int16_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t<int16_t,  int8_t, int32_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t<int32_t,  int8_t, int16_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t<int16_t, int32_t,  int8_t>>);
static_assert(std::is_same_v<int32_t, select_max_digits_t< int8_t, int16_t, int32_t>>);

static_assert(std::is_same_v<uint32_t, select_max_digits_t<uint32_t,  int32_t, uint16_t,  int16_t>>);
static_assert(std::is_same_v<uint32_t, select_max_digits_t< int16_t, uint32_t,  int32_t, uint16_t>>);
static_assert(std::is_same_v<uint32_t, select_max_digits_t<uint16_t,  int16_t, uint32_t,  int32_t>>);
static_assert(std::is_same_v<uint32_t, select_max_digits_t<uint32_t, uint16_t,  int16_t, uint32_t>>);

static_assert(std::is_same_v<int32_t, select_common_t<uint32_t,  int32_t, uint16_t,  int16_t>>);
static_assert(std::is_same_v<int32_t, select_common_t< int16_t, uint32_t,  int32_t, uint16_t>>);
static_assert(std::is_same_v<int32_t, select_common_t<uint16_t,  int16_t, uint32_t,  int32_t>>);
static_assert(std::is_same_v<int32_t, select_common_t<uint32_t, uint16_t,  int16_t, uint32_t>>);

static_assert(std::is_same_v<std::array< int32_t, 1>, values_t<integer<int32_t,    1>>>);
static_assert(std::is_same_v<std::array< int32_t, 1>, values_t<integer<int32_t,   31>>>);
static_assert(std::is_same_v<std::array< int32_t, 2>, values_t<integer<int32_t,   32>>>);
static_assert(std::is_same_v<std::array< int32_t, 2>, values_t<integer<int32_t,   33>>>);
static_assert(std::is_same_v<std::array< int32_t, 2>, values_t<integer<int32_t,   63>>>);
static_assert(std::is_same_v<std::array< int32_t, 3>, values_t<integer<int32_t,   64>>>);
static_assert(std::is_same_v<std::array< int32_t, 3>, values_t<integer<int32_t,   65>>>);
static_assert(std::is_same_v<std::array< int32_t, 4>, values_t<integer<int32_t,  127>>>);
static_assert(std::is_same_v<std::array< int32_t, 5>, values_t<integer<int32_t,  128>>>);
static_assert(std::is_same_v<std::array< int32_t, 5>, values_t<integer<int32_t,  129>>>);

static_assert(std::is_same_v<std::array<uint32_t, 1>, values_t<integer<uint32_t,   1>>>);
static_assert(std::is_same_v<std::array<uint32_t, 1>, values_t<integer<uint32_t,  31>>>);
static_assert(std::is_same_v<std::array<uint32_t, 1>, values_t<integer<uint32_t,  32>>>);
static_assert(std::is_same_v<std::array<uint32_t, 2>, values_t<integer<uint32_t,  33>>>);
static_assert(std::is_same_v<std::array<uint32_t, 2>, values_t<integer<uint32_t,  63>>>);
static_assert(std::is_same_v<std::array<uint32_t, 2>, values_t<integer<uint32_t,  64>>>);
static_assert(std::is_same_v<std::array<uint32_t, 3>, values_t<integer<uint32_t,  65>>>);
static_assert(std::is_same_v<std::array<uint32_t, 4>, values_t<integer<uint32_t, 127>>>);
static_assert(std::is_same_v<std::array<uint32_t, 4>, values_t<integer<uint32_t, 128>>>);
static_assert(std::is_same_v<std::array<uint32_t, 5>, values_t<integer<uint32_t, 129>>>);

template <std::size_t DigitsV>
using uint = integer<uint32_t, DigitsV>;

template <std::size_t DigitsV>
using sint = integer<int32_t, DigitsV>;

static_assert(std::is_same_v<sint<16>, decltype(+uint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(+sint<16>{})>);

static_assert(std::is_same_v<sint<16>, decltype(-uint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(-sint<16>{})>);

static_assert(std::is_same_v<uint<17>, decltype(uint<16>{} + uint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(uint<16>{} + sint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(sint<16>{} + uint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(sint<16>{} + sint<16>{})>);

static_assert(std::is_same_v<uint<25>, decltype(uint<24>{} + uint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(uint<24>{} + sint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint<24>{} + uint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint<24>{} + sint< 8>{})>);

static_assert(std::is_same_v<uint<25>, decltype(uint< 8>{} + uint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(uint< 8>{} + sint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint< 8>{} + uint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint< 8>{} + sint<24>{})>);

static_assert(std::is_same_v<sint<17>, decltype(uint<16>{} - uint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(uint<16>{} - sint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(sint<16>{} - uint<16>{})>);
static_assert(std::is_same_v<sint<17>, decltype(sint<16>{} - sint<16>{})>);

static_assert(std::is_same_v<sint<25>, decltype(uint<24>{} - uint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(uint<24>{} - sint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint<24>{} - uint< 8>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint<24>{} - sint< 8>{})>);

static_assert(std::is_same_v<sint<25>, decltype(uint< 8>{} - uint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(uint< 8>{} - sint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint< 8>{} - uint<24>{})>);
static_assert(std::is_same_v<sint<25>, decltype(sint< 8>{} - sint<24>{})>);

static_assert(std::is_same_v<uint<32>, decltype(uint<16>{} * uint<16>{})>);
static_assert(std::is_same_v<sint<32>, decltype(uint<16>{} * sint<16>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint<16>{} * uint<16>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint<16>{} * sint<16>{})>);

static_assert(std::is_same_v<uint<32>, decltype(uint<24>{} * uint< 8>{})>);
static_assert(std::is_same_v<sint<32>, decltype(uint<24>{} * sint< 8>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint<24>{} * uint< 8>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint<24>{} * sint< 8>{})>);

static_assert(std::is_same_v<uint<32>, decltype(uint< 8>{} * uint<24>{})>);
static_assert(std::is_same_v<sint<32>, decltype(uint< 8>{} * sint<24>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint< 8>{} * uint<24>{})>);
static_assert(std::is_same_v<sint<32>, decltype(sint< 8>{} * sint<24>{})>);

static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} / uint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(uint<16>{} / sint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} / uint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} / sint<16>{})>);

static_assert(std::is_same_v<uint<24>, decltype(uint<24>{} / uint< 8>{})>);
static_assert(std::is_same_v<sint<24>, decltype(uint<24>{} / sint< 8>{})>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} / uint< 8>{})>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} / sint< 8>{})>);

static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} / uint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(uint< 8>{} / sint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} / uint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} / sint<24>{})>);

static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} % uint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(uint<16>{} % sint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} % uint<16>{})>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} % sint<16>{})>);

static_assert(std::is_same_v<uint< 8>, decltype(uint<24>{} % uint< 8>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(uint<24>{} % sint< 8>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint<24>{} % uint< 8>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint<24>{} % sint< 8>{})>);

static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} % uint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(uint< 8>{} % sint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} % uint<24>{})>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} % sint<24>{})>);

static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} << 16)>);
static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} << 16)>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} << 16)>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} << 16)>);

static_assert(std::is_same_v<uint<24>, decltype(uint<24>{} <<  8)>);
static_assert(std::is_same_v<uint<24>, decltype(uint<24>{} <<  8)>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} <<  8)>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} <<  8)>);

static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} << 24)>);
static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} << 24)>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} << 24)>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} << 24)>);

static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} >> 16)>);
static_assert(std::is_same_v<uint<16>, decltype(uint<16>{} >> 16)>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} >> 16)>);
static_assert(std::is_same_v<sint<16>, decltype(sint<16>{} >> 16)>);

static_assert(std::is_same_v<uint<24>, decltype(uint<24>{} >>  8)>);
static_assert(std::is_same_v<uint<24>, decltype(uint<24>{} >>  8)>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} >>  8)>);
static_assert(std::is_same_v<sint<24>, decltype(sint<24>{} >>  8)>);

static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} >> 24)>);
static_assert(std::is_same_v<uint< 8>, decltype(uint< 8>{} >> 24)>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} >> 24)>);
static_assert(std::is_same_v<sint< 8>, decltype(sint< 8>{} >> 24)>);

static_assert(std::is_same_v<uint<16>, decltype(abs(uint<16>{}))>);
static_assert(std::is_same_v<uint<17>, decltype(abs(sint<16>{}))>);

static_assert(std::is_same_v<uint<16>, decltype(max(uint<16>{}, uint<16>{}))>);
static_assert(std::is_same_v<uint<16>, decltype(max(uint<16>{}, sint<16>{}))>);
static_assert(std::is_same_v<uint<16>, decltype(max(sint<16>{}, uint<16>{}))>);
static_assert(std::is_same_v<sint<16>, decltype(max(sint<16>{}, sint<16>{}))>);

static_assert(std::is_same_v<uint<24>, decltype(max(uint<24>{}, uint< 8>{}))>);
static_assert(std::is_same_v<uint<24>, decltype(max(uint<24>{}, sint< 8>{}))>);
static_assert(std::is_same_v<uint<24>, decltype(max(sint<24>{}, uint< 8>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(max(sint<24>{}, sint< 8>{}))>);

static_assert(std::is_same_v<uint<24>, decltype(max(uint< 8>{}, uint<24>{}))>);
static_assert(std::is_same_v<uint<24>, decltype(max(uint< 8>{}, sint<24>{}))>);
static_assert(std::is_same_v<uint<24>, decltype(max(sint< 8>{}, uint<24>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(max(sint< 8>{}, sint<24>{}))>);

static_assert(std::is_same_v<uint<16>, decltype(min(uint<16>{}, uint<16>{}))>);
static_assert(std::is_same_v<sint<16>, decltype(min(uint<16>{}, sint<16>{}))>);
static_assert(std::is_same_v<sint<16>, decltype(min(sint<16>{}, uint<16>{}))>);
static_assert(std::is_same_v<sint<16>, decltype(min(sint<16>{}, sint<16>{}))>);

static_assert(std::is_same_v<uint<24>, decltype(min(uint<24>{}, uint< 8>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(uint<24>{}, sint< 8>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(sint<24>{}, uint< 8>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(sint<24>{}, sint< 8>{}))>);

static_assert(std::is_same_v<uint<24>, decltype(min(uint< 8>{}, uint<24>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(uint< 8>{}, sint<24>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(sint< 8>{}, uint<24>{}))>);
static_assert(std::is_same_v<sint<24>, decltype(min(sint< 8>{}, sint<24>{}))>);

static_assert(!is_power_of_two_v< 0>);
static_assert( is_power_of_two_v< 1>);
static_assert( is_power_of_two_v< 2>);
static_assert(!is_power_of_two_v< 3>);
static_assert( is_power_of_two_v< 4>);
static_assert(!is_power_of_two_v< 5>);
static_assert(!is_power_of_two_v< 6>);
static_assert(!is_power_of_two_v< 7>);
static_assert( is_power_of_two_v< 8>);
static_assert(!is_power_of_two_v< 9>);
static_assert(!is_power_of_two_v<10>);
static_assert(!is_power_of_two_v<11>);
static_assert(!is_power_of_two_v<12>);
static_assert(!is_power_of_two_v<13>);
static_assert(!is_power_of_two_v<14>);
static_assert(!is_power_of_two_v<15>);
static_assert( is_power_of_two_v<16>);
static_assert(!is_power_of_two_v<17>);
static_assert(!is_power_of_two_v<18>);
static_assert(!is_power_of_two_v<19>);

using namespace literals;

static_assert(std::is_same_v<integer<uint_fast8_t,  7>, decltype(0b0000000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t,  7>, decltype(0b1111111_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(0b000000000000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(0b111111111111_ivec)>);
static_assert(std::is_same_v<integer<     uint8_t,  7>, decltype(0b0000000_ivec8)>);
static_assert(std::is_same_v<integer<    uint16_t,  7>, decltype(0b1111111_ivec16)>);
static_assert(std::is_same_v<integer<    uint32_t, 12>, decltype(0b000000000000_ivec32)>);
static_assert(std::is_same_v<integer<    uint64_t, 12>, decltype(0b111111111111_ivec64)>);

static_assert(std::is_same_v<integer<uint_fast8_t,  6>, decltype(000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t,  6>, decltype(077_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(00000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(07777_ivec)>);
static_assert(std::is_same_v<integer<     uint8_t,  6>, decltype(000_ivec8)>);
static_assert(std::is_same_v<integer<    uint16_t,  6>, decltype(077_ivec16)>);
static_assert(std::is_same_v<integer<    uint32_t, 12>, decltype(00000_ivec32)>);
static_assert(std::is_same_v<integer<    uint64_t, 12>, decltype(07777_ivec64)>);

static_assert(std::is_same_v<integer<uint_fast8_t,  7>, decltype(10_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t,  7>, decltype(99_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 14>, decltype(1000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 14>, decltype(9999_ivec)>);
static_assert(std::is_same_v<integer<     uint8_t,  7>, decltype(10_ivec8)>);
static_assert(std::is_same_v<integer<    uint16_t,  7>, decltype(99_ivec16)>);
static_assert(std::is_same_v<integer<    uint32_t, 14>, decltype(1000_ivec32)>);
static_assert(std::is_same_v<integer<    uint64_t, 14>, decltype(9999_ivec64)>);

static_assert(std::is_same_v<integer<uint_fast8_t,  8>, decltype(0x00_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t,  8>, decltype(0xff_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(0x000_ivec)>);
static_assert(std::is_same_v<integer<uint_fast8_t, 12>, decltype(0xfff_ivec)>);
static_assert(std::is_same_v<integer<     uint8_t,  8>, decltype(0X00_ivec8)>);
static_assert(std::is_same_v<integer<    uint16_t,  8>, decltype(0XFF_ivec16)>);
static_assert(std::is_same_v<integer<    uint32_t, 12>, decltype(0X000_ivec32)>);
static_assert(std::is_same_v<integer<    uint64_t, 12>, decltype(0XFFF_ivec64)>);

template <integer_variant T, integral ... Ts>
constexpr bool expect(T const & val, Ts const ... values) {
	T const want{scith::ivec::ctag::raw, values ...};
	return val == want;
}

static_assert(expect(0b0_ivec8, 0));
static_assert(expect(0B1_ivec8, 1));
static_assert(expect(0b11110100001001000000_ivec8, 0x40, 0x42, 0x0f));
static_assert(expect(0b11110100001001000000_ivec16, 0x4240, 0x000f));
static_assert(expect(0b1111111111111111111111111111111111111111111111111111111111111111_ivec16,
 0xffff, 0xffff, 0xffff, 0xffff));

static_assert(expect(0_ivec8, 0));
static_assert(expect(01_ivec8, 1));
static_assert(expect(03641100_ivec8, 0x40, 0x42, 0x0f));
static_assert(expect(03641100_ivec16, 0x4240, 0x000f));
static_assert(expect(01777777777777777777777_ivec16, 0xffff, 0xffff, 0xffff, 0xffff));

static_assert(expect(1_ivec8, 1));
static_assert(expect(1000000_ivec8, 0x40, 0x42, 0x0f));
static_assert(expect(1000000_ivec16, 0x4240, 0x000f));
static_assert(expect(18446744073709551615_ivec16, 0xffff, 0xffff, 0xffff, 0xffff));

static_assert(expect(0x0_ivec8, 0));
static_assert(expect(0x1_ivec8, 1));
static_assert(expect(0xf4240_ivec8, 0x40, 0x42, 0x0f));
static_assert(expect(0Xf4240_ivec16, 0x4240, 0x000f));
static_assert(expect(0xfFfFfFfFfFfFfFfF_ivec16, 0xffff, 0xffff, 0xffff, 0xffff));

/* isnan */

static_assert(!isnan(uint<24>{ctag::raw, 0x00ffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0x01ffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0x0fffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0xffffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0xff000000}));
static_assert( isnan(uint<24>{ctag::raw, 0xccffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0x55ffffff}));
static_assert( isnan(uint<24>{ctag::raw, 0x80ffffff}));

static_assert(!isnan(sint<24>{ctag::raw, 0x00ffffff}));
static_assert( isnan(sint<24>{ctag::raw, 0x01ffffff}));
static_assert( isnan(sint<24>{ctag::raw, 0x0fffffff}));
static_assert(!isnan(sint<24>{ctag::raw, 0xffffffff}));
static_assert(!isnan(sint<24>{ctag::raw, 0xff000000}));
static_assert( isnan(sint<24>{ctag::raw, 0xccffffff}));
static_assert( isnan(sint<24>{ctag::raw, 0x55ffffff}));
static_assert( isnan(sint<24>{ctag::raw, 0x80ffffff}));

/* comparison */

static_assert( ( 123_ivec8 ==  123));
static_assert(!( 123_ivec8 ==  124));
static_assert(!( 123_ivec8 == -123));
static_assert( (-  0_ivec8 ==    0));
static_assert( (-123_ivec8 == -123));
static_assert(!(-123_ivec8 == -124));
static_assert(!(-123_ivec8 ==  123));
static_assert( (   0_ivec8 ==    0));

static_assert(!( 123_ivec8 !=  123));
static_assert( ( 123_ivec8 !=  124));
static_assert( ( 123_ivec8 != -123));
static_assert(!(-  0_ivec8 !=    0));
static_assert(!(-123_ivec8 != -123));
static_assert( (-123_ivec8 != -124));
static_assert( (-123_ivec8 !=  123));
static_assert(!(   0_ivec8 !=    0));

static_assert(!( 123_ivec8 <   123));
static_assert( ( 123_ivec8 <   124));
static_assert(!( 123_ivec8 <  -123));
static_assert(!(-  0_ivec8 <     0));
static_assert(!(-123_ivec8 <  -123));
static_assert(!(-123_ivec8 <  -124));
static_assert( (-123_ivec8 <   123));
static_assert(!(   0_ivec8 <     0));

static_assert( ( 123_ivec8 <=  123));
static_assert( ( 123_ivec8 <=  124));
static_assert(!( 123_ivec8 <= -123));
static_assert( (-  0_ivec8 <=    0));
static_assert( (-123_ivec8 <= -123));
static_assert(!(-123_ivec8 <= -124));
static_assert( (-123_ivec8 <=  123));
static_assert( (   0_ivec8 <=    0));

static_assert(!( 123_ivec8 >   123));
static_assert(!( 123_ivec8 >   124));
static_assert( ( 123_ivec8 >  -123));
static_assert(!(-  0_ivec8 >     0));
static_assert(!(-123_ivec8 >  -123));
static_assert( (-123_ivec8 >  -124));
static_assert(!(-123_ivec8 >   123));
static_assert(!(   0_ivec8 >     0));

static_assert( ( 123_ivec8 >=  123));
static_assert(!( 123_ivec8 >=  124));
static_assert( ( 123_ivec8 >= -123));
static_assert( (-  0_ivec8 >=    0));
static_assert( (-123_ivec8 >= -123));
static_assert( (-123_ivec8 >= -124));
static_assert(!(-123_ivec8 >=  123));
static_assert( (   0_ivec8 >=    0));

/* operator + */

static_assert( 1_ivec == ( 1_ivec8 + ( 0_ivec8)));
static_assert( 0_ivec == (-1_ivec8 + ( 1_ivec8)));
static_assert( 2_ivec == ( 1_ivec8 + ( 1_ivec8)));
static_assert( 3_ivec == ( 1_ivec8 + ( 2_ivec8)));
static_assert(-2_ivec == (-1_ivec8 + (-1_ivec8)));
static_assert(-3_ivec == (-1_ivec8 + (-2_ivec8)));

static_assert( 0x10000000000000000_ivec == ( 0xffffffffffffffff_ivec8 + ( 1)));
static_assert( 0x1fffffffffffffffe_ivec == ( 0xffffffffffffffff_ivec8 + ( 0xffffffffffffffff_ivec8)));
static_assert( 0x10202020202020201_ivec == ( 0xf0f0f0f0f0f0f0f0_ivec8 + ( 0x1111111111111111_ivec8)));
static_assert(  0xfffffffffffffffe_ivec == ( 0xffffffffffffffff_ivec8 + (-1)));
static_assert(                 0x0_ivec == ( 0xffffffffffffffff_ivec8 + (-0xffffffffffffffff_ivec8)));
static_assert(  0xdfdfdfdfdfdfdfdf_ivec == ( 0xf0f0f0f0f0f0f0f0_ivec8 + (-0x1111111111111111_ivec8)));
static_assert(- 0xfffffffffffffffe_ivec == (-0xffffffffffffffff_ivec8 + ( 1)));
static_assert(                 0x0_ivec == (-0xffffffffffffffff_ivec8 + ( 0xffffffffffffffff_ivec8)));
static_assert(- 0xdfdfdfdfdfdfdfdf_ivec == (-0xf0f0f0f0f0f0f0f0_ivec8 + ( 0x1111111111111111_ivec8)));
static_assert(-0x10000000000000000_ivec == (-0xffffffffffffffff_ivec8 + (-1)));
static_assert(-0x1fffffffffffffffe_ivec == (-0xffffffffffffffff_ivec8 + (-0xffffffffffffffff_ivec8)));
static_assert(-0x10202020202020201_ivec == (-0xf0f0f0f0f0f0f0f0_ivec8 + (-0x1111111111111111_ivec8)));

/* operator - */

static_assert( 1_ivec == ( 1_ivec8 - ( 0_ivec8)));
static_assert(-2_ivec == (-1_ivec8 - ( 1_ivec8)));
static_assert( 0_ivec == ( 1_ivec8 - ( 1_ivec8)));
static_assert(-1_ivec == ( 1_ivec8 - ( 2_ivec8)));
static_assert( 0_ivec == (-1_ivec8 - (-1_ivec8)));
static_assert( 1_ivec == (-1_ivec8 - (-2_ivec8)));

static_assert(  0xfffffffffffffffe_ivec == ( 0xffffffffffffffff_ivec8 - ( 1)));
static_assert(                 0x0_ivec == ( 0xffffffffffffffff_ivec8 - ( 0xffffffffffffffff_ivec8)));
static_assert(  0xdfdfdfdfdfdfdfdf_ivec == ( 0xf0f0f0f0f0f0f0f0_ivec8 - ( 0x1111111111111111_ivec8)));
static_assert( 0x10000000000000000_ivec == ( 0xffffffffffffffff_ivec8 - (-1)));
static_assert( 0x1fffffffffffffffe_ivec == ( 0xffffffffffffffff_ivec8 - (-0xffffffffffffffff_ivec8)));
static_assert( 0x10202020202020201_ivec == ( 0xf0f0f0f0f0f0f0f0_ivec8 - (-0x1111111111111111_ivec8)));
static_assert(-0x10000000000000000_ivec == (-0xffffffffffffffff_ivec8 - ( 1)));
static_assert(-0x1fffffffffffffffe_ivec == (-0xffffffffffffffff_ivec8 - ( 0xffffffffffffffff_ivec8)));
static_assert(-0x10202020202020201_ivec == (-0xf0f0f0f0f0f0f0f0_ivec8 - ( 0x1111111111111111_ivec8)));
static_assert(- 0xfffffffffffffffe_ivec == (-0xffffffffffffffff_ivec8 - (-1)));
static_assert(-                0x0_ivec == (-0xffffffffffffffff_ivec8 - (-0xffffffffffffffff_ivec8)));
static_assert(- 0xdfdfdfdfdfdfdfdf_ivec == (-0xf0f0f0f0f0f0f0f0_ivec8 - (-0x1111111111111111_ivec8)));

/* operator * */

static_assert(           1_ivec == ( 1_ivec8) * ( 1_ivec8));
static_assert(           2_ivec == ( 1_ivec8) * ( 2_ivec8));
static_assert(           8_ivec == ( 1_ivec8) * ( 8_ivec8));
static_assert( 0x100000000_ivec == ( 1_ivec8) * ( 0x100000000_ivec8));
static_assert(-          1_ivec == ( 1_ivec8) * (-1_ivec8));
static_assert(-          2_ivec == ( 1_ivec8) * (-2_ivec8));
static_assert(-          8_ivec == ( 1_ivec8) * (-8_ivec8));
static_assert(-0x100000000_ivec == ( 1_ivec8) * (-0x100000000_ivec8));
static_assert(-          1_ivec == (-1_ivec8) * ( 1_ivec8));
static_assert(-          2_ivec == (-1_ivec8) * ( 2_ivec8));
static_assert(-          8_ivec == (-1_ivec8) * ( 8_ivec8));
static_assert(-0x100000000_ivec == (-1_ivec8) * ( 0x100000000_ivec8));
static_assert(           1_ivec == (-1_ivec8) * (-1_ivec8));
static_assert(           2_ivec == (-1_ivec8) * (-2_ivec8));
static_assert(           8_ivec == (-1_ivec8) * (-8_ivec8));
static_assert( 0x100000000_ivec == (-1_ivec8) * (-0x100000000_ivec8));

static_assert(  0xffffffff0000_ivec == ( 0xffff_ivec8) * ( 0x100010000_ivec8));
static_assert( 0x10000fffe0000_ivec == ( 0xffff_ivec8) * ( 0x100020000_ivec8));
static_assert(  0xfffeffff0001_ivec == ( 0xffff_ivec8) * (  0xffffffff_ivec8));
static_assert(- 0xffffffff0000_ivec == ( 0xffff_ivec8) * (-0x100010000_ivec8));
static_assert(-0x10000fffe0000_ivec == ( 0xffff_ivec8) * (-0x100020000_ivec8));
static_assert(- 0xfffeffff0001_ivec == ( 0xffff_ivec8) * (- 0xffffffff_ivec8));
static_assert(- 0xffffffff0000_ivec == (-0xffff_ivec8) * ( 0x100010000_ivec8));
static_assert(-0x10000fffe0000_ivec == (-0xffff_ivec8) * ( 0x100020000_ivec8));
static_assert(- 0xfffeffff0001_ivec == (-0xffff_ivec8) * (  0xffffffff_ivec8));
static_assert(  0xffffffff0000_ivec == (-0xffff_ivec8) * (-0x100010000_ivec8));
static_assert( 0x10000fffe0000_ivec == (-0xffff_ivec8) * (-0x100020000_ivec8));
static_assert(  0xfffeffff0001_ivec == (-0xffff_ivec8) * (- 0xffffffff_ivec8));

static_assert( 299_ivec == ( 13_ivec8) * ( 23_ivec8));
static_assert(-299_ivec == ( 13_ivec8) * (-23_ivec8));
static_assert(-299_ivec == (-13_ivec8) * ( 23_ivec8));
static_assert( 299_ivec == (-13_ivec8) * (-23_ivec8));

/* abs() */

static_assert(0_ivec                    == abs( 0_ivec8));
static_assert(1_ivec                    == abs( 1_ivec8));
static_assert(1000000_ivec              == abs( 1000000_ivec8));
static_assert(1000000_ivec              == abs( 1000000_ivec16));
static_assert(18446744073709551615_ivec == abs(+18446744073709551615_ivec16));
static_assert(0_ivec                    == abs(-0_ivec8));
static_assert(1_ivec                    == abs(-1_ivec8));
static_assert(1000000_ivec              == abs(-1000000_ivec8));
static_assert(1000000_ivec              == abs(-1000000_ivec16));
static_assert(18446744073709551615_ivec == abs(-18446744073709551615_ivec16));

/* max() */

static_assert(    3_ivec == max(3_ivec8));
static_assert(    3_ivec == max(3_ivec8, 2, 1, 0, -1, -100));
static_assert(    3_ivec == max(2, 3_ivec8, 1, 0, -1, -100));
static_assert(    3_ivec == max(2, 1, 3_ivec8, 0, -1, -100));
static_assert(   10_ivec == max(3, 2, 1, 10, -1, -100_ivec8));
static_assert(  100_ivec == max(3, 2, 1, 0, -1, 100_ivec8));
static_assert(    0_ivec == max(-3, -2_ivec8, -1, 0, -1, -100));
static_assert(-  55_ivec == max(-120_ivec8, -89, -500, -55, -1000));
static_assert( 1000_ivec == max(120_ivec8, -89, 1000, -500, -55, -1000));

/* min() */

static_assert(-   3_ivec == min(-3_ivec8));
static_assert(-   3_ivec == min(-3_ivec8, -2, -1, 0, 1, 100));
static_assert(-   3_ivec == min(-2, -3_ivec8, -1, 0, 1, 100));
static_assert(-   3_ivec == min(-2, -1, -3_ivec8, 0, 1, 100));
static_assert(-  10_ivec == min(-3, -2, -1, -10, 1, 100_ivec8));
static_assert(- 100_ivec == min(-3, -2, -1, 0, 1, -100_ivec8));
static_assert(    0_ivec == min(3, 2_ivec8, 1, 0, 1, 100));
static_assert(   55_ivec == min(120_ivec8, 89, 500, 55, 1000));
static_assert(-1000_ivec == min(-120_ivec8, 89, -1000, 500, 55, 1000));

} /* namespace */
