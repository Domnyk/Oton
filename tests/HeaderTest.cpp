#include <iostream>
#include "HeaderTest.hpp"
#include "../src/network/protocol/message_type.hpp"

using namespace std;

void HeaderTest::when_data_is_correct_it_should_properly_parse_msg_type() {
    const short NUM_OF_MSG_TYPES = 7;

    for (int i = 0; i < NUM_OF_MSG_TYPES; ++i) {
        auto expected_value = static_cast<protocol::message_type>(i);
        std::string str = "0" + to_string(i) + "00000000000000000000000000000000";
        char data[35];
        strcpy(data, str.c_str());

        protocol::Header header(data);
        header.parse_msg_type();

        QCOMPARE(header.get_msg_type(), expected_value);
    }
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_body_len() {
    unsigned short expected_value = 10;
    string str = "00000000" + to_string(expected_value) + "000000000000000000000000";
    char data[35];
    strcpy(data, str.c_str());

    protocol::Header header(data);
    header.parse_body_len();

    QCOMPARE(header.get_body_len(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_cols() {
    unsigned short expected_value = 9999;
    string str = "0122222222" + to_string(expected_value) + "44440000000000000000";
    char data[35];
    strcpy(data, str.c_str());

    protocol::Header header(data);
    header.parse_num_of_cols();

    QCOMPARE(header.get_num_of_cols(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_rows() {
    unsigned short expected_value = 9999;
    string str = "01000000001234" + to_string(expected_value) + "1111111100000000";
    char data[35];
    strcpy(data, str.c_str());

    protocol::Header header(data);
    header.parse_num_of_rows();

    QCOMPARE(header.get_num_of_rows(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_num_of_frames() {
    unsigned int expected_value = 99999999;
    string str = "010000000000006787" + to_string(expected_value) + "12345678";
    char data[35];
    strcpy(data, str.c_str());

    protocol::Header header(data);
    header.parse_num_of_frames();

    QCOMPARE(header.get_num_of_frames(), expected_value);
}

void HeaderTest::when_data_is_correct_it_should_properly_parse_frame_num() {
    unsigned int expected_value = 99999999;
    string str = "01000000000000678712345678" + to_string(expected_value);
    char data[35];
    strcpy(data, str.c_str());

    protocol::Header header(data);
    header.parse_frame_num();

    QCOMPARE(header.get_frame_num(), expected_value);
}

void HeaderTest::encode_constructor_should_set_proper_values() {
    char* data = new char[35];
    protocol::message_type msg_type = protocol::GET_MOVIE_LIST;
    unsigned int body_len = 1000;
    unsigned short num_of_cols = 256;
    unsigned short num_of_rows = 144;
    unsigned int num_of_frames = 1000;
    unsigned int frame_num = 1;

    protocol::Header header(data, msg_type, body_len, num_of_cols, num_of_rows, num_of_frames, frame_num);

    QCOMPARE(header.get_msg_type(), msg_type);
    QCOMPARE(header.get_body_len(), body_len);
    QCOMPARE(header.get_num_of_cols(), num_of_cols);
    QCOMPARE(header.get_num_of_rows(), num_of_rows);
    QCOMPARE(header.get_num_of_frames(), num_of_frames);
    QCOMPARE(header.get_frame_num(), frame_num);

    delete[] data;
}

void HeaderTest::should_properly_encode_header() {
    char* data = new char[35];
    protocol::message_type msg_type = protocol::GET_MOVIE_LIST;
    unsigned int body_len = 1000;
    unsigned short num_of_cols = 256;
    unsigned short num_of_rows = 144;
    unsigned int num_of_frames = 1000;
    unsigned int frame_num = 1;

    protocol::Header header(data, msg_type, body_len, num_of_cols, num_of_rows, num_of_frames, frame_num);
    header.encode();
    header.parse();

    QCOMPARE(header.get_msg_type(), msg_type);
    QCOMPARE(header.get_body_len(), body_len);
    QCOMPARE(header.get_num_of_cols(), num_of_cols);
    QCOMPARE(header.get_num_of_rows(), num_of_rows);
    QCOMPARE(header.get_num_of_frames(), num_of_frames);
    QCOMPARE(header.get_frame_num(), frame_num);

    delete[] data;
}




