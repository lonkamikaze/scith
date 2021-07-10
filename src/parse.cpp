#include "parse.hpp"

using namespace scith::parse;

namespace {

static_assert(0 == true_index());
static_assert(1 == true_index(0));
static_assert(0 == true_index(1));
static_assert(2 == true_index(0, 0));
static_assert(1 == true_index(0, 1));
static_assert(0 == true_index(1, 0));
static_assert(0 == true_index(1, 1));
static_assert(3 == true_index(0, 0, 0));
static_assert(2 == true_index(0, 0, 1));
static_assert(1 == true_index(0, 1, 0));
static_assert(1 == true_index(0, 1, 1));
static_assert(0 == true_index(1, 0, 0));
static_assert(0 == true_index(1, 0, 1));
static_assert(0 == true_index(1, 1, 0));
static_assert(0 == true_index(1, 1, 1));

static_assert(4 == match_index(1,  0, 0, 0, 0, 1, 0));
static_assert(1 == match_index(2,  0, 2, 0, 0, 1, 0));
static_assert(2 == match_index(2,  0, 0, 2, 2, 2, 2));
static_assert(6 == match_index(1,  0, 0, 0, 0, 0, 0));

static_assert(1 == prefix_index("foobarbaz",  "foobarbam", "foobar", "foo"));
static_assert(0 == prefix_index("foobarbaz",  "foo", "foobarbam", "foobar"));
static_assert(0 == prefix_index("foobarbaz",  'f', "foobarbam", "foobar", "foo"));
static_assert(3 == prefix_index("foxbarbaz",  "foobarbam", "foobar", "foo"));

static_assert('b' == tolower('b'));
static_assert('b' == tolower('B'));
static_assert('x' == tolower('x'));
static_assert('x' == tolower('X'));
static_assert('0' == tolower('0'));
static_assert('1' == tolower('1'));
static_assert('9' == tolower('9'));

static_assert("foobar1337" == literal_view<'F', 'o', 'O', 'b', 'a', 'R', '1', '3', '3', '7'>{});

static_assert( 8 == radix_v<'0'>);
static_assert(10 == radix_v<'1'>);
static_assert(10 == radix_v<'9'>);
static_assert( 0 == radix_v<'x'>);
static_assert( 0 == radix_v<'X'>);
static_assert( 0 == radix_v<'c'>);
static_assert( 0 == radix_v<'C'>);

static_assert( 2 == radix_v<'0', 'b'>);
static_assert( 2 == radix_v<'0', 'B'>);
static_assert( 2 == radix_v<'0', 'b', 'x', 'x'>);
static_assert( 2 == radix_v<'0', 'B', '1', '2'>);
static_assert( 8 == radix_v<'0', '1'>);
static_assert( 8 == radix_v<'0', 'a'>);
static_assert(16 == radix_v<'0', 'x'>);
static_assert(16 == radix_v<'0', 'X'>);
static_assert(16 == radix_v<'0', 'x', 'x', 'x'>);
static_assert(16 == radix_v<'0', 'X', '1', '2'>);

static_assert( 0 == digit('0'));
static_assert( 1 == digit('1'));
static_assert( 2 == digit('2'));
static_assert( 3 == digit('3'));
static_assert( 4 == digit('4'));
static_assert( 5 == digit('5'));
static_assert( 6 == digit('6'));
static_assert( 7 == digit('7'));
static_assert( 8 == digit('8'));
static_assert( 9 == digit('9'));
static_assert(10 == digit('a'));
static_assert(11 == digit('b'));
static_assert(12 == digit('c'));
static_assert(13 == digit('d'));
static_assert(14 == digit('e'));
static_assert(15 == digit('f'));

static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'0', 'b', '1', '2', 'f', 'c', '7'>);
static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'0', 'B', '1', '2', 'f', 'c', '7'>);
static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'0', '1', '2', 'f', 'c', '7'>);
static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'1', '2', 'f', 'c', '7'>);
static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'0', 'x', '1', '2', 'f', 'c', '7'>);
static_assert(std::array{1, 2, 15, 12, 7} == digits_intset<'0', 'X', '1', '2', 'f', 'c', '7'>);
static_assert(std::array{0}               == digits_intset<'0'>);
static_assert(std::array{0}               == digits_intset<'0', '0'>);
static_assert(std::array{0, 0}            == digits_intset<'0', '0', '0'>);

static_assert( 5 == digits_v<'0', 'b', '1', '2', 'f', 'c', '7'>);
static_assert( 5 == digits_v<'0', 'B', '1', '2', 'f', 'c', '7'>);
static_assert(15 == digits_v<'0', '1', '2', 'f', 'c', '7'>);
static_assert(17 == digits_v<'1', '2', 'f', 'c', '7'>);
static_assert(20 == digits_v<'0', 'x', '1', '2', 'f', 'c', '7'>);
static_assert(20 == digits_v<'0', 'X', '1', '2', 'f', 'c', '7'>);
static_assert( 3 == digits_v<'0'>);
static_assert( 3 == digits_v<'0', '0'>);
static_assert( 6 == digits_v<'0', '0', '0'>);

