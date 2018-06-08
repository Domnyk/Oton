#include <iostream>
#include <array>
#include "HeaderTest.hpp"
#include "../src/network/protocol/message_type.hpp"

using namespace std;

void HeaderTest::when_data_is_correct_it_should_properly_parse_msg_type() {
    const unsigned short NUM_OF_MSG_TYPES = 7;
    const unsigned short POS_OF_MSG_TYPE = 1;
    const unsigned short SHIFT_FOR_ASCII = 48;

    for (int i = 0; i < NUM_OF_MSG_TYPES; ++i) {
        auto expected_value = static_cast<protocol::message_type>(i);
        std::array<char, protocol::HEADER_LENGTH> header_data{{'0'}};
        header_data.fill('0');
        header_data.at(POS_OF_MSG_TYPE) = static_cast<char>(i + SHIFT_FOR_ASCII);

        protocol::Header header;
        header.parse(header_data.data());

        QCOMPARE(header.get_msg_type(), expected_value);
    }
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_body_len() {
    const unsigned short POS_OF_BODY_LEN = 8;
    const unsigned short expected_value = 10;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_BODY_LEN) = '1';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_body_len(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_cols() {
    const unsigned short POS_OF_NUM_OF_COLS = 10;
    const unsigned short expected_value = 9999;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_NUM_OF_COLS) = '9';
    header_data.at(POS_OF_NUM_OF_COLS + 1) = '9';
    header_data.at(POS_OF_NUM_OF_COLS + 2) = '9';
    header_data.at(POS_OF_NUM_OF_COLS + 3) = '9';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_num_of_cols(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_rows() {
    const unsigned short POS_OF_NUM_OF_ROWS = 14;
    const unsigned short expected_value = 9999;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_NUM_OF_ROWS) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 1) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 2) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 3) = '9';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_num_of_rows(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_frames() {
    const unsigned short POS_OF_NUM_OF_ROWS = 18;
    const unsigned int expected_value = 99999999;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_NUM_OF_ROWS) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 1) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 2) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 3) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 4) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 5) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 6) = '9';
    header_data.at(POS_OF_NUM_OF_ROWS + 7) = '9';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_num_of_frames(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_frame_num() {
    const unsigned short POS_OF_FRAME_NUM = 26;
    const unsigned int expected_value = 99999999;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_FRAME_NUM) = '9';
    header_data.at(POS_OF_FRAME_NUM + 1) = '9';
    header_data.at(POS_OF_FRAME_NUM + 2) = '9';
    header_data.at(POS_OF_FRAME_NUM + 3) = '9';
    header_data.at(POS_OF_FRAME_NUM + 4) = '9';
    header_data.at(POS_OF_FRAME_NUM + 5) = '9';
    header_data.at(POS_OF_FRAME_NUM + 6) = '9';
    header_data.at(POS_OF_FRAME_NUM + 7) = '9';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_frame_num(), expected_value);
}

void HeaderTest::when_frame_is_keyframe_parse_method_should_return_true() {
    const unsigned short POS_OF_IS_KEY_FRAME = 34;
    const bool expected_value = true;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_IS_KEY_FRAME) = '1';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_is_key_frame(), expected_value);
}

void HeaderTest::when_frame_is_not_keyframe_parse_method_should_return_false() {
    const unsigned short POS_OF_IS_KEY_FRAME = 34;
    const bool expected_value = false;
    std::array<char, protocol::HEADER_LENGTH> header_data{{ '0' }};
    header_data.fill('0');
    header_data.at(POS_OF_IS_KEY_FRAME) = '0';

    protocol::Header header;
    header.parse(header_data.data());

    QCOMPARE(header.get_is_key_frame(), expected_value);
}

void HeaderTest::should_properly_encode_header() {
    protocol::message_type msg_type = protocol::GET_MOVIE_LIST;
    unsigned int body_len = 1000;
    unsigned short num_of_cols = 256;
    unsigned short num_of_rows = 144;
    unsigned int num_of_frames = 1000;
    unsigned int frame_num = 1;
    bool is_key_frame = true;

    protocol::Header header;
    header.set_msg_type(msg_type);
    header.set_body_len(body_len);
    header.set_num_of_cols(num_of_cols);
    header.set_num_of_rows(num_of_rows);
    header.set_num_of_frames(num_of_frames);
    header.set_frame_num(frame_num);
    header.set_is_key_frame(is_key_frame);

    std::string encoded_header = header.encode();
    header.parse(encoded_header.data());

    QCOMPARE(header.get_msg_type(), msg_type);
    QCOMPARE(header.get_body_len(), body_len);
    QCOMPARE(header.get_num_of_cols(), num_of_cols);
    QCOMPARE(header.get_num_of_rows(), num_of_rows);
    QCOMPARE(header.get_num_of_frames(), num_of_frames);
    QCOMPARE(header.get_frame_num(), frame_num);
    QCOMPARE(header.get_is_key_frame(), is_key_frame);
}




