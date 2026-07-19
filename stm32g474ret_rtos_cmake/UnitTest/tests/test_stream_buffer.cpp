#include <iostream>
#include <string>

#include "stream.h"
#include "test_stream_buffer.h"

#include <gmock/gmock.h>
using ::testing::ElementsAreArray;



#define DEF_BUFFER_SIZE     16U

static Stream_t _stream;
static uint8_t _buf[DEF_BUFFER_SIZE+1];


void StreamBufferTest::SetUpTestSuite()
{
    STREAM_Initialize(&_stream, _buf, DEF_BUFFER_SIZE);
}

void StreamBufferTest::TearDownTestSuite()
{

}

// TEST_F(StreamBufferTest, FailTest)
// {
//     FAIL();
// }

TEST_F(StreamBufferTest, FlushAndEmpty)
{
    const uint8_t in[] = {0x00, 0x01, 0x02, 0x03};
    ASSERT_TRUE(STREAM_Write(&_stream, in, sizeof(in)));

    ASSERT_FALSE(STREAM_IsEmpty(&_stream));

    STREAM_Flush(&_stream);

    ASSERT_TRUE(STREAM_IsEmpty(&_stream));
}

TEST_F(StreamBufferTest, Write)
{
    STREAM_Flush(&_stream);

    const uint8_t expected[] = {0x00, 0x01, 0x02, 0x03};
    
    STREAM_Write(&_stream, expected, sizeof(expected));
    
    uint8_t actual[] = {_buf[0], _buf[1], _buf[2], _buf[3]};

    EXPECT_THAT(actual, ElementsAreArray(expected));
}

TEST_F(StreamBufferTest, ReadWrite)
{
    //RecordProperty("Goal", "Test number of byte received.\nTest compare bytes read.");

    STREAM_Flush(&_stream);

    const uint8_t in[] = {0x00, 0x01, 0x02, 0x03};
    STREAM_Write(&_stream, in, sizeof(in));

    uint8_t out[] = {0, 0, 0, 0, 0};
    uint16_t numberOfBytesRead = STREAM_Read(&_stream, out, sizeof(out));

    EXPECT_EQ(numberOfBytesRead, sizeof(in));
    
    std::vector<uint8_t> actual = {out[0], out[1], out[2], out[3]};
    std::vector<uint8_t> expected = {in[0], in[1], in[2], in[3]};

    EXPECT_EQ(actual, expected);
}

TEST_F(StreamBufferTest, Length)
{
    STREAM_Flush(&_stream);
    
    const uint8_t in[] = {0x00, 0x01, 0x02, 0x03};
    STREAM_Write(&_stream, in, sizeof(in));

    EXPECT_EQ(STREAM_Length(&_stream), static_cast<uint16_t>(sizeof(in)));
}

TEST_F(StreamBufferTest, Look)
{
    // Write 5 bytes
    // Read  4 bytes
    // expecte data read same as write
    // expecte number of bytes write unchanged

    STREAM_Flush(&_stream);
    
    const uint8_t in[] = {0x00, 0x01, 0x02, 0x03, 0x04};
    STREAM_Write(&_stream, in, sizeof(in));

    uint8_t actual[] = {0, 0, 0, 0};
    uint16_t offset = 1;
    STREAM_Look(&_stream, offset, actual, sizeof(actual));

    const uint8_t expected[] = {0x01, 0x02, 0x03, 0x04};

    EXPECT_THAT(actual, ElementsAreArray(expected));

    EXPECT_EQ(STREAM_Length(&_stream), static_cast<uint16_t>(sizeof(in)));
}

TEST_F(StreamBufferTest, Wraparound)
{
    STREAM_Flush(&_stream);
    
    // 15 items +
    const uint8_t in[] = { 
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E
    };
    STREAM_Write(&_stream, in, sizeof(in));

    EXPECT_EQ(STREAM_Length(&_stream), static_cast<uint16_t>(sizeof(in)));

    // 6 items -
    uint8_t out[] = {0, 0, 0, 0, 0, 0};
    uint16_t numberOfBytesRead = STREAM_Read(&_stream, out, static_cast<uint16_t>(sizeof(out)));

    EXPECT_EQ(numberOfBytesRead, static_cast<uint16_t>(sizeof(out)));

    std::vector<uint8_t> actual_1 = { out[0], out[1], out[2], out[3], out[4], out[5] };
    std::vector<uint8_t> expected_1 = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    EXPECT_EQ(actual_1, expected_1);

    // 5 items +
    const uint8_t extra_in[] = { 0x0F, 0x10, 0x11, 0x12, 0x13 };
    STREAM_Write(&_stream, extra_in, sizeof(extra_in));

    uint16_t count_expected = static_cast<uint16_t>(sizeof(in) - sizeof(out) + sizeof(extra_in));
    EXPECT_EQ(STREAM_Length(&_stream), count_expected);

    // 15 - 6 + 5 = 14
    uint8_t look[14] = { 0 };
    STREAM_Look(&_stream, 0, look, sizeof(look));

    const uint8_t look_expected[] = { 
        0x06, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D,
        0x0E, 0x0F, 0x10, 0x11,
        0x12, 0x13
    };

    EXPECT_THAT(look, ElementsAreArray(look_expected));
}
