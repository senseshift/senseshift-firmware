#include <senseshift/utility.hpp>
#include <unity.h>

#include <senseshift/buffer.hpp>

using namespace SenseShift;

void test_ring_buffer(void)
{
    auto buffer = RingBuffer<char, 5>();

    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());

    buffer.push('a'); // [] => [a]
    TEST_ASSERT_EQUAL_UINT8(1, buffer.getLength());
    TEST_ASSERT_EQUAL_INT8('a', buffer.get(0));
    TEST_ASSERT_EQUAL_UINT8(-1, buffer.get(1));
    TEST_ASSERT_EQUAL_UINT8(-1, buffer.get(2));

    // pop value and check length
    TEST_ASSERT_EQUAL_INT8('a', buffer.pop()); // [a] => []
    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());

    // add 5 values and check length
    buffer.push('a'); // []           => [a]
    buffer.push('b'); // [a]          => [a, b]
    buffer.push('c'); // [a, b]       => [a, b, c]
    buffer.push('d'); // [a, b, c]    => [a, b, c, d]
    buffer.push('e'); // [a, b, c, d] => [a, b, c, d, e]
    TEST_ASSERT_EQUAL_UINT8(5, buffer.getLength());

    // check values
    TEST_ASSERT_EQUAL_INT8('a', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('b', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('c', buffer.get(2));
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(3));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(4));

    // check values after pop
    TEST_ASSERT_EQUAL_INT8('a', buffer.pop()); // [a, b, c, d, e] => [b, c, d, e]
    TEST_ASSERT_EQUAL_INT8('b', buffer.pop()); // [b, c, d, e]    => [c, d, e]

    TEST_ASSERT_EQUAL_INT8('c', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(2));

    // check clear
    buffer.clear();
    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(0));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(1));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(2));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(3));

    // check overflow
    buffer.push('c'); // []              => [c]
    buffer.push('d'); // [c]             => [c, d]
    buffer.push('e'); // [c, d]          => [c, d, e]
    buffer.push('f'); // [c, d, e]       => [c, d, e, f]
    buffer.push('g'); // [c, d, e, f]    => [c, d, e, f, g]
    buffer.push('h'); // [c, d, e, f, g] => [d, e, f, g, h]

    // Buffer must shift values to the left when it is full
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('f', buffer.get(2));
    TEST_ASSERT_EQUAL_INT8('g', buffer.get(3));
    TEST_ASSERT_EQUAL_INT8('h', buffer.get(4));

    // check underflow
    TEST_ASSERT_EQUAL_INT8('d', buffer.pop()); // [d, e, f, g, h] => [e, f, g, h]
    TEST_ASSERT_EQUAL_INT8('e', buffer.pop()); // [e, f, g, h]    => [f, g, h]
    TEST_ASSERT_EQUAL_INT8('f', buffer.pop()); // [f, g, h]       => [g, h]
    TEST_ASSERT_EQUAL_INT8('g', buffer.pop()); // [g, h]          => [h]
    TEST_ASSERT_EQUAL_INT8('h', buffer.pop()); // [h]             => []
}

void test_flat_buffer(void)
{
    auto buffer = FixedSizeBuffer<char, 5>();

    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());

    buffer.push('a'); // [] => [a]
    TEST_ASSERT_EQUAL_UINT8(1, buffer.getLength());
    TEST_ASSERT_EQUAL_INT8('a', buffer.get(0));
    TEST_ASSERT_EQUAL_UINT8(-1, buffer.get(1));
    TEST_ASSERT_EQUAL_UINT8(-1, buffer.get(2));

    // pop value and check length
    TEST_ASSERT_EQUAL_INT8('a', buffer.pop()); // [a] => []
    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());

    // add 5 values and check length
    buffer.push('a'); // []           => [a]
    buffer.push('b'); // [a]          => [a, b]
    buffer.push('c'); // [a, b]       => [a, b, c]
    buffer.push('d'); // [a, b, c]    => [a, b, c, d]
    buffer.push('e'); // [a, b, c, d] => [a, b, c, d, e]
    TEST_ASSERT_EQUAL_UINT8(5, buffer.getLength());

    // check values
    TEST_ASSERT_EQUAL_INT8('a', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('b', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('c', buffer.get(2));
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(3));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(4));

    // check values after pop
    TEST_ASSERT_EQUAL_INT8('a', buffer.pop()); // [a, b, c, d, e] => [b, c, d, e]
    TEST_ASSERT_EQUAL_INT8('b', buffer.pop()); // [b, c, d, e]    => [c, d, e]

    TEST_ASSERT_EQUAL_INT8('c', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(2));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(3));

    // check clear
    buffer.clear();
    TEST_ASSERT_EQUAL_UINT8(0, buffer.getLength());
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(0));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(1));
    TEST_ASSERT_EQUAL_INT8(-1, buffer.get(2));

    // check overflow
    buffer.push('c'); // []              => [c]
    buffer.push('d'); // [c]             => [c, d]
    buffer.push('e'); // [c, d]          => [c, d, e]
    buffer.push('f'); // [c, d, e]       => [c, d, e, f]
    buffer.push('g'); // [c, d, e, f]    => [c, d, e, f, g]
    buffer.push('h'); // [c, d, e, f, g] => [c, d, e, f, g]

    // Buffer must not change values when it is full
    TEST_ASSERT_EQUAL_INT8('c', buffer.get(0));
    TEST_ASSERT_EQUAL_INT8('d', buffer.get(1));
    TEST_ASSERT_EQUAL_INT8('e', buffer.get(2));
    TEST_ASSERT_EQUAL_INT8('f', buffer.get(3));
    TEST_ASSERT_EQUAL_INT8('g', buffer.get(4));
}

int process(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ring_buffer);
    RUN_TEST(test_flat_buffer);

    return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup(void)
{
    process();
}

void loop(void) {}

#else

int main(int argc, char** argv)
{
    return process();
}

#endif