static_assert(!integer_literal<'F', 'o', 'O', 'b', 'a', 'R', '1', '3', '3', '7'>);
static_assert( integer_literal<'0'>);
static_assert( integer_literal<'1'>);
static_assert( integer_literal<'9'>);
static_assert(!integer_literal<'x'>);
static_assert(!integer_literal<'X'>);
static_assert(!integer_literal<'c'>);
static_assert(!integer_literal<'C'>);
static_assert(!integer_literal<'0', 'b'>);
static_assert(!integer_literal<'0', 'B'>);
static_assert(!integer_literal<'0', 'b', 'x', 'x'>);
static_assert(!integer_literal<'0', 'B', '1', '2'>);
static_assert( integer_literal<'0', '1'>);
static_assert(!integer_literal<'0', 'a'>);
static_assert(!integer_literal<'0', 'x'>);
static_assert(!integer_literal<'0', 'X'>);
static_assert(!integer_literal<'0', 'x', 'x', 'x'>);
static_assert( integer_literal<'0', 'X', '1', '2'>);
static_assert(!integer_literal<'0', 'b', '1', '2', 'f', 'c', '7'>);
static_assert(!integer_literal<'0', 'B', '1', '2', 'f', 'c', '7'>);
static_assert(!integer_literal<'0', '1', '2', 'f', 'c', '7'>);
static_assert(!integer_literal<'1', '2', 'f', 'c', '7'>);
static_assert( integer_literal<'0', 'x', '1', '2', 'f', 'c', '7'>);
static_assert( integer_literal<'0', 'X', '1', '2', 'f', 'c', '7'>);
static_assert( integer_literal<'0'>);
static_assert( integer_literal<'0', '0'>);
static_assert( integer_literal<'0', '0', '0'>);
/* radix 2 */
static_assert( integer_literal<'0', 'b', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', 'B', '0', '1', '1', '0', '1'>);
static_assert(!integer_literal<'0', 'B', '0', '1', '1', '2', '1'>);
/* radix 8 */
static_assert( integer_literal<'0', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', '7', '1', '3', '4', '1'>);
static_assert(!integer_literal<'0', '7', '1', '3', '8', '1'>);
/* radix 10 */
static_assert( integer_literal<'1', '1', '0', '1'>);
static_assert( integer_literal<'7', '1', '3', '4', '1'>);
static_assert( integer_literal<'7', '1', '3', '8', '1'>);
static_assert( integer_literal<'7', '9', '3', '8', '1'>);
static_assert(!integer_literal<'7', '9', '3', 'a', '1'>);
/* radix 16 */
static_assert( integer_literal<'0', 'x', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', 'X', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', 'x', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', 'X', '0', '1', '1', '0', '1'>);
static_assert( integer_literal<'0', 'x', '7', '1', '3', '4', '1'>);
static_assert( integer_literal<'0', 'X', '7', '1', '3', '4', '1'>);
static_assert( integer_literal<'0', 'x', '7', 'f', 'a', '4', '1'>);
static_assert( integer_literal<'0', 'X', '7', 'f', 'a', '4', '1'>);
static_assert(!integer_literal<'0', 'X', '7', 'f', 'a', '4', 'g'>);
/* no fraction */
static_assert(!integer_literal<'0', 'b', '0', '1', '1', '0', '1', '.'>);
static_assert(!integer_literal<'0', '7', '1', '3', '4', '1', '.'>);
static_assert(!integer_literal<'7', '9', '3', '8', '1', '.'>);
static_assert(!integer_literal<'0', 'x', '7', 'f', 'a', '4', '1', '.', '1'>);
static_assert(!integer_literal<'0', 'b', '0', '1', '1', '0', '1', '.', '1'>);
static_assert(!integer_literal<'0', '7', '1', '3', '4', '1', '.', '1'>);
static_assert(!integer_literal<'7', '9', '3', '8', '1', '.', '1'>);
static_assert(!integer_literal<'0', 'x', '7', 'f', 'a', '4', '1', '.', '1'>);
/* no exponent */
static_assert(!integer_literal<'0', 'b', '0', '1', '1', '0', '1', 'p', '1'>);
static_assert(!integer_literal<'0', '7', '1', '3', '4', '1', 'p', '1'>);
static_assert(!integer_literal<'7', '9', '3', '8', '1', 'e', '1'>);
static_assert(!integer_literal<'0', 'x', '7', 'f', 'a', '4', '1', 'p', '1'>);

} /* namespace */
