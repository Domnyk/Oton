#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QtTest/QtTest>
#include "FrameTest.hpp"
#include "../src/movie/Resolution.hpp"

using namespace cv;

void FrameTest::it_should_clone_data_matrix() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat data_matrix = imread(path_to_file, IMREAD_COLOR);
    if (data_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }
    unsigned expected_num_of_cols = data_matrix.cols;
    unsigned expected_num_of_rows = data_matrix.rows;

    unsigned frame_num = 0;
    Frame frame(frame_num, data_matrix);

    QCOMPARE(frame.getPixelMatrix().cols, expected_num_of_cols);
    QCOMPARE(frame.getPixelMatrix().rows, expected_num_of_rows);
    bool is_memory_same = (memcmp(frame.getPixelMatrix().data, data_matrix.data, *data_matrix.size) == 0);
    QVERIFY(is_memory_same == true);
    bool is_diffrent_addr = (frame.getPixelMatrix().data != data_matrix.data);
    QVERIFY(is_diffrent_addr == true);
}

void FrameTest::it_should_throw_exception_when_resizing_to_bigger_resolution() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat data_matrix = imread(path_to_file, IMREAD_COLOR);
    if (data_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }

    unsigned frame_num = 0;
    Frame frame(frame_num, data_matrix);

    /*
     * Dimensions of loaded image: 320 x 320 pixels
     */
    QVERIFY_EXCEPTION_THROWN(frame.resize(Resolution::get1080p()), std::runtime_error);
}

void FrameTest::it_should_resize_image_when_resolution_is_smaller_than_original() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat data_matrix = imread(path_to_file, IMREAD_COLOR);
    if (data_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }
    const cv::Size newResolution = Resolution::get144p();
    const unsigned expected_num_of_cols = newResolution.width;
    const unsigned expected_num_of_rows = newResolution.height;
    const unsigned expected_num_of_channels = data_matrix.channels();

    unsigned frame_num = 0;
    Frame frame(frame_num, data_matrix);
    Frame resizedFrame = frame.resize(Resolution::get144p());

    QCOMPARE(resizedFrame.getPixelMatrix().cols, expected_num_of_cols);
    QCOMPARE(resizedFrame.getPixelMatrix().rows, expected_num_of_rows);
    QCOMPARE(resizedFrame.getPixelMatrix().channels(), expected_num_of_channels);
}

void FrameTest::is_key_frame_should_always_return_true() {
    Frame frame;

    QCOMPARE(frame.is_key_frame(), true);
}



