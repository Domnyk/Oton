#ifndef HEADERTEST_H
#define HEADERTEST_H

#include <QtTest/QtTest>
#include <../src/network/protocol/Header.hpp>

class HeaderTest: public QObject
{
    Q_OBJECT

private slots:
    void when_data_is_correct_it_should_properly_parse_msg_type();
    void when_data_is_correct_it_should_properly_parse_body_len();
    void when_data_is_correct_it_should_properly_parse_num_of_cols();
    void when_data_is_correct_it_should_properly_parse_num_of_rows();
    void when_data_is_correct_it_should_properly_parse_num_of_frames();
    void when_data_is_correct_it_should_properly_parse_frame_num();

    void encode_constructor_should_set_proper_values();
    void should_properly_encode_header();
};

#endif // HEADERTEST_H
