#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QtTest/QtTest>
#include "FrameTest.hpp"
#include "../src/movie/Resolution.hpp"

using namespace cv;

void FrameTest::constructor_should_clone_pixel_matrix() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat pixel_matrix = imread(path_to_file, IMREAD_COLOR);
    if (pixel_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }
    unsigned expected_num_of_cols = pixel_matrix.cols;
    unsigned expected_num_of_rows = pixel_matrix.rows;

    unsigned frame_num = 0;
    Frame frame(frame_num, pixel_matrix);

    QCOMPARE(frame.get_pixel_matrix().cols, expected_num_of_cols);
    QCOMPARE(frame.get_pixel_matrix().rows, expected_num_of_rows);
    bool is_memory_same = (memcmp(frame.get_pixel_matrix().data, pixel_matrix.data, *pixel_matrix.size) == 0);
    QVERIFY(is_memory_same == true);
    bool is_diffrent_addr = (frame.get_pixel_matrix().data != pixel_matrix.data);
    QVERIFY(is_diffrent_addr == true);
}

void FrameTest::resize_should_throw_exception_when_resizing_to_bigger_resolution() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat pixel_matrix = imread(path_to_file, IMREAD_COLOR);
    if (pixel_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }

    unsigned frame_num = 0;
    Frame frame(frame_num, pixel_matrix);

    /*
     * Dimensions of loaded image: 320 x 320 pixels
     */
    QVERIFY_EXCEPTION_THROWN(frame.resize(Resolution::get1080p()), std::runtime_error);
}

void FrameTest::resize_should_resize_image_when_resolution_is_smaller_than_original() {
    std::string path_to_file = "/Users/dominiktrusinski/Programowanie/C++/Oton2/tests/data/butterfly.jpg";
    Mat pixel_matrix = imread(path_to_file, IMREAD_COLOR);
    if (pixel_matrix.empty()) {
        throw std::runtime_error("Could not find file: " + path_to_file);
    }
    const cv::Size newResolution = Resolution::get144p();
    const unsigned expected_num_of_cols = newResolution.width;
    const unsigned expected_num_of_rows = newResolution.height;
    const unsigned expected_num_of_channels = pixel_matrix.channels();

    unsigned frame_num = 0;
    Frame frame(frame_num, pixel_matrix);
    Frame resizedFrame = frame.resize(Resolution::get144p());

    QCOMPARE(resizedFrame.get_pixel_matrix().cols, expected_num_of_cols);
    QCOMPARE(resizedFrame.get_pixel_matrix().rows, expected_num_of_rows);
    QCOMPARE(resizedFrame.get_pixel_matrix().channels(), expected_num_of_channels);
}

void FrameTest::frame_with_number_divisible_by_param_should_be_key_frame() {
    Frame frame;

    QCOMPARE(frame.is_key_frame(), true);
}

void FrameTest::frame_with_number_not_divisible_by_param_should_not_be_key_frame() {
    cv::Mat mat;
    unsigned short frame_num = 11;

    Frame frame(frame_num, mat);

    QCOMPARE(frame.is_key_frame(), false);
}



